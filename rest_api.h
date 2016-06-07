/*
   "rest_api.h"

   Description: This inlcudes functions that make REST calls to Intel Cloud Analytics.
 
 Edit: Changes for get_observations function. Enables ability to obtain up to 5 observations at one run.
   Edited by: Shuo Wang Hsu
   shuowanghsu@gmail.com
  
   original file created by In Hwan "Chris" Baek
   UCLA Wireless Health Institute
   chris.inhwan.baek@gmail.com
*/

#ifndef _REST_API_H_
#define _REST_API_H_

void get_user_token(char*, char*);
void get_user_id();
void get_account_id(char*);
void create_device(char*, char*, char*);
void generate_activation_code(char*);
void activate_device(char*, char*);
void list_all_devices(char* );
long * get_observations(char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, int);
void send_actuation_led(char*, char*, char*);
void observation_triggered_actuation_demo(char*, char*, char*, char*);
void get_user_header();

#endif
