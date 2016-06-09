/*
   "rest_api.c"

   Description: This inlcudes functions that make REST calls to Intel Cloud Analytics.
   
   Edit: Multiple changes made across file. Most changes happened in get_observations function. Enables ability to 
		 obtain up to 5 observations at one run.
   Edited by: Shuo Wang Hsu
   shuowanghsu@gmail.com
   
   original file created by In Hwan "Chris" Baek
   UCLA Wireless Health Institute
   chris.inhwan.baek@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "rest_api.h"
#include "request.h"

#define MAX_LEN 1024

const char* base_url = "https://dashboard.us.enableiot.com/v1/api";


void get_user_token(char* username, char* password) 
{
	char *header;
	char url[MAX_LEN];
	char data[MAX_LEN];

	//format URL
	snprintf(url, sizeof(url), "%s/auth/token", base_url);
	header = "Content-Type:application/json";
	//Data to send	
	//switch to Langston's json formatting code!
	snprintf(data, sizeof(data), "{\"username\": \"%s\", \"password\": \"%s\"}", username, password);

	//make a request
	request_with_data("POST", url, header, data);
	
	//Save user token in a text file and print to stdout
	system("grep token response_5.txt | sed 's/{\"token\":\"//' | sed 's/\"}//' > user_token.txt");
}


void get_user_id()
{
	FILE *user_header_txt;
	char header1[MAX_LEN];
	char header2[MAX_LEN];
	char url[MAX_LEN];
	int i;

	//format URL
	snprintf(url, sizeof(url), "%s/auth/tokenInfo", base_url);
	
	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);
        for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//make a request
	request_without_data_two_headers("GET", url, header1, header2);
	
	//Save user ID in a text file
	system("sed 's/\"/ /g' response_5.txt | tr ' ' '\n' | awk '/sub/{getline; getline; print}' > user_id.txt");
		
}

void get_account_id(char* account_name)
{
	FILE *user_header_txt, *user_id_txt;
	char user_id[MAX_LEN];
	char header1[MAX_LEN];
	char header2[MAX_LEN];
	char url[MAX_LEN];
	char cmd[MAX_LEN];
	int i;

	//get user id
	user_id_txt = fopen("./user_id.txt", "r");
	if (user_id_txt == NULL) {
		get_user_id();
		user_id_txt = fopen("./user_id.txt", "r");
	}	
	fgets(user_id, sizeof(user_id), user_id_txt);
	fclose(user_id_txt);

	//format URL
	snprintf(url, sizeof(url), "\"%s/users/%s\"", base_url, user_id);
	
	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}

	fgets(header1, sizeof(header1), user_header_txt);
        for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}	
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//make a request
	request_without_data_two_headers("GET", url, header1, header2);
	
	//Save account ID in a text file
	snprintf(cmd, sizeof(cmd), "sed 's/\"/ /g' response_5.txt | tr ' ' '\n' | grep -B 4 %s | sed -n 1p > %s_account_id.txt", 
		account_name, account_name);
	system(cmd);		
}

void create_device(char* account_name, char* device_id, char* device_name)
{
	FILE *user_header_txt, *account_id_txt;
	char account_id[MAX_LEN];
	char header1[MAX_LEN];
	char header2[MAX_LEN];
	char url[MAX_LEN];
	char filename[MAX_LEN];
	char data[MAX_LEN];
	int i;

	//get account id
	snprintf(filename, sizeof(filename), "%s_account_id.txt", account_name);
	account_id_txt = fopen(filename, "r");
	if (account_id_txt == NULL) {
		get_account_id(account_name);
		account_id_txt = fopen(filename, "r");
	}
	fscanf(account_id_txt, "%s", account_id);
	printf("account id: %s\n", account_id);
	fclose(account_id_txt);

	//format URL
	snprintf(url, sizeof(url), "%s/accounts/%s/devices", base_url, account_id);

	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);
        for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//format data in JSON
	snprintf(data, sizeof(data), "{\"deviceId\":\"%s\", \"gatewayId\": \"%s\", \"name\": \"%s\"}", device_id, device_id, device_name);

	//make a request
	request_with_data_two_headers("POST", url, header1, header2, data);
		
}

void generate_activation_code(char* account_name)
{
	FILE *user_header_txt, *account_id_txt;
	char *cmd_temp;
	char account_id[MAX_LEN];
	char header1[MAX_LEN];
	char header2[MAX_LEN];
	char url[MAX_LEN];
	char filename[MAX_LEN]; //account id txt file
	char cmd[MAX_LEN];
	int i;

	//get account id
	snprintf(filename, sizeof(filename), "%s_account_id.txt", account_name);
	account_id_txt = fopen(filename, "r");
	if (account_id_txt == NULL) {
		get_account_id(account_name);
		account_id_txt = fopen(filename, "r");
	}
	fscanf(account_id_txt, "%s", account_id);
	printf("account id: %s\n", account_id);
	fclose(account_id_txt);

	//format URL
	snprintf(url, sizeof(url), "%s/accounts/%s/activationcode/refresh", base_url, account_id);

	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);
        for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//make a request
	request_without_data_two_headers("PUT", url, header1, header2);

	//Save activation code in a text file
	cmd_temp = "grep activ response_5.txt | sed 's/\"/ /g' | tr ' ' '\n' | awk '/activationCode/{getline; getline; print}'";
	snprintf(cmd, sizeof(cmd), "%s > %s_activation_code.txt", cmd_temp, account_name); 
	system(cmd);	
}

void activate_device(char* account_name, char* device_id)
{
	FILE *user_header_txt, *account_id_txt, *activation_code_txt;
	char account_id[MAX_LEN];
	char activation_code[MAX_LEN];
	char header1[MAX_LEN]; //authorization header
	char header2[MAX_LEN]; //content type header
	char url[MAX_LEN];
	char accnt_id_filename[MAX_LEN]; //acount id txt file
	char activ_code_filename[MAX_LEN]; //activation code txt file
	char data[MAX_LEN];
	char cmd[MAX_LEN];
	char* cmd_temp = "grep deviceToken response_5.txt | sed 's/\"/ /g' | tr ' ' '\n' | awk '/deviceToken/{getline; getline; print}'";
	int i;

	//get account id
	snprintf(accnt_id_filename, sizeof(accnt_id_filename), "%s_account_id.txt", account_name);
	account_id_txt = fopen(accnt_id_filename, "r");
	if (account_id_txt == NULL) {
		get_account_id(account_name);
		account_id_txt = fopen(accnt_id_filename, "r");
	}
	fscanf(account_id_txt, "%s", account_id);
	fclose(account_id_txt);

	//format URL
	snprintf(url, sizeof(url), "%s/accounts/%s/devices/%s/activation", base_url, account_id, device_id);

	//get headers
	get_user_header();	
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);        
	for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//get activation code associated with the account
	//Because activation code expires, we need to get a fresh activation code.
	generate_activation_code(account_name);
	snprintf(activ_code_filename, sizeof(activ_code_filename), "%s_activation_code.txt", account_name);
	activation_code_txt = fopen(activ_code_filename, "r");
	fscanf(activation_code_txt, "%s", activation_code);
	printf("activation code: %s", activation_code);
	fclose(activation_code_txt);

	//format data in JSON.	
	snprintf(data, sizeof(data), "{\"activationCode\": \"%s\"}", activation_code);

	request_with_data_two_headers("PUT", url, header1, header2, data);  

	//save device token in a text file
	snprintf(cmd, sizeof(cmd), "%s > %s_device_token.txt", cmd_temp, device_id);
	system(cmd);
}

/*
void create_component(char* account_name, char* component_type_name, char* name)
{
	FILE *user_header_txt, *account_id_txt, *device_id_txt;
	char account_id[MAX_LEN];
	char device_id[MAX_LEN];
	char header1[MAX_LEN]; //authorization header
	char header2[MAX_LEN]; //content type header
	char url[MAX_LEN];
	char accnt_id_filename[MAX_LEN]; //acount id txt file
	char device_id_filename[MAX_LEN]; //activation code txt file
	char data[MAX_LEN];
	char cmd[MAX_LEN];
	char* cmd_temp = "grep deviceToken response_5.txt | sed 's/\"/ /g' | tr ' ' '\n' | awk '/deviceToken/{getline; getline; print}'";
	int i;



}
*/

