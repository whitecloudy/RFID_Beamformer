#ifndef __VNA_COMMUNICATOR__
#define __VNA_COMMUNICATOR__

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>



class VNA_communicator
{
  private:
    sockaddr_in server_addr;
    int listenSockFD = 0;
  public:
    VNA_communicator();
    ~VNA_communicator();
    int readData(int clientFD, uint8_t * buffer, unsigned int buffer_size);
    int sendData(int clientFD, uint8_t * buffer, uint32_t send_size);
    int wait_connection(void);

};

#endif
