
#pragma once

#include <vector>

#include "osc.h"
#include "osc_bundle_u.h"

#include "o.max.pcl.h"

class omax_pcl_fn_queue
{
public:
    
    omax_pcl_fn_queue()
    {
        m_bndl = osc_bundle_u_alloc();
    }
    
    ~omax_pcl_fn_queue()
    {
        osc_bundle_u_free( m_bndl );
        clear();
    }
    
    void clear();

    t_osc_bndl_u *getStates();
    t_osc_bndl_u *FullPacketBroadcast( long len, char *ptr );
    
//    typedef t_osc_bndl_u* (omax_pcl::*getStateFn)();
//    typedef t_osc_bndl_u* (omax_pcl::*FullPacketFn)(long len, char *ptr);
    
    inline void setQueryNode(getStateFn func ) {  m_queries.push_back( func ); }
    inline void setFullPacketNode(FullPacketFn func ) {  m_fullpackets.push_back( func ); }
    
private:
    
    t_osc_bndl_u *m_bndl;

    std::vector<omax_pcl::FullPacketFn> m_fullpackets;
    std::vector<omax_pcl::getStateFn> m_queries;
    
};

