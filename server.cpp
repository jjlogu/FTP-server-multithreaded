/*
   Copyright 2017 Logu J

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <cstdio>
#include <cstdlib>
#include <utility>
#include "serversocket.h"
#include "ftp.h"

using namespace std;

void ftp_handler(Stream* tcp_stream) {
  //ssize_t len;
  //char line[256];
  string str;
  /* while ((len = tcp_stream->receive(line, sizeof(line))) > 0) {
    line[len] = 0;
    printf("received - %s\n", line);
    tcp_stream->send(line, len);
  } */
  pair<string, string> request;
  FTPHandle ftp_handle(tcp_stream);
  ftp_handle.stream->send(FTPResponse::greeting);
  for(;;) {
    str.clear();
    ftp_handle.stream->receive(&str);
    request = ftp_handle.command_parser(str.c_str());
    printf("%s %s\n", request.first.c_str(), request.second.c_str());
    if("USER" == request.first)
      ftp_handle.stream->send(FTPResponse::allow_user + " " + request.second + "\012");
    else if("SYST" == request.first)
      ftp_handle.stream->send(FTPResponse::system_str);
    else if("QUIT" == request.first)
      break;
    else
      ftp_handle.stream->send(FTPResponse::bad_command);
  }
  delete tcp_stream;
}

int main(int argc, char** argv)
{
  if (argc < 2 || argc > 4) {
    printf("usage: server <port> [<ip>]\n");
    exit(1);
  }

  Stream* tcp_stream = NULL;
  ServerSocket* server = NULL;
  if (argc == 3) {
    server = new ServerSocket(atoi(argv[1]), argv[2]);
  }
  else {
    server = new ServerSocket(atoi(argv[1]));
  }
  if (server->start() == 0) {
    while (1) {
      tcp_stream = server->accept();
      if (tcp_stream != NULL) {
        ftp_handler(tcp_stream);
      }
    }
  }
  exit(0);
}