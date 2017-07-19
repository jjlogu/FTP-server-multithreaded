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

#ifndef __serversocket_h__
#define __serversocket_h__

#include <string>
#include <netinet/in.h>
#include "stream.h"

using namespace std;

class ServerSocket {

  private:
    int    lsd;
    int    port;
    string ip_address;
    bool   listening;

    ServerSocket() {}

  public:
    ServerSocket(int port, const char* address="");
    ~ServerSocket();

    int start();
    Stream* accept();

};

#endif