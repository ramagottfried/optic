
#include "o.max.pcl.h"


void xyzrgbnorm_to_bundle( pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud, t_osc_bndl_u *dst_bndl )

{
    const float bad_point = std::numeric_limits<float>::quiet_NaN();

    t_osc_message_u *x_msg = osc_message_u_allocWithAddress((char *)"/x");
    t_osc_message_u *y_msg = osc_message_u_allocWithAddress((char *)"/y");
    t_osc_message_u *z_msg = osc_message_u_allocWithAddress((char *)"/z");
    t_osc_message_u *size_msg = osc_message_u_allocWithAddress((char *)"/size");
    osc_message_u_appendInt32(size_msg, cloud->width);
    osc_message_u_appendInt32(size_msg, cloud->height);
    
    for (long i = 0; i < cloud->points.size(); i++)
    {
        if( cloud->points[i].x != bad_point && cloud->points[i].y != bad_point && cloud->points[i].z != bad_point)
        {
            osc_message_u_appendFloat(x_msg, cloud->points[i].x);
            osc_message_u_appendFloat(y_msg, cloud->points[i].y);
            osc_message_u_appendFloat(z_msg, cloud->points[i].z);
        }
    }
    
    osc_bundle_u_addMsg(dst_bndl, size_msg);
    osc_bundle_u_addMsg(dst_bndl, x_msg);
    osc_bundle_u_addMsg(dst_bndl, y_msg);
    osc_bundle_u_addMsg(dst_bndl, z_msg);

}

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




/* -------------------------- */


// other helper functions which should probably be moved somewhere else:

