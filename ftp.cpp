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

#include "ftp.h"

/* FTPResponse */
const string FTPResponse::greeting     = "220 Hello World!\012";
const string FTPResponse::bad_command  = "451 Bad Command\012";
const string FTPResponse::close        = "221 Close\012";
const string FTPResponse::system_str   = "215 Unknown\012"; // TODO: system_str need to be dynamic
const string FTPResponse::allow_user   = "230 Hey";

/* FTPHandle */
FTPHandle::FTPHandle(Stream* stream): cstream(stream) {
  pthread_mutex_lock(&mutex);
  active_handles_count++;
  pthread_mutex_unlock(&mutex);
}

FTPHandle::~FTPHandle() {
  pthread_mutex_lock(&mutex);
  active_handles_count--;
  pthread_mutex_unlock(&mutex);
  delete cstream; // close stream
}

size_t FTPHandle::active_handles_count = 0; // initialize count
pthread_mutex_t FTPHandle::mutex;

Stream* FTPHandle::get_cstream() {
  return cstream;
}

pair<string, string> FTPHandle::command_parser(const char* request) {
  pair<string, string> req;
  regex e("(\\w+)\\s?(\\w+)?\r\n");
  cmatch cm;
  
  if( regex_match(request, cm, e) && (cm.size() == 3) ) {
    req = make_pair(cm[1], cm[2]);
  }
  else {
    req = make_pair("BAD","BAD");
  }
  
  return req;
}

pair<string, string> FTPHandle::read_command() {
  return command_parser(cstream->receive().c_str());
}

size_t FTPHandle::get_active_handles_count() {
  return active_handles_count;
}

void FTPHandle::send_greeting() {
  cstream->send(FTPResponse::greeting);
}

void FTPHandle::send_syst() {
  cstream->send(FTPResponse::system_str);
}

void FTPHandle::allow_user(const string username) {
  cstream->send(FTPResponse::allow_user + " " + username + "\012");
}

void FTPHandle::send_bad_command() {
  cstream->send(FTPResponse::bad_command);
}
