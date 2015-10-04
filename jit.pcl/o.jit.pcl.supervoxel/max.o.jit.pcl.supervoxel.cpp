
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>
#include <pcl/segmentation/supervoxel_clustering.h>
#include <pcl/segmentation/lccp_segmentation.h>

//#include <pcl/filters/passthrough.h>
#include <pcl/surface/convex_hull.h>

#include "osc.h"
#include "osc_bundle_u.h"
#include "osc_bundle_s.h"
#include "osc_timetag.h"
#include "omax_util.h"

#include "jit.common.h"
#include "max.jit.mop.h"

typedef struct _o_jit_pcl_supervoxel
{
	t_object	ob;
    void		*obex;
    void        *outlet;
    t_critical  lock;

    int         disable_transform;
    double      voxel_resolution;
    double      seed_resolution;
    double      color_importance;
    double      spatial_importance;
    double      normal_importance;
    
} t_o_jit_pcl_supervoxel;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err	jit_pcl_supervoxel_init(void);
void		*o_jit_pcl_supervoxel_new(t_symbol *s, long argc, t_atom *argv);
void		o_jit_pcl_supervoxel_free(t_o_jit_pcl_supervoxel *x);
t_jit_err   o_jit_pcl_supervoxel_matrix_calc(t_o_jit_pcl_supervoxel *x, t_symbol *s, long argc, t_atom *argv);

END_USING_C_LINKAGE

// globals
static void	*o_jit_pcl_supervoxel_class = NULL;

t_jit_err jit_xyzrgb2jit(t_o_jit_pcl_supervoxel *x, pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud, t_jit_matrix_info *out_minfo, void **out_matrix)
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

