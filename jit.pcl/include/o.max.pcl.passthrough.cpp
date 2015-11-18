
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/impl/instantiate.hpp>
#include "o.max.pcl.passthrough.h"


template <typename PointT>
const char * const OPCL_passthrough<PointT>::m_selectors[] = { "/passthrough/profile", "/passthrough/z_min", "/passthrough/z_max" };


template <typename PointT>
void OPCL_passthrough<PointT>::FullPacket(long len, char *ptr, t_osc_bundle_u **dst )
{
    for (int i = 0; i < m_nselectors; i++)
    {
        double val = 0;
        t_osc_msg_ar_s *ar = osc_bundle_s_lookupAddress(len, ptr, m_selectors[i], 1);
        if( OPCL_passthrough::getDouble(ar, &val) )
        {
            switch (i)
            {
                case (int)OSC_API::Z_MIN:
                    OPCL_passthrough::setZmin( val );
                    OPCL_passthrough::set_state_bundle_double( (char *)m_selectors[i], val );
                    break;
                    
                case (int)OSC_API::Z_MAX:
                    OPCL_passthrough::setZmax( val );
                    OPCL_passthrough::set_state_bundle_double( (char *)m_selectors[i], val );
                    break;
                    
                case (int)OSC_API::PROFILE:
                {
                    bool t = val > 0.0;
                    OPCL_passthrough::setProfiling( t );
                    OPCL_passthrough::set_state_bundle_bool( (char *)m_selectors[i], t );
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // pass bundle to possible subclasses here
    
    
    OPCL_passthrough::getState( dst );
    
}


template <typename PointT>
void OPCL_passthrough<PointT>::calc(    typename pcl::PointCloud<PointT>::Ptr cloud,
                                        typename pcl::PointCloud<PointT>::Ptr *passthrough_dst,
                                        t_osc_bndl_u **bndl )
{
    t_osc_timetag start_time, dur;
    if( m_profile )
    {
        start_time = osc_timetag_now();
    }
    
    typename pcl::PointCloud<PointT>::Ptr filteredCloud(new pcl::PointCloud<PointT>);
    pcl::PassThrough<PointT> pass_filter;
    pass_filter.setInputCloud( cloud );
    
    // Filter out all points with Z values not in the [0-2] range.
    pass_filter.setFilterFieldName( "z" );
    pass_filter.setFilterLimits( m_z_min, m_z_max );
    pass_filter.filter( *filteredCloud );
    
    *passthrough_dst = filteredCloud;
    
    if( m_profile && *bndl )
    {
        dur = osc_timetag_subtract( osc_timetag_now(), start_time );
        
        t_osc_bndl_u *time_bndl = osc_bundle_u_alloc();
        t_osc_msg_u *tt_msg = osc_message_u_allocWithFloat( (char *)"/passthrough/elapsed_time", osc_timetag_timetagToFloat(dur) );
        osc_bundle_u_addMsg(time_bndl, tt_msg );
        
        OPCL_passthrough::unionState( time_bndl, bndl );
        
        if( time_bndl )
            osc_bundle_u_free( time_bndl );
        
    }
}

#define PCL_INSTANTIATE_OPCL_passthrough(T) template class PCL_EXPORTS OPCL_passthrough<T>;

PCL_INSTANTIATE(OPCL_passthrough, PCL_XYZ_POINT_TYPES);
