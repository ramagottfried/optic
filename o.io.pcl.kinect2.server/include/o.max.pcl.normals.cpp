#include "o.max.pcl.normals.h"


const char * const OPCL_normals::m_selectors[] = { "/normals/profile", "/normals/depthFactor", "/normals/smoothingSize" };

void OPCL_normals::FullPacket(long len, char *ptr, t_osc_bundle_u **dst )
{
    
    for (int i = 0; i < m_nselectors; i++)
    {
        double val = 0;
        t_osc_msg_ar_s *ar = osc_bundle_s_lookupAddress(len, ptr, m_selectors[i], 1);
        if( OPCL_normals::getDouble(ar, &val) )
        {
            switch (i)
            {
                case (int)OSC_API::DEPTH:
                    OPCL_normals::setDepth( val );
                    OPCL_normals::set_state_bundle_double( m_selectors[i], val );
                    break;
                    
                case (int)OSC_API::SMOOTHNESS:
                    OPCL_normals::setSmoothingSize( val );
                    OPCL_normals::set_state_bundle_double( m_selectors[i], val );
                    break;
                    
                case (int)OSC_API::PROFILE:
                {
                    bool t = val > 0.0;
                    OPCL_normals::setProfiling( t );
                    OPCL_normals::set_state_bundle_bool( m_selectors[i], t );
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // pass bundle to possible subclasses here
    OPCL_normals::getState( dst );
    
}

void OPCL_normals::calc( pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud,
                        pcl::PointCloud<pcl::Normal>::Ptr *normals_dst,
                        t_osc_bndl_u **bndl )
{
    t_osc_timetag start_time, dur;
    if( m_profile )
    {
        start_time = osc_timetag_now();
    }
    
    pcl::PointCloud<pcl::Normal>::Ptr normals( new pcl::PointCloud<pcl::Normal> );
    pcl::IntegralImageNormalEstimation<pcl::PointXYZRGB, pcl::Normal> normalEstimation;
    
    normalEstimation.setInputCloud(cloud);
    
    // Other estimation methods: COVARIANCE_MATRIX, AVERAGE_DEPTH_CHANGE, SIMPLE_3D_GRADIENT.
    // They determine the smoothness of the result, and the running time.
    normalEstimation.setNormalEstimationMethod(normalEstimation.AVERAGE_3D_GRADIENT);
    
    // Depth threshold for computing object borders based on depth changes, in meters.
    normalEstimation.setMaxDepthChangeFactor( m_depth_factor );
    
    // Factor that influences the size of the area used to smooth the normals.
    normalEstimation.setNormalSmoothingSize( m_smoothing_size );
    normalEstimation.compute(*normals);
    
    *normals_dst = normals;
    
    if( m_profile && *bndl )
    {
        dur = osc_timetag_subtract( osc_timetag_now(), start_time );
        
        t_osc_bndl_u *time_bndl = osc_bundle_u_alloc();
        t_osc_msg_u *tt_msg = osc_message_u_allocWithFloat( (char *)"/normals/elapsed_time", osc_timetag_timetagToFloat(dur) );
        osc_bundle_u_addMsg(time_bndl, tt_msg );

        OPCL_normals::unionState( time_bndl, bndl ); //<< maybe make this stateAccum?
        
        if( time_bndl )
            osc_bundle_u_free( time_bndl );
        
    }
}