t_jit_err   o_jit_pcl_supervoxel_matrix_calc(t_o_jit_pcl_supervoxel *x, t_symbol *s, long argc, t_atom *argv)
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
        pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
        cloud->width    = (uint32_t)dim[0];
        cloud->height   = (uint32_t)dim[1];
        cloud->points.resize (cloud->width * cloud->height);
        
        size_t count = 0;
        float _x, _y, _z;
        uint8_t _r, _g, _b;
        const float bad_point = std::numeric_limits<float>::quiet_NaN();
        for (j = 0; j < dim[0]; ++j)
        {
            fip = in_bp + j * in_minfo.dimstride[0];
            
            for( i = 0; i < dim[1]; ++i)
            {
                if(count < cloud->points.size())
                {
                    _x = ((float *)fip)[0];
                    _y = ((float *)fip)[1];
                    _z = ((float *)fip)[2];
                    _r = (uint8_t)(((float *)fip)[3] * 255.0);
                    _g = (uint8_t)(((float *)fip)[4] * 255.0);
                    _b = (uint8_t)(((float *)fip)[5] * 255.0);
                    if( !_x && !_y && !_z && !_r && !_g && !_b )
                    {
                        cloud->points[count].x = bad_point;
                        cloud->points[count].y = bad_point;
                        cloud->points[count].z = bad_point;
                        cloud->points[count].r = bad_point;
                        cloud->points[count].g = bad_point;
                        cloud->points[count].b = bad_point;
                        cloud->points[count].a = bad_point;
                    }
                    else
                    {
                        cloud->points[count].x = _x;
                        cloud->points[count].y = _y;
                        cloud->points[count].z = _z;
                        cloud->points[count].r = _r;
                        cloud->points[count].g = _g;
                        cloud->points[count].b = _b;
                        cloud->points[count].a = 255;
                    }
                }
                count++;
                fip += in_minfo.dimstride[1];
            }
        }
        
        {
            typedef pcl::PointXYZRGBA PointT;
            
            pcl::SupervoxelClustering<PointT> super (x->voxel_resolution, x->seed_resolution);
            
            float concavity_tolerance_threshold = 10;
            float smoothness_threshold = 0.1;
            uint32_t min_segment_size = 0;
            bool use_extended_convexity = false;
            bool use_sanity_criterion = false;
            
            if (x->disable_transform)
                super.setUseSingleCameraTransform (false);
            
            super.setInputCloud ( cloud );
            super.setColorImportance (x->color_importance);
            super.setSpatialImportance (x->spatial_importance);
            super.setNormalImportance (x->normal_importance);
            if( cloud->height > 1)
            {
                    super.setUseSingleCameraTransform (false);
            }
            
            std::map <uint32_t, pcl::Supervoxel<PointT>::Ptr > supervoxel_clusters;
            super.extract (supervoxel_clusters);
            
            
            pcl::PointCloud<PointT>::Ptr voxel_centroid_cloud = super.getVoxelCentroidCloud ();
            pcl::PointCloud<pcl::PointXYZL>::Ptr labeled_voxel_cloud = super.getLabeledVoxelCloud ();
            pcl::PointCloud<pcl::PointNormal>::Ptr sv_normal_cloud = super.makeSupervoxelNormalCloud (supervoxel_clusters);
            
            std::multimap<uint32_t, uint32_t> supervoxel_adjacency;
            super.getSupervoxelAdjacency (supervoxel_adjacency);
            
            
            // LCCP
            pcl::LCCPSegmentation<PointT> lccp;
            lccp.setConcavityToleranceThreshold (concavity_tolerance_threshold);
            lccp.setSanityCheck (use_sanity_criterion);
            lccp.setSmoothnessCheck (true, x->voxel_resolution, x->seed_resolution, smoothness_threshold);
            
            uint k_factor = 0;
            if (use_extended_convexity)
                k_factor = 1;
            
            lccp.setKFactor (k_factor);
            lccp.segment (supervoxel_clusters, supervoxel_adjacency);
            
            if (min_segment_size > 0)
            {
                post ("Merging small segments\n");
                lccp.mergeSmallSegments (min_segment_size);
            }
            
            pcl::PointCloud<pcl::PointXYZL>::Ptr sv_labeled_cloud = super.getLabeledCloud ();
            pcl::PointCloud<pcl::PointXYZL>::Ptr lccp_labeled_cloud = sv_labeled_cloud->makeShared ();
            lccp.relabelCloud (*lccp_labeled_cloud);
            
/*
            typedef pcl::LCCPSegmentation<PointT>::SupervoxelAdjacencyList SuperVoxelAdjacencyList;
            typedef pcl::LCCPSegmentation<PointT>::VertexIterator VertexIterator;
            typedef pcl::LCCPSegmentation<PointT>::AdjacencyIterator AdjacencyIterator;
            typedef pcl::LCCPSegmentation<PointT>::EdgeID EdgeID;
            
            SuperVoxelAdjacencyList sv_adjacency_list;
            lccp.getSVAdjacencyList (sv_adjacency_list);
*/

            std::map<uint32_t, std::set<uint32_t> > segment_supervoxel_map;
            lccp.getSegmentSupervoxelMap( segment_supervoxel_map );
            
            std::map<uint32_t, std::set<uint32_t> >::iterator map_iter;
            
            t_osc_bndl_u *bndl = osc_bundle_u_alloc();
            char buf[2048];
            ssize_t count = 0;
            for( map_iter = segment_supervoxel_map.begin(); map_iter != segment_supervoxel_map.end(); ++map_iter)
            {
                uint32_t group_label = map_iter->first;
//                ssize_t ngroup_voxel_pts = map_iter->second.size();
                
                std::set<uint32_t>::iterator pt_id_iter;
                for( pt_id_iter = map_iter->second.begin(); pt_id_iter != map_iter->second.end(); ++pt_id_iter)
                {
                    sprintf(buf, "/supervoxel/pt/%ld", ++count);
                    
                    t_osc_msg_u *supervoxel_group = osc_message_u_allocWithAddress(buf);
                    t_osc_bndl_u *subbndl = osc_bundle_u_alloc();
                    
                    t_osc_msg_u *pt_num = osc_message_u_allocWithAddress((char *)"/pt_id");
                    osc_message_u_appendInt64(pt_num, count);
                    osc_bundle_u_addMsg(subbndl, pt_num);
                    
                    t_osc_msg_u *segment_num = osc_message_u_allocWithAddress((char *)"/group_id");
                    osc_message_u_appendInt32(segment_num, group_label);
                    osc_bundle_u_addMsg(subbndl, segment_num);
                    
                    pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr supervoxel = supervoxel_clusters.at( *pt_id_iter );
                    
                    t_osc_msg_u *centroid = osc_message_u_allocWithAddress((char *)"/centroid/xyz");
                    osc_message_u_appendFloat(centroid, supervoxel->centroid_.x);
                    osc_message_u_appendFloat(centroid, supervoxel->centroid_.y);
                    osc_message_u_appendFloat(centroid, supervoxel->centroid_.z);
                    osc_bundle_u_addMsg(subbndl, centroid);
                    
                    t_osc_msg_u *centroid_color = osc_message_u_allocWithAddress((char *)"/centroid/rgb");
                    osc_message_u_appendFloat(centroid_color, supervoxel->centroid_.r);
                    osc_message_u_appendFloat(centroid_color, supervoxel->centroid_.g);
                    osc_message_u_appendFloat(centroid_color, supervoxel->centroid_.b);
                    osc_bundle_u_addMsg(subbndl, centroid_color);
                    
                    pcl::PointCloud<pcl::PointXYZRGBA> adjacent_supervoxel_centers;
                    
                    t_osc_msg_u *adjx = osc_message_u_allocWithAddress((char *)"/adjacent/x");
                    t_osc_msg_u *adjy = osc_message_u_allocWithAddress((char *)"/adjacent/y");
                    t_osc_msg_u *adjz = osc_message_u_allocWithAddress((char *)"/adjacent/z");
                    
                    std::multimap<uint32_t,uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(*pt_id_iter).first;
                    for ( ; adjacent_itr!=supervoxel_adjacency.equal_range(*pt_id_iter).second; ++adjacent_itr)
                    {
                        pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr neighbor_supervoxel = supervoxel_clusters.at(adjacent_itr->second);
                        osc_message_u_appendFloat(adjx, neighbor_supervoxel->centroid_.x);
                        osc_message_u_appendFloat(adjy, neighbor_supervoxel->centroid_.y);
                        osc_message_u_appendFloat(adjz, neighbor_supervoxel->centroid_.z);
                        
                        // line from supervoxel->centroid_ to each adjacent_supervoxel_centers
                        
                    }
                    
                    osc_bundle_u_addMsg(subbndl, adjx);
                    osc_bundle_u_addMsg(subbndl, adjy);
                    osc_bundle_u_addMsg(subbndl, adjz);
                    
                    osc_message_u_appendBndl_u(supervoxel_group, subbndl);
                    osc_bundle_u_addMsg(bndl, supervoxel_group);

                }
                
                

//                post("%d %d", group_label, ngroup_voxel_pts);
            }

            
//            for( int i = 0; i < lccp_labeled_cloud->points.size(); i++ )
//                post("%d %d", lccp_labeled_cloud->points.size(), lccp_labeled_cloud->points[i].label);
            
 /*

            
            std::set<EdgeID> edge_drawn;
            
                     //The vertices in the supervoxel adjacency list are the supervoxel centroids
            //This iterates through them, finding the edges
            std::pair<VertexIterator, VertexIterator> vertex_iterator_range;
            vertex_iterator_range = boost::vertices (sv_adjacency_list);
            

            t_osc_bndl_u *bndl = osc_bundle_u_alloc();
            char buf[2048];
            
            
            for (VertexIterator itr = vertex_iterator_range.first; itr != vertex_iterator_range.second; ++itr)
            {
                const uint32_t sv_label = sv_adjacency_list[*itr];
                pcl::Supervoxel<PointT>::Ptr supervoxel = supervoxel_clusters.at (sv_label);
                pcl::PointXYZRGBA vert_curr = supervoxel->centroid_;
//                pcl::PointXYZL lccp_pt = lccp_labeled_cloud->at ( sv_label );
                const uint32_t group_label = lccp_labeled_cloud->at ( sv_label ).label;

                //                const uint32_t group_label = lccp_labeled_cloud->points[ sv_label ].label;

                
                sprintf(buf, "/supervoxel/%d/%d", group_label, sv_label);
                post("%s", buf);
                t_osc_msg_u *supervoxel_group = osc_message_u_allocWithAddress(buf);
                t_osc_bndl_u *subbndl = osc_bundle_u_alloc();

                t_osc_msg_u *centroid = osc_message_u_allocWithAddress((char *)"/centroid/xyz");
                osc_message_u_appendFloat(centroid, vert_curr.x);
                osc_message_u_appendFloat(centroid, vert_curr.y);
                osc_message_u_appendFloat(centroid, vert_curr.z);
                osc_bundle_u_addMsg(subbndl, centroid);
                
                t_osc_msg_u *centroid_color = osc_message_u_allocWithAddress((char *)"/centroid/rgb");
                osc_message_u_appendFloat(centroid_color, vert_curr.r);
                osc_message_u_appendFloat(centroid_color, vert_curr.g);
                osc_message_u_appendFloat(centroid_color, vert_curr.b);
                osc_bundle_u_addMsg(subbndl, centroid_color);
                
                t_osc_msg_u *adjx = osc_message_u_allocWithAddress((char *)"/adjacent/x");
                t_osc_msg_u *adjy = osc_message_u_allocWithAddress((char *)"/adjacent/y");
                t_osc_msg_u *adjz = osc_message_u_allocWithAddress((char *)"/adjacent/z");
                
                t_osc_msg_u *is_conv = osc_message_u_allocWithAddress((char *)"/vertex/convex");
                
                std::pair<AdjacencyIterator, AdjacencyIterator> neighbors = boost::adjacent_vertices (*itr, sv_adjacency_list);
                
                for (AdjacencyIterator itr_neighbor = neighbors.first; itr_neighbor != neighbors.second; ++itr_neighbor)
                {
                    EdgeID connecting_edge = boost::edge (*itr, *itr_neighbor, sv_adjacency_list).first;  //Get the edge
                    osc_message_u_appendBool(is_conv, sv_adjacency_list[connecting_edge].is_convex);

                    const uint32_t sv_neighbor_label = sv_adjacency_list[*itr_neighbor];
                    pcl::Supervoxel<PointT>::Ptr supervoxel_neigh = supervoxel_clusters.at (sv_neighbor_label);
                    pcl::PointXYZRGBA vert_neigh = supervoxel_neigh->centroid_;
                    
                    osc_message_u_appendFloat(adjx, vert_neigh.x);
                    osc_message_u_appendFloat(adjy, vert_neigh.y);
                    osc_message_u_appendFloat(adjz, vert_neigh.z);

                    
                }
                
                osc_bundle_u_addMsg(subbndl, adjx);
                osc_bundle_u_addMsg(subbndl, adjy);
                osc_bundle_u_addMsg(subbndl, adjz);
                osc_bundle_u_addMsg(subbndl, is_conv);

                
                osc_message_u_appendBndl_u(supervoxel_group, subbndl);
                osc_bundle_u_addMsg(bndl, supervoxel_group);
            }
            post("--------------");
*/
            
/*
            //To make a graph of the supervoxel adjacency, we need to iterate through the supervoxel adjacency multimap
            std::multimap<uint32_t,uint32_t>::iterator label_itr = supervoxel_adjacency.begin ();
            for ( ; label_itr != supervoxel_adjacency.end (); )
            {
                //First get the label
                uint32_t supervoxel_label = label_itr->first;
                pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr supervoxel = supervoxel_clusters.at( supervoxel_label );

                sprintf(buf, "/supervoxel/%d", supervoxel_label);
                t_osc_msg_u *supervoxel_group = osc_message_u_allocWithAddress(buf);
                
                t_osc_bndl_u *subbndl = osc_bundle_u_alloc();

                t_osc_msg_u *centroid = osc_message_u_allocWithAddress((char *)"/centroid/xyz");
                osc_message_u_appendFloat(centroid, supervoxel->centroid_.x);
                osc_message_u_appendFloat(centroid, supervoxel->centroid_.y);
                osc_message_u_appendFloat(centroid, supervoxel->centroid_.z);
                osc_bundle_u_addMsg(subbndl, centroid);
                
                t_osc_msg_u *centroid_color = osc_message_u_allocWithAddress((char *)"/centroid/rgb");
                osc_message_u_appendFloat(centroid_color, supervoxel->centroid_.r);
                osc_message_u_appendFloat(centroid_color, supervoxel->centroid_.g);
                osc_message_u_appendFloat(centroid_color, supervoxel->centroid_.b);
                osc_bundle_u_addMsg(subbndl, centroid_color);
                
                
                pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_hull_ (new pcl::PointCloud<pcl::PointXYZRGBA>);
                std::vector<pcl::Vertices> vertices_;

                //get convex hull of supervoxel region:

                pcl::ConvexHull<pcl::PointXYZRGBA> hr;
                hr.setDimension(3);
                hr.setInputCloud(supervoxel->voxels_);
                //            cloud_hull_.reset (new Cloud); << save on reallocating memory later?
                hr.reconstruct (*cloud_hull_, vertices_);

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
                
                //Now we need to iterate through the adjacent supervoxels and make a point cloud of them
//                pcl::PointCloud<pcl::PointXYZRGBA> adjacent_supervoxel_centers;
  
                t_osc_msg_u *adjx = osc_message_u_allocWithAddress((char *)"/adjacent/x");
                t_osc_msg_u *adjy = osc_message_u_allocWithAddress((char *)"/adjacent/y");
                t_osc_msg_u *adjz = osc_message_u_allocWithAddress((char *)"/adjacent/z");
                
                std::multimap<uint32_t,uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(supervoxel_label).first;
                for ( ; adjacent_itr!=supervoxel_adjacency.equal_range(supervoxel_label).second; ++adjacent_itr)
                {
                    pcl::Supervoxel<pcl::PointXYZRGBA>::Ptr neighbor_supervoxel = supervoxel_clusters.at(adjacent_itr->second);
                    osc_message_u_appendFloat(adjx, neighbor_supervoxel->centroid_.x);
                    osc_message_u_appendFloat(adjy, neighbor_supervoxel->centroid_.y);
                    osc_message_u_appendFloat(adjz, neighbor_supervoxel->centroid_.z);
                    
                    // line from supervoxel->centroid_ to each adjacent_supervoxel_centers
                    
                }
                
                osc_bundle_u_addMsg(subbndl, adjx);
                osc_bundle_u_addMsg(subbndl, adjy);
                osc_bundle_u_addMsg(subbndl, adjz);
                
                osc_message_u_appendBndl_u(supervoxel_group, subbndl);
                osc_bundle_u_addMsg(bndl, supervoxel_group);
                
                //Move iterator forward to next label
                label_itr = supervoxel_adjacency.upper_bound (supervoxel_label);
            }
*/
            omax_util_outletOSC_u(x->outlet, bndl);
            
            if(bndl)
                osc_bundle_u_free(bndl);

        }

    }
