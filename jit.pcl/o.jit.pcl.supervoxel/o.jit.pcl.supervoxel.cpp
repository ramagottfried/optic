#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>
#include <pcl/segmentation/supervoxel_clustering.h>
#include <pcl/filters/passthrough.h>

#include "jit.common.h"

typedef struct _jit_pcl_supervoxel
{
    t_object	ob;
    
    int disable_transform;
    double voxel_resolution;
    double seed_resolution;
    double color_importance;
    double spatial_importance;
    double normal_importance;
    
} t_jit_pcl_supervoxel;


typedef pcl::PointXYZRGBA t_PointT;
typedef pcl::PointCloud<t_PointT> t_PointCloudT;
typedef pcl::PointNormal t_PointNT;
typedef pcl::PointCloud<t_PointNT> t_PointNCloudT;
typedef pcl::PointXYZL t_PointLT;
typedef pcl::PointCloud<t_PointLT> t_PointLCloudT;

// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err                   jit_pcl_supervoxel_init				(void);
t_jit_pcl_supervoxel       *jit_pcl_supervoxel_new				(void);
void                        jit_pcl_supervoxel_free				(t_jit_pcl_supervoxel *x);
t_jit_err                   jit_pcl_supervoxel_matrix_calc		(t_jit_pcl_supervoxel *x, void *inputs, void *outputs);
void                        jit_pcl_supervoxel_calculate_ndim	(t_jit_pcl_supervoxel *x, long dim, long *dimsize, long planecount, t_jit_matrix_info *in_minfo,char *bip, t_jit_matrix_info *out_minfo, char *bop);
END_USING_C_LINKAGE

pcl::PointCloud<pcl::Normal>::Ptr jit_pcl_supervoxel_normals( pcl::PointCloud<pcl::PointXYZ>::Ptr cloud );
pcl::PointCloud<pcl::PointXYZ>::Ptr jit_pcl_supervoxel_genTriangles( pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals );


// globals
static void *s_jit_pcl_supervoxel_class = NULL;


/************************************************************************************/

t_jit_err jit_pcl_supervoxel_init(void)
{
    long			attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
    t_jit_object	*attr;
    t_jit_object	*mop;
    t_atom a[1];
    
    s_jit_pcl_supervoxel_class = jit_class_new("jit_pcl_supervoxel", (method)jit_pcl_supervoxel_new, (method)jit_pcl_supervoxel_free, sizeof(t_jit_pcl_supervoxel), 0);
    
    // add matrix operator (mop)
    // args are  num inputs and num outputs
    mop = (t_jit_object *)jit_object_new(_jit_sym_jit_mop, 1, 1);
    jit_mop_input_nolink(mop,1);
    
    
    attr = (t_jit_object *)jit_object_method(mop, _jit_sym_getinput, 1);
    jit_attr_setlong(attr,_jit_sym_dimlink,0);
    jit_attr_setlong(attr,_jit_sym_typelink,0);
    jit_attr_setlong(attr,_jit_sym_planelink,0);
    jit_object_method(attr,_jit_sym_ioproc,jit_mop_ioproc_copy_adapt);
    jit_atom_setsym(a, _jit_sym_float32);
    jit_object_method(attr, _jit_sym_types,1, a);
    
    attr = (t_jit_object *)jit_object_method(mop, _jit_sym_getoutput, 1);
    jit_attr_setlong(attr, _jit_sym_dimlink,0);
    
    jit_class_addadornment(s_jit_pcl_supervoxel_class, mop);
    
    // add method(s)
    jit_class_addmethod(s_jit_pcl_supervoxel_class, (method)jit_pcl_supervoxel_matrix_calc, "matrix_calc", A_CANT, 0);

    // add attribute(s)
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "disable_transform",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_supervoxel, disable_transform));
    jit_attr_addfilterset_clip(attr, 0, 1, true, true);
    jit_class_addattr(s_jit_pcl_supervoxel_class, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "voxel_resolution",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_supervoxel, voxel_resolution));
    jit_class_addattr(s_jit_pcl_supervoxel_class, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "seed_resolution",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_supervoxel, seed_resolution));
    
    jit_class_addattr(s_jit_pcl_supervoxel_class, attr);
    

    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "color_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_supervoxel, color_importance));
    jit_class_addattr(s_jit_pcl_supervoxel_class, attr);

    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "spatial_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_supervoxel, spatial_importance));
    jit_class_addattr(s_jit_pcl_supervoxel_class, attr);

    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "normal_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_supervoxel, normal_importance));
    jit_class_addattr(s_jit_pcl_supervoxel_class, attr);
    
    // finalize class
    jit_class_register(s_jit_pcl_supervoxel_class);
    return JIT_ERR_NONE;
}

