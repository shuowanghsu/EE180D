/*
   "request.h"

   prerequisite: installation of curl
   Description: This inlcudes functions that make "GET", "POST", and "PUT" requests.   
		These requests are required to make REST calls.

   created by In Hwan "Chris" Baek
   UCLA Wireless Health Institute
   chris.inhwan.baek@gmail.com
*/

#ifndef _REQUEST_H_
#define _REQUEST_H_

extern int display_flag;
void request_with_data(char*, char*, char*, char*);
void request_without_data(char*, char*, char*);
void request_with_data_two_headers(char*, char*, char*, char*, char*);
void request_without_data_two_headers(char*, char*, char*, char*);

#endif
