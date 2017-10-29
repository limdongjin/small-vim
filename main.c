#include "main.h" // draw function and Lib and #defined and getch()

#include "commod.h" // Header File about Command_mode
#include "excommod.h" // Header File about Ex_Command_mode
#include "editmod.h" // Header File about Editor_mode

void draw(char **lines , int x , int y,char **vimrc);
int Editor_mode(char **lines,int *x, int *y,char *filename,char **vimrcset);
int Command_mode(char **lines, int *x, int *y,char *filename,char **vimrcset,int *status);
int Ex_command_mode(char **lines, int *x, int *y,char *filename,char **vimrcset,int *status);
int All_Func(char **lines, int *x, int *y,int *status,char *filename,char**	vimrcset);  //Function about VIM EDITOR's Mechanism
 
int main(int argc,char *argv[]) {
    	char *filename = argv[1]; //argv[1] : edit filename
 	int i,j; 

	char d=1; //d : read char in file
	char c=0; // c : read comand
	int x = 0; // x : x line
	int y = 0; // y : y line

	FILE* file1; //text file 
	FILE* vimrc; //vimrc file

	int status = COMA; 
	/*
		status : 
		defined COMA ---> Command_mode
		defined EDIT ---> Editor_mode
		defined EXCO ---> Ex_Command_mode
		defined exit ---> if command is 'q' in Ex_command_mode, status is exit
		defined EXIT ---> if able exit, status is EXIT, Program is end. 
		defined FAIL ---> if unable exit, status is FAIL
		defined SAVE ---> if command is 'w', status is SAVE
		defined SAEX ---> if command is 'wq', status is SAEX
	  */

	int save = 1; 
	/*
	 * initializing editor start 
    	 */
    	char **lines = (char **)malloc(sizeof(char*)*300); //text file allocate 
	char **vimset =(char **)malloc(sizeof(char*)*300); //vimrc memory allocate
    	for ( i = 0 ; i < 300; i++ ) {
        	*(lines+i) = (char*)malloc(sizeof(char)*888);
        	for ( j = 0 ; j < 888 ; j++ ) {
			*(*(lines+i)+j) = '\0'; 
		}
    	}
	for( i=0;i <300;i++){
		*(vimset+i) = (char*)malloc(sizeof(char)*300);
		for(j=0;j<300;j++){
			vimset[i][j] = '\0';
		}
	}

	/*
	 * file open
	 */
     
	if(argc == 2){
		file1 = fopen(filename,"r");
		if(file1 == 0){
			file1 = fopen(filename, "w");
		}
	}
	else{
		printf("Usage\n./myvi test.txt\n");
		return 0;
	}
	
	vimrc = fopen("setting.vimrc","r"); // vimrc file 
	
	if(vimrc == 0){
		printf("no file");
		return 0;
	}
	/*
	 * File Read and Copy to lines. START
	 */
	for(i=0;i <300; i++){
		for(j=0;j<888;j++){
			lines[i][j] ='\0';
		}
	}
	
	char *lineexistcheck = (char*)malloc(sizeof(char)*25); 
	// for check linee exist, 
	
	int linenumber = 0; // linenumber variable
	
	for(i=0;i < HEIGHT; i++){
		for(j=0;j < WIDTH;j++){	

				if(feof(file1)){ // if file end, break;
					i = HEIGHT;
					j = WIDTH;
					break;
				}
				
				d = fgetc(file1); //read char 
				if(d == -1){ // if file end 
					i = HEIGHT;
					j = WIDTH;
					break;
				}
				
				if(d == 0 && j == 0){ 
					i = HEIGHT;
					j = WIDTH;
					break;
				}
				
				
				if(d == '\n'){   
					if(j == 0){ 
						lineexistcheck[i] = 'q';
						j = -1;
						linenumber += 1;
						lines[i][j] = ' ';
						continue;
					}
				
					if(j == 0)
						lines[i][j] = ' ';
					
					linenumber += 1;
					j = WIDTH;
					continue;
					
				}
				if(d == '\t'){
					lines[i][j] = '\t'; 	
					continue;
				}
				lines[i][j]  = d;
		
		}
	}
	fclose(file1); 
	/*
	 *  Read and Copy END. TEXT FILE CLOSE
	 */

	for(i=0;i<23;i++){
		if(lineexistcheck[i] == '1' || lineexistcheck[i] == 'q')
			linenumber += 1;
	}
	
	for(i=0;i<HEIGHT;i++){
		if(i < linenumber){
			if(lines[i][0] == '\0'){
				lines[i][0] = ' ';
			}
		}
	}


	/*
	 *  VIMRC FILE READ START
	 */

	for(i=0;i<300;i++){
		for(j=0;j<300;j++){
			
			if(feof(vimrc) == 1){  // vimrc file is end, and loop break.
				i = 300;
				j = 300;
				
				break;
			}
			
				d = fgetc(vimrc); //read char 

				if(d == -1){ //vimrc file end
					i =300;j=300;
					break;
				}

				if(d == '\n') {
					*(*(vimset+i)+j) = '\0';
					j = 300;

					continue;
				}else {
					*(*(vimset+i)+j) = d;
		
				}
			
		}
	
	}

	
	fclose(vimrc);
	
	/*
	 * READ END, CLOSE VIMRC FILE
	 */
	free(lineexistcheck); // FREE check variable
	
	/*
     *
	 * initialize end
	 *
	 * 
     */

	/*
     * inf loop start
     */
    while ( true ) {	

		status = All_Func(lines,&x,&y,&status,filename,vimset);
		if(status == EXIT){
			break;
		}
   }
	/*
	 * inf loop end
	 */

	/*
	 * close and free start
	 */
	
	for( i=0;i <300;i++){
		free(*(vimset+i));
	}


	for(i=0; i < 300; i++){
		free(*(lines+i));
	}

		printf("EXIT\n");

	/*
	 * EXIT Program
	 */
	return 0;
}

int All_Func(char **lines, int *x, int *y,int *status,char *filename,char** vimrcset){
	

	if(*status == COMA || *status == SAEX || *status == exit || *status == SAVE
			|| *status == SEAR){
		*status = Command_mode(lines,x,y,filename,vimrcset,status);
	}else if(*status == EDIT){
		*status = Editor_mode(lines, x, y,filename,vimrcset);
	}else if(*status == EXCO || *status == SEARR ){
		*status = Ex_command_mode(lines,x,y,filename,vimrcset,status);
	}
	return *status;
}


