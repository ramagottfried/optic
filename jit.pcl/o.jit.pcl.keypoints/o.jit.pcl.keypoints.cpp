
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>
//#include <pcl/features/integral_image_normal.h>

//#include <pcl/range_image/range_image_planar.h>
//#include <pcl/features/range_image_border_extractor.h>
//#include <pcl/keypoints/narf_keypoint.h>

#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/filters/voxel_grid.h>

#include <pcl/features/fpfh.h>

//#include <pcl/filters/passthrough.h>

#include <pcl/filters/extract_indices.h>
#include <pcl/keypoints/iss_3d.h>

#include "osc.h"
#include "osc_bundle_u.h"
#include "osc_bundle_s.h"
#include "osc_timetag.h"
#include "omax_util.h"
#include "o.h"

#include "jit.common.h"
#include "max.jit.mop.h"

//#include "o.max.pcl.h"
//#include "o.max.pcl.fn_queue.h"
#include "o.max.pcl.normals.h"
#include "o.max.pcl.passthrough.h"

typedef struct _o_jit_pcl_keypoints
{
	t_object	ob;
    void		*obex;
    void        *outlet;
    t_critical  lock;
    
    double      z_min;
    double      z_max;

    double      maxAngleWidth;
    double      maxAngleHeight;
    double      angularResolution;
    
    int         disable_transform;
    double      voxel_resolution;
    double      seed_resolution;
    double      color_importance;
    double      spatial_importance;
    double      normal_importance;
    
    OPCL_normals    *norm;
    OPCL_passthrough<pcl::PointXYZRGBNormal> *passthrough;
//    omax_pcl_fn_queue   *queue;
    
} t_o_jit_pcl_keypoints;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err	jit_pcl_keypoints_init(void);
void		*o_jit_pcl_keypoints_new(t_symbol *s, long argc, t_atom *argv);
void		o_jit_pcl_keypoints_free(t_o_jit_pcl_keypoints *x);
t_jit_err   o_jit_pcl_keypoints_matrix_calc(t_o_jit_pcl_keypoints *x, t_symbol *s, long argc, t_atom *argv);

END_USING_C_LINKAGE

// globals
static void	*o_jit_pcl_keypoints_class = NULL;

double computeCloudResolution(const pcl::PointCloud<pcl::PointXYZRGBNormal>::ConstPtr& cloud)
{
    double resolution = 0.0;
    int numberOfPoints = 0;
    ssize_t nres;
    std::vector<int> indices(2);
    std::vector<float> squaredDistances(2);
    pcl::search::KdTree<pcl::PointXYZRGBNormal> tree;
    tree.setInputCloud(cloud);
    
    for (int i = 0; i < cloud->size(); ++i)
    {
        if (! pcl_isfinite((*cloud)[i].x))
            continue;
        
        // Considering the second neighbor since the first is the point itself.
        nres = tree.nearestKSearch(i, 2, indices, squaredDistances);
        if (nres == 2)
        {
            resolution += sqrt(squaredDistances[1]);
            ++numberOfPoints;
        }
    }
    if (numberOfPoints != 0)
        resolution /= numberOfPoints;
    
    return resolution;
}