t_jit_err
jit_xyzrgb2jit( t_object *x,
                         pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud,
                         t_jit_matrix_info *out_minfo,
                         void **out_matrix )
{
    char *out_bp = NULL;
    char *fop;
    
    float scalar = 1.0 / 255.;
    //*****
    // send back to jitter
    jit_object_method(*out_matrix, _jit_sym_getinfo, out_minfo);
    
    out_minfo->dim[0] = cloud->width;
    out_minfo->dim[1] = cloud->height;
    out_minfo->type = _jit_sym_float32;
    out_minfo->planecount = 6;
    jit_object_method(*out_matrix, _jit_sym_setinfo, out_minfo);
    jit_object_method(*out_matrix, _jit_sym_getinfo, out_minfo);
    
    jit_object_method(*out_matrix, _jit_sym_getdata, &out_bp);
    if (!out_bp) {
        object_error(x, "no output data" );
        return JIT_ERR_INVALID_OUTPUT;
    }
    
    long count = 0;
    for (int j = 0; j < out_minfo->dim[0]; j++)
    {
        fop =  out_bp + j * out_minfo->dimstride[0];
        
        for( int i = 0; i < out_minfo->dim[1]; i++)
        {
            if( count < cloud->points.size() )
            {
                ((float *)fop)[0] = cloud->points[count].x;
                ((float *)fop)[1] = cloud->points[count].y;
                ((float *)fop)[2] = cloud->points[count].z;
                ((float *)fop)[3] = (float)cloud->points[count].r * scalar;
                ((float *)fop)[4] = (float)cloud->points[count].g * scalar;
                ((float *)fop)[5] = (float)cloud->points[count].b * scalar;
                
            }
            count++;
            fop += out_minfo->dimstride[1];
        }
    }
    return JIT_ERR_NONE;
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr
jit2cloudXYZRGB(char *in_bp, t_jit_matrix_info in_minfo)
{
    char *fip;
    long dim[2];
    dim[0] = in_minfo.dim[0];
    dim[1] = in_minfo.dim[1];
    
    //convert to point cloud
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
    cloud->width    = (uint32_t)dim[0];
    cloud->height   = (uint32_t)dim[1];
    cloud->points.resize (cloud->width * cloud->height);
    
    size_t count = 0;
//    float _x, _y, _z;
//    uint8_t _r, _g, _b;
//    const float bad_point = std::numeric_limits<float>::quiet_NaN();
    const long x_dim_stride = in_minfo.dimstride[0];
    const long y_dim_stride = in_minfo.dimstride[1];
    if( dim[0] == 640 && dim[1] == 480 )
    {
        for (int j = 0; j < 640; ++j)
        {
            fip = in_bp + j * x_dim_stride;
            
            for( int i = 0; i < 480; ++i)
            {
                if(count < cloud->points.size())
                {
                    cloud->points[count].x = ((float *)fip)[0];
                    cloud->points[count].y = ((float *)fip)[1];
                    cloud->points[count].z = ((float *)fip)[2];
                    cloud->points[count].r = (uint8_t)(((float *)fip)[3] * 255.0);
                    cloud->points[count].g = (uint8_t)(((float *)fip)[4] * 255.0);
                    cloud->points[count].b = (uint8_t)(((float *)fip)[5] * 255.0);
                }
                count++;
                fip += y_dim_stride;
            }
        }
    
    }
    else
    {
        
        for (int j = 0; j < dim[0]; ++j)
        {
            fip = in_bp + j * in_minfo.dimstride[0];
            
            for( int i = 0; i < dim[1]; ++i)
            {
                if(count < cloud->points.size())
                {
                    cloud->points[count].x = ((float *)fip)[0];
                    cloud->points[count].y = ((float *)fip)[1];
                    cloud->points[count].z = ((float *)fip)[2];
                    cloud->points[count].r = (uint8_t)(((float *)fip)[3] * 255.0);
                    cloud->points[count].g = (uint8_t)(((float *)fip)[4] * 255.0);
                    cloud->points[count].b = (uint8_t)(((float *)fip)[5] * 255.0);
                    /*
                    _x = ((float *)fip)[0];
                    _y = ((float *)fip)[1];
                    _z = ((float *)fip)[2];
                    _r = (uint8_t)(((float *)fip)[3] * 255.0);
                    _g = (uint8_t)(((float *)fip)[4] * 255.0);
                    _b = (uint8_t)(((float *)fip)[5] * 255.0);
                    if( !_x && !_y && !_z && !_r && !_g && !_b )
                    {
                        cloud->points[count].x = bad_point;
                        cloud->points[count].y = bad_point;
                        cloud->points[count].z = bad_point;
                        cloud->points[count].r = bad_point;
                        cloud->points[count].g = bad_point;
                        cloud->points[count].b = bad_point;
                        //                        cloud->points[count].a = bad_point;
                    }
                    else
                    {
                        cloud->points[count].x = _x;
                        cloud->points[count].y = _y;
                        cloud->points[count].z = _z;
                        cloud->points[count].r = _r;
                        cloud->points[count].g = _g;
                        cloud->points[count].b = _b;
                        //                        cloud->points[count].a = 255;
                    }
                     */
                }
                count++;
                fip += in_minfo.dimstride[1];
            }
        }
    }
    return cloud;
}

pcl::PointCloud<pcl::PointXYZRGBA>::Ptr jit2cloudXYZRGBA(char *in_bp, t_jit_matrix_info in_minfo)
{
    char *fip;
    long dim[2];
    dim[0] = in_minfo.dim[0];
    dim[1] = in_minfo.dim[1];
    
    //convert to point cloud
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
    cloud->width    = (uint32_t)dim[0];
    cloud->height   = (uint32_t)dim[1];
    cloud->points.resize (cloud->width * cloud->height);
    
    size_t count = 0;
    float _x, _y, _z;
    uint8_t _r, _g, _b;
    const float bad_point = std::numeric_limits<float>::quiet_NaN();
    for (int j = 0; j < dim[0]; ++j)
    {
        fip = in_bp + j * in_minfo.dimstride[0];
        
        for( int i = 0; i < dim[1]; ++i)
        {
            if(count < cloud->points.size())
            {
                _x = ((float *)fip)[0];
                _y = ((float *)fip)[1];
                _z = ((float *)fip)[2];
                _r = (uint8_t)(((float *)fip)[3] * 255.0);
                _g = (uint8_t)(((float *)fip)[4] * 255.0);
                _b = (uint8_t)(((float *)fip)[5] * 255.0);
                if( !_x && !_y && !_z && !_r && !_g && !_b )
                {
                    cloud->points[count].x = bad_point;
                    cloud->points[count].y = bad_point;
                    cloud->points[count].z = bad_point;
                    cloud->points[count].r = bad_point;
                    cloud->points[count].g = bad_point;
                    cloud->points[count].b = bad_point;
                    cloud->points[count].a = bad_point;
                }
                else
                {
                    cloud->points[count].x = _x;
                    cloud->points[count].y = _y;
                    cloud->points[count].z = _z;
                    cloud->points[count].r = _r;
                    cloud->points[count].g = _g;
                    cloud->points[count].b = _b;
                    cloud->points[count].a = 255;
                }
            }
            count++;
            fip += in_minfo.dimstride[1];
        }
    }
    return cloud;
}