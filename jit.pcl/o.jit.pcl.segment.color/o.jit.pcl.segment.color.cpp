
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>
#include <pcl/common/centroid.h>
#include <pcl/features/moment_of_inertia_estimation.h>
#include <pcl/surface/convex_hull.h>

#include <pcl/filters/voxel_grid.h>
#include <pcl/search/kdtree.h>
#include <pcl/segmentation/region_growing_rgb.h>


#include "osc.h"
#include "osc_bundle_u.h"
#include "osc_bundle_s.h"
#include "osc_timetag.h"
#include "omax_util.h"

#include "jit.common.h"
#include "max.jit.mop.h"

typedef struct _o_jit_pcl_segcolor
{
	t_object	ob;
    void		*obex;
    void        *outlet;
    t_critical  lock;
    
    long min_cluster_size;
    float dist_thresh;
    float pt_color_thresh;
    float region_color_thresh;
    float leaf_size;
    
} t_o_jit_pcl_segcolor;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err	jit_pcl_segcolor_init(void);
void		*o_jit_pcl_segcolor_new(t_symbol *s, long argc, t_atom *argv);
void		o_jit_pcl_segcolor_free(t_o_jit_pcl_segcolor *x);
t_jit_err   o_jit_pcl_segcolor_matrix_calc(t_o_jit_pcl_segcolor *x, t_symbol *s, long argc, t_atom *argv);

END_USING_C_LINKAGE

// globals
static void	*o_jit_pcl_segcolor_class = NULL;

t_jit_err jit_xyzrgb2jit(t_o_jit_pcl_segcolor *x, pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud, t_jit_matrix_info *out_minfo, void **out_matrix)
{
    char *out_bp = NULL;
    char *fop;
    
    float scalar = 1.0 / 255.;
    //*****
    // send back to jitter
    jit_object_method(*out_matrix, _jit_sym_getinfo, out_minfo);
    
    out_minfo->dim[0] = cloud->width;
    out_minfo->dim[1] = cloud->height;
    out_minfo->type = _jit_sym_float32;
    out_minfo->planecount = 6;
    jit_object_method(*out_matrix, _jit_sym_setinfo, out_minfo);
    jit_object_method(*out_matrix, _jit_sym_getinfo, out_minfo);
    
    jit_object_method(*out_matrix, _jit_sym_getdata, &out_bp);
    if (!out_bp) {
        object_error((t_object *)x, "no output data" );
        return JIT_ERR_INVALID_OUTPUT;
    }
    
    long count = 0;
    for (int j = 0; j < out_minfo->dim[0]; j++)
    {
        fop =  out_bp + j * out_minfo->dimstride[0];
        
        for( int i = 0; i < out_minfo->dim[1]; i++)
        {
            if( count < cloud->points.size() )
            {
                ((float *)fop)[0] = cloud->points[count].x;
                ((float *)fop)[1] = cloud->points[count].y;
                ((float *)fop)[2] = cloud->points[count].z;
                ((float *)fop)[3] = (float)cloud->points[count].r * scalar;
                ((float *)fop)[4] = (float)cloud->points[count].g * scalar;
                ((float *)fop)[5] = (float)cloud->points[count].b * scalar;
                
            }
            count++;
            fop += out_minfo->dimstride[1];
        }
    }
    return JIT_ERR_NONE;
    
}


