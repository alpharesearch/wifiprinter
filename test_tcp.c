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
    perror(msg);
    endwin();
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
       fprintf(stderr,"usage: %s <hostname or IP>\n", argv[0]);
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
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
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
    
    string test = "test";
    
    int c;
    int i;
	timeout(-1);
    do
    {
		clear();
		printw("F1=bold;F2=X;F3=X;F4=X;F5=X\nPlease enter text, press ESC to send or Ctrl-C to exit:\n");
		bzero(buf, BUFSIZE);
		i = 0;
		buf[i] = 27;
		i++;
		buf[i] = '@';
		i++;
		mvprintw(2,0,"%s\n",buf);
		printw(test.c_str());
		while((c = getch()) != 27)
		{       switch(c)
				{	
				case KEY_F(1):
					
					buf[i] = 27;
					i++;
					buf[i] = 'E';
					i++;
					buf[i] = '1';
					i++;
					clear();
		            printw("F1=BOLD;F2=X;F3=X;F4=X;F5=X\nPlease enter text, press ESC to send or Ctrl-C to exit:\n");
					printw("bold on");
					mvprintw(2,0,"%s\n",buf);
					break;
				case KEY_F(2):
					
					buf[i] = 27;
					i++;
					buf[i] = 'E';
					i++;
					buf[i] = '0';
					i++;
					clear();
					printw("F1=bold;F2=X;F3=X;F4=X;F5=X\nPlease enter text, press ESC to send or Ctrl-C to exit:\n");
					printw("bold off");
					mvprintw(2,0,"%s\n",buf);
					break;
				case KEY_F(3):
					printw("3");
					break;
				case KEY_F(4):
					printw("4");
					break;
				case KEY_F(5):
					printw("5");
					move(2,0);
					break;
				case KEY_BACKSPACE:
					if(i>0)buf[i-1] = 0;
					i--;
					if(i<0) i=0;
					mvprintw(2,0,"%s\n",buf);
					if(i<=0)move(2,0);
					break;
				case 3: //ctrl-c
					printw("Exit");
					close(sockfd);
					endwin();
					return 0;
					break;
				case 10: //enter
					mvprintw(2,0,"%s\n",buf);
					buf[i] = c;
					i++;
					break;
				default:
					//printw("KEY NAME : %s - %d\n", keyname(c),c);
					buf[i] = c;
					i++;
					mvprintw(2,0,"%s\n",buf);
					break;
			}
		}
		printw("sending... \n");
	
		/* send the message line to the server */
		buf[i] = '\n';
		n = write(sockfd, buf, strlen(buf));
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
