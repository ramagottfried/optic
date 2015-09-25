#pragma once

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/features/integral_image_normal.h>

#include "osc.h"
#include "osc_bundle_s.h"
#include "osc_bundle_u.h"
#include "osc_message_s.h"
#include "osc_timetag.h"
#include "osc_match.h"
#include "omax_util.h"

#include "jit.common.h"
#include "max.jit.mop.h"

#include "o.max.pcl.h"

class OPCL_normals : public omax_pcl
{
    
public:
    
    OPCL_normals() : omax_pcl()
    {
        set_state_bundle_double( "/normals/depthFactor", m_depth_factor );
        set_state_bundle_double( "/normals/smoothingSize", m_smoothing_size );
        set_state_bundle_bool(   "/normals/profile", m_profile );
        
    }
    
    ~OPCL_normals(){}
   
    void calc(  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud,
                pcl::PointCloud<pcl::Normal>::Ptr *normals_dst,
                t_osc_bndl_u **bndl );
    
    inline void setDepth ( double d ) { m_depth_factor = d; }
    inline void setSmoothingSize ( double d ) { m_smoothing_size = d; }
    inline void setProfiling ( bool b ) { m_profile = b; }

    void FullPacket(long len, char *ptr, t_osc_bundle_u **dst );

private:
    //parameters
    bool                m_profile = false;
    double              m_depth_factor = 0.02;
    double              m_smoothing_size = 10.0;
    
    //internal osc system
    enum class OSC_API
    {
        PROFILE,
        DEPTH,
        SMOOTHNESS
    };
    
    static const int            m_nselectors = 3;
    static const char * const   m_selectors[ m_nselectors ];
    
};
