/*
   "rest_main.c"

   Description: A simple code that lets users make REST calls to Intel Cloud Analytics. 
	
    Edit: Multiple changes made across file.Enables ability to code and obtain up to 5 observations at one run.
   Edited by: Shuo Wang Hsu
   shuowanghsu@gmail.com
   
   created by In Hwan "Chris" Baek
   UCLA Wireless Health Institute
   chris.inhwan.baek@gmail.com
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "rest_api.h"
#include "request.h"

int main()
{
	char username[50] = "/*input username*/";
	char password[50] = "/*input password*/";
	char account_name[50] = "/*input account name*/";
	char device_name[50]; 
	char device_id[50] = "/*device 1 id*/";
	char device2_id[50] = "/*device 2 id*/";
	char device3_id[50] = "/*device 3 id*/";
	char device4_id[50] = "/*device 4 id*/";
	char device5_id[50] = "/*device 5 id*/";
	char component_id[50] ="/*component 1 id*/";
	char component2_id[50] = "/*component 2 id*/";
	char component3_id[50] = "/*component 3 id*/";
	char component4_id[50] = "/*component 4 id*/";
	char component5_id[50] = "/*component 5 id*/";
	
	char value[50];
	char display;
	int request_type;
	
	int classification = 0;

	printf("\n\n==========================================================\n");
	printf("=========== Posture Monitoring Data Retrieval ============\n");
	printf("==========================================================\n\n");

	display_flag = 0; //do not show the output
	get_user_token(username, password);
	get_user_id();
	get_account_id(account_name);
	while(1)
	{
		get_observations(account_name, device_id, device2_id, device3_id, device4_id, device5_id, component_id, component2_id, component3_id, component4_id, component5_id, 1);
	
	}

	return 0;
}
		 