t_jit_err   o_jit_pcl_keypoints_matrix_calc(t_o_jit_pcl_keypoints *x, t_symbol *s, long argc, t_atom *argv)
{
    void                *matrix = NULL;
    t_jit_err			err = JIT_ERR_NONE;
    long				in_savelock;
    t_jit_matrix_info	in_minfo;
    char				*in_bp;
    long				i, j;
    long				dimcount;
    long				planecount;
    long				dim[JIT_MATRIX_MAX_DIMCOUNT];
    char                *fip;
    
    if( argc && argv )
    {
        matrix = jit_object_findregistered(jit_atom_getsym(argv));
    }
    else
        return JIT_ERR_INVALID_INPUT;
    
    if( matrix == NULL || !jit_object_method(matrix, _jit_sym_class_jit_matrix))
        return JIT_ERR_INVALID_PTR;

    in_savelock = (long)jit_object_method(matrix, _jit_sym_lock, 1);
    jit_object_method(matrix, _jit_sym_getinfo, &in_minfo);
    jit_object_method(matrix, _jit_sym_getdata, &in_bp);
    
    if (!in_bp)
        return JIT_ERR_INVALID_INPUT;
    
    //get dimensions/planecount
    dimcount   = in_minfo.dimcount;
    planecount = in_minfo.planecount;
    
    if( planecount < 6 )
    {
        object_error((t_object *)x, "requires a 6 plane matrix (xyzrgb)");
        err = JIT_ERR_INVALID_INPUT;
        goto out;
    }
    if( in_minfo.type != _jit_sym_float32)
    {
        object_error((t_object *)x, "received: %s jit.pcl uses only float32 matrixes", in_minfo.type->s_name );
        err = JIT_ERR_INVALID_INPUT;
        goto out;
    }
    
    //if dimsize is 1, treat as infinite domain across that dimension.
    //otherwise truncate if less than the output dimsize
    for (i=0; i<dimcount; i++) {
        dim[i] = in_minfo.dim[i];
        if ( in_minfo.dim[i]<dim[i] && in_minfo.dim[i]>1) {
            dim[i] = in_minfo.dim[i];
        }
    }
    
    
    {
        //convert to point cloud
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud = jit2cloudXYZRGB( in_bp, in_minfo );
        
        t_osc_bndl_u *bndl = osc_bundle_u_alloc();
        
        // return dst cloud?
        pcl::PointCloud<pcl::Normal>::Ptr normals;
//        x->norm->calc( cloud, &normals, &bndl );
        //post("xyzrgbn %f %f %f %f %f %f", cloud->points[0].x, cloud->points[0].y, cloud->points[0].z, cloud->points[0].r, cloud->points[0].g, cloud->points[0].b);

        typedef pcl::PointXYZRGBNormal PointT;
        
        pcl::PointCloud<PointT>::Ptr cloudXYZRGBN(new pcl::PointCloud<PointT>);
    //    pcl::concatenateFields( *cloud, *normals, *cloudXYZRGBN );
        
        pcl::PointCloud<PointT>::Ptr filteredCloud;
  //      x->passthrough->calc( cloudXYZRGBN, &filteredCloud, &bndl );
        
        //post("xyzrgbn %f %f %f %f %f %f %f %f %f", cloudXYZRGBN->points[0].x, cloudXYZRGBN->points[0].y, cloudXYZRGBN->points[0].z, cloudXYZRGBN->points[0].r, cloudXYZRGBN->points[0].g, cloudXYZRGBN->points[0].b, cloudXYZRGBN->points[0].normal_x, cloudXYZRGBN->points[0].normal_y, cloudXYZRGBN->points[0].normal_z);
       
        {
            /*
            typedef pcl::PointXYZRGBNormal PointT;
            
            pcl::PointCloud<PointT>::Ptr filteredCloud(new pcl::PointCloud<PointT>);
            pcl::PassThrough<PointT> pass_filter;
            pass_filter.setInputCloud(cloudXYZRGBN);
            // Filter out all points with Z values not in the [0-2] range.
            pass_filter.setFilterFieldName("z");
            pass_filter.setFilterLimits(x->z_min, x->z_max );
            pass_filter.filter(*filteredCloud);
            */
//            post("src size %d filtered cloud size %d", cloudXYZRGBN->size(), filteredCloud->size() );

                 /*
            
            pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
            
            pcl::IntegralImageNormalEstimation<pcl::PointXYZRGB, pcl::Normal> normalEstimation;
            normalEstimation.setInputCloud(cloud);
            // Other estimation methods: COVARIANCE_MATRIX, AVERAGE_DEPTH_CHANGE, SIMPLE_3D_GRADIENT.
            // They determine the smoothness of the result, and the running time.
            normalEstimation.setNormalEstimationMethod(normalEstimation.AVERAGE_3D_GRADIENT);
            // Depth threshold for computing object borders based on depth changes, in meters.
            normalEstimation.setMaxDepthChangeFactor(0.02f);
            // Factor that influences the size of the area used to smooth the normals.
            normalEstimation.setNormalSmoothingSize(10.0f);
            normalEstimation.compute(*normals);
            */
            

            
            /*
            cloudXYZRGBN->points.resize( cloud->points.size() );
            
            uint32_t count = 0;
            for( i = 0; i < cloud->points.size(); i++ )
            {
                if( cloud->points[i].z > x->z_min && cloud->points[i].z < x->z_max )
                {
                    cloudXYZRGBN->points[count].x = cloud->points[i].x;
                    cloudXYZRGBN->points[count].y = cloud->points[i].y;
                    cloudXYZRGBN->points[count].z = cloud->points[i].z;
                    cloudXYZRGBN->points[count].r = cloud->points[i].r;
                    cloudXYZRGBN->points[count].g = cloud->points[i].g;
                    cloudXYZRGBN->points[count].b = cloud->points[i].b;
                    cloudXYZRGBN->points[count].normal_x = normals->points[i].normal_x;
                    cloudXYZRGBN->points[count].normal_y = normals->points[i].normal_y;
                    cloudXYZRGBN->points[count].normal_z = normals->points[i].normal_z;
                    count++;
                }
            }
            cloudXYZRGBN->height = 1;
            cloudXYZRGBN->width = count;
        */

     /*
            pcl::VoxelGrid<PointT> grid;
            pcl::PointCloud<PointT>::Ptr cloud_voxel_ (new pcl::PointCloud<PointT>);
            
            grid.setLeafSize (x->voxel_resolution, x->voxel_resolution, x->voxel_resolution);
            grid.setInputCloud (cloudXYZRGBN);
            grid.filter ( *cloud_voxel_ );
      */
/*
            pcl::PointCloud<PointT>::Ptr downsampledCloud(new pcl::PointCloud<PointT>);

            pcl::UniformSampling<PointT> filter;
            filter.setInputCloud(cloudXYZRGBN);
            // We set the size of every voxel to be 1x1x1cm
            // (only one point per every cubic centimeter will survive).
            filter.setRadiusSearch(0.02f);
            // We need an additional object to store the indices of surviving points.
            pcl::PointCloud<int> downsampIndices;
            filter.compute(downsampIndices);
            
            pcl::copyPointCloud(*cloud, downsampIndices.points, *downsampledCloud);
*/
            
            /*
            
            pcl::PointCloud<PointT>::Ptr filteredCloud(new pcl::PointCloud<PointT>);
            pcl::PointCloud<pcl::Normal>::Ptr filteredNormals(new pcl::PointCloud<pcl::Normal>);
            filteredCloud->height = 1;
            filteredNormals->height = 1;
            size_t count = 1;
            for (size_t i = 0; i < downsampledCloud->points.size(); i++)
            {
                if( downsampledCloud->points[i].z > 0.0 && downsampledCloud->points[i].z < 2.0 )
                {
                    filteredCloud->points.push_back( downsampledCloud->points[i] );
                    filteredNormals->points.push_back( downsampledNormals->points[i] );
                    count++;
                }
            }
            filteredCloud->width = (uint32_t)count;
            filteredNormals->width = (uint32_t)count;
*/
            /*
            pcl::PassThrough<PointT> pass_filter;
            pass_filter.setInputCloud(downsampledCloud);
            // Filter out all points with Z values not in the [0-2] range.
            pass_filter.setFilterFieldName("z");
            pass_filter.setFilterLimits(0.0, 2.0);
            pass_filter.filter(*filteredCloud);
            
            pcl::IndicesConstPtr filterIndices = pass_filter.getIndices();
            
            pcl::ExtractIndices<pcl::Normal> extractNormals;
            extractNormals.setIndices(filterIndices);
            extractNormals.setInputCloud(downsampledNormals);
            extractNormals.setNegative(false);
            extractNormals.filter (*filteredNormals);
             post("filterIndices size %d %d", downsampledCloud->size(), downsampledCloud->size() - filterIndices->size());

            */
/*
            post("downsampIndices size %d %d", downsampledCloud->width, downsampledCloud->height);
            post("normals size %d %d", downsampledNormals->width, downsampledNormals->height);

            post("filteredCloud size %d %d", filteredCloud->width, filteredCloud->height);
            post("filteredNormals size %d %d", filteredNormals->width, filteredNormals->height);


            // Object for storing the PFH descriptors for each point.
            pcl::PointCloud<pcl::FPFHSignature33>::Ptr descriptors(new pcl::PointCloud<pcl::FPFHSignature33>());
            
            pcl::FPFHEstimation<PointT, pcl::Normal, pcl::FPFHSignature33> fpfh;
            fpfh.setInputCloud(filteredCloud2);
            fpfh.setInputNormals(normals);
            pcl::search::KdTree<PointT>::Ptr kdtree(new pcl::search::KdTree<PointT>);
            fpfh.setSearchMethod(kdtree);
            // Search radius, to look for neighbors. Note: the value given here has to be
            // larger than the radius used to estimate the normals.
            fpfh.setRadiusSearch(0.05);
            
            fpfh.compute(*descriptors);
 */
/*
            pcl::PointCloud<PointT>::Ptr keypoints(new pcl::PointCloud<PointT>);
            pcl::ISSKeypoint3D<PointT, PointT> detector;
            detector.setInputCloud(cloud_voxel_);

            pcl::search::KdTree<PointT>::Ptr kdtree(new pcl::search::KdTree<PointT>);
            detector.setSearchMethod(kdtree);
            
            double resolution = computeCloudResolution( cloud_voxel_ );
            t_osc_message_u *res = osc_message_u_allocWithAddress((char *)"/resolusion");
            osc_message_u_appendDouble(res, resolution);
            osc_bundle_u_addMsg(bndl, res);

            // Set the radius of the spherical neighborhood used to compute the scatter matrix.
            detector.setSalientRadius(6 * resolution);
            // Set the radius for the application of the non maxima supression algorithm.
            detector.setNonMaxRadius(4 * resolution);
            // Set the minimum number of neighbors that has to be found while applying the non maxima suppression algorithm.
            detector.setMinNeighbors(1);
            // Set the upper bound on the ratio between the second and the first eigenvalue.
            detector.setThreshold21(0.975);
            // Set the upper bound on the ratio between the third and the second eigenvalue.
            detector.setThreshold32(0.975);
            // Set the number of prpcessing threads to use. 0 sets it to automatic.
            detector.setNumberOfThreads(4);
            
            detector.compute(*keypoints);


            for(size_t i = 0; i < keypoints->size(); i++)
            {
                sprintf(buf, "/keypoint/%ld/xyz", i);
                t_osc_message_u *mes = osc_message_u_allocWithAddress(buf);
                osc_message_u_appendDouble(mes, keypoints->points[i].x);
                osc_message_u_appendDouble(mes, keypoints->points[i].y);
                osc_message_u_appendDouble(mes, keypoints->points[i].z);
                osc_bundle_u_addMsg(bndl, mes);
            }
 */
            /*
            pcl::PointCloud<int>::Ptr keypoints(new pcl::PointCloud<int>);
            
            int imageSizeX = 640;
            int imageSizeY = 480;
            // Center of projection. here, we choose the middle of the image.
            float centerX = 640.0f / 2.0f;
            float centerY = 480.0f / 2.0f;
            // Focal length. The value seen here has been taken from the original depth images.
            // It is safe to use the same value vertically and horizontally.
            float focalLengthX = 525.0f, focalLengthY = focalLengthX;
            // Sensor pose. Thankfully, the cloud includes the data.
            Eigen::Affine3f sensorPose = Eigen::Affine3f(Eigen::Translation3f(cloud->sensor_origin_[0],
                                                                              cloud->sensor_origin_[1],
                                                                              cloud->sensor_origin_[2])) *
            Eigen::Affine3f(cloud->sensor_orientation_);
            // Noise level. If greater than 0, values of neighboring points will be averaged.
            // This would set the search radius (e.g., 0.03 == 3cm).
            float noiseLevel = 0.0f;
            // Minimum range. If set, any point closer to the sensor than this will be ignored.
            float minimumRange = 0.0f;
            
            // Planar range image object.
            pcl::RangeImagePlanar rangeImagePlanar;
            rangeImagePlanar.createFromPointCloudWithFixedSize(*cloud, imageSizeX, imageSizeY,
                                                               centerX, centerY, focalLengthX, focalLengthY,
                                                               sensorPose, pcl::RangeImage::CAMERA_FRAME,
                                                               noiseLevel, minimumRange);

            
            pcl::RangeImageBorderExtractor borderExtractor;
            // Keypoint detection object.
            pcl::NarfKeypoint detector(&borderExtractor);
            detector.setRangeImage(&rangeImagePlanar);
            // The support size influences how big the surface of interest will be,
            // when finding keypoints from the border information.
            detector.getParameters().support_size = 0.2f;
            
            detector.compute(*keypoints);
            */
//            x->norm->getState( &bndl );
//            x->passthrough->getState( &bndl );
            omax_util_outletOSC_u(x->outlet, bndl);
            
            if(bndl)
                osc_bundle_u_free(bndl);

        }

    }
out:
    jit_object_method(matrix, _jit_sym_lock, in_savelock);
    return err;
}

