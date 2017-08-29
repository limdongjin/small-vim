int System_check(char **lines, char* filename,int status,char **vimrcset){ // EXIT FAIL COMA
	
    FILE* beforeFile; // before save, text file
    FILE* afterFile; //after save, text file
    char **beforeStr; // buffer for copy file

    char d;
    int i,j;
	
	/*
	 * SAVE('w') or SAVE&EXIT('wq')
	 */

    if(status == SAVE || status == SAEX){
		/*
		 * File open for save lines
		 */
        afterFile = fopen(filename, "w");
	
		/*
		 * lines Read&Copy Start. Save to afterFile 
		 */
        
		for(i=0;i < HEIGHT; i++){
            for(j=0;j <WIDTH;j++){

                if(j==0 && lines[i][j] == '\0') 
                    break;

                if(lines[i][j] == '\0'){
                    fputc('\n',afterFile);
                    j =WIDTH;
					
                    continue;
                }
                if(j == WIDTH-1){
					if(lines[i][j] == ' ' || lines[i][j] == '\0'){

                    	fputc('\n',afterFile);
						
					}
					else{
						fputc(lines[i][j],afterFile);
						fputc('\n',afterFile);
						
					}

                    j = WIDTH;
                    continue;
                }else if(*(*(lines+i)+j) == '\t'){

                    fputc('\t',afterFile);
					
                }else{

                    fputc(*(*(lines+i)+j), afterFile);
					
                }

            }
        }
        fclose(afterFile);
        
		/*
		 * lines Read&Copy End.Close afterFile
		 */

		if(status == SAVE){
		   //change mode. command_mode		
            return COMA;
		}
        else{
			//SAEX	
            return EXIT;
		}
    }
	/*
	 * exit('q')
	 */
    if(status == exit){
        
		/*
		 * Open beforeFile and Allocate beforeStr
		 */

		beforeFile = fopen(filename,"r");

        beforeStr = (char **)malloc(sizeof(char *)*33);
		
        for(i=0;i<33;i++)
            *(beforeStr+i) = (char*)malloc(sizeof(char)*88);

        for(i=0;i<33;i++){
            for(j=0;j<88;j++){
                beforeStr[i][j] = '\0';
            }
        }

		/*
		 *  Read beforeFile and Copy beforeFile to beforeStr. START
		 */

        for(i=0;i<HEIGHT;i++){
            for(j=0;j<WIDTH;j++){
                	if(feof(beforeFile)){
						i = HEIGHT;
						j = WIDTH;
						
						break;

					}

                    d = fgetc(beforeFile);
					
					if(d == -1){
						i = HEIGHT;
						j = WIDTH;
						break;
					}

					if(d == '\n'){
						if(j == 0){
							j = -1;
							continue;
						}
                        d = ' ';
                        beforeStr[i][j] = d;
                        j = WIDTH;
                        continue;
                    }else{
                        beforeStr[i][j] = d;
                    }
			}		
		}

		/*
		 * Read beforeFile END , beforeFile Close
		 */

        fclose(beforeFile);
		
      // " draw(beforeStr,0,0,vimrcset);



	   /*
		*   Compare beforeStr and lines. START
		*/

        for(i=0;i<HEIGHT;i++){
            for(j=0;j<WIDTH;j++){
                if(lines[i][j] == '\0'){
                    j = WIDTH;
                    continue;
                }
                if(lines[i][j] != beforeStr[i][j]){
                  //  printf("#Not Same%d %d. %c %c %d %d\n",i,j,lines[i][j],beforeStr[i][j], lines[i][j], beforeStr[i][j]);
				//	system("clear");
					for(i=0;i<33;i++) {
				//		printf("free beforeStr+%d\n",i);
				//		system("clear");
						free(*(beforeStr+i));
					}
					return FAIL;
                }else{
				//	 printf("#Same%d %d. %c %c %d %d\n",i,j,lines[i][j],beforeStr[i][j], lines[i][j], beforeStr[i][j]);
				//	system("clear");
                    continue;
                }

            }
        }

		/*
		 * Compare END  
		 */
	
	/*
	 * FREE befoeStr 
	 */

	for(i=0;i<33;i++){
	//	printf("#free beforeStr+%d\n",i);
	//	system("clear");
		free(*(beforeStr+i));
	}
	return EXIT;


    }

}

int Ex_command_mode(char **lines, int *x, int *y,char *filename,char
		**vimrcset,int *status){
	char c;
	char *command;
	int i=0;
	int vimrclen=0; 

	command = (char *)malloc(sizeof(char)*10);

	draw(lines,*x,*y,vimrcset);
	
	/*
	 * find vimrc length 
	 */
	
	for(i=299;i >= 0; i--){
		if(vimrcset[i][0] != '\0'){
			vimrclen = i;break;
		}
	}
	
	printf("[NORMAL] > ./%s > [Cursor]%d:%d\n",filename,*y,*x);
	printf(":");
	/*
	 * read command. or setting ---> save command
	 * command : 'w' , 'wq' , 'q' , 'q!', ':int'
	 * setting : "set nu" , "set nonu"
	 */
	for(i=0;i<10;i++)
		command[i] ='\0';
	if(*status == SEARR){
	gets(command);
	gets(command);
	}else{
		gets(command);
	}
	if(strncmp(command,"w",1) == 0 && strlen(command) == 1){
		/*
		 * SAVE
		 */
		free(command);
		return SAVE;
	}else if(strncmp(command, "q",1)== 0 && strlen(command) == 1){
		/*
		 * exit
		 */
		free(command);
		return exit;
	}else if(strncmp(command, "wq",2)==0 && strlen(command) == 2)  {
		/*
		 * SAVE and EXIT
		 */
		free(command);
		return SAEX;
	}else if(strncmp(command, "q!",2)==0 && strlen(command) == 2) {
		/*
		 *  EXIT
		 */
		free(command); //
		return EXIT;
	}else if( ('0' <= *(command+0) && *(command+0) <= '9')){
		
		/*
		 *  Move 'int'line's first index
		 */

		if(*(command+1) == '\0' || ( '0' <= *(command+1) && *(command+1) <='9')){
			if(strlen(command) == 1){
				*x = 0;
				*y = *(command+0)-'0'-1;
				free(command);
				return COMA;
			}else{
				if(strlen(command) != 2)
					return COMA;
				*x = 0;
				*y = (*(command+0) - '0')*10 + (*(command+1) - '0')-1;
				if(*y > 23)
					*y = 23;
				free(command);
				return COMA;
			}
		}
	}else if( strncmp(command,"set nu",6) == 0 && strlen(command) == 6){
		
		/*
		 *  save "set nu" in vimrcset
		 */
		
		strcpy(vimrcset[vimrclen+1],"set nu");
		return COMA;
	}else if (strncmp(command, "set nonu", 8) == 0 && strlen(command) == 8){
		
		/*
		 * remove "set nu" in vimrcset
		 */

		for(i=0;i<vimrclen+1;i++){
			if(strncmp(vimrcset[i] , "set nu", 6) == 0 && strlen(vimrcset[i])== 6)
		//		printf("OK\n");
				strcpy(vimrcset[i], "    ");
		}
		free(command);
		return COMA;
	}else{
		/*
		 *  wrong command or setting
		 */
		printf("%s\n",command);
		free(command);
		return COMA;
	}

	
}

