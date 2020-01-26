#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <string.h>
#include <sys/stat.h>

char imageheader[]="HTTP/1.1 200 OK\r\n"
"Content-Type: image/png\r\n\r\n";
char imageheader2[]="HTTP/1.1 200 OK\r\n"
"Content-Type: video/mp4\r\n\r\n";

char html[]= "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>"
"<html><head><title>BASIC WEB SERVER</title></head>"
"<body bgcolor=mediumseagreen><center><h2 style=background-color:dodgerblue><U><font color=RED size=18px>Computer Networks - Assignment</font></U></h2>"
"<font color=darkblue size=12px><h6>The server is designed in C language.</h6></font>"
"<h3>The below image is sent from the server.</h3><br>"
"<img src=\"webserver.png\"></center>"
"<source src=\"1.mp4\"></body></html>";

int main(){
	int port = 9001;
   int image,option;
  char browser[2048];
	printf("the port is %d\n",port);
	printf("enter 1 to send image any other key to send video\n");
	scanf("%d",&option);
	int server_socket;
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in server_address;
		
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);
		server_address.sin_addr.s_addr = INADDR_ANY;
		

	    int binding =1;
            setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&binding,sizeof(binding));
	    int b = bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));	
		if (b==-1)
		{
			printf("error in binding");
			exit(0);
		}
		listen(server_socket,5);
		while(1){
		int client_socket;
		client_socket = accept(server_socket,NULL,NULL);
			if(client_socket==-1){
				printf("connection not successful\n");
				exit(1);
			}
			else
				printf("connection successful\n");
	if(option==1){
		if(!fork())
		{
			close(server_socket);
			memset(browser,0,2048);
			read(client_socket,browser,2047);
				if(!strncmp(browser,"GET /favicon.ico",80))
				{
					image = open("favicon.ico",O_RDONLY);
														                
				}

				else if(strncmp(browser,"GET /webserver.png",80) >= 0)
				{
					image = open("webserver.png",O_RDONLY);
					write(client_socket,imageheader,sizeof(imageheader)-1);
					sendfile(client_socket,image,NULL,41000);
					close(image);
					
				}
				else
					write(client_socket,html,sizeof(html)-1);

			close(client_socket);
			exit(1);
		}
	}
	else{
		if(!fork())
		{
			close(server_socket);
			memset(browser,0,2048);
			read(client_socket,browser,2047);
				if(!strncmp(browser,"GET /favicon.ico",80))
				{
					image = open("favicon.ico",O_RDONLY);
														                
				}

				else if(strncmp(browser,"GET /1.mp4",555800) >= 0)
				{
					image = open("1.mp4",O_RDONLY);
					
write(client_socket, "HTTP/1.1 200 OK\r\n", 17);
write(client_socket,imageheader2,sizeof(imageheader2)-1);
					sendfile(client_socket,image,NULL,555800);
					close(image);
					
				}
				else
					write(client_socket,html,sizeof(html)-1);

			close(client_socket);
			exit(1);
		}
	}
		
		close(client_socket);
		}
		
		close(server_socket);
		return(0);
	 
}