#define c11_osc_bundle_s_wrap_naked_message(len, ptr){\
    if(ptr && len >= 8){\
        if(strncmp("#bundle\0", (char *)(ptr), 8)){	\
            char *oldptr = (char *)ptr;\
            long oldlen = len;\
            len += 4 + OSC_HEADER_SIZE;\
            ptr = (char *)alloca(len);\
            char alloc = 0;\
            osc_bundle_s_wrapMessage(oldlen, oldptr, &len, (char **)(&ptr), &alloc);\
        }\
    }\
}

void o_jit_pcl_keypoints_fullPacket(t_o_jit_pcl_keypoints *x, t_symbol *msg, int argc, t_atom *argv)
{
    OMAX_UTIL_GET_LEN_AND_PTR
    c11_osc_bundle_s_wrap_naked_message(len, ptr);
    if(len == OSC_HEADER_SIZE)
        return;
    
    t_osc_bundle_u* out_bndl = NULL;
    x->norm->FullPacket( len, ptr, &out_bndl );
    x->passthrough->FullPacket( len, ptr, &out_bndl );
    
    omax_util_outletOSC_u(x->outlet, out_bndl);
    
    if(out_bndl)
        osc_bundle_u_free(out_bndl);
}

void o_jit_pcl_keypoints_bang(t_o_jit_pcl_keypoints *x)
{
    t_osc_bundle_u* out_bndl = NULL;
    x->norm->getState( &out_bndl );
    x->passthrough->getState( &out_bndl );
    
    omax_util_outletOSC_u(x->outlet, out_bndl);
    
    if(out_bndl)
        osc_bundle_u_free(out_bndl);

}