void list_all_devices(char* account_name)
{
	FILE *user_header_txt, *account_id_txt, *device_list, *component_list;
	char account_id[MAX_LEN];
	char header1[MAX_LEN]; //authorization header
	char url[MAX_LEN];
	char accnt_id_filename[MAX_LEN]; //acount id txt file
	char cmd[MAX_LEN];
	char device_id[MAX_LEN];
	char component[MAX_LEN];	
	int i;

	//get account id
	snprintf(accnt_id_filename, sizeof(accnt_id_filename), "%s_account_id.txt", account_name);
	account_id_txt = fopen(accnt_id_filename, "r");
	if (account_id_txt == NULL) {
		get_account_id(account_name);
		account_id_txt = fopen(accnt_id_filename, "r");
	}
	fscanf(account_id_txt, "%s", account_id);
	fclose(account_id_txt);

	//format URL
	snprintf(url, sizeof(url), "%s/accounts/%s/devices", base_url, account_id);

	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);        
	for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fclose(user_header_txt);

	request_without_data("GET", url, header1);

	snprintf(cmd, sizeof(cmd), "sed 's/\"/ /g' response_5.txt | tr ' ' '\n' | awk '/deviceId/{getline; getline; print}'");
	device_list = popen(cmd, "r");
	while(fgets(device_id, sizeof(device_id), device_list)) {
		printf("Device 1: ");
		device_id[strlen(device_id)-1] = '\0';
		printf("%s\n", device_id);
	
		snprintf(cmd, sizeof(cmd), "sed 's/components/ /g' response_5.txt | tr ' ' '\n' | grep %s | sed 's/},{/ /g' | tr ' ' '\n' | sed 's/\"/ /g' | sed 's/}],/~/g' | tr '~' '\n' | grep type | sed 's/:\\[{ //g' | sed 's/ : /: /g' | sed 's/ , /, /g'", device_id);
		component_list = popen(cmd, "r");
		
		while(fgets(component, sizeof(component), component_list)) {
			printf("\t%s", component);
		}
		
	} 		
}

