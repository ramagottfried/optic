#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "sys/socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "unistd.h"

#include "osc_bundle_u.h"
#include "osc_bundle_s.h"

class o_io_socket
{
public:
    o_io_socket(std::string ip_address, int port);
    ~o_io_socket(){};
    
    void send_osc(t_osc_bndl_u *bndl);
    void send_sstream(std::stringstream& data);
private:
    unsigned long       m_countfails = 0;
    struct sockaddr_in  m_sendto_addr;
    int                 m_fd;
    int                 m_port;
    std::string         m_address;
};
