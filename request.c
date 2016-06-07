/*
   "request.c"

   Prerequisite: installation of curl
   Description: This inlcudes functions that make "GET", "POST", and "PUT" requests.   
		These requests are required to make REST calls.

   created by In Hwan "Chris" Baek
   UCLA Wireless Health Institute
   chris.inhwan.baek@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "request.h"

int display_flag;

void request_with_data(char* request_type, char* url, char* header, char* data)
{
	char *type_parameter;
  	char path[1035];
	char curl_command[1024];
	char command2run[1024];
	
	if (!strcmp(request_type, "GET")) {
		type_parameter = "";
	}
	else if (!strcmp(request_type, "POST")) {
		type_parameter = "-i -X POST ";
	}
	else if (!strcmp(request_type, "PUT")) {
		type_parameter = "-i -X PUT ";
	}
	
	snprintf(curl_command, sizeof(curl_command), "curl -s %s-H \"%s\" %s -d '%s'", type_parameter, header, url, data);
	
	//run curl command and save the response to a text file.
	snprintf(command2run, sizeof(command2run), "%s > response_5.txt", curl_command);
	system(command2run);

	if (display_flag == 1) {
		printf("\n\n=========================== Running curl command... ============================================\n");
		printf("%s\n", curl_command);

		printf("\n\n============================ Response from Cloud ===============================================\n");	
		system("cat response_5.txt");
		printf("\n\n");
	}

	//log
	system("echo $'\n\n=====================================================' >> rest_log.txt");
	system("echo Logged at $(date) >> rest_log.txt");
	system("echo ===================================================== >> rest_log.txt");
	system("cat response_5.txt >> rest_log.txt");
}

void request_without_data(char* request_type, char* url, char* header)
{
	char *type_parameter;
  	char path[1035];
	char curl_command[1024];
	char command2run[1024];
	
	if (!strcmp(request_type, "GET")) {
		type_parameter = "";
	}
	else if (!strcmp(request_type, "POST")) {
		type_parameter = "-i -X POST ";
	}
	else if (!strcmp(request_type, "PUT")) {
		type_parameter = "-i -X PUT ";
	}
	
	snprintf(curl_command, sizeof(curl_command), "curl -s %s-H \"%s\" %s", type_parameter, header, url);

	//run curl command and save the response to a text file.
	snprintf(command2run, sizeof(command2run), "%s > response_5.txt", curl_command);
	system(command2run);

	if (display_flag == 1) {
		printf("\n\n=========================== Running curl command... ============================================\n");
		printf("%s\n", curl_command);

		printf("\n\n============================ Response from Cloud ===============================================\n");	
		system("cat response_5.txt");
		printf("\n\n");
	}

	//log
	system("echo $'\n\n=====================================================' >> rest_log.txt");
	system("echo Logged at $(date) >> rest_log.txt");
	system("echo ===================================================== >> rest_log.txt");
	system("cat response_5.txt >> rest_log.txt");
}

void request_with_data_two_headers(char* request_type, char* url, char* header1, char* header2, char* data)
{
	char *type_parameter;
  	char path[1035];
	char curl_command[1024];
	char command2run[1024];
	
	if (!strcmp(request_type, "GET")) {
		type_parameter = "";
	}
	else if (!strcmp(request_type, "POST")) {
		type_parameter = "-i -X POST ";
	}
	else if (!strcmp(request_type, "PUT")) {
		type_parameter = "-i -X PUT ";
	}
	
	snprintf(curl_command, sizeof(curl_command), "curl -s %s-H \"%s\" -H \"%s\" %s -d '%s'", type_parameter, header1, header2, url, data);

	//run curl command and save the response to a text file.
	snprintf(command2run, sizeof(command2run), "%s > response_5.txt", curl_command);
	system(command2run);

	if (display_flag == 1) {
		printf("\n\n=========================== Running curl command... ============================================\n");
		printf("%s\n", curl_command);

		printf("\n\n============================ Response from Cloud ===============================================\n");	
		system("cat response_5.txt");
		printf("\n\n");
	}

	//log
	system("echo $'\n\n=====================================================' >> rest_log.txt");
	system("echo Logged at $(date) >> rest_log.txt");
	system("echo ===================================================== >> rest_log.txt");
	system("cat response_5.txt >> rest_log.txt");
}		

void request_without_data_two_headers(char* request_type, char* url, char* header1, char* header2)
{
	char *type_parameter;
  	char path[1035];
	char curl_command[1024];
	char command2run[1024];
	
	if (!strcmp(request_type, "GET")) {
		type_parameter = "";
	}
	else if (!strcmp(request_type, "POST")) {
		type_parameter = "-i -X POST ";
	}
	else if (!strcmp(request_type, "PUT")) {
		type_parameter = "-i -X PUT ";
	}
	
	snprintf(curl_command, sizeof(curl_command), "curl -s %s-H \"%s\" -H \"%s\" %s", type_parameter, header1, header2, url);

	//run curl command and save the response to a text file.
	snprintf(command2run, sizeof(command2run), "%s > response_5.txt", curl_command);
	system(command2run);

	if (display_flag == 1) {
		printf("\n\n=========================== Running curl command... ============================================\n");
		printf("%s\n", curl_command);

		printf("\n\n============================ Response from Cloud ===============================================\n");	
		system("cat response_5.txt");
		printf("\n\n");
	}

	//log
	system("echo $'\n\n=====================================================' >> rest_log.txt");
	system("echo Logged at $(date) >> rest_log.txt");
	system("echo ===================================================== >> rest_log.txt");
	system("cat response_5.txt >> rest_log.txt");
}		
