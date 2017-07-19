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
#include "serversocket.h"

using namespace std;

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
                ssize_t len;
                char line[256];
                while ((len = tcp_stream->receive(line, sizeof(line))) > 0) {
                    line[len] = 0;
                    printf("received - %s\n", line);
                    tcp_stream->send(line, len);
                }
                delete tcp_stream;
            }
        }
    }
    exit(0);
}