#include "VNA_comm.hpp"

VNA_communicator::VNA_communicator(void)
{
  // listen socket 생성
  listenSockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(listenSockFD < 0)
  {
    std::cerr << std::endl << "socket create error" << std::endl;
    exit(1);
  }

  // 소켓 초기화
  int on = 1;
  if(setsockopt(listenSockFD, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) < 0)
  {
    std::cerr << "set option curLen = 0;화error!!" << std::endl;
    exit(1);
  }

  // 서버에서 연결을 수락할 IP 주소 설정
  // INADDR_ANY -> 모든 주소 허용
  server_addr.sin_addr.s_addr = INADDR_ANY;
  // AF_INET ->  ipv4
  server_addr.sin_family = AF_INET;
  // 포트 셋팅
  server_addr.sin_port = htons(30000);

  // listen socket에 주소 할당
  if(bind(listenSockFD, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
  {
    std::cerr << "bind error" << std::endl;
    exit(1);
  }
}//end of VNA_communicator(void)


//terminate socket
VNA_communicator::~VNA_communicator()
{
  close(listenSockFD);
}


int VNA_communicator::wait_connection(void)
{
  if(listen(listenSockFD, 1) < 0)
  {
    std::cerr << "listen error" << std::endl;
  }
  sockaddr_in client;
  int sock_len = sizeof(sockaddr_in);

  int client_fd = accept(listenSockFD, (struct sockaddr *) &client, (socklen_t*) &sock_len);

  if(client_fd < 0)
  {
    std::cerr << "Accept error " <<client_fd << std::endl;
    std::cerr << sock_len << std::endl;
  }

  return client_fd;  
}

int VNA_communicator::readData(int clientFD, uint8_t * buffer, unsigned int buffer_size)
{
  int readLen = 0;

  //get Data len header
  uint8_t dataLenByte[sizeof(uint32_t)];
  uint32_t dataLen = 0;

  while(dataLen < sizeof(uint32_t))
  {
    int len = recv(clientFD, dataLenByte + dataLen, sizeof(uint32_t)-dataLen, 0);
    if(len < 0)
    {
      std::cerr << "Recv error" <<std::endl;
      exit(1);
    }else
    {
      dataLen += len;
    }
  }

  memcpy(&dataLen, dataLenByte, sizeof(uint32_t));

  printf("%x %x %x %x\n", dataLenByte[0], dataLenByte[1], dataLenByte[2], dataLenByte[3]);

  std::cout << dataLen << std::endl;

  //get Acutal data
  while((unsigned int)readLen < dataLen)
  {
    int len = recv(clientFD, buffer + readLen, dataLen - readLen, 0);
    if(len < 0)
    {
      std::cerr << "Recv error" <<std::endl;
      exit(1);
    }else
    {
      readLen += len;
    }
  }

  return readLen;
}


int VNA_communicator::sendData(int clientFD, uint8_t * buffer, uint32_t send_size)
{
  uint8_t send_len_buffer[sizeof(uint32_t)];
  memcpy(send_len_buffer, &send_size, sizeof(uint32_t));
  
  int sendByteLen = send(clientFD, send_len_buffer, sizeof(uint32_t), 0);

  if(sendByteLen < 0)
  {
    std::cerr << "send error"<<std::endl;
    exit(1);
  }

  sendByteLen = send(clientFD, buffer, send_size, 0);

  if(sendByteLen < 0)
  {
    std::cerr << "send error"<<std::endl;
    exit(1);
  }

  return sendByteLen;
}