out:
    jit_object_method(matrix, _jit_sym_lock, in_savelock);
    return err;
}

void o_jit_pcl_supervoxel_free(t_o_jit_pcl_supervoxel *x)
{
    max_jit_mop_free(x);
    jit_object_free(max_jit_obex_jitob_get(x));
    max_jit_object_free(x);
}

void *o_jit_pcl_supervoxel_new(t_symbol *s, long argc, t_atom *argv)
{
    t_o_jit_pcl_supervoxel	*x;
    
    x = (t_o_jit_pcl_supervoxel *)max_jit_object_alloc((t_class *)o_jit_pcl_supervoxel_class, NULL);
    if (x)
    {
        x->disable_transform = 1;
        x->voxel_resolution = 0.008f;
        x->seed_resolution = 0.1f;
        x->color_importance = 0.2f;
        x->spatial_importance = 0.4f;
        x->normal_importance = 1.0f;

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

	c = class_new("o.jit.pcl.supervoxel", (method)o_jit_pcl_supervoxel_new, (method)o_jit_pcl_supervoxel_free, sizeof(t_o_jit_pcl_supervoxel), NULL, A_GIMME, 0);
	max_jit_class_obex_setup(c, calcoffset(t_o_jit_pcl_supervoxel, obex));
    
    class_addmethod(c, (method)o_jit_pcl_supervoxel_matrix_calc, "jit_matrix", A_GIMME, 0);
	class_addmethod(c, (method)max_jit_mop_assist, "assist", A_CANT, 0);	// standard matrix-operator (mop) assist fn

    attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;

    // add attribute(s)
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "disable_transform",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_supervoxel, disable_transform));
    jit_attr_addfilterset_clip(attr, 0, 1, true, true);
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "voxel_resolution",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_supervoxel, voxel_resolution));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "seed_resolution",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_supervoxel, seed_resolution));
    
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "color_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_supervoxel, color_importance));
    jit_class_addattr(c, attr);
    
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "spatial_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_supervoxel, spatial_importance));
    jit_class_addattr(c, attr);
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "normal_importance",
                                          _jit_sym_float64,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_o_jit_pcl_supervoxel, normal_importance));
    jit_class_addattr(c, attr);
    
	class_register(CLASS_BOX, c);
	o_jit_pcl_supervoxel_class = c;
	return 0;
}
END_USING_C_LINKAGE


