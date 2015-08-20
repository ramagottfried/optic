#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>

#include <pcl/segmentation/extract_clusters.h>

#include "jit.common.h"

// Our Jitter object instance data
typedef struct _jit_pcl_segeuclidean {
    t_object	ob;
    double		leafsize;
    long        npoints;
    double      stdthresh;
    
    double      cluster_tol;
    
} t_jit_pcl_segeuclidean;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err                   jit_pcl_segeuclidean_init				(void);
t_jit_pcl_segeuclidean       *jit_pcl_segeuclidean_new				(void);
void                        jit_pcl_segeuclidean_free				(t_jit_pcl_segeuclidean *x);
t_jit_err                   jit_pcl_segeuclidean_matrix_calc		(t_jit_pcl_segeuclidean *x, void *inputs, void *outputs);
void                        jit_pcl_segeuclidean_calculate_ndim	(t_jit_pcl_segeuclidean *x, long dim, long *dimsize, long planecount, t_jit_matrix_info *in_minfo,char *bip, t_jit_matrix_info *out_minfo, char *bop);
END_USING_C_LINKAGE

pcl::PointCloud<pcl::Normal>::Ptr jit_pcl_segeuclidean_normals( pcl::PointCloud<pcl::PointXYZ>::Ptr cloud );
pcl::PointCloud<pcl::PointXYZ>::Ptr jit_pcl_segeuclidean_genTriangles( pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals );


// globals
static void *s_jit_pcl_segeuclidean_class = NULL;


/************************************************************************************/

t_jit_err jit_pcl_segeuclidean_init(void)
{
    long			attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
    t_jit_object	*attr;
    t_jit_object	*mop;
    t_atom a[1];
    
    s_jit_pcl_segeuclidean_class = jit_class_new("jit_pcl_segeuclidean", (method)jit_pcl_segeuclidean_new, (method)jit_pcl_segeuclidean_free, sizeof(t_jit_pcl_segeuclidean), 0);
    
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
    
    jit_class_addadornment(s_jit_pcl_segeuclidean_class, mop);
    
    
    
    // add method(s)
    jit_class_addmethod(s_jit_pcl_segeuclidean_class, (method)jit_pcl_segeuclidean_matrix_calc, "matrix_calc", A_CANT, 0);
    
    // add attribute(s)
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "leafsize",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_segeuclidean, leafsize));
    
    jit_class_addattr(s_jit_pcl_segeuclidean_class, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "cluster_tol",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_segeuclidean, cluster_tol));
    
    jit_class_addattr(s_jit_pcl_segeuclidean_class, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "npoints",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_segeuclidean, npoints));
    jit_attr_addfilterget_clip(attr, 1, 10000, true, false);
    jit_class_addattr(s_jit_pcl_segeuclidean_class, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "stdthresh",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_pcl_segeuclidean, stdthresh));
    
    jit_class_addattr(s_jit_pcl_segeuclidean_class, attr);

    
    // finalize class
    jit_class_register(s_jit_pcl_segeuclidean_class);
    return JIT_ERR_NONE;
}

/************************************************************************************/
// Object Life Cycle

t_jit_pcl_segeuclidean *jit_pcl_segeuclidean_new(void)
{
    t_jit_pcl_segeuclidean	*x = NULL;
    
    x = (t_jit_pcl_segeuclidean*)jit_object_alloc(s_jit_pcl_segeuclidean_class);
    if (x) {
        x->leafsize = 0.02;
        x->npoints = 50;
        x->stdthresh = 1.;
        x->cluster_tol = 0.5;
    }
    return x;
}


void jit_pcl_segeuclidean_free(t_jit_pcl_segeuclidean *x)
{
    ;	// nothing to free for our simple object
}


// PCL

// conversions

t_jit_err jit_xyzrgb2jit(t_jit_pcl_segeuclidean *x, pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud, t_jit_matrix_info *out_minfo, void **out_matrix)
{
    char *out_bp = NULL;
    float *fop;
    
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
                fop[3] = (float)cloud->points[count].r * scalar;
                fop[4] = (float)cloud->points[count].g * scalar;
                fop[5] = (float)cloud->points[count].b * scalar;
                
            }
            count++;
            fop += rowstride;
        }
    }
    return JIT_ERR_NONE;
    
}

