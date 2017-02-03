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

#define UPC_A   65
#define UPC_E   66
#define EAN13   67
#define EAN8    68
#define CODE39  69
#define ITF     70
#define CODABAR 71
#define CODE93  72
#define CODE128 73

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
       endwin();
       printf("usage: %s <hostname or IP>\n", argv[0]);
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
        endwin();
        printf("ERROR, no such host as %s\n", hostname);
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
		string f6 = "left";
		
		bool f1_t= false;
		bool f2_t= false;
		int  f3_t= 1;
		bool f4_t= false;
		bool f5_t= false;
		int  f6_t= 1;
		int last_f = 0;
		int iBar = 0;
		bool send = false;
		
		clear();
		bzero(buf, BUFSIZE);
		i = 0;
		//buf[i] = 27;
		//i++;
		//buf[i] = '@';
		//i++;
		mvprintw(0,0,"F1=%s; F2=%s; F3=%s; F4=%s; F5=%s; F6=%s\nF7= Tab setup; F8= Reset; F9= Send/Feed\n^F1=ASCII_ESC 27; ^F2=ASCII_GS 29; ^F3=ASCII_DC2 18; ^F4=ASCII_FS 28;\n^F5=Bar Code Hight; ^F6=Bar Code Lable place; ^F7=Bar Code Width\n^F8=Bar Code Type; ^F9= Nul/Send Bar Code;\nPlease enter text, press ESC to send or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str(),f6.c_str());
		mvprintw(7,0,"%s",buf);
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
					f5_t = !f5_t;
					last_f = 5;
					break;
				case KEY_F(6):
					if(last_f == 6){
						i = i - 3;
						if(i<0) i=0;
					}
					if(f6_t==0)
					{
						buf[i] = 27;
						i++;
						buf[i] = 'a';
						i++;
						buf[i] = '0';
						i++;
						clear();
						f6 = "left";
						f6_t =1;
					}
					else if(f6_t==1)
					{
						buf[i] = 27;
						i++;
						buf[i] = 'a';
						i++;
						buf[i] = '1';
						i++;
						clear();
						f6 = "center";
						f6_t =2;
					}
					else if(f6_t==2)
					{
						buf[i] = 27;
						i++;
						buf[i] = 'a';
						i++;
						buf[i] = '2';
						i++;
						clear();
						f6 = "right";
						f6_t =0;
					}
					last_f = 6;
					break;
				case KEY_F(25)://^F1
					buf[i] = ASCII_ESC;
					i++;
					break;
				case KEY_F(26):
					buf[i] = ASCII_GS;
					i++;
					break;
				case KEY_F(27):
					buf[i] = ASCII_DC2;
					i++;
					break;
				case KEY_F(28):
					buf[i] = ASCII_FS;
					i++;
					break;
				case KEY_F(29): //^F5
					buf[i] = ASCII_GS;
					i++;
					buf[i] = 'h';
					i++;
					buf[i] = 50;
					i++;
					clear();
					break;
				case KEY_F(30):
					buf[i] = ASCII_ESC;
					i++;
					buf[i] = 'd';
					i++;
					buf[i] = 1;
					i++;
					buf[i] = ASCII_GS;
					i++;
					buf[i] = 'H';
					i++;
					buf[i] = 2;
					i++;
					clear();
					break;
				case KEY_F(31):
					buf[i] = ASCII_GS;
					i++;
					buf[i] = 'w';
					i++;
					buf[i] = 3;
					i++;
					break;
				case KEY_F(32):
					buf[i] = ASCII_GS;
					i++;
					buf[i] = 'k';
					i++;
					buf[i] = 73;
					i++;
					buf[i] = 'X';
					iBar = i;
					i++;
					break;
				case KEY_F(33)://F9
					//buf[i] = 0;
					buf[iBar] = i-iBar-1;
					//i++;
					break;
				case KEY_F(7):
					buf[i] = ASCII_ESC;
					i++;
					buf[i] = 'D';
					i++;
					buf[i] = 4;
					i++;
					buf[i] = 8;
					i++;
					buf[i] = 12;
					i++;
					buf[i] = 16;
					i++;
					buf[i] = 20;
					i++;
					buf[i] = 24;
					i++;
					buf[i] = 28;
					i++;
					buf[i] = 0;
					i++;
					break;
				case KEY_F(8):
					buf[i] = ASCII_ESC;
					i++;
					buf[i] = '@';
					i++;
					break;
				case KEY_F(9)://F9
					buf[i] = '\n';
					i++;
					buf[i] = '\n';
					i++;
					buf[i] = '\n';
					i++;
					send = true;
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
				case 410: //screen rezise
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
		clear();
		mvprintw(0,0,"F1=%s; F2=%s; F3=%s; F4=%s; F5=%s; F6=%s\nF7= Tab setup; F8= Reset; F9= Send/Feed\n^F1=ASCII_ESC 27; ^F2=ASCII_GS 29; ^F3=ASCII_DC2 18; ^F4=ASCII_FS 28;\n^F5=Bar Code Hight; ^F6=Bar Code Lable place; ^F7=Bar Code Width\n^F8=Bar Code Type; ^F9= Nul/Send Bar Code;\nPlease enter text, press ESC to send or Ctrl-C to exit:\n",f1.c_str(),f2.c_str(),f3.c_str(),f4.c_str(),f5.c_str(),f6.c_str());
		mvprintw(7,0,"%s",buf);
		if(send) break;
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
