
#ifndef __jit_pcl__o_max_pcl_passthrough__
#define __jit_pcl__o_max_pcl_passthrough__

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>

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

template <typename PointT>
class OPCL_passthrough : public omax_pcl
{
private:
    //parameters
    bool                m_profile = false;
    double              m_z_min = 0.0;
    double              m_z_max = 2.0;
    
    //internal osc system
    enum class OSC_API
    {
        PROFILE,
        Z_MIN,
        Z_MAX
    };
    
    static const int            m_nselectors = 3;
    static const char * const   m_selectors[ m_nselectors ];
    
public:
    
    OPCL_passthrough() : omax_pcl()
    {
        set_state_bundle_double(    "/passthrough/z_min",   m_z_min );
        set_state_bundle_double(    "/passthrough/z_max",   m_z_max );
        set_state_bundle_bool(      "/passthrough/profile", m_profile );

    }
    
    ~OPCL_passthrough(){}
   
    inline void setZmin ( double d ) { m_z_min = d; }
    inline void setZmax ( double d ) { m_z_max = d; }
    inline void setProfiling ( bool b ) { m_profile = b; }
    
    
    void calc(  typename pcl::PointCloud<PointT>::Ptr cloud,
                typename pcl::PointCloud<PointT>::Ptr *passthrough_dst,
              t_osc_bndl_u **bndl );
    
    void FullPacket(long len, char *ptr, t_osc_bundle_u **dst );

};

#endif
