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
#include <iostream>
#include <string>
#include "clientsocket.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }
    
    int len;
    string message;
    char line[256];
    ClientSocket* csocket = new ClientSocket();
    Stream* tcp_stream = csocket->connect(argv[2], atoi(argv[1]));
    if (tcp_stream) {
        printf("Enter message: ");
        getline(cin, message);
        tcp_stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = tcp_stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete tcp_stream;
    }

    exit(0);
}