

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#include "kinect_device_wrapper.h"
#include "o.io.socket.h"

#include "osc_bundle_u.h"
#include "osc_message_u.h"

#include <sstream>

o_io_socket *udp_socket;


auto d_prev_time = std::chrono::system_clock::now();
bool protonect_shutdown = false; ///< Whether the running application should shut down.

void sigint_handler(int s)
{
    protonect_shutdown = true;
}

void compressedPointCloud_to_osc(std::stringstream& compressedData)
{
    t_osc_bndl_u *bndl = osc_bundle_u_alloc();
    
    t_osc_message_u *data_msg = osc_message_u_allocWithAddress((char *)"/compressedCloud");
    char c;
    while( compressedData >> c )
    {
//        std::cout << "_" << c;
        int i = static_cast<int>(c);
        osc_message_u_appendInt32(data_msg, i);
    }
    std::cout << std::endl;
    osc_bundle_u_addMsg(bndl, data_msg);
    
    udp_socket->send_osc( bndl );
    
    if( bndl )
        osc_bundle_u_free( bndl );

}

void cloud_to_osc(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud)
{
    t_osc_bndl_u *bndl = osc_bundle_u_alloc();
    t_osc_message_u *x_msg = osc_message_u_allocWithAddress((char *)"/x");
    t_osc_message_u *y_msg = osc_message_u_allocWithAddress((char *)"/y");
    t_osc_message_u *z_msg = osc_message_u_allocWithAddress((char *)"/z");
    t_osc_message_u *size_msg = osc_message_u_allocWithAddress((char *)"/size");
    osc_message_u_appendInt32(size_msg, cloud->width);
    osc_message_u_appendInt32(size_msg, cloud->height);
    
    for (long i = 0; i < cloud->points.size(); ++i)
    {
        osc_message_u_appendFloat(x_msg, cloud->points[i].x);
        osc_message_u_appendFloat(y_msg, cloud->points[i].y);
        osc_message_u_appendFloat(z_msg, cloud->points[i].z);
    }
    
    osc_bundle_u_addMsg(bndl, size_msg);
    osc_bundle_u_addMsg(bndl, x_msg);
    osc_bundle_u_addMsg(bndl, y_msg);
    osc_bundle_u_addMsg(bndl, z_msg);
    
    udp_socket->send_osc( bndl );
    
    if( bndl )
        osc_bundle_u_free( bndl );
    
}

void callback(io_kinect_device& k)
{
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - d_prev_time;
    d_prev_time = current_time;
    std::cout << "depth since last: " << elapsed_seconds.count() << std::endl;

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud = k.getPointCloud();

    cloud_to_osc( cloud );
    
    /*
    std::stringstream compressedData;
    k.getCompressedPointCloud( &compressedData );
    compressedPointCloud_to_osc( compressedData );
    */
}

int main(int argc, const char * argv[])
{
    udp_socket = new o_io_socket("127.0.0.1", 3333 );
    
    io_kinect_device kinect2;
    
    std::function<void(io_kinect_device&)> cb = std::bind(callback, std::ref(kinect2));
    kinect2.setCallbackType( cb, libfreenect2::Frame::Type::Depth );
    
    if ( !kinect2.acitve() )
    {
        std::cout << "no active kinect" << std::endl;
        return 0;
    }
    
    signal(SIGINT,sigint_handler);
    protonect_shutdown = false;
    
    while (!protonect_shutdown )
    {
        ;
    }
    
    delete udp_socket;
    return 0;
}
