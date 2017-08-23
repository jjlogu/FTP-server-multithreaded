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
#include <regex>

/* FTPResponse */
const string FTPResponse::greeting     = "220 Hello World!\012";
const string FTPResponse::bad_command  = "451 Bad Command\012";
const string FTPResponse::close        = "221 Close\012";
const string FTPResponse::system_str   = "215 Ubuntu\012"; // TODO: system_str need to be dynamic
const string FTPResponse::allow_user   = "230 Hey";

/* FTPHandle */
FTPHandle::FTPHandle(Stream* stream): stream(stream) {}

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