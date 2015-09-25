#ifndef jit_pcl_o_max_pcl_passthrough_hpp
#define jit_pcl_o_max_pcl_passthrough_hpp

#include "o.max.pcl.passthrough.h"

template <typename PointT>
const char * const OPCL_passthrough<PointT>::m_selectors[] = {
    "/passthrough/profile",
    "/passthrough/depthFactor",
    "/passthrough/smoothingSize" };

template <typename PointT>
t_osc_bundle_u *OPCL_passthrough<PointT>::FullPacket(long len, char *ptr )
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
    
    
    return OPCL_passthrough::getState();
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
    pass_filter.setFilterFieldName("z");
    pass_filter.setFilterLimits( m_z_min, m_z_max );
    pass_filter.filter(*filteredCloud);
    
    *passthrough_dst = filteredCloud;
    
    if( m_profile && *bndl )
    {
        dur = osc_timetag_subtract( osc_timetag_now(), start_time );
        
        t_osc_bndl_u *time_bndl = osc_bundle_u_alloc();
        t_osc_msg_u *tt_msg = osc_message_u_allocWithFloat( (char *)"/passthrough/elapsed_time", osc_timetag_timetagToFloat(dur) );
        osc_bundle_u_addMsg(time_bndl, tt_msg );
        
        *bndl = OPCL_passthrough::unionState( time_bndl );
        if( time_bndl )
            osc_bundle_u_free( time_bndl );
        
    }
}


#endif
