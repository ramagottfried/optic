
#include "o.max.pcl.h"


void omax_pcl::unionAccum( t_osc_bndl_u *src, t_osc_bndl_u **src_dst )
{
    critical_enter( m_lock );
    t_osc_bndl_u *tmp = osc_bundle_u_alloc();
    if( *src_dst )
    {
        osc_bundle_u_copy( &tmp, *src_dst );
        osc_bundle_u_free( *src_dst );
        *src_dst = NULL;
    }
    osc_bundle_u_union( src, tmp, src_dst );
    critical_exit( m_lock );

    if( tmp )
        osc_bundle_u_free( tmp );
}


void omax_pcl::unionState(t_osc_bndl_u *src, t_osc_bndl_u **dst)
{
    //send out copy of internal state bundle which gets freed in max
    critical_enter( m_lock );
    t_osc_bndl_u *tmp = osc_bundle_u_alloc();
    if( *dst )
    {
        osc_bundle_u_union( src, *dst, &tmp );
        osc_bundle_u_free( *dst );
        *dst = NULL;
    }
    osc_bundle_u_union( tmp, m_bndl, dst );
    critical_exit( m_lock );
    
    osc_bundle_u_free( tmp );
}

void omax_pcl::getState( t_osc_bndl_u **dst )
{
    //send out copy of internal state bundle which gets freed in max
    omax_pcl::unionAccum( m_bndl, dst );
}


void omax_pcl::set_state_bundle_double( const char *address, const double value )
{
    t_osc_bndl_u *new_bndl = osc_bundle_u_alloc();
    t_osc_msg_u *depth_val = osc_message_u_allocWithDouble((char *)address, value);
    osc_bundle_u_addMsg(new_bndl, depth_val);
    
    critical_enter( m_lock );
    t_osc_bndl_u *tmp = NULL;
    osc_bundle_u_copy( &tmp, m_bndl );
    osc_bundle_u_free( m_bndl );
    m_bndl = NULL;
    osc_bundle_u_union( new_bndl, tmp, &m_bndl );
    critical_exit( m_lock );
    
    osc_bundle_u_free( new_bndl );
    osc_bundle_u_free( tmp );
}

void omax_pcl::set_state_bundle_bool( const char *address, const bool value )
{
    t_osc_bndl_u *new_bndl = osc_bundle_u_alloc();
    t_osc_msg_u *msg = osc_message_u_allocWithAddress((char *)address );
    osc_message_u_appendBool( msg, value );
    osc_bundle_u_addMsg( new_bndl, msg );
    
    critical_enter( m_lock );
    t_osc_bndl_u *tmp = NULL;
    osc_bundle_u_copy( &tmp, m_bndl );
    osc_bundle_u_free( m_bndl );
    m_bndl = NULL;
    osc_bundle_u_union( new_bndl, tmp, &m_bndl );
    critical_exit( m_lock );
    
    osc_bundle_u_free( new_bndl );
    osc_bundle_u_free( tmp );
    
}

int omax_pcl::getInt( t_osc_msg_ar_s *ar, int *val )
{
    int ret = 0;
    if(ar)
    {
        t_osc_msg_s *m = osc_message_array_s_get(ar, 0);
        if(osc_message_s_getArgCount(m) > 0)
        {
            t_osc_atom_s *a = NULL;
            osc_message_s_getArg( osc_message_array_s_get(ar, 0), 0, &a );
            char type = osc_atom_s_getTypetag(a);
            if(type)
            {
                switch (osc_message_s_getTypetag(m, 0))
                {
                    case 'd':
                        *val = (int)osc_atom_s_getDouble(a);
                        break;
                    case 'f':
                        *val = (int)osc_atom_s_getFloat(a);
                        break;
                    case 'i':
                        *val = osc_atom_s_getInt(a);
                        break;
                    case 'T':
                        *val = osc_atom_s_getBool(a);
                        break;
                    default:
                        post("found non-numeric type %c", type);
                        break;
                }
                
                osc_atom_s_free(a);
            }
        }
        ret = 1;
    }
    osc_message_array_s_free(ar);
    return ret;
}

int omax_pcl::getDouble( t_osc_msg_ar_s *ar, double *val )
{
    int ret = 0;
    if(ar)
    {
        t_osc_msg_s *m = osc_message_array_s_get(ar, 0);
        if(osc_message_s_getArgCount(m) > 0)
        {
            t_osc_atom_s *a = NULL;
            osc_message_s_getArg( osc_message_array_s_get(ar, 0), 0, &a );
            char type = osc_atom_s_getTypetag(a);
            if(type)
            {
                switch (osc_message_s_getTypetag(m, 0))
                {
                    case 'd':
                        *val = osc_atom_s_getDouble(a);
                        break;
                    case 'f':
                        *val = (double)osc_atom_s_getFloat(a);
                        break;
                    case 'i':
                        *val = (double)osc_atom_s_getInt(a);
                        break;
                    case 'T':
                        *val = (double)osc_atom_s_getBool(a);
                        break;
                    default:
                        post("found non-numeric type %c", type);
                        break;
                }
                
                osc_atom_s_free(a);
            }
        }
        ret = 1;
    }
    osc_message_array_s_free(ar);
    return ret;
}

