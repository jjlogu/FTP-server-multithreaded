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

#include <arpa/inet.h>
#include "stream.h"

Stream::Stream(int sd, struct sockaddr_in* address) : sd(sd) {
    char ip[50];
    inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
    peerIP = ip;
    peerPort = ntohs(address->sin_port);
}

Stream::~Stream() {
    close(sd);
}

ssize_t Stream::send(const char* buffer, size_t len) {
    return write(sd, buffer, len);
}

ssize_t Stream::receive(char* buffer, size_t len) {
    return read(sd, buffer, len);
}

string Stream::getPeerIP() {
    return peerIP;
}

int Stream::getPeerPort() {
    return peerPort;
}