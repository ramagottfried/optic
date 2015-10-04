#include "kinect_device_wrapper.h"

bool io_kinect_listener::onNewFrame(libfreenect2::Frame::Type type, libfreenect2::Frame *_frame)
{
    std::unique_lock<std::mutex> _lock( mutex );
    
    if (type == libfreenect2::Frame::Ir )
    {
        if ( ir_frame )
        {
            delete ir_frame;
            ir_frame = NULL;
        }
        ir_frame = _frame;
    }
    else
    {
        if( frame )
        {
            delete frame;
            frame = NULL;
        }
        frame = _frame;
    }
    _lock.unlock();
    
    if( m_cb_set && m_obj && type == m_type )
        m_cb(m_obj);
    
    return true;
}

io_kinect_device::~io_kinect_device()
{
    if( m_open )
    {
        dev->stop();
        
//        dev->setColorFrameListener( NULL );
//        dev->setIrAndDepthFrameListener( NULL );
        
        dev->close();
        
        post("------------ closed");
        if( registration )
            delete registration;
        
    }
    std::cout << "context clean" << std::endl;
}

io_kinect_device::io_kinect_device()
{
    post("checking devices...");
    if(freenect2.enumerateDevices() == 0)
    {
        post("no device connected!");
        return;
    }
    post("found devices");
    
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
    
    //  pipeline = new libfreenect2::CpuPacketPipeline();
    pipeline = new libfreenect2::OpenCLPacketPipeline();
    
    if(pipeline)
    {
        dev = freenect2.openDevice(serial, pipeline);
    }
    
    if(dev == 0)
    {
        post("failure opening device!");
        return;
    }
    
    dev->setColorFrameListener( &rgb_listener );
    dev->setIrAndDepthFrameListener( &depth_listener );
    dev->start();
    
//    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
//    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    std::string s = dev->getSerialNumber();
    post("device serial: %s", s.c_str() );

    m_depth_params = dev->getIrCameraParams();
    m_color_params = dev->getColorCameraParams();
    registration = new libfreenect2::Registration( m_depth_params, m_color_params );

    if( !registration )
    {
        post("failed to register!");
        dev->stop();
        dev->close();
        return;
    }

    
    m_open = true;
    
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr io_kinect_device::getPointCloud()
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
        for (int c = 0; c < depth_listener.frame->width; c++)
        {
            for (int r = 0; r < depth_listener.frame->height; r++)
            {
                registration->getPointXYZRGB(&undistorted, &registered, r, c, x, y, z, rgb);
                
                cloud->points[count].x = x;
                cloud->points[count].y = y;
                cloud->points[count].z = z;
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
    else
        return NULL;
}

void io_kinect_device::frame2matrix_xyz( void *matrix, bool &err)
{
    if( !undistorted.data || !registered.data || !registered.width || !registered.height )
    {
        err = 1;
        error("%s no frame", __func__ );
        return;
    }
    char *out_bp = NULL;
    float *dstfp = NULL;
    t_jit_matrix_info dst_info;
    long savelock;
    
    savelock = (long) jit_object_method(matrix, _jit_sym_lock, 1);
    jit_object_method(matrix, _jit_sym_getinfo, &dst_info);
    
    if( dst_info.dim[0] != registered.width || dst_info.dim[1] != registered.height || dst_info.planecount != 3 )
    {
        dst_info.dim[0] = registered.width;
        dst_info.dim[1] = registered.height;
        dst_info.type = _jit_sym_float32;
        dst_info.planecount = 3;
        jit_object_method(matrix, _jit_sym_setinfo, &dst_info);
        jit_object_method(matrix, _jit_sym_getinfo, &dst_info);
    }
    
    jit_object_method(matrix, _jit_sym_getdata, &out_bp);
    
    if( !out_bp )
    {
        jit_object_method(matrix, _jit_sym_lock, savelock);
        err = 1;
        error("%s no matrix data", __func__ );
        return;
    }
    
    const float *depth_data = (float *)undistorted.data;

    const float cx(m_depth_params.cx);
    const float cy(m_depth_params.cy);
    const float fx(1.0/m_depth_params.fx), fy(1.0/m_depth_params.fy);
    
    float x,y,z,rgb;
    for( int i = 0; i < registered.height; i++ )
    {
        dstfp = (float *)(out_bp + dst_info.dimstride[1] * i);
        
        for(int j = 0 ; j < registered.width; j++)
        {
            //registration->getPointXYZRGB(&undistorted, &registered, i, j, x, y, z, rgb);
            
            z = *depth_data++;
            
            z = (z <= 0.0001) ? 0.0 : z / 1000.0f;
            
            x = (j + 0.5 - cx) * fx * z;
            y = (i + 0.5 - cy) * fy * z;
            
            *dstfp++ = x;
            *dstfp++ = y;
            *dstfp++ = z;
          //  *dstfp++ = 0;
            
//            dstfp += dst_info.dimstride[0];
        }
    }
    
    jit_object_method(matrix, _jit_sym_lock, savelock);
    
    err = 0;
}

void io_kinect_device::frame2matrix_depth( void *matrix, libfreenect2::Frame *frame, bool &err)
{
    if( !frame || !frame->data )
    {
        err = 1;
        error("%s no frame", __func__ );
        return;
    }
    char *out_bp = NULL;

    float *dstfp = NULL;
    t_jit_matrix_info dst_info;
    long savelock;

    savelock = (long) jit_object_method(matrix, _jit_sym_lock, 1);
    jit_object_method(matrix, _jit_sym_getinfo, &dst_info);
    jit_object_method(matrix, _jit_sym_getdata, &out_bp);
    
    if( !out_bp )
    {
        jit_object_method(matrix, _jit_sym_lock, savelock);
        err = 1;
        error("%s no matrix data", __func__ );
        return;
    }
    
    if( dst_info.dim[0] != frame->width || dst_info.dim[1] != frame->height || dst_info.dimstride[0] != frame->bytes_per_pixel )
    {
        error("matrix frame size mismatch!");
        jit_object_method(matrix, _jit_sym_lock, savelock);
        err = 1;
        return;
    }
    
    const float *srcp = (float *)frame->data;
    
    for( int i = 0; i < frame->height; i++ )
    {
        dstfp = (float *)(out_bp + dst_info.dimstride[1] * i);
        
        for(int j = 0 ; j < frame->width; j++)
        {
            *dstfp++ = *srcp++;
        }
    }
    
    jit_object_method(matrix, _jit_sym_lock, savelock);
    
    err = 0;
}

void io_kinect_device::frame2matrix_rgb( void *matrix, libfreenect2::Frame *frame, bool &err)
{
    if( !frame )
    {
        err = 1;
        error("%s no frame", __func__ );
        return;
    }
    unsigned char *dstp = NULL, *dst_start = NULL;
    unsigned char *srcp;
    t_jit_matrix_info dst_info;
    long savelock;
    
    savelock = (long) jit_object_method(matrix, _jit_sym_lock, 1);
    jit_object_method(matrix, _jit_sym_getinfo, &dst_info);
    
    if( dst_info.dim[0] != frame->width || dst_info.dim[1] != frame->height || dst_info.planecount != 3)
    {
        dst_info.dim[0] = frame->width;
        dst_info.dim[1] = frame->height;
        dst_info.type = _jit_sym_char;
        dst_info.planecount = 3;
        jit_object_method(matrix, _jit_sym_setinfo, &dst_info);
        jit_object_method(matrix, _jit_sym_getinfo, &dst_info);
    }
    
    jit_object_method(matrix, _jit_sym_getdata, &dstp);
    
    if( !dstp )
    {
        jit_object_method(matrix, _jit_sym_lock, savelock);
        err = 1;
        error("%s no matrix data", __func__ );
        return;
    }

    srcp = frame->data;
    dst_start = dstp;
    
    for( int i = 0; i < frame->height; i++ )
    {
        dstp = dst_start + dst_info.dimstride[1] * i;
        
        for(int j = 0 ; j < frame->width; j++)
        {
            dstp[0] = srcp[2];
            dstp[1] = srcp[1];
            dstp[2] = srcp[0];
            
            dstp += dst_info.dimstride[0];
            srcp += frame->bytes_per_pixel;
        }
    }
    
    jit_object_method(matrix, _jit_sym_lock, savelock);

    err = 0;
}

bool io_kinect_device::getMatrixes( void *rgb_matrix, void *depth_matrix )
{
    bool err1 = 0, err2 = 0;
    
    if( m_register_rgb )
    {
        std::unique_lock<std::mutex> lock1(rgb_listener.mutex, std::defer_lock);
        std::unique_lock<std::mutex> lock2(depth_listener.mutex, std::defer_lock);
        std::lock(lock1, lock2);
        
        if( rgb_listener.frame && depth_listener.frame )
        {
            registration->apply(rgb_listener.frame, depth_listener.frame, &undistorted, &registered);
        
            io_kinect_device::frame2matrix_rgb( rgb_matrix, &registered, err1 );
            
            //io_kinect_device::frame2matrix_depth( depth_matrix, &undistorted, err2 );

            io_kinect_device::frame2matrix_xyz( depth_matrix, err2 );
        }
    }
    else
    {
        std::unique_lock<std::mutex> lock1(rgb_listener.mutex);
        io_kinect_device::frame2matrix_rgb( rgb_matrix, rgb_listener.frame, err1 );
        lock1.unlock();

        std::unique_lock<std::mutex> lock2(depth_listener.mutex);
        io_kinect_device::frame2matrix_depth( depth_matrix, depth_listener.frame, err2 );
        lock2.unlock();
    }
    return (err1 || err2) ;
    
}



void io_kinect_device::getFrames(   libfreenect2::Frame ** _rgb, libfreenect2::Frame ** _depth, libfreenect2::Frame ** _ir )
{
    std::unique_lock<std::mutex> lock1(rgb_listener.mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(depth_listener.mutex, std::defer_lock);
    std::lock(lock1, lock2);

    if( rgb_listener.frame && depth_listener.frame && depth_listener.ir_frame )
    {
            
        if( *_rgb )
            delete (*_rgb);
        
        (*_rgb) = new libfreenect2::Frame( rgb_listener.frame->width,
                                          rgb_listener.frame->height,
                                          rgb_listener.frame->bytes_per_pixel );
        memcpy((*_rgb)->data, rgb_listener.frame->data, (*_rgb)->width * (*_rgb)->height * (*_rgb)->bytes_per_pixel + 64 );
        
        if( *_depth )
            delete (*_depth);
        
        (*_depth) = new libfreenect2::Frame( depth_listener.frame->width,
                                          depth_listener.frame->height,
                                          depth_listener.frame->bytes_per_pixel );
        memcpy((*_depth)->data, rgb_listener.frame->data, (*_depth)->width * (*_depth)->height * (*_depth)->bytes_per_pixel + 64);

        /*
        if( *_ir )
            delete (*_ir);
        
        (*_ir) = new libfreenect2::Frame( depth_listener.ir_frame->width,
                                            depth_listener.ir_frame->height,
                                            depth_listener.ir_frame->bytes_per_pixel );
        *((*_ir)->data) = (*(depth_listener.ir_frame->data));
        */
    }
}


void io_kinect_device::getRegisteredFrames()
{
    std::unique_lock<std::mutex> lock1(rgb_listener.mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(depth_listener.mutex, std::defer_lock);
    std::lock(lock1, lock2);
    if( rgb_listener.frame && depth_listener.frame )
    {
        registration->apply(rgb_listener.frame, depth_listener.frame, &undistorted, &registered);
//        std::cout << "-w " << rgb_listener.frame->width << " -h "<< rgb_listener.frame->height << std::endl;
    }
}