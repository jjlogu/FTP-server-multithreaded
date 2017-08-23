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
#include <cstring>
#include <arpa/inet.h>
#include "serversocket.h"

ServerSocket::ServerSocket(int port, const char* address)
    : lsd(0), port(port), ip_address(address), listening(false) {} 

ServerSocket::~ServerSocket()
{
    if (lsd > 0) {
        close(lsd);
    }
}

int ServerSocket::start()
{
    if (listening == true) {
        return 0;
    }

    lsd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_port = htons(port);
    if (ip_address.size() > 0) {
        inet_pton(PF_INET, ip_address.c_str(), &(address.sin_addr));
    }
    else {
        address.sin_addr.s_addr = INADDR_ANY;
    }
    
    int optval = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval); 
    
    int result = bind(lsd, (struct sockaddr*)&address, sizeof(address));
    if (result != 0) {
        perror("bind() failed");
        return result;
    }
    
    result = listen(lsd, 5);
    if (result != 0) {
        perror("listen() failed");
        return result;
    }
    listening = true;
    return result;
}

Stream* ServerSocket::accept()
{
    if (listening == false) {
        return NULL;
    }

    struct sockaddr_in address;
    socklen_t len = sizeof(address);
    memset(&address, 0, sizeof(address));
    int sd = ::accept(lsd, (struct sockaddr*)&address, &len);
    if (sd < 0) {
        perror("accept() failed");
        return NULL;
    }
    return new Stream(sd, &address);
}