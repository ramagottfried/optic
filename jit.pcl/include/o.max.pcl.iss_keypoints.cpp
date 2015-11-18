
// NOT SETUP YET


#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/impl/instantiate.hpp>
#include "o.max.pcl.iss_keypoints.h"


template <typename PointT>
const char * const OPCL_iss_3d<PointT>::m_selectors[] = { "/iss_3d/profile", "/iss_3d/leafsize"};


template <typename PointT>
void OPCL_iss_3d<PointT>::FullPacket(long len, char *ptr, t_osc_bundle_u **dst )
{
    for (int i = 0; i < m_nselectors; i++)
    {
        double val = 0;
        t_osc_msg_ar_s *ar = osc_bundle_s_lookupAddress(len, ptr, m_selectors[i], 1);
        if( OPCL_iss_3d::getDouble(ar, &val) )
        {
            switch (i)
            {
                case (int)OSC_API::LEAFSIZE:
                    OPCL_iss_3d::setLeafsize( val );
                    OPCL_iss_3d::set_state_bundle_double( (char *)m_selectors[i], val );
                    break;

                case (int)OSC_API::PROFILE:
                {
                    bool t = val > 0.0;
                    OPCL_iss_3d::setProfiling( t );
                    OPCL_iss_3d::set_state_bundle_bool( (char *)m_selectors[i], t );
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // pass bundle to possible subclasses here
    
    
    OPCL_iss_3d::getState( dst );
    
}

template <typename PointT>
double OPCL_iss_3d<PointT>::computeCloudResolution(const typename pcl::PointCloud<PointT>::ConstPtr& cloud)
{
    double resolution = 0.0;
    int numberOfPoints = 0;
    ssize_t nres;
    std::vector<int> indices(2);
    std::vector<float> squaredDistances(2);
    pcl::search::KdTree<PointT> tree;
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


template <typename PointT>
void OPCL_iss_3d<PointT>::calc(     typename pcl::PointCloud<PointT>::Ptr cloud,
                                    typename pcl::PointCloud<PointT>::Ptr *iss_3d_dst,
                                    t_osc_bndl_u **bndl )
{
    t_osc_timetag start_time, dur;
    if( m_profile )
    {
        start_time = osc_timetag_now();
    }
    
    
    {
        typename pcl::PointCloud<PointT>::Ptr keypoints(new pcl::PointCloud<PointT>);
        pcl::ISSKeypoint3D<PointT, PointT> detector;
        detector.setInputCloud( cloud );
        
        typename pcl::search::KdTree<PointT>::Ptr kdtree(new pcl::search::KdTree<PointT>);
        detector.setSearchMethod(kdtree);
        
        double resolution = OPCL_iss_3d<PointT>::computeCloudResolution( cloud );
        /*
        t_osc_message_u *res = osc_message_u_allocWithAddress((char *)"/resolusion");
        osc_message_u_appendDouble(res, resolution);
        osc_bundle_u_addMsg(bndl, res);
        */
        
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
        
        *iss_3d_dst = keypoints;
    }
    
    
    if( m_profile && *bndl )
    {
        dur = osc_timetag_subtract( osc_timetag_now(), start_time );
        
        t_osc_bndl_u *time_bndl = osc_bundle_u_alloc();
        t_osc_msg_u *tt_msg = osc_message_u_allocWithFloat( (char *)"/iss_3d/elapsed_time", osc_timetag_timetagToFloat(dur) );
        osc_bundle_u_addMsg(time_bndl, tt_msg );
        
        OPCL_iss_3d::unionState( time_bndl, bndl );
        
        if( time_bndl )
            osc_bundle_u_free( time_bndl );
        
    }
}

#define PCL_INSTANTIATE_OPCL_iss_3d(T) template class PCL_EXPORTS OPCL_iss_3d<T>;

PCL_INSTANTIATE(OPCL_iss_3d, PCL_XYZ_POINT_TYPES);