long * get_observations(char* account_name, char* device_id, char* device2_id, char* device3_id, char* device4_id, char* device5_id, char* component_id, char* component2_id , char* component3_id, char* component4_id, char* component5_id, int display)
{
	FILE *user_header_txt, *account_id_txt, *output, *output_txt;
	char account_id[MAX_LEN];
	char header1[MAX_LEN]; //authorization header
	char header2[MAX_LEN]; //content type header
	char url[MAX_LEN];
	char accnt_id_filename[MAX_LEN]; //acount id txt file
	char data[MAX_LEN];
	char response[MAX_LEN];
	char timestamp[MAX_LEN];
	char value[MAX_LEN];
	char value2[MAX_LEN];
	char value3[MAX_LEN];
	char value4[MAX_LEN];
	char value5[MAX_LEN];
	//char max_time;
	int i, j;
	static long time_val_pair[2]; 

	//get account id
	snprintf(accnt_id_filename, sizeof(accnt_id_filename), "%s_account_id.txt", account_name);
	account_id_txt = fopen(accnt_id_filename, "r");
	if (account_id_txt == NULL) {
		get_account_id(account_name);
		account_id_txt = fopen(accnt_id_filename, "r");
	}
	fscanf(account_id_txt, "%s", account_id);
	fclose(account_id_txt);

	//format URL
	snprintf(url, sizeof(url), "%s/accounts/%s/data/search", base_url, account_id);

	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);        
	for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//format data in JSON
	snprintf(data, sizeof(data), "{\"from\": -10, \"targetFilter\": { \"deviceList\": [\"%s\"]}, \"metrics\": [{ \"id\": \"%s\"}]}", device_id, component_id);

	request_with_data_two_headers("POST", url, header1, header2, data);

	//extract timestamp and value
	output = popen("grep from response_5.txt | sed -E 's/points\":/ /g' | sed -e 's/},{\"/ /g' | tr ' ' '\n' | grep ts" , "r");
	while (fgets(response, sizeof(response), output)) {
		i = 6; //skip "ts":"
		j = 0;
		while (response[i] != '\"') {
			timestamp[j] = response[i];
			//max_time = timestamp[j];
			i++;
			j++;		
		}
		timestamp[j] = '\0';
		i += 11; // skip ","value":"
		j = 0;
		while (response[i] != '\"') {
			value[j] = response[i];
			i++;
			j++;		
		}
		value[j] = '\0';
		
	}
	
	snprintf(data, sizeof(data), "{\"from\": -10, \"targetFilter\": { \"deviceList\": [\"%s\"]}, \"metrics\": [{ \"id\": \"%s\"}]}", device2_id, component2_id);

	request_with_data_two_headers("POST", url, header1, header2, data);

	//extract timestamp and value
	output = popen("grep from response_5.txt | sed -E 's/points\":/ /g' | sed -e 's/},{\"/ /g' | tr ' ' '\n' | grep ts" , "r");
	while (fgets(response, sizeof(response), output)) {
		i = 6; //skip "ts":"
		j = 0;
		while (response[i] != '\"') {
			timestamp[j] = response[i];
			//max_time = timestamp[j];
			i++;
			j++;		
		}
		timestamp[j] = '\0';
		i += 11; // skip ","value":"
		j = 0;
		while (response[i] != '\"') {
			value2[j] = response[i];
			i++;
			j++;		
		}
		value2[j] = '\0';
		
	}
	

	snprintf(data, sizeof(data), "{\"from\": -10, \"targetFilter\": { \"deviceList\": [\"%s\"]}, \"metrics\": [{ \"id\": \"%s\"}]}", device3_id, component3_id);

	request_with_data_two_headers("POST", url, header1, header2, data);

	//extract timestamp and value
	output = popen("grep from response_5.txt | sed -E 's/points\":/ /g' | sed -e 's/},{\"/ /g' | tr ' ' '\n' | grep ts" , "r");
	while (fgets(response, sizeof(response), output)) {
		i = 6; //skip "ts":"
		j = 0;
		while (response[i] != '\"') {
			timestamp[j] = response[i];
			//max_time = timestamp[j];
			i++;
			j++;		
		}
		timestamp[j] = '\0';
		i += 11; // skip ","value":"
		j = 0;
		while (response[i] != '\"') {
			value3[j] = response[i];
			i++;
			j++;		
		}
		value3[j] = '\0';
		
	}
	snprintf(data, sizeof(data), "{\"from\": -10, \"targetFilter\": { \"deviceList\": [\"%s\"]}, \"metrics\": [{ \"id\": \"%s\"}]}", device4_id, component4_id);

	request_with_data_two_headers("POST", url, header1, header2, data);

	//extract timestamp and value
	output = popen("grep from response_5.txt | sed -E 's/points\":/ /g' | sed -e 's/},{\"/ /g' | tr ' ' '\n' | grep ts" , "r");
	while (fgets(response, sizeof(response), output)) {
		i = 6; //skip "ts":"
		j = 0;
		while (response[i] != '\"') {
			timestamp[j] = response[i];
			//max_time = timestamp[j];
			i++;
			j++;		
		}
		timestamp[j] = '\0';
		i += 11; // skip ","value":"
		j = 0;
		while (response[i] != '\"') {
			value4[j] = response[i];
			i++;
			j++;		
		}
		value4[j] = '\0';
		
	}
	snprintf(data, sizeof(data), "{\"from\": -10, \"targetFilter\": { \"deviceList\": [\"%s\"]}, \"metrics\": [{ \"id\": \"%s\"}]}", device5_id, component5_id);

	request_with_data_two_headers("POST", url, header1, header2, data);

	//extract timestamp and value
	output = popen("grep from response_5.txt | sed -E 's/points\":/ /g' | sed -e 's/},{\"/ /g' | tr ' ' '\n' | grep ts" , "r");
	while (fgets(response, sizeof(response), output)) {
		i = 6; //skip "ts":"
		j = 0;
		while (response[i] != '\"') {
			timestamp[j] = response[i];
			//max_time = timestamp[j];
			i++;
			j++;		
		}
		timestamp[j] = '\0';
		i += 11; // skip ","value":"
		j = 0;
		while (response[i] != '\"') {
			value5[j] = response[i];
			i++;
			j++;		
		}
		value5[j] = '\0';
		
	}
	if (display == 1)
	{
		int data1 = atoi(value);
		int data2 = atoi(value2);
		int data3 = atoi(value3);
		int data4 = atoi(value4);
		int data5 = atoi(value5);

		printf("Value1: %d, Value2: %d, Value3: %d, Value4: %d, Value5: %d\n", data1, data2, data3, data4, data5);

	}
	pclose(output);
	

	time_val_pair[0] = atol(timestamp);
	time_val_pair[1] = atol(value);
	return time_val_pair;
}

