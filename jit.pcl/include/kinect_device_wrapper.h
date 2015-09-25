
#ifndef __jit_pcl__kinect_device_wrapper__
#define __jit_pcl__kinect_device_wrapper__

#include "libfreenect.h"
#include "libfreenect_sync.h"

#include <string>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>

#include "jit.common.h"
#include "max.jit.mop.h"


//#define LIBFREENECT2_THREADING_STDLIB

class kinect_device
{
public:
    kinect_device(t_object *x);
    ~kinect_device()
    {
        dev->stop();
        dev->close();
        delete listener;
        delete undistorted;
        delete registered;
        delete registration;
        
        /*
        if( m_loop_thread )
        {
            if( poll )
                m_loop_thread->join();

            delete m_loop_thread;
        }*/
    };
    
    void getFrames( char **xyz, char **rgb );

    void enable_polling( bool t );
    
private:
    
    uint64_t framecount = 0;
    
    void loop();
    std::thread m_loop_thread;
    
    t_object *_x;
    
    bool poll = false;
    
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = 0;
    libfreenect2::PacketPipeline *pipeline = 0;
    
    libfreenect2::SyncMultiFrameListener *listener;
// probably will eventually split into two listeners to keep high frame rate
    
    libfreenect2::FrameMap frames;
    bool new_frame = false;
    libfreenect2::Frame *undistorted;
    libfreenect2::Frame *registered;
    libfreenect2::Registration* registration;
};


#endif
