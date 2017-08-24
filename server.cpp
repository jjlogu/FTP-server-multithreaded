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

#include <iostream>
#include <utility>
#include "serversocket.h"
#include "ftp.h"

using namespace std;

void* ftp_handler(void* arg) {
  FTPHandle* ftp_handle =(FTPHandle*)arg;
  pair<string, string> command;
  cout<<"Connected: "<<ftp_handle->get_cstream()->getPeerIP()<<endl;
  cout<<"Number of active threads : "<<FTPHandle::get_active_handles_count()<<endl;
  ftp_handle->send_greeting();
  for(;;) {
    command = ftp_handle->read_command();
    //printf("%s %s\n", command.first.c_str(), command.second.c_str());
    if("USER" == command.first)
      ftp_handle->allow_user(command.second); // by default allow all user
    else if("SYST" == command.first)
      ftp_handle->send_syst();
    else if("QUIT" == command.first)
      break;
    else
      ftp_handle->send_bad_command();
  }
  cout<<"Disconnecting: "<<ftp_handle->get_cstream()->getPeerIP()<<endl;
  delete ftp_handle;
  return NULL;
}

int main(int argc, char** argv)
{
  if (argc < 2 || argc > 4) {
    cout<<"usage: server <port> [<ip>]\n";
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
        pthread_t pid;
        if(pthread_create(&pid, NULL, &ftp_handler, (void*)new FTPHandle(tcp_stream))  != 0) {
          perror("Error creating thread\n");
          delete tcp_stream;
        }
      }
    }
  }
  exit(0);
}