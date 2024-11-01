#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")


int main(void) {
  //Key constants
  const char SERVER_IP[] = "127.0.0.1";					// Enter IPv4 address of Server
  const short SERVER_PORT_NUM = 80;				// Enter Listening port on Server side
  const short BUFF_SIZE = 1024;					// Maximum size of buffer for exchange info between server and client

  // Key variables for all program
  int erStat;										// For checking errors in sockets functions

  //IP in string format to numeric format for socket functions. Data is in "ip_to_num"
  in_addr ip_to_num;
  inet_pton(AF_INET, SERVER_IP, &ip_to_num);


  // WinSock initialization
  WSADATA wsData;
  erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

  if (erStat != 0) {
	std::cout << "Error WinSock version initializaion #";
	std::cout << WSAGetLastError();
	return 1;
  }
  else
	std::cout << "WinSock initialization is OK" << std::endl;

  // Socket initialization
  SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

  if (ClientSock == INVALID_SOCKET) {
	std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
	closesocket(ClientSock);
	WSACleanup();
  }
  else
	std::cout << "Client socket initialization is OK" << std::endl;

  // Establishing a connection to Server
  sockaddr_in servInfo;

  ZeroMemory(&servInfo, sizeof(servInfo));

  servInfo.sin_family = AF_INET;
  servInfo.sin_addr = ip_to_num;
  servInfo.sin_port = htons(SERVER_PORT_NUM);

  erStat = connect(ClientSock, (sockaddr *) &servInfo, sizeof(servInfo));

  if (erStat != 0) {
	std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
	closesocket(ClientSock);
	WSACleanup();
	return 1;
  }
  else
	std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << std::endl;



  std::vector<char> server_buffer(BUFF_SIZE), client_buffer(BUFF_SIZE);	
  std::string message = "";
  short packet_size = 0;											

  while (true) {

	std::cout << "Your (Client) message to Server: ";
	std::getline(std::cin, message);
	std::vector<char> convert(message.begin(), message.end());
	client_buffer = convert;
	
	packet_size = send(ClientSock, client_buffer.data(), client_buffer.size(), 0);

	if (packet_size == SOCKET_ERROR) {
	  std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
	  closesocket(ClientSock);
	  WSACleanup();
	  return 1;
	}

	packet_size = recv(ClientSock, server_buffer.data(), server_buffer.size(), 0);

	if (packet_size == SOCKET_ERROR) {
	  std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
	  closesocket(ClientSock);
	  WSACleanup();
	  return 1;
	}
	else
	  std::cout << "Server message: " << server_buffer.data() << std::endl;

  }

  closesocket(ClientSock);
  WSACleanup();

  return 0;
}