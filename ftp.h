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

#ifndef __ftp_h__
#define __ftp_h__

#include <utility>
#include <string>
#include <regex>
#include <pthread.h>
#include "stream.h"
using namespace std;

class FTPResponse {
  public:
    static const string greeting;
    static const string bad_command;
    static const string allow_user;
    static const string system_str;
    static const string close;
};

class FTPHandle {
  private:
    Stream* cstream; // control stream
    static size_t active_handles_count; //thread count
    static pthread_mutex_t mutex;
    FTPHandle();
  public:
    FTPHandle(Stream* stream);
    ~FTPHandle();
    Stream* get_cstream();
    pair<string, string> command_parser(const char* request);
    pair<string, string> read_command();
    static size_t get_active_handles_count();
    void send_greeting();
    void send_syst();
    void allow_user(const string username);
    void send_bad_command();
};

#endif