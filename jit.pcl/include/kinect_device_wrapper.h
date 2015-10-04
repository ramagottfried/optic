
#pragma once

#include <iostream>
#include <signal.h>

//#include "libfreenect.h"
//#include "libfreenect_sync.h"

#include <string>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>
#include <pcl/compression/octree_pointcloud_compression.h>

#include "ext.h"
#include "jit.common.h"



//#define LIBFREENECT2_THREADING_STDLIB
class io_kinect_device;

class io_kinect_listener : public libfreenect2::FrameListener
{
public:
    io_kinect_listener(){};
    ~io_kinect_listener(){};
    
    bool onNewFrame( libfreenect2::Frame::Type type, libfreenect2::Frame *frame );
    bool hasNew(){ return m_has_new; };
    inline void setCallback(std::function<void(t_object *)> cb, t_object* obj, libfreenect2::Frame::Type type){ m_cb = cb; m_obj = obj; m_type = type; m_cb_set = true; };
    
    libfreenect2::Frame*                    frame = NULL;
    libfreenect2::Frame*                    ir_frame = NULL;
    std::mutex                              mutex;

private:
    t_object*                               m_obj = NULL;
    libfreenect2::Frame::Type               m_type;
    bool                                    m_has_new = false;
    std::function<void(t_object *)>  m_cb;
    bool                                    m_cb_set = false;
    std::chrono::system_clock::time_point   m_prev_time = std::chrono::system_clock::now();
    uint64_t                                framecount = 0;

};


class io_kinect_device
{
public:
    io_kinect_device();
    ~io_kinect_device();

    bool                                    getMatrixes( void *rgb_matrix, void *depth_matrix );
    void                                    getFrames(libfreenect2::Frame **_rgb,
                                                      libfreenect2::Frame **_depth,
                                                      libfreenect2::Frame **_ir );
    
    void                                    getRegisteredFrames();
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr  getPointCloud();
    
    inline bool acitve(){ return m_open; };
    inline void setCallbackType( std::function<void(t_object *)> cb, t_object *x, libfreenect2::Frame::Type type )
    {
        m_type = type;
        if (m_type == libfreenect2::Frame::Type::Color)
            rgb_listener.setCallback( cb, x, m_type );
        else
            depth_listener.setCallback( cb, x, m_type );
    };
    inline void setRegister( bool t ){ m_register_rgb = t; };
private:
    
    void frame2matrix_rgb( void *matrix, libfreenect2::Frame *frame, bool &err);
    void frame2matrix_depth( void *matrix, libfreenect2::Frame *frame, bool &err);
    void frame2matrix_xyz( void *matrix, bool &err);

    libfreenect2::Freenect2         freenect2;
    libfreenect2::Freenect2Device*  dev = 0;
    libfreenect2::PacketPipeline*   pipeline = 0;
    
    libfreenect2::Freenect2Device::ColorCameraParams m_color_params;
    libfreenect2::Freenect2Device::IrCameraParams m_depth_params;
    
    bool                            m_open = false;
    libfreenect2::Frame::Type       m_type = libfreenect2::Frame::Type::Depth;

    io_kinect_listener              depth_listener;
    io_kinect_listener              rgb_listener;
    
    bool                            m_register_rgb = true;
    libfreenect2::Frame             undistorted = libfreenect2::Frame(512, 424, 4);
    libfreenect2::Frame             registered = libfreenect2::Frame(512, 424, 4);
    libfreenect2::Registration*     registration = NULL;
};