t_jit_err   o_jit_pcl_segcolor_matrix_calc(t_o_jit_pcl_segcolor *x, t_symbol *s, long argc, t_atom *argv)
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
    
    t_osc_bndl_u *bndl = osc_bundle_u_alloc();
    char buf[2048];

    
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
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
        cloud->width    = (uint32_t)dim[0];
        cloud->height   = (uint32_t)dim[1];
        cloud->points.resize (cloud->width * cloud->height);
        
        size_t count = 0;
        
        for (j = 0; j < dim[0]; ++j)
        {
            fip = in_bp + j * in_minfo.dimstride[0];
            
            for( i = 0; i < dim[1]; ++i)
            {
                if(count < cloud->points.size())
                {
                    cloud->points[count].x = ((float *)fip)[0];
                    cloud->points[count].y = ((float *)fip)[1];
                    cloud->points[count].z = ((float *)fip)[2];
                    cloud->points[count].r = (uint8_t)(((float *)fip)[3] * 255.0);
                    cloud->points[count].g = (uint8_t)(((float *)fip)[4] * 255.0);
                    cloud->points[count].b = (uint8_t)(((float *)fip)[5] * 255.0);
                }
                count++;
                fip += in_minfo.dimstride[1];
            }
        }
        
       
            
            {
                //filter
                pcl::VoxelGrid<pcl::PointXYZRGB> grid;
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_voxel_ (new pcl::PointCloud<pcl::PointXYZRGB>);
                
                grid.setLeafSize (x->leaf_size, x->leaf_size, x->leaf_size);
                grid.setInputCloud (cloud);
                grid.filter (*cloud_voxel_);
                
                
                pcl::search::KdTree<pcl::PointXYZRGB>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZRGB>);
                kdtree->setInputCloud(cloud_voxel_);
                
                // Color-based region growing clustering object.
                pcl::RegionGrowingRGB<pcl::PointXYZRGB> clustering;
                clustering.setInputCloud(cloud_voxel_);
                clustering.setSearchMethod(kdtree);
                
                // Here, the minimum cluster size affects also the postprocessing step:
                // clusters smaller than this will be merged with their neighbors.
                clustering.setMinClusterSize((int)x->min_cluster_size);
                
                // Set the distance threshold, to know which points will be considered neighbors.
                clustering.setDistanceThreshold(x->dist_thresh);
                
                // Color threshold for comparing the RGB color of two points.
                clustering.setPointColorThreshold(x->pt_color_thresh);
                
                // Region color threshold for the postprocessing step: clusters with colors
                // within the threshold will be merged in one.
                clustering.setRegionColorThreshold(x->region_color_thresh);
                
                std::vector <pcl::PointIndices> clusters;
                clustering.extract(clusters);
                
                // For every cluster...
                
                if( clusters.size() > 0 )
                {
                    int count = 0;
                    
                    for (std::vector<pcl::PointIndices>::const_iterator i = clusters.begin(); i != clusters.end(); ++i)
                    {
                        sprintf(buf, "/cluster/%d", count);
                        t_osc_msg_u *cluster_group = osc_message_u_allocWithAddress(buf);
                        t_osc_bndl_u *subbndl = osc_bundle_u_alloc();
                        
                        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZRGB>);
                        for (std::vector<int>::const_iterator point = i->indices.begin(); point != i->indices.end(); point++)
                            cluster->points.push_back(cloud->points[*point]);
                        cluster->width = (uint32_t)cluster->points.size();
                        cluster->height = 1;
                        cluster->is_dense = true;

                        
                        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_hull_ (new pcl::PointCloud<pcl::PointXYZRGB>);
                        std::vector<pcl::Vertices> vertices_;
                        
                        pcl::ConvexHull<pcl::PointXYZRGB> hr;
                        hr.setDimension(3);
                        hr.setComputeAreaVolume(true);
                        hr.setInputCloud (cluster);
                        hr.reconstruct (*cloud_hull_, vertices_);

                        t_osc_msg_u *volume = osc_message_u_allocWithAddress((char *)"/hull/volume");
                        osc_message_u_appendFloat(volume, hr.getTotalVolume() );
                        osc_bundle_u_addMsg(subbndl, volume);
                        
                        /*
                        Eigen::Vector4f centroid;
                        pcl::ConstCloudIterator<pcl::PointXYZRGB> source_it (*cloud_hull_);
                        pcl::compute3DCentroid (source_it, centroid);

                        t_osc_msg_u *o_centroid = osc_message_u_allocWithAddress((char *)"/hull/centroid/xyz");
                        osc_message_u_appendFloat(o_centroid, centroid[0] );
                        osc_message_u_appendFloat(o_centroid, centroid[1] );
                        osc_message_u_appendFloat(o_centroid, centroid[2] );
                        osc_bundle_u_addMsg(subbndl, o_centroid);
                         */
                        
                        
                        /*
                        t_osc_msg_u *cvx_x = osc_message_u_allocWithAddress((char *)"/convexhull/pts/x");
                        t_osc_msg_u *cvx_y = osc_message_u_allocWithAddress((char *)"/convexhull/pts/y");
                        t_osc_msg_u *cvx_z = osc_message_u_allocWithAddress((char *)"/convexhull/pts/z");
                        for( long j = 0; j < cloud_hull_->points.size(); ++j)
                        {
                            osc_message_u_appendFloat(cvx_x, cloud_hull_->points[j].x);
                            osc_message_u_appendFloat(cvx_y, cloud_hull_->points[j].y);
                            osc_message_u_appendFloat(cvx_z, cloud_hull_->points[j].z);
                        }
                        osc_bundle_u_addMsg(subbndl, cvx_x);
                        osc_bundle_u_addMsg(subbndl, cvx_y);
                        osc_bundle_u_addMsg(subbndl, cvx_z);
                        
                        t_osc_msg_u *vert0 = osc_message_u_allocWithAddress((char *)"/convexhull/vertex/idx/0");
                        t_osc_msg_u *vert1 = osc_message_u_allocWithAddress((char *)"/convexhull/vertex/idx/1");
                        t_osc_msg_u *vert2 = osc_message_u_allocWithAddress((char *)"/convexhull/vertex/idx/2");
                        
                        std::vector<pcl::Vertices>::iterator iter_vertices;
                        for( iter_vertices = vertices_.begin(); iter_vertices != vertices_.end(); ++iter_vertices)
                        {
                            osc_message_u_appendInt32(vert0, (*iter_vertices).vertices[0]);
                            osc_message_u_appendInt32(vert1, (*iter_vertices).vertices[1]);
                            osc_message_u_appendInt32(vert2, (*iter_vertices).vertices[3]);
                            
                        }
                        osc_bundle_u_addMsg(subbndl, vert0);
                        osc_bundle_u_addMsg(subbndl, vert1);
                        osc_bundle_u_addMsg(subbndl, vert2);
*/
                        
                        
                        pcl::MomentOfInertiaEstimation <pcl::PointXYZRGB> feature_extractor;
                        feature_extractor.setInputCloud (cluster); // use hull instead?
                        feature_extractor.compute ();
                        
                        std::vector <float> moment_of_inertia;
                        std::vector <float> eccentricity;
                        pcl::PointXYZRGB min_point_AABB;
                        pcl::PointXYZRGB max_point_AABB;
                        pcl::PointXYZRGB min_point_OBB;
                        pcl::PointXYZRGB max_point_OBB;
                        pcl::PointXYZRGB position_OBB;
                        Eigen::Matrix3f rotational_matrix_OBB;
                        float major_value, middle_value, minor_value;
                        Eigen::Vector3f major_vector, middle_vector, minor_vector;
                        Eigen::Vector3f mass_center;
                        
                        feature_extractor.getMomentOfInertia (moment_of_inertia);
                        feature_extractor.getEccentricity (eccentricity);
                        feature_extractor.getAABB (min_point_AABB, max_point_AABB);
                        feature_extractor.getOBB (min_point_OBB, max_point_OBB, position_OBB, rotational_matrix_OBB);
                        feature_extractor.getEigenValues (major_value, middle_value, minor_value);
                        feature_extractor.getEigenVectors (major_vector, middle_vector, minor_vector);
                        feature_extractor.getMassCenter (mass_center);
                        
                        t_osc_msg_u *masspt = osc_message_u_allocWithAddress((char *)"/mass_center/xyz");
                        osc_message_u_appendFloat(masspt, mass_center[0]);
                        osc_message_u_appendFloat(masspt, mass_center[1]);
                        osc_message_u_appendFloat(masspt, mass_center[2]);
                        osc_bundle_u_addMsg(subbndl, masspt);
                        
                        Eigen::Vector3f position (position_OBB.x, position_OBB.y, position_OBB.z);
                        Eigen::Quaternionf quat (rotational_matrix_OBB);
                        // addCube (position, quat, max_point_OBB.x - min_point_OBB.x, max_point_OBB.y - min_point_OBB.y, max_point_OBB.z - min_point_OBB.z, "OBB");
/*
                        Eigen::Vector3f p1 (min_point_OBB.x, min_point_OBB.y, min_point_OBB.z);
                        Eigen::Vector3f p2 (min_point_OBB.x, min_point_OBB.y, max_point_OBB.z);
                        Eigen::Vector3f p3 (max_point_OBB.x, min_point_OBB.y, max_point_OBB.z);
                        Eigen::Vector3f p4 (max_point_OBB.x, min_point_OBB.y, min_point_OBB.z);
                        Eigen::Vector3f p5 (min_point_OBB.x, max_point_OBB.y, min_point_OBB.z);
                        Eigen::Vector3f p6 (min_point_OBB.x, max_point_OBB.y, max_point_OBB.z);
                        Eigen::Vector3f p7 (max_point_OBB.x, max_point_OBB.y, max_point_OBB.z);
                        Eigen::Vector3f p8 (max_point_OBB.x, max_point_OBB.y, min_point_OBB.z);
                        
                        p1 = rotational_matrix_OBB * p1 + position;
                        p2 = rotational_matrix_OBB * p2 + position;
                        p3 = rotational_matrix_OBB * p3 + position;
                        p4 = rotational_matrix_OBB * p4 + position;
                        p5 = rotational_matrix_OBB * p5 + position;
                        p6 = rotational_matrix_OBB * p6 + position;
                        p7 = rotational_matrix_OBB * p7 + position;
                        p8 = rotational_matrix_OBB * p8 + position;
                        
                        pcl::PointXYZ pt1 (p1 (0), p1 (1), p1 (2));
                        pcl::PointXYZ pt2 (p2 (0), p2 (1), p2 (2));
                        pcl::PointXYZ pt3 (p3 (0), p3 (1), p3 (2));
                        pcl::PointXYZ pt4 (p4 (0), p4 (1), p4 (2));
                        pcl::PointXYZ pt5 (p5 (0), p5 (1), p5 (2));
                        pcl::PointXYZ pt6 (p6 (0), p6 (1), p6 (2));
                        pcl::PointXYZ pt7 (p7 (0), p7 (1), p7 (2));
                        pcl::PointXYZ pt8 (p8 (0), p8 (1), p8 (2));
                 
                        viewer->addLine (pt1, pt2, 1.0, 0.0, 0.0, "1 edge");
                        viewer->addLine (pt1, pt4, 1.0, 0.0, 0.0, "2 edge");
                        viewer->addLine (pt1, pt5, 1.0, 0.0, 0.0, "3 edge");
                        viewer->addLine (pt5, pt6, 1.0, 0.0, 0.0, "4 edge");
                        viewer->addLine (pt5, pt8, 1.0, 0.0, 0.0, "5 edge");
                        viewer->addLine (pt2, pt6, 1.0, 0.0, 0.0, "6 edge");
                        viewer->addLine (pt6, pt7, 1.0, 0.0, 0.0, "7 edge");
                        viewer->addLine (pt7, pt8, 1.0, 0.0, 0.0, "8 edge");
                        viewer->addLine (pt2, pt3, 1.0, 0.0, 0.0, "9 edge");
                        viewer->addLine (pt4, pt8, 1.0, 0.0, 0.0, "10 edge");
                        viewer->addLine (pt3, pt4, 1.0, 0.0, 0.0, "11 edge");
                        viewer->addLine (pt3, pt7, 1.0, 0.0, 0.0, "12 edge");
                  */
                        
                        
                        
                        osc_message_u_appendBndl_u(cluster_group, subbndl);
                        osc_bundle_u_addMsg(bndl, cluster_group);

                        
                        count++;
                    }
                }
                else
                {
                    ; // do something besides send an empty bundle
                }
            }
            
            omax_util_outletOSC_u(x->outlet, bndl);
            
            if(bndl)
                osc_bundle_u_free(bndl);

        }
