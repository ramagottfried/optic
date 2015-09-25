#include "kinect_device_wrapper.h"


kinect_device::kinect_device(t_object *x)
{
    _x = x;
    
    if(freenect2.enumerateDevices() == 0)
    {
        object_error(_x, "no device connected!");
        return;
    }
    
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
    
    //  pipeline = new libfreenect2::CpuPacketPipeline();
    pipeline = new libfreenect2::OpenCLPacketPipeline();
    
    if(pipeline)
    {
        dev = freenect2.openDevice(serial, pipeline);
    }
    
    if(dev == 0)
    {
        object_error(_x, "failure opening device!");
        return;
    }
    
    listener =  new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
    undistorted = new libfreenect2::Frame(512, 424, 4);
    registered = new libfreenect2::Frame(512, 424, 4);
    
    dev->setColorFrameListener( listener );
    dev->setIrAndDepthFrameListener( listener );
    dev->start();
    
    object_post(_x, "device serial: %s", dev->getSerialNumber().c_str() );
    object_post(_x, "device firmware: %s", dev->getFirmwareVersion().c_str() );
    
    registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());

}

void kinect_device::getFrames( char **xyz_o, char **rgb_o )
{
    // probably need to make this a separate thread?
        listener->waitForNewFrame( frames );
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
        
//        object_post(_x,"%d %d %d\n", rgb->data[0], rgb->data[1], rgb->data[2]);
        registration->apply( rgb, depth, undistorted, registered );
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

//        framecount++;
        
        listener->release(frames);
}