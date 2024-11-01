#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <zlib.h>

std::vector<std::string> TokenizeEncodings(std::string string_of_encodings) {
  std::vector<std::string> encodings;
  int num_of_encodings = 1;
  string_of_encodings == "encoding-1, encoding-2, encoding-3";

  //TODO refactor this shit

  for (int i = 0; i < string_of_encodings.size(); i++) {
    if (string_of_encodings[i] == ',') 
      num_of_encodings++;
  }

  while (num_of_encodings--) {
    encodings.push_back(string_of_encodings.substr(0, string_of_encodings.find(',')));
    string_of_encodings.erase(string_of_encodings.begin(), string_of_encodings.begin() + string_of_encodings.find(',') + 2);
  }

  return encodings;
}

std::string CompressGzip(std::string &data) {

  z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error("deflateInit2 failed while compressing.");
    }
    zs.next_in = (Bytef *)data.data();
    zs.avail_in = data.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        throw std::runtime_error("Exception during zlib compression: (" + std::to_string(ret) + ") " + zs.msg);
    }
    return outstring;

}

void ParseMessage(int &client_fd, std::vector<char> &server_buffer, std::string dir) {

  std::string response;
  std::string respBody;

  int packet_size = recv(client_fd, server_buffer.data(), server_buffer.size(), 0);

  std::string message(server_buffer.begin(), server_buffer.end());

  if (packet_size < 0) return;

  //TODO refactor this shit

  if (message.starts_with("GET / HTTP/1.1")) {
    response = "HTTP/1.1 200 OK\r\n\r\n";
    packet_size = send(client_fd, response.data(), response.size(), 0);
  } 
  else if (message.starts_with("GET /echo")) {
    respBody = message.substr(10);
    respBody = respBody.substr(0, respBody.find(" "));
    response =
        "HTTP/1.1 200 OK\r\nContent-Type: "
        "text/plain\r\nContent-Length: " +
        std::to_string(respBody.size()) + "\r\n\r\n" + respBody;

    if (message.find("Accept-Encoding") != std::string::npos) {
      std::string compressions = message.substr(message.find("Accept-Encoding: ") + 17);
      compressions = compressions.substr(0, compressions.find('\r'));

      std::vector<std::string> encodings = TokenizeEncodings(compressions);
      std::vector<std::string>::iterator it = std::find(encodings.begin(), encodings.end(), "gzip");

      respBody = CompressGzip(respBody);

      if (it != encodings.end())
        response = "HTTP/1.1 200 OK\r\nContent-Type: "
          "text/plain\r\nContent-Encoding: " + encodings[it - encodings.begin()] + "\r\nContent-Length: " +
          std::to_string(respBody.size()) + "\r\n\r\n" + respBody;
    }
    packet_size = send(client_fd, response.data(), response.size(), 0);
  } 
  else if (message.starts_with("GET /user-agent")) {
    int pos = message.find("User-Agent: ") + 12;
    respBody = message.substr(pos);
    respBody = respBody.substr(0, respBody.find(13));
    response =
        "HTTP/1.1 200 OK\r\nContent-Type: "
        "text/plain\r\nContent-Length: " +
        std::to_string(respBody.size()) + "\r\n\r\n" + respBody;
    packet_size = send(client_fd, response.data(), response.size(), 0);
  }
  else if (message.starts_with("GET /files/")) {
    std::string path = message.substr(11);
    int pos = path.find(" ");
    path = path.substr(0, pos);
    path = dir + path;
    std::fstream fp;
    fp.open(path);

    int data_size;
    std::string data;

    if (!fp.is_open()) {
      std::cout << "file didn't open" << std::endl;
      response = "HTTP/1.1 404 Not Found\r\n\r\n";
      packet_size = send(client_fd, response.data(), response.size(), 0);
    }
    else {
      std::cout << "file is opening" << std::endl;
    }

    while (!fp.eof()) {
      std::getline(fp, data);
    }

    data_size = data.size();
    fp.close();

    response = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: " +
    std::to_string(data_size) + "\r\n\r\n" + data;

    packet_size = send(client_fd, response.data(), response.size(), 0);
  }
  else if (message.starts_with("POST /files/")) {
    std::string path = message.substr(12);
    path = path.substr(0, path.find(" "));
    path = dir + path;
    std::ofstream fp;
    fp.open(path);
    
    std::string data = message.substr(message.find("Content-Length: ") + 16);
    data = data.substr(data.find("\r\n\r\n") + 4);
    data = data.substr(0, data.find('\x00'));
    fp << data;
    fp.close();

    response = "HTTP/1.1 201 Created\r\n\r\n";

    packet_size = send(client_fd, response.data(), response.size(), 0);
  }
  else {
    response = "HTTP/1.1 404 Not Found\r\n\r\n";
    packet_size = send(client_fd, response.data(), response.size(), 0);
  }

}

int main(int argc, char **argv) {
  std::cout << "Logs from your program will appear here!" << std::endl;

  int packet_size = 0;
  int error_code;
  int buf_size = 256;
  std::vector<char> server_buffer(buf_size), client_buffer(buf_size);

  std::string dir;
  if (argc == 3 && strcmp(argv[1], "--directory") == 0)
  {
  	dir = argv[2];
  }

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket" << std::endl;
    return 1;
  } 
  else {
    std::cout << "Server socket created successfully" << std::endl;
  }

  // since the tester restarts your program quite often, setting so_reuseaddr
  // ensures that we don't run into 'address already in use' errors
  int reuse = 1;
  error_code =
      setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  if (error_code < 0) {
    std::cerr << "setsockopt failed" << std::endl;
    return 1;
  } 
  else {
    std::cout << "setsockopt successful" << std::endl;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  error_code = bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr));
  if (error_code != 0) {
    std::cerr << "failed to bind to port 4221" << std::endl;
    return 1;
  }

  int connection_backlog = 5;
  error_code = listen(server_fd, connection_backlog);
  if (error_code != 0) {
    std::cerr << "listen failed" << std::endl;
    return 1;
  }

  sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  std::cout << "waiting for a client to connect..." << std::endl;

  int client_fd;

  while (true) {
    client_fd = accept(server_fd, (sockaddr *)&client_addr,
                       (socklen_t *)&client_addr_len);

    if (client_fd < 0) {
      std::cerr << "error handling client connection" << std::endl;
      close(server_fd);
      return 1;
    }
    else {
      std::cout << "client connected" << std::endl;
    }

    if (!fork())
		{
      close(server_fd);
			ParseMessage(client_fd, server_buffer, dir);
			close(client_fd);
			exit(0);
		}
		close(client_fd);
  }

  return 0;

}