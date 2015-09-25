#include "kinect_device_wrapper.h"

bool io_kinect_listener::onNewFrame(libfreenect2::Frame::Type type, libfreenect2::Frame *_frame)
{
    std::unique_lock<std::mutex> _lock( mutex );
    
    if (type == libfreenect2::Frame::Ir )
    {
        if ( ir_frame )
            delete ir_frame;
        ir_frame = _frame;
    }
    else
    {
        if( frame )
            delete frame;
        frame = _frame;
    }
    _lock.unlock();
    
    if( m_cb_set && type == m_type )
        m_cb(*m_obj);
        
    return true;
}

io_kinect_device::io_kinect_device()
{
    if(freenect2.enumerateDevices() == 0)
    {
        std::cout << "no device connected!" << std::endl;
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
        std::cout << "failure opening device!" << std::endl;
        return;
    }
    
    dev->setColorFrameListener( &rgb_listener );
    dev->setIrAndDepthFrameListener( &depth_listener );
    dev->start();
    
    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    
    registration = new libfreenect2::Registration( dev->getIrCameraParams(), dev->getColorCameraParams() );

    PointCloudEncoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB> (compressionProfile, showStatistics);

    
    m_open = true;
    
}

void io_kinect_device::getCompressedPointCloud(std::stringstream *compressedData)
{
    std::unique_lock<std::mutex> lock1(rgb_listener.mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(depth_listener.mutex, std::defer_lock);
    std::lock(lock1, lock2);
    
    if( rgb_listener.frame && depth_listener.frame )
    {
        registration->apply(rgb_listener.frame, depth_listener.frame, &undistorted, &registered);
        
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
        cloud->width    = (uint32_t)depth_listener.frame->width;
        cloud->height   = (uint32_t)depth_listener.frame->height;
        cloud->points.resize (cloud->width * cloud->height);
        
        float x,y,z,rgb;
        long count = 0;
        for (int i = 0; i < depth_listener.frame->width; ++i)
        {
            for (int j = 0; j < depth_listener.frame->height; ++j)
            {
                registration->getPointXYZRGB(&undistorted, &registered, i, j, x, y, z, rgb);
                cloud->points[count].x = x;
                cloud->points[count].y = y;
                cloud->points[count].z = x;
                cloud->points[count].rgb = rgb;
                count++;
                /*
                 later:
                 uint32_t rgb = *reinterpret_cast<int*>(&p.rgb);
                 uint8_t r = (rgb >> 16) & 0x0000ff;
                 uint8_t g = (rgb >> 8)  & 0x0000ff;
                 uint8_t b = (rgb)       & 0x0000ff;
                 */
            }
        }
//        std::cout << "count " << count << std::endl;
        PointCloudEncoder->encodePointCloud (cloud, *compressedData);
    }
}



pcl::PointCloud<pcl::PointXYZRGB>::Ptr io_kinect_device::getPointCloud()
{
    std::unique_lock<std::mutex> lock1(rgb_listener.mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(depth_listener.mutex, std::defer_lock);
    std::lock(lock1, lock2);
    
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
    cloud->width    = (uint32_t)depth_listener.frame->width;
    cloud->height   = (uint32_t)depth_listener.frame->height;
    cloud->points.resize (cloud->width * cloud->height);

    float x,y,z,rgb;
    long count = 0;
    for (int i = 0; i < depth_listener.frame->width; ++i)
    {
        for (int j = 0; j < depth_listener.frame->height; ++j)
        {
            registration->getPointXYZRGB(&undistorted, &registered, i, 10, x, y, z, rgb);
            cloud->points[count].x = x;
            cloud->points[count].y = y;
            cloud->points[count].z = x;
            cloud->points[count].rgb = rgb;
            count++;
            /*
             later:
             uint32_t rgb = *reinterpret_cast<int*>(&p.rgb);
             uint8_t r = (rgb >> 16) & 0x0000ff;
             uint8_t g = (rgb >> 8)  & 0x0000ff;
             uint8_t b = (rgb)       & 0x0000ff;
             */
        }
    }
    
    
    return cloud;
}


void io_kinect_device::getRegisteredFrames()
{
    std::unique_lock<std::mutex> lock1(rgb_listener.mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(depth_listener.mutex, std::defer_lock);
    std::lock(lock1, lock2);
    if( rgb_listener.frame && depth_listener.frame )
    {
        registration->apply(rgb_listener.frame, depth_listener.frame, &undistorted, &registered);
        float x,y,z,rgb;
        for (int i = 0; i < depth_listener.frame->width; ++i) {
            registration->getPointXYZRGB(&undistorted, &registered, i, 10, x, y, z, rgb);
            std::cout << "x " << x << " y " << y << " z " << z << " rgb " << rgb << std::endl;

        }
        
//        std::cout << "-w " << rgb_listener.frame->width << " -h "<< rgb_listener.frame->height << std::endl;
    }
}