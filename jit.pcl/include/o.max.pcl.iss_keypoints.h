
#ifndef __jit_pcl__o_max_pcl_iss_3d__
#define __jit_pcl__o_max_pcl_iss_3d__

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/keypoints/iss_3d.h>

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

// NOT SETUP YET!!

template <typename PointT>
class OPCL_iss_3d : public omax_pcl
{
private:
    //parameters
    bool                m_profile = false;
    double              m_leafsize = 0.02;
    
    //internal osc system
    enum class OSC_API
    {
        PROFILE,
        LEAFSIZE
    };
    
    static const int            m_nselectors = 2;
    static const char * const   m_selectors[ m_nselectors ];
    
    double computeCloudResolution(const typename pcl::PointCloud<PointT>::ConstPtr& cloud);

    
public:
    
    OPCL_iss_3d() : omax_pcl()
    {
        set_state_bundle_double(    "/iss_3d/leafsize",     m_leafsize );
        set_state_bundle_bool(      "/iss_3d/profile",      m_profile );
    }
    
    ~OPCL_iss_3d(){}
   
    inline void setLeafsize ( double d ) { m_leafsize = d; }
    inline void setProfiling ( bool b ) { m_profile = b; }
    
    
    void calc(  typename pcl::PointCloud<PointT>::Ptr cloud,
                typename pcl::PointCloud<PointT>::Ptr *iss_3d_dst,
                t_osc_bndl_u **bndl );
    
    void FullPacket(long len, char *ptr, t_osc_bundle_u **dst );

};

#endif
