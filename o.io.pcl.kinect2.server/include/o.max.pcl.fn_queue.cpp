
#include "o.max.pcl.fn_queue.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

void omax_pcl_fn_queue::clear()
{
    m_fullpackets.clear();
    m_fullpackets.shrink_to_fit();
    
    m_queries.clear();
    m_queries.shrink_to_fit();
}

t_osc_bndl_u *omax_pcl_fn_queue::getStates()
{
    //send out copy of internal state bundle which gets freed in max
    t_osc_bndl_u *bndl = NULL, *u_bndl = NULL;
    
    for( int i = 0; i < m_queries.size(); i++ )
    {
        
        if( bndl )
            osc_bundle_u_free( bndl );
        
        bndl = NULL;
        bndl = (this->*m_queries[i])();
        
        t_osc_bndl_u * tmp = NULL;
        osc_bundle_u_union( bndl, tmp, &u_bndl );

        if(u_bndl)
            osc_bundle_u_free( u_bndl );

        u_bndl = NULL;
        osc_bundle_u_copy( &u_bndl, tmp );
        
        if( tmp )
            osc_bundle_u_free( tmp );
        
    }

    return bndl;
}

t_osc_bndl_u *omax_pcl_fn_queue::FullPacketBroadcast(long len, char *ptr)
{
    
    //send out copy of internal state bundle which gets freed in max
    t_osc_bndl_u *bndl = NULL, *u_bndl = NULL;
    
    for( int i = 0; i < m_fullpackets.size(); i++ )
    {
        
        if( bndl )
            osc_bundle_u_free( bndl );
        
        bndl = NULL;
//        bndl = m_fullpackets[i]( len, ptr );
        
        t_osc_bndl_u * tmp = NULL;
        osc_bundle_u_union( bndl, tmp, &u_bndl );
        
        if(u_bndl)
            osc_bundle_u_free( u_bndl );
        
        u_bndl = NULL;
        osc_bundle_u_copy( &u_bndl, tmp );
        
        if( tmp )
            osc_bundle_u_free( tmp );
        
    }
    
    return bndl;
}