void send_actuation_led (char* account_name, char* component_id, char* value)
{	
	FILE *user_header_txt, *account_id_txt;
	char account_id[MAX_LEN];
	char header1[MAX_LEN]; //authorization header
	char header2[MAX_LEN]; //content type header
	char url[MAX_LEN];
	char accnt_id_filename[MAX_LEN]; //acount id txt file
	char data[MAX_LEN];
	int i;

	//get account id
	snprintf(accnt_id_filename, sizeof(accnt_id_filename), "%s_account_id.txt", account_name);
	account_id_txt = fopen(accnt_id_filename, "r");
	if (account_id_txt == NULL) {
		get_account_id(account_name);
		account_id_txt = fopen(accnt_id_filename, "r");
	}
	fscanf(account_id_txt, "%s", account_id);
	fclose(account_id_txt);

	//format URL
	snprintf(url, sizeof(url), "%s/accounts/%s/control", base_url, account_id);

	//get headers
	get_user_header();
	user_header_txt = fopen("./user_header.txt", "r");	
	if (user_header_txt == NULL) {
		get_user_header();
		user_header_txt = fopen("./user_header.txt", "r");
	}
	fgets(header1, sizeof(header1), user_header_txt);        
	for (i=0; i<sizeof(header1); i++) {
		if (header1[i] == '\n') {
			header1[i] = '\0';
			break;
		}
	}
	fgets(header2, sizeof(header2), user_header_txt);
	fclose(user_header_txt);

	//format data in JSON
	snprintf(data, sizeof(data), "{\"commands\":[{\"componentId\": \"%s\", \"parameters\": [{\"name\":\"LED\", \"value\":\"%s\"}], \"transport\":\"mqtt\"}],\"complexCommands\":[ ]}", component_id, value);

	//send a request	
	request_with_data_two_headers("POST", url, header1, header2, data);

}
 
