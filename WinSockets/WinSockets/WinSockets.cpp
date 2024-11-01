#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <chrono>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define LOCAL_IP "127.0.0.1"

int Server() {

  WSAData wsa_data;
  int error_status; 

  addrinfo *result = NULL;
  addrinfo hints;

  SOCKET server_socket;
  SOCKET client_socket;
  
  int buffer_size = 1024;
  int packet_size = 0;

  std::vector<char> server_buffer(buffer_size), client_buffer(buffer_size);
  std::string message;


  error_status = WSAStartup(MAKEWORD(2, 2), &wsa_data);

  if (error_status != 0) {
	std::cout << "WSAStartup failed :(" << std::endl;
	std::cout << WSAGetLastError() << std::endl;
	WSACleanup();
	return 1;
  }
  else {
	std::cout << "WSAStartup is successful :), you can't tell the same about your life can you?"
	  << std::endl;
  }


  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  error_status = getaddrinfo(NULL, "80", &hints, &result);
  if (error_status != 0) {
	std::cout << "Error defining socket parameters D:" << std::endl;
	std::cout << "Error #" << WSAGetLastError() << std::endl;
	WSACleanup();
	return 1;
  }
  else {
	std::cout << "Socket parameters successfully defined" << std::endl;
  }

  server_socket = socket(
	result->ai_family, 
	result->ai_socktype, 
	result->ai_protocol
  );

  if (server_socket == INVALID_SOCKET) {
	std::cout << "Socket initialization error #" << WSAGetLastError() << std::endl;
	closesocket(server_socket);
	WSACleanup();
	return 1;
  }
  else {
	std::cout << "Socket initialization successful" << std::endl;
  }

  error_status = bind(server_socket, result->ai_addr, result->ai_addrlen);
  if (error_status == SOCKET_ERROR) {
	std::cout << "Binding socket error #" << WSAGetLastError() << std::endl;
	freeaddrinfo(result);
	closesocket(server_socket);
	WSACleanup();
	return 1;
  }
  else {
	freeaddrinfo(result);
	std::cout << "binding socket is successful" << std::endl;
  }

  error_status = listen(server_socket, SOMAXCONN);

  if (error_status != 0) {
	std::cout << "Listen failed error #" << WSAGetLastError() << std::endl;
	closesocket(server_socket);
	WSAGetLastError();
	return 1;
  }
  else {
	std::cout << "Listening..." << std::endl;
  }

  sockaddr_in client_info;
  int client_info_size = sizeof(client_info);


  ZeroMemory(&client_info, client_info_size);


  client_socket = accept(server_socket, (sockaddr*)&client_info, &client_info_size);

  if (client_socket == INVALID_SOCKET) {
	std::cout << "Client detected, but can't connect to a client. Error # "
	  << WSAGetLastError() << std::endl;
	closesocket(server_socket);
	closesocket(client_socket);
	WSACleanup();
	return 1;
  }
  else {
	std::cout << "Connection to a client is successful" << std::endl;
  }

  while (true) {
	packet_size = recv(client_socket, server_buffer.data(), server_buffer.size(), 0);
	message = server_buffer.data();
	std::cout << "Client's message: " << message << std::endl;

	std::cout << "Your (host) message: ";
	std::getline(std::cin, message);
	std::vector<char> convert(message.begin(), message.end());
	client_buffer = convert;

	packet_size = send(client_socket, client_buffer.data(), client_buffer.size(), 0);

	if (packet_size == SOCKET_ERROR) {
	  std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
	  closesocket(client_socket);
	  closesocket(server_socket);
	  WSACleanup();
	  return 1;
	}
  }

  closesocket(server_socket);
  WSACleanup();

  return 0;
}

int main() {
  
  Server();



}