void o_jit_pcl_keypoints_free(t_o_jit_pcl_keypoints *x)
{
    max_jit_mop_free(x);
    jit_object_free(max_jit_obex_jitob_get(x));
    max_jit_object_free(x);

    delete x->norm;
    delete x->passthrough;
    
}

void *o_jit_pcl_keypoints_new(t_symbol *s, long argc, t_atom *argv)
{
    t_o_jit_pcl_keypoints	*x;
    
    x = (t_o_jit_pcl_keypoints *)max_jit_object_alloc((t_class *)o_jit_pcl_keypoints_class, NULL);
    if (x)
    {
        //hard set to kinect 1 right now
        x->z_min = 0.0;
        x->z_max = 1.0;
        
        x->maxAngleWidth = (57.0 * (M_PI / 180.0));
        x->maxAngleHeight =  (43.0 * (M_PI / 180.0));
        x->angularResolution = 57.0 / 640. * (M_PI/180.0);
        
        x->disable_transform = 1;
        x->voxel_resolution = 0.02f;
        x->seed_resolution = 0.1f;
        x->color_importance = 0.2f;
        x->spatial_importance = 0.4f;
        x->normal_importance = 1.0f;

        x->outlet = outlet_new(x, "FullPacket");
        
        x->norm = new OPCL_normals;
        x->passthrough = new OPCL_passthrough<pcl::PointXYZRGBNormal>;
        
        //no normal args, no matrices
        max_jit_attr_args(x,argc,argv); //handle attribute args
        
    }
    return (x);
}



