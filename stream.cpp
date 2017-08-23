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
#include <cstdio>
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

void Stream::send(const string& str) {
  int left_chars = str.length();
	int written_chars = 0;

	int temp;
	const char* buffer = str.c_str();

	// This will write untill the no of characters specified have been written
	while( left_chars > 0 )
	{
		if( (temp = this->send(buffer + written_chars, left_chars)) <= 0 )
		{
			// Error
			if( temp < 0 && errno == EINTR )
			{
				continue;
			}
			else
			{
				perror("Error with writing\n");
				break;
			}
		}
		written_chars += temp;
		left_chars -= left_chars;
	}
}

ssize_t Stream::receive(char* buffer, size_t len) {
  return read(sd, buffer, len);
}

void Stream::receive(string* rcv) {
  const unsigned int MAX_BUF_LENGTH = 4096;
  vector<char> buffer(MAX_BUF_LENGTH);
  int bytesReceived = 0;
  do {
      bytesReceived = read(sd, buffer.data(), buffer.size());
      // append string from buffer.
      if ( bytesReceived == -1 ) { 
        printf("Error while read as string\n");
      } else {
        rcv->append( buffer.cbegin(), buffer.cend() );
      }
  } while ( bytesReceived == MAX_BUF_LENGTH );
}

string Stream::getPeerIP() {
  return peerIP;
}

int Stream::getPeerPort() {
  return peerPort;
}