out:
    jit_object_method(matrix, _jit_sym_lock, in_savelock);
    return err;
}

void o_jit_pcl_segcolor_free(t_o_jit_pcl_segcolor *x)
{
    max_jit_mop_free(x);
    jit_object_free(max_jit_obex_jitob_get(x));
    max_jit_object_free(x);
}

void *o_jit_pcl_segcolor_new(t_symbol *s, long argc, t_atom *argv)
{
    t_o_jit_pcl_segcolor	*x;
    
    x = (t_o_jit_pcl_segcolor *)max_jit_object_alloc((t_class *)o_jit_pcl_segcolor_class, NULL);
    if (x)
    {
        x->dist_thresh = 10.;
        x->pt_color_thresh = 6.;
        x->region_color_thresh = 5.;
        x->min_cluster_size = 10;
        x->leaf_size = 0.02;

        x->outlet = outlet_new(x, "FullPacket");
        
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

	c = class_new("o.jit.pcl.segment.color", (method)o_jit_pcl_segcolor_new, (method)o_jit_pcl_segcolor_free, sizeof(t_o_jit_pcl_segcolor), NULL, A_GIMME, 0);
	max_jit_class_obex_setup(c, calcoffset(t_o_jit_pcl_segcolor, obex));
    
    class_addmethod(c, (method)o_jit_pcl_segcolor_matrix_calc, "jit_matrix", A_GIMME, 0);
	class_addmethod(c, (method)max_jit_mop_assist, "assist", A_CANT, 0);	// standard matrix-operator (mop) assist fn

    attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;

    // add attribute(s)
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "dist_thresh",
                                          _jit_sym_float32,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_segcolor, dist_thresh));
    
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "pt_color_thresh",
                                          _jit_sym_float32,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_segcolor, pt_color_thresh));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "region_color_thresh",
                                          _jit_sym_float32,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_segcolor, region_color_thresh));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "min_cluster_size",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_segcolor, min_cluster_size));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "leaf_size",
                                          _jit_sym_float32,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_segcolor, leaf_size));
    jit_class_addattr(c, attr);
    
	class_register(CLASS_BOX, c);
	o_jit_pcl_segcolor_class = c;
	return 0;
}
END_USING_C_LINKAGE