BEGIN_USING_C_LINKAGE
int C74_EXPORT main(void)
{
	t_class         *c;
    t_jit_object	*attr;
    long            attrflags;

	c = class_new("o.jit.pcl.keypoints", (method)o_jit_pcl_keypoints_new, (method)o_jit_pcl_keypoints_free, sizeof(t_o_jit_pcl_keypoints), NULL, A_GIMME, 0);
	max_jit_class_obex_setup(c, calcoffset(t_o_jit_pcl_keypoints, obex));
    
    class_addmethod(c, (method)o_jit_pcl_keypoints_matrix_calc, "jit_matrix", A_GIMME, 0);
    class_addmethod(c, (method)o_jit_pcl_keypoints_fullPacket, "FullPacket", A_GIMME, 0);
    class_addmethod(c, (method)o_jit_pcl_keypoints_bang, "bang", 0);
    class_addmethod(c, (method)max_jit_mop_assist, "assist", A_CANT, 0);	// standard matrix-operator (mop) assist fn

    attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;

    // add attribute(s)
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "disable_transform",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, disable_transform));
    jit_attr_addfilterset_clip(attr, 0, 1, true, true);
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "voxel_resolution",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, voxel_resolution));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "seed_resolution",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, seed_resolution));
    
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "color_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, color_importance));
    jit_class_addattr(c, attr);
    
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "spatial_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, spatial_importance));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "normal_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, normal_importance));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "z_min",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, z_min));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "z_max",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_keypoints, z_max));
    jit_class_addattr(c, attr);

    
    
    
	class_register(CLASS_BOX, c);
	o_jit_pcl_keypoints_class = c;
	return 0;
}
END_USING_C_LINKAGE


