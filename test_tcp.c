/* 
 * tcpclient.c - A simple TCP client
 * cc test_tcp.c -o test_tcp -lcurses
 * usage: test_tcp <host>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <curses.h>
#include <iostream>
#include <string>
using namespace std;

#define BUFSIZE 1024

// ASCII codes used by some of the printer config commands:
#define ASCII_TAB '\t' // Horizontal tab
#define ASCII_LF  '\n' // Line feed
#define ASCII_FF  '\f' // Form feed
#define ASCII_CR  '\r' // Carriage return
#define ASCII_DC2  18  // Device control 2
#define ASCII_ESC  27  // Escape
#define ASCII_FS   28  // Field separator
#define ASCII_GS   29  // Group separator

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    endwin();
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
	initscr();                      /* Start curses mode            */
    raw();                          /* Line buffering disabled      */
    keypad(stdscr, TRUE);           /* We get F1, F2 etc..          */
    noecho();                       /* Don't echo() while we do getch */
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    /* check command line arguments */
    if (argc != 2) {
       printw("usage: %s <hostname or IP>\n", argv[0]);
       endwin();
       exit(0);
    }
    hostname = argv[1];
    portno = 9100;

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        printw("ERROR, no such host as %s\n", hostname);
        endwin();
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
      error("ERROR connecting");

    /* get message line from the user */
    
    int c;
    int i=0;
	timeout(-1);
    do
    {
		string f1 = "bold";
		string f2 = "bw";
		string f3 = "normal 1x";
		string f4 = "up";
		string f5 = "underline off";
		
		bool f1_t= false;
		bool f2_t= false;
		int f3_t= 1;
		bool f4_t= false;
		bool f5_t= false;
		
		int last_f = 0;
		
		clear();
		printw("F1=%s;F2=%s;F3=%s;F4=%s;F5=%s\nPlease enter text, press ESC to send %i %i or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str(),strlen(buf), i);
		bzero(buf, BUFSIZE);
		i = 0;
		buf[i] = 27;
		i++;
		buf[i] = '@';
		i++;
		mvprintw(2,0,"%s\n",buf);
		while((c = getch()) != 27)
		{       switch(c)
				{	
				case KEY_F(1):
					if(last_f == 1){
						i = i - 3;
						if(i<0) i=0;
					}
					if(f1_t)
					{
						buf[i] = 27;
						i++;
						buf[i] = 'E';
						i++;
						buf[i] = '0';
						i++;
						clear();
						f1 = "bold";
					}
					else
					{
						buf[i] = 27;
						i++;
						buf[i] = 'E';
						i++;
						buf[i] = '1';
						i++;
						clear();
						f1 = "BOLD";
					}
		            printw("F1=%s;F2=%s;F3=%s;F4=%s;F5=%s\nPlease enter text, press ESC to send %i %i or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str(),strlen(buf), i);
					printw("bold on");
					mvprintw(2,0,"%s\n",buf);
					f1_t = !f1_t;
					last_f = 1;
					break;
				case KEY_F(2):
					if(last_f == 2){
						i = i - 3;
						if(i<0) i=0;
					}
					if(f2_t)
					{
						buf[i] = 29;
						i++;
						buf[i] = 'B';
						i++;
						buf[i] = '0';
						i++;
						clear();
						f2 = "bw";
					}
					else
					{
						buf[i] = 29;
						i++;
						buf[i] = 'B';
						i++;
						buf[i] = '1';
						i++;
						clear();
						f2 = "wb";
					}
		            printw("F1=%s;F2=%s;F3=%s;F4=%s;F5=%s\nPlease enter text, press ESC to send %i %i or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str(),strlen(buf), i);
					printw("bold on");
					mvprintw(2,0,"%s\n",buf);
					f2_t = !f2_t;
					last_f = 2;
					break;
				case KEY_F(3):
					if(last_f == 3){
						i = i - 3;
						if(i<0) i=0;
					}
					if(f3_t==0)
					{
						buf[i] = 29;
						i++;
						buf[i] = '!';
						i++;
						buf[i] = 136;
						i++;
						clear();
						f3 = "normal 1x";
						f3_t =1;
					}
					else if(f3_t==1)
					{
						buf[i] = 29;
						i++;
						buf[i] = '!';
						i++;
						buf[i] = 17;
						i++;
						clear();
						f3 = "small 2x";
						f3_t =2;
					}
					else if(f3_t==2)
					{
						buf[i] = 29;
						i++;
						buf[i] = '!';
						i++;
						buf[i] = 51;
						i++;
						clear();
						f3 = "big 3x";
						f3_t =3;
					}else if(f3_t==3)
					{
						buf[i] = 29;
						i++;
						buf[i] = '!';
						i++;
						buf[i] = 119;
						i++;
						clear();
						f3 = "biggest 4x";
						f3_t =0;
					}
		            printw("F1=%s;F2=%s;F3=%s;F4=%s;F5=%s\nPlease enter text, press ESC to send or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str());
					printw("bold on");
					mvprintw(2,0,"%s\n",buf);
					last_f = 3;
					break;
				case KEY_F(4):
					if(last_f == 4){
						i = i - 3;
						if(i<0) i=0;
					}
					if(f4_t)
					{
						buf[i] = 27;
						i++;
						buf[i] = 123;
						i++;
						buf[i] = '0';
						i++;
						clear();
						f4 = "up";
					}
					else
					{
						buf[i] = 27;
						i++;
						buf[i] = 123;
						i++;
						buf[i] = '1';
						i++;
						clear();
						f4 = "down";
					}
		            printw("F1=%s;F2=%s;F3=%s;F4=%s;F5=%s\nPlease enter text, press ESC to send or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str());
					printw("bold on");
					mvprintw(2,0,"%s\n",buf);
					f4_t = !f4_t;
					last_f = 4;
					break;
				case KEY_F(5):
					if(last_f == 5){
						i = i - 3;
						if(i<0) i=0;
					}
					if(f5_t)
					{
						buf[i] = 27;
						i++;
						buf[i] = 45;
						i++;
						buf[i] = '0';
						i++;
						clear();
						f5 = "underline off";
					}
					else
					{
						buf[i] = 27;
						i++;
						buf[i] = 45;
						i++;
						buf[i] = '2';
						i++;
						clear();
						f5 = "underline on";
					}
		            printw("F1=%s;F2=%s;F3=%s;F4=%s;F5=%s\nPlease enter text, press ESC to send or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str());
					printw("bold on");
					mvprintw(2,0,"%s\n",buf);
					f5_t = !f5_t;
					move(2,0);
					last_f = 5;
					break;
				case KEY_BACKSPACE:
					if(i>0)buf[i-1] = 0;
					i--;
					if(i<0) i=0;
					mvprintw(2,0,"%s\n",buf);
					if(i<=0)move(2,0);
					last_f = 0;
					break;
				case 3: //ctrl-c
					printw("Exit");
					close(sockfd);
					endwin();
					last_f = 0;
					return 0;
					break;
				case 10: //enter
					mvprintw(2,0,"%s\n",buf);
					buf[i] = c;
					i++;
					last_f = 0;
					break;
				default:
					//printw("KEY NAME : %s - %d\n", keyname(c),c);
					buf[i] = c;
					i++;
					mvprintw(2,0,"%s\n",buf);
					last_f = 0;
					break;
			}
		}
		printw("sending... \n");
	
		/* send the message line to the server */
		buf[i] = '\n';
		i++;
		//n = write(sockfd, buf, strlen(buf));
		n = write(sockfd, buf, i);
		if (n < 0){ 
		  error("ERROR writing to socket");
		  break;
		}
	}while(1);
    /* print the server's reply 
    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s", buf);
    */
    close(sockfd);
    endwin();
    return 0;
}