void observation_triggered_actuation_demo (char* account_name, char* device_id, char* temp_comp_id, char* led_comp_id)
{
	/*
	while (get_observations(account_name, device_id, temp_comp_id, 0, 0, 0)[1] < 100) {}
	printf("\n\nTemperature is above 100. Turning on LED...\n");
	send_actuation_led(account_name, led_comp_id, "1");
	
	while (get_observations(account_name, device_id, temp_comp_id, 0, 0, 0)[1] >= 100) {}
	printf("Temperature is below 100. Turning off LED...\n");
	send_actuation_led(account_name, led_comp_id, "0");*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

void get_user_header(){
	FILE *user_token_txt, *user_header_txt;
	char user_token[MAX_LEN];
	char user_header[MAX_LEN];

	user_token_txt = fopen("./user_token.txt", "r");

	if (user_token_txt == NULL) {
		char username[50], password[50];
		printf("Cannot find user_token.txt.\n");
		printf("Getting user token now...\n");
		printf("Enter username: ");
		scanf("%s", username);
		printf("Enter password: ");
		scanf("%s", password);
		get_user_token(username, password);
		user_token_txt = fopen("./user_token.txt", "r");		
	}
	
	fscanf(user_token_txt, "%s", user_token);
	fclose(user_token_txt);

	//format mutiple headers
	//first line: Authorization header
	//second line: Content-type header
	user_header_txt = fopen("./user_header.txt", "w");
	fprintf(user_header_txt, "Authorization: Bearer %s\nContent-Type:application/json", user_token);
	fclose(user_header_txt);
}

