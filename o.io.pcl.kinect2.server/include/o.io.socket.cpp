#include "o.io.socket.h"

o_io_socket::o_io_socket(std::string ip_address, int port)
{
    m_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_fd < 0)
    {
        throw std::runtime_error("failed to create socket");
    }
    
    m_port = port;
    m_address = ip_address;
    
    int buffer_size = 8 * 1024 * 1024 ;
    setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
    
    memset((char *)&m_sendto_addr, 0, sizeof(m_sendto_addr));
    m_sendto_addr.sin_family = AF_INET;
    m_sendto_addr.sin_port = htons(m_port);
    m_sendto_addr.sin_addr.s_addr = inet_addr( m_address.c_str() );
    
    std::cout << "bound to " << m_address << " at port " << m_port << std::endl;

    
}

void o_io_socket::send_osc(t_osc_bndl_u *bndl)
{
    if(bndl)
    {
        t_osc_bndl_s *s_bndl = osc_bundle_u_serialize(bndl);
        if(s_bndl)
        {
            if( sendto(m_fd, osc_bundle_s_getPtr(s_bndl), osc_bundle_s_getLen(s_bndl), 0,
                       (struct sockaddr *)&m_sendto_addr, sizeof(m_sendto_addr)) < 0)
            {
                std::cout << "failed to send " << m_countfails++ << " size " << osc_bundle_s_getLen(s_bndl) <<std::endl;
            }
            osc_bundle_s_deepFree(s_bndl);
        }
    }
}

void o_io_socket::send_sstream(std::stringstream& data)
{
    if( sendto(m_fd, data.str().data(), data.str().size(), 0,
                       (struct sockaddr *)&m_sendto_addr, sizeof(m_sendto_addr)) < 0)
    {
        std::cout << "failed to send " << m_countfails++ << std::endl;
    }
}