/************************************************************************************/
// Object Life Cycle

t_jit_pcl_supervoxel *jit_pcl_supervoxel_new(void)
{
    t_jit_pcl_supervoxel	*x = NULL;
    
    x = (t_jit_pcl_supervoxel*)jit_object_alloc(s_jit_pcl_supervoxel_class);
    if (x) {
        x->disable_transform = 1;
        x->voxel_resolution = 0.008f;
        x->seed_resolution = 0.1f;
        x->color_importance = 0.2f;
        x->spatial_importance = 0.4f;
        x->normal_importance = 1.0f;
    }
    return x;
}


void jit_pcl_supervoxel_free(t_jit_pcl_supervoxel *x)
{
    ;	// nothing to free for our simple object
}


// PCL

// conversions

t_jit_err jit_xyzrgb2jit(t_jit_pcl_supervoxel *x, pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud, t_jit_matrix_info *out_minfo, void **out_matrix)
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

t_jit_err jit_cloud2jit(t_jit_pcl_supervoxel *x, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, t_jit_matrix_info *out_minfo, void **out_matrix)
{
    char *out_bp = NULL;
    float *fop;
    
    //*****
    // send back to jitter
    jit_object_method(*out_matrix, _jit_sym_getinfo, out_minfo);
    
    out_minfo->dim[0] = cloud->width;
    out_minfo->dim[1] = cloud->height;
    out_minfo->type = _jit_sym_float32;
    out_minfo->planecount = 3;
    jit_object_method(*out_matrix, _jit_sym_setinfo, out_minfo);
    jit_object_method(*out_matrix, _jit_sym_getinfo, out_minfo);
    
    jit_object_method(*out_matrix, _jit_sym_getdata, &out_bp);
    if (!out_bp) {
        object_error((t_object *)x, "no output data" );
        return JIT_ERR_INVALID_OUTPUT;
    }
    
    long rowstride = out_minfo->dimstride[1];
    long count = 0;
    for (int j = 0; j < out_minfo->dim[0]; j++)
    {
        fop =  (float *)(out_bp + j * out_minfo->dimstride[0]);
        
        for( int i = 0; i < out_minfo->dim[1]; i++)
        {
            if( count < cloud->points.size() )
            {
                fop[0] = cloud->points[count].x;
                fop[1] = cloud->points[count].y;
                fop[2] = cloud->points[count].z;
                
            }
            count++;
            fop += rowstride;
        }
    }
    return JIT_ERR_NONE;
    
}


void pcl_xyzrgb2xyz(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz, pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud_xyzrgb )
{
    cloud_xyz->points.resize(cloud_xyzrgb->size());
    for (size_t i = 0; i < cloud_xyzrgb->points.size(); i++) {
        cloud_xyz->points[i].x = cloud_xyzrgb->points[i].x;
        cloud_xyz->points[i].y = cloud_xyzrgb->points[i].y;
        cloud_xyz->points[i].z = cloud_xyzrgb->points[i].z;
    }
}

