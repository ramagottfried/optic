
#ifndef __jit_pcl__o_max_pcl_voxel_grid__
#define __jit_pcl__o_max_pcl_voxel_grid__

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

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
class OPCL_voxel_grid : public omax_pcl
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
    
public:
    
    OPCL_voxel_grid() : omax_pcl()
    {
        set_state_bundle_double(    "/voxel_grid/leafsize",     m_leafsize );
        set_state_bundle_bool(      "/voxel_grid/profile",      m_profile );
    }
    
    ~OPCL_voxel_grid(){}
   
    inline void setLeafsize ( double d ) { m_leafsize = d; }
    inline void setProfiling ( bool b ) { m_profile = b; }
    
    
    void calc(  typename pcl::PointCloud<PointT>::Ptr cloud,
                typename pcl::PointCloud<PointT>::Ptr *voxel_grid_dst,
                t_osc_bndl_u **bndl );
    
    void FullPacket(long len, char *ptr, t_osc_bundle_u **dst );

};

#endif
