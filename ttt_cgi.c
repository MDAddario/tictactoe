/*********************************************************
* FILE: ttt_cgi.c
* 
* Takes in input from html page, parses input and passes
* it to the tic tac toe server, and returns the response.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: Jan 09 2019
***********************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){

	char* output_string = (char*)malloc(5000*sizeof(char));
	memset(output_string,'\0',5000);
	
	// Read the query string
	char query_info[5][200];
	char* data = getenv("QUERY_STRING");
	if (data == NULL){
		strcpy(output_string, "<P>FAILED TO READ QUERY STRING.");
		for (int i = 0; i<5; i++)
			query_info[i][0] = '\0';
	}
	else{
		int j;
		for (int i = 0; i<5; i++){
			
			while(*data != '=' && *data != '\0'){
				data++;
			}
			data++;
			j = 0;
			while(*data != '&' && *data != '\0'){
				query_info[i][j] = *data;
				data++;
				j++;
			}
			query_info[i][j] = '\0';
		}
	}
	
	// Format message for server
	char* message = (char*)malloc(500*sizeof(char));
	int position = 0;
	// Command
	strcpy(message, query_info[4]);
	position += strlen(query_info[4]);
	message[position] = ',';
	position++;
	// Username
	strcpy(message+position, query_info[0]);
	position += strlen(query_info[0]);
	message[position] = ',';
	position++;
	// Password
	strcpy(message+position, query_info[1]);
	position += strlen(query_info[1]);
	message[position] = ',';
	position++;
	// Board name
	strcpy(message+position, query_info[2]);
	position += strlen(query_info[2]);
	message[position] = ',';
	position++;
	// Square
	strcpy(message+position, query_info[3]);
	position += strlen(query_info[3]);
	message[position] = '\0';
	
	// Communicate with the server
	int sock;
	struct sockaddr_in server;
	int server_response = 0;
	char* incoming_msg = (char*)malloc(2000*sizeof(char));
	char* temp         = (char*)malloc(2000*sizeof(char));
	
	server.sin_addr.s_addr = inet_addr( MYIP );
	server.sin_family = AF_INET;
	server.sin_port = htons( MYPORT );
		
	while(1){
		//Create socket
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1){
			strcpy(output_string, "<P>FAILED TO CREATE SOCKET.");
			break;
		}
		
		//Connect to remote server
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
			strcpy(output_string, "<P>FAILED TO CONNECT TO SERVER.");
			break;
		}
		
		//Send some data
		if( send(sock , message , 200, 0) < 0){
			strcpy(output_string, "<P>FAILED TO SEND DATA.");
			break;
		}
		
		size_t read_size;
		int bytes_read = 0;
		int failure = 0;
		while( bytes_read < 2000 ){
			read_size = recv(sock , temp, 2000, 0);
			if(read_size <= 0){
				failure = 1;
				strcpy(output_string, "<P>SERVER DISCONNECTED.");
				fflush(stdout);
				break;
			}
			memcpy( incoming_msg+bytes_read, temp, read_size );
			bytes_read += read_size;
		}
		if (failure)
				break;
		server_response = 1;
		break;
	}
	close(sock);
	
	// Transform server output to HTML friendly
	if (server_response){
		char* in_ptr = incoming_msg;
		char* out_ptr = output_string;
		
		while(*in_ptr){
			
			if (*in_ptr == ' '){
				strcpy(out_ptr, "&ensp;");
				out_ptr += 6;
			}
			else if (*in_ptr == '-'){
				strcpy(out_ptr, "&ndash;");
				out_ptr += 7;
			}
			else if (*in_ptr == '\n'){
				strcpy(out_ptr, "<br>");
				out_ptr += 4;
			}
			else{
				*out_ptr = *in_ptr;
				out_ptr += 1;
			}
			in_ptr++;
		}
		*out_ptr = '\0';
	}

	// Format HTML form
	printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1",13,10);
	printf("<html>\n<body>\n<h1>Perfect Tic Tac Toe Play</h1>\n");
	printf("<h2>Dimension: %d</h2>\n", DIMENSION);
	printf("<form action=\"ttt.cgi\">\n");
	printf("\t<b>Username: <input type=\"text\" name=\"username\" size=\"20\" value=\"%s\"><br />\n", query_info[0]);
	printf("\t<b>Password: <input type=\"text\" name=\"password\" size=\"20\" value=\"%s\"><br />\n", query_info[1]);
	printf("\t<b>Gamename: <input type=\"text\" name=\"gamename\" size=\"20\" value=\"%s\"><br />\n", query_info[2]);
	printf("\t<b>Square: <input type=\"text\" name=\"square\" size=\"20\" value=\"%s\"><br />\n", query_info[3]);
	printf("\t<input type=\"submit\" value=\"LOGIN\" name=\"LOGIN\">\n");
	printf("\t<input type=\"submit\" value=\"CREATE\" name=\"CREATE\">\n");
	printf("\t<input type=\"submit\" value=\"MOVE\" name=\"MOVE\">\n");
	printf("</form>\n");
	printf("<p>%s</p>\n", output_string);
	printf("</body>\n</html>\n");

	return 0;
}