/************************************************************************************/
// Methods bound to input/inlets
t_jit_err jit_pcl_supervoxel_matrix_calc(t_jit_pcl_supervoxel *x, void *inputs, void *outputs)
{
    t_jit_err			err = JIT_ERR_NONE;
    long				in_savelock;
    long				out_savelock;
    t_jit_matrix_info	in_minfo;
    t_jit_matrix_info	out_minfo;
    char				*in_bp;
    char				*out_bp;
    long				i, j;
    long				dimcount;
    long				planecount;
    long				dim[JIT_MATRIX_MAX_DIMCOUNT];
    void				*in_matrix;
    void				*out_matrix;
    
    char *fip, *fop;
    
    
    in_matrix 	= jit_object_method(inputs,_jit_sym_getindex,0);
    out_matrix 	= jit_object_method(outputs,_jit_sym_getindex,0);
    
    if (x && in_matrix && out_matrix)
    {
        in_savelock = (long) jit_object_method(in_matrix, _jit_sym_lock, 1);
        out_savelock = (long) jit_object_method(out_matrix, _jit_sym_lock, 1);
        
        jit_object_method(in_matrix, _jit_sym_getinfo, &in_minfo);
        jit_object_method(in_matrix, _jit_sym_getdata, &in_bp);
        
        if (!in_bp) {
            err=JIT_ERR_INVALID_INPUT;
            goto out;
        }
        
        //get dimensions/planecount
        dimcount   = in_minfo.dimcount;
        planecount = in_minfo.planecount;
        
        if( planecount < 6 )
        {
            object_error((t_object *)x, "requires a 6 plane matrix (xyzrgb)");
            goto out;
        }
        if( in_minfo.type != _jit_sym_float32)
        {
            object_error((t_object *)x, "received: %s jit.pcl uses only float32 matrixes", in_minfo.type->s_name );
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
        
        // PCL stuff
        
        pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
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
                    cloud->points[count].a = 255;

                }
                count++;
                fip += in_minfo.dimstride[1];
            }
        }
        

        {

            pcl::SupervoxelClustering<pcl::PointXYZRGBA> super (x->voxel_resolution, x->seed_resolution);
            if (x->disable_transform)
                super.setUseSingleCameraTransform (false);
            
            super.setInputCloud ( cloud );
            super.setColorImportance (x->color_importance);
            super.setSpatialImportance (x->spatial_importance);
            super.setNormalImportance (x->normal_importance);
            
            std::map <uint32_t, pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr > supervoxel_clusters;
            
            super.extract (supervoxel_clusters);
//            post ("Found %d supervoxels\n", supervoxel_clusters.size ());
            
            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr voxel_centroid_cloud = super.getVoxelCentroidCloud ();
            pcl::PointCloud<pcl::PointXYZL>::Ptr labeled_voxel_cloud = super.getLabeledVoxelCloud ();
            pcl::PointCloud<pcl::PointNormal>::Ptr sv_normal_cloud = super.makeSupervoxelNormalCloud (supervoxel_clusters);
            
            std::multimap<uint32_t, uint32_t> supervoxel_adjacency;
            super.getSupervoxelAdjacency (supervoxel_adjacency);

            //To make a graph of the supervoxel adjacency, we need to iterate through the supervoxel adjacency multimap
            std::multimap<uint32_t,uint32_t>::iterator label_itr = supervoxel_adjacency.begin ();
            for ( ; label_itr != supervoxel_adjacency.end (); )
            {
                //First get the label
                uint32_t supervoxel_label = label_itr->first;
                
                //Now get the supervoxel corresponding to the label
                pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr supervoxel = supervoxel_clusters.at( supervoxel_label );
                
                //Now we need to iterate through the adjacent supervoxels and make a point cloud of them
                pcl::PointCloud<pcl::PointXYZRGBA> adjacent_supervoxel_centers;
                
                std::multimap<uint32_t,uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(supervoxel_label).first;
                for ( ; adjacent_itr!=supervoxel_adjacency.equal_range(supervoxel_label).second; ++adjacent_itr)
                {
                    pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr neighbor_supervoxel = supervoxel_clusters.at (adjacent_itr->second);
                    adjacent_supervoxel_centers.push_back (neighbor_supervoxel->centroid_);
                    
                    // line from supervoxel->centroid_ to each adjacent_supervoxel_centers
                    
                }
                
                //Move iterator forward to next label
                label_itr = supervoxel_adjacency.upper_bound (supervoxel_label);
            }
            
            
 //           err = jit_xyzrgb2jit(x, voxel_centroid_cloud, &out_minfo, &out_matrix );
            if( err != JIT_ERR_NONE )
                goto out;
            
        }
      
        // unable to make use of jitter's parallel methods since we need all the data together
        //jit_parallel_ndim_simplecalc2((method)jit_pcl_supervoxel_calculate_ndim,
        //	x, dimcount, dim, planecount, &in_minfo, in_bp, &out_minfo, out_bp,
        //	0 /* flags1 */, 0 /* flags2 */);
        
    }
    else
        return JIT_ERR_INVALID_PTR;
    
out:
    jit_object_method( out_matrix, _jit_sym_lock, out_savelock );
    jit_object_method( in_matrix, _jit_sym_lock, in_savelock );
    return err;
}
