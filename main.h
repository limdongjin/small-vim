
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <termio.h>

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
	if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
}

#ifndef false
#define false 0
#endif
#ifndef true
#define true !(false)
#endif
#define WIDTH   80
#define HEIGHT  24
#define MAX_COMMAND_SIZE 128
#define MAX_BUFFER_SIZE 128
#define TAB_LENGTH 4
#define ESC 27
#define BACKSPACE 127
#define ENTER 10
#define TAB 9
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

#define FAIL 21
#define EXIT 11
#define EDIT 12
#define EXCO 13
#define COMA 14
#define exit 22
#define SAVE 15
#define SAEX 16
#define SEAR 17
#define SEARR 18
void draw(char **lines , int x , int y,char **vimrc) {
    int i,j;
	int* tabnumber;
    int shownu = 0; // check "set nu" is exist?
	int len = 0; 
	tabnumber = (int *)malloc(sizeof(int)*25); //check one line, tab number 
	int vimrclen = 0; 
	
	for(i=0;i<24;i++)
		tabnumber[i] = 0; 
	/*
	 * find vimrc length
	 */
	for(i=299;i>=0;i--){

		if(vimrc[i][0] != '\0'){
			vimrclen = i;
			break;
		}

	}
	/*
	 * find length
	 */
	for(i=HEIGHT-1;i >= 0;i--){
		if(lines[i][0] != '\0'){
			len = i;
			break;
		}
	}
	/*
	 * Read vimrc. START
	 */
	for(i=0;i<vimrclen+1;i++){
		if(strncmp(vimrc[i] , "set nu",6) == 0 && strlen(vimrc[i]) == 6)
				shownu = 1;
		
	}
    /*
	 * Read END
	 */
	system("clear");
   
   if(shownu == 0)  printf("┏");
   else printf(" ");
	
    /*
	 * Find tabnumber 
	 */
    
   for(i=0;i< HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if(lines[i][j] == '\t')
				tabnumber[i] += 1;
		}
	}

	/*
	 * Display START
	 */
    for (  i = 0 ; i < WIDTH ; i++ )
        printf("━");

    printf("┓\n");

    for (  i = 0 ; i < HEIGHT ; i++ ) {
        if(shownu == 1){
			if(i <= len){
				if(i < 9)
					printf(" %d",i+1);
				else 
					printf("%d",i+1);
			}
			else{ 
				if(i > 8)
					printf(" ~");
				else 
					printf(" ~");
			}
		}
		else
			printf("┃");
		
        for (  j = 0 ; j < WIDTH-tabnumber[i]*2; j++ ) {
            if( j == WIDTH - 1 - tabnumber[i]*2 && shownu == 1)
				continue;
			if ( y == i && x == j  ){
				if(lines[i][j] == '\t')
					printf("I  ");
				else
              printf("I");
            } else {
                if(lines[i][j] == '\0'){
                    printf(" ");
				        }
				else if(lines[i][j] == '\n'){
					printf("n");
				}
				else if(lines[i][j] == ' '){
					printf(" ");
				}
				else if(lines[i][j] =='\t'){
					printf("   ");

				}
				else{
                 	printf("%c",lines[i][j]);
				}
        }
        }
       printf("┃");
        puts("");
    }


   if(shownu == 0) printf("┗");
   else printf(" ");

    for (  i = 0 ; i < WIDTH ; i++ )
        printf("━");

    printf("┘\n");
	
	/*
	 * Display END
	 */

	free(tabnumber);
}