t_jit_err jit_cloud2jit(t_jit_pcl_segeuclidean *x, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, t_jit_matrix_info *out_minfo, void **out_matrix)
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
t_jit_err jit_pcl_segeuclidean_matrix_calc(t_jit_pcl_segeuclidean *x, void *inputs, void *outputs)
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
    
    long rowstride;
    float *fip, *fop;
    
    
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
        
        if( planecount < 3 )
        {
            object_error((t_object *)x, "jit.pcl requires a 3 plane matrix (xyz)");
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
        
        //******
        // PCL stuff
        
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
        cloud->width    = (uint32_t)dim[0];
        cloud->height   = (uint32_t)dim[1];
        cloud->is_dense = false;
        cloud->points.resize (cloud->width * cloud->height);
        
        rowstride = in_minfo.dimstride[1];// >> 2L;
        size_t count = 0;
        
        for (j = 0; j < dim[0]; j++)
        {
            fip =  (float *)(in_bp + j * in_minfo.dimstride[0]);
            
            for( i = 0; i < dim[1]; i++)
            {
                cloud->points[count].x = ((float *)fip)[0];
                cloud->points[count].y = ((float *)fip)[1];
                cloud->points[count].z = ((float *)fip)[2];
        //        cloud->points[count].r = (uint8_t)(((float *)fip)[3] * 255.0);
        //        cloud->points[count].g = (uint8_t)(((float *)fip)[4] * 255.0);
        //        cloud->points[count].b = (uint8_t)(((float *)fip)[5] * 255.0);
                count++;
                fip += rowstride;
            }
        }
        
      
        {

            /*
            //filter
            pcl::VoxelGrid<pcl::PointXYZRGB> grid;
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_voxel_ (new pcl::PointCloud<pcl::PointXYZRGB>);
            
            grid.setLeafSize (x->leafsize, x->leafsize, x->leafsize);
            grid.setInputCloud (cloud);
            grid.filter (*cloud_voxel_);
            
            
            pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor;
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_sor_ (new pcl::PointCloud<pcl::PointXYZRGB>);
            
            sor.setInputCloud(cloud_voxel_);
            sor.setMeanK( (uint32_t)x->npoints );
            sor.setStddevMulThresh(x->stdthresh);
            sor.filter(*cloud_sor_);
            */
            
            pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
            kdtree->setInputCloud(cloud);
            
            // Euclidean clustering object.
            pcl::EuclideanClusterExtraction<pcl::PointXYZ> clustering;
            // Set cluster tolerance to 2cm (small values may cause objects to be divided
            // in several clusters, whereas big values may join objects in a same cluster).
            clustering.setClusterTolerance(x->cluster_tol);
            // Set the minimum and maximum number of points that a cluster can have.
            clustering.setMinClusterSize(10);
            clustering.setMaxClusterSize(25000);
            clustering.setSearchMethod(kdtree);
            clustering.setInputCloud(cloud);
            
            std::vector<pcl::PointIndices> clusters;
            clustering.extract(clusters);
            // For every cluster...
            
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZRGB>);
            cluster->points.resize(cloud->size());
            cluster->width = (uint32_t)cloud->points.size();
            cluster->height = 1;
            cluster->is_dense = true;


            if( clusters.size() > 0 )
            {
                double segment_inc = 255. / clusters.size();
          
                int count = 0;
                int clusterN = 0;
                for (std::vector<pcl::PointIndices>::const_iterator i = clusters.begin(); i != clusters.end(); ++i)
                {

                    for (std::vector<int>::const_iterator p = i->indices.begin(); p != i->indices.end(); ++p)
                    {

                        cluster->points[count].x = cloud->points[ *p ].x;
                        cluster->points[count].y = cloud->points[ *p ].y;
                        cluster->points[count].z = cloud->points[ *p ].z;

                        cluster->points[count].r = (uint8_t)(segment_inc * clusterN);
                        cluster->points[count].g = (uint8_t)(segment_inc * clusterN);
                        cluster->points[count].b = 255;

                        count++;
                    }
                    clusterN++;
                }
            }
            err = jit_xyzrgb2jit(x, cluster, &out_minfo, &out_matrix );
            if( err != JIT_ERR_NONE )
                goto out;
            
        }
      
        // unable to make use of jitter's parallel methods since we need all the data together
        //jit_parallel_ndim_simplecalc2((method)jit_pcl_segeuclidean_calculate_ndim,
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
