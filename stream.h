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

#ifndef __stream_h__
#define __stream_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <vector>

using namespace std;

class Stream {
  private:
    int sd;     // socket descriptor
    string peerIP;
    int peerPort;

    Stream(int sd, struct sockaddr_in* address);
    Stream();
    Stream(const Stream& stream);

  public:
    friend class ServerSocket;
    friend class ClientSocket;

    ~Stream();

    ssize_t send(const char* buffer, size_t len);
    void send(const string& str);
    ssize_t receive(char* buffer, size_t len);
    void receive(string* rcv);

    string getPeerIP();
    int    getPeerPort();

};

#endif