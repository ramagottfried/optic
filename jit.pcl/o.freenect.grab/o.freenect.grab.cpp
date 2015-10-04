

#include "osc.h"
#include "osc_bundle_u.h"
#include "osc_bundle_s.h"
#include "osc_timetag.h"
#include "omax_util.h"
#include "o.h"

#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "ext_critical.h"

#include "kinect_device_wrapper.h"
#include <pcl/filters/voxel_grid.h>

static t_class *o_freenect_grab_class;

BEGIN_USING_C_LINKAGE
typedef struct _o_freenect_grab
{
    t_object            ob;
    void*               outlet;
    t_critical          lock;
    void*               clock;
    double              leafsize;
    io_kinect_device*   kinect;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr  cloud;
} t_o_freenect_grab;
END_USING_C_LINKAGE


void o_freenect_grab_output(t_o_freenect_grab *x)
{
    //filter
    pcl::VoxelGrid<pcl::PointXYZRGB> grid;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_voxel_ (new pcl::PointCloud<pcl::PointXYZRGB>);
    
    grid.setLeafSize (x->leafsize, x->leafsize, x->leafsize);

    critical_enter(x->lock);
    grid.setInputCloud (x->cloud);
    grid.filter (*cloud_voxel_);
    critical_exit(x->lock);

    
    const float bad_point = std::numeric_limits<float>::quiet_NaN();

    if( cloud_voxel_ )
    {
        t_osc_bndl_u *bndl = osc_bundle_u_alloc();
        
        t_osc_message_u *x_msg = osc_message_u_allocWithAddress((char *)"/x");
        t_osc_message_u *y_msg = osc_message_u_allocWithAddress((char *)"/y");
        t_osc_message_u *z_msg = osc_message_u_allocWithAddress((char *)"/z");
        t_osc_message_u *size_msg = osc_message_u_allocWithAddress((char *)"/size");
        osc_message_u_appendInt32(size_msg, cloud_voxel_->width);
        osc_message_u_appendInt32(size_msg, cloud_voxel_->height);
        
        for (long i = 0; i < cloud_voxel_->points.size(); i++)
        {
            if( cloud_voxel_->points[i].x != bad_point && cloud_voxel_->points[i].y != bad_point && cloud_voxel_->points[i].z != bad_point)
            {
                osc_message_u_appendFloat(x_msg, cloud_voxel_->points[i].x);
                osc_message_u_appendFloat(y_msg, cloud_voxel_->points[i].y);
                osc_message_u_appendFloat(z_msg, cloud_voxel_->points[i].z);
            }
        }
        
        osc_bundle_u_addMsg(bndl, size_msg);
        osc_bundle_u_addMsg(bndl, x_msg);
        osc_bundle_u_addMsg(bndl, y_msg);
        osc_bundle_u_addMsg(bndl, z_msg);
        
        omax_util_outletOSC_u(x->outlet, bndl);
        
        if( bndl )
            osc_bundle_u_free( bndl );
        
    }
    
    critical_enter(x->lock);
    x->cloud.reset();
    x->cloud = NULL;
    critical_exit(x->lock);

}


void o_freenect_grab_callback(t_object *x_obj)
{
    t_o_freenect_grab *x = (_o_freenect_grab *)x_obj;
    
//    x->kinect->getRegisteredFrames();
    critical_enter(x->lock);
    x->cloud = x->kinect->getPointCloud();

    critical_exit(x->lock);

    if (x->cloud)
    {
        x->cloud.reset();
        x->cloud = NULL;
    //    clock_fdelay(x->clock, 0.);
    }
    
   
}

void o_freenect_grab_connect(t_o_freenect_grab *x)
{
    critical_enter(x->lock);
    x->kinect = new io_kinect_device;
    
    t_object *x_obj = (t_object *)x;
    std::function<void(t_object*)> cb = std::bind(o_freenect_grab_callback, x_obj);
    x->kinect->setCallbackType( cb, x_obj, libfreenect2::Frame::Type::Depth );
    
    if ( ! x->kinect->acitve() )
    {
        post("no active kinect");
        return;
    }
    
    critical_exit(x->lock);

}


void o_freenect_grab_int(t_o_freenect_grab *x, int i)
{
    if( i > 0 && !x->kinect)
    {
        o_freenect_grab_connect(x);
    }
    else if( i == 0 )
    {
        if( x->kinect )
        {
            critical_enter(x->lock);
            delete x->kinect;
            x->kinect = NULL;
            critical_exit(x->lock);
        }
    }

}

void o_freenect_grab_assist(t_o_freenect_grab *x, void *b, long m, long a, char *s)
{
    ; // to do later
}

void o_freenect_grab_free( t_o_freenect_grab *x)
{
    if( x->kinect )
        delete x->kinect;
    
    x->kinect = NULL;
    critical_free(x->lock);
    
    clock_unset(x->clock);
    object_free(x->clock);
}

void *o_freenect_grab_new(t_symbol* s, short argc, t_atom* argv)
{
    t_o_freenect_grab *x = (t_o_freenect_grab *)object_alloc(o_freenect_grab_class);
    if(x)
    {
        x->outlet = outlet_new((t_object *)x, NULL);
        x->leafsize = 0.02;
        x->cloud = NULL;
        
        x->clock = clock_new((t_object *)x, (method)o_freenect_grab_output);
        critical_new(&(x->lock));
        
    }
    return (void *)x;
}

BEGIN_USING_C_LINKAGE
int C74_EXPORT main(void)
{
    
    t_class *c = class_new("o.freenect.grab", (method)o_freenect_grab_new, (method)o_freenect_grab_free, sizeof(t_o_freenect_grab), 0L, A_GIMME, 0);
    //class_addmethod(c, (method)osvg_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
    class_addmethod(c, (method)o_freenect_grab_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)o_freenect_grab_int, "int", A_LONG, 0);
    
    class_register(CLASS_BOX, c);
    o_freenect_grab_class = c;
    
    common_symbols_init();
    
    post("test");
    return 0;

}
END_USING_C_LINKAGE