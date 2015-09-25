#pragma once

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>

#include "osc.h"
#include "osc_bundle_u.h"
#include "osc_bundle_s.h"
#include "osc_timetag.h"


#define critical_new(x)
#define critical_enter(x)
#define critical_exit(x)
#define critical_free(x)

#define post printf
// base class

class omax_pcl
{
    
public:

    omax_pcl()
    {
        m_bndl = osc_bundle_u_alloc();
        critical_new( &m_lock );
    }
    
    virtual ~omax_pcl()
    {
        osc_bundle_u_free( m_bndl );
        critical_free( m_lock );
    }
    
    void getState( t_osc_bndl_u **dst );
    
    void unionState(t_osc_bndl_u *new_bndl, t_osc_bndl_u **accum);
    void unionAccum( t_osc_bndl_u *src, t_osc_bndl_u **dst );

    void set_state_bundle_double( const char *address, const double value );
    void set_state_bundle_bool( const char *address, const bool value );
    int getInt( t_osc_msg_ar_s *ar, int *val );
    int getDouble( t_osc_msg_ar_s *ar, double *val );
    
    
private:
    
    t_osc_bndl_u    *m_bndl;
//    t_critical      m_lock;
};
