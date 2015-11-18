
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/impl/instantiate.hpp>
#include "o.max.pcl.voxel_grid.h"


template <typename PointT>
const char * const OPCL_voxel_grid<PointT>::m_selectors[] = { "/voxel_grid/profile", "/voxel_grid/leafsize"};


template <typename PointT>
void OPCL_voxel_grid<PointT>::FullPacket(long len, char *ptr, t_osc_bundle_u **dst )
{
    for (int i = 0; i < m_nselectors; i++)
    {
        double val = 0;
        t_osc_msg_ar_s *ar = osc_bundle_s_lookupAddress(len, ptr, m_selectors[i], 1);
        if( OPCL_voxel_grid::getDouble(ar, &val) )
        {
            switch (i)
            {
                case (int)OSC_API::LEAFSIZE:
                    OPCL_voxel_grid::setLeafsize( val );
                    OPCL_voxel_grid::set_state_bundle_double( (char *)m_selectors[i], val );
                    break;

                case (int)OSC_API::PROFILE:
                {
                    bool t = val > 0.0;
                    OPCL_voxel_grid::setProfiling( t );
                    OPCL_voxel_grid::set_state_bundle_bool( (char *)m_selectors[i], t );
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // pass bundle to possible subclasses here
    
    
    OPCL_voxel_grid::getState( dst );
    
}


template <typename PointT>
void OPCL_voxel_grid<PointT>::calc(    typename pcl::PointCloud<PointT>::Ptr cloud,
                                        typename pcl::PointCloud<PointT>::Ptr *voxel_grid_dst,
                                        t_osc_bndl_u **bndl )
{
    t_osc_timetag start_time, dur;
    if( m_profile )
    {
        start_time = osc_timetag_now();
    }
    
    
    {
        typename pcl::PointCloud<PointT>::Ptr cloud_voxel_( new pcl::PointCloud<PointT> );

        pcl::VoxelGrid<PointT> grid;
        grid.setLeafSize ( m_leafsize, m_leafsize, m_leafsize);
        grid.setInputCloud ( cloud );
        grid.filter ( *cloud_voxel_ );
        
        *voxel_grid_dst = cloud_voxel_;
    }
    
    
    if( m_profile && *bndl )
    {
        dur = osc_timetag_subtract( osc_timetag_now(), start_time );
        
        t_osc_bndl_u *time_bndl = osc_bundle_u_alloc();
        t_osc_msg_u *tt_msg = osc_message_u_allocWithFloat( (char *)"/voxel_grid/elapsed_time", osc_timetag_timetagToFloat(dur) );
        osc_bundle_u_addMsg(time_bndl, tt_msg );
        
        OPCL_voxel_grid::unionState( time_bndl, bndl );
        
        if( time_bndl )
            osc_bundle_u_free( time_bndl );
        
    }
}

#define PCL_INSTANTIATE_OPCL_voxel_grid(T) template class PCL_EXPORTS OPCL_voxel_grid<T>;

PCL_INSTANTIATE(OPCL_voxel_grid, PCL_XYZ_POINT_TYPES);
