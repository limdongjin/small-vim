int System_check(char **lines, char* filename,int status,char **vimrcset); // EXIT FAIL COMA
	
void line_swap(char *a, char *b);  // line a ,line b swap ---> line b , line a
void Cursor(char **lines, int *x, int *y,int value); // Cursor Move include 'h' 'j' 'k' 'l' 'w' 'b' '0' '^' '$' command 

int Command_mode(char **lines, int *x, int *y,char *filename,char **vimrcset,int *status){
	
	char c; //command	
	int	i,j,k; 
	int linenumber = 0; //linenumber
	char* pattern;
	pattern = (char *)malloc(sizeof(char)*30);
	for(i=0;i<30;i++)
		pattern[i] = '\0';
	/*
	 *  System_check() in "commod.h".  
	 */
	
	if(*status == SAEX){
			*status = System_check(lines,filename,*status,vimrcset); 

			return EXIT; //Program Exit
	}

	if(*status == exit){
		*status = System_check(lines,filename,*status,vimrcset);

		if(*status == EXIT){
			return EXIT; 
		}
	}

	draw(lines, *x, *y,vimrcset);
	
	printf("[NORMAL] > ./%s > [Cursor]%d:%d\n",filename,*y,*x);
	printf("-- COMMAND --\n");
	
	if(*status == SAVE){
		*status = System_check(lines,filename,*status,vimrcset);
		printf("system> saved\n"); 
	}else if(*status == FAIL){
			printf("system> No write since last change (add ! to override)\n");
	}

	c = getch();

	if(c == 'd'){
		c = getch();
		
		if(c == 'd'){ 
			
			 /*			   						  			   						   
			  *
			  *  dd command : now line remove and all line up
			  *
			  */
			
			for(i=0;i<HEIGHT;i++){
				for(j=0;j<WIDTH;j++){
					if(lines[i][j] == '\0' ) {
						
						/*
						 * FIND line number
						 */

						if(i == HEIGHT-1) { //special case
							linenumber = 23;
							break;
						}
						if(j == 0 && *(*(lines+i)+j) =='\0'){ //last+1 line	case
							i = HEIGHT;
							j = WIDTH;
							break;
						}
						linenumber += 1; //linenumber += 1
						j = WIDTH; 
						continue; 
					}else if(j == WIDTH-1){ // line end and linenumber += 1
						linenumber += 1; 
					}else{           
						continue;
					}
				}
			}

			if(linenumber == 1){ //special case. last line index is 0
				for(j=0;j<WIDTH;j++){
					lines[linenumber-1][j] = '\0'; //line clear
				}
				*x = 0;
				*y = 0;
				return COMA;
			}
			if(*y == HEIGHT-1){  
				/*
				 *special case. last line is HEIGHT - 1, *y += 1 and only nowline remove
				*/
				
				for(i=0;i<WIDTH;i++)
					lines[*y][i] = '\0';  //now line clear

				linenumber -= 1;
				*y -= 1;
				return COMA;
			} 
			if(*y == linenumber) return COMA; // special case. prevent Segment Fault
			
			for(i=0;i<WIDTH;i++){
				lines[*y][i] = ' ';
			}
			
			for(i=linenumber-1;i >= *y;i--){
				line_swap(lines[linenumber - 1],lines[i]); // all line UP		
			}
			
			for(j=0;j<WIDTH;j++){
					lines[linenumber-1][j] = '\0'; //last line clear
			}
			if(linenumber-1 == *y) *y -= 1; 
			
			return COMA;
		}
		return COMA;
	}
	if(c == 'o'){
		/*
		 *  'o' command : Cursor is DOWN and Cursor's y+i line All Down. 
		 */

		for(i=0;i<HEIGHT;i++){
				for(j=0;j<WIDTH;j++){
					/*
					 *  FIND line number 
					 */
					if(lines[i][j] == '\0' || lines[i][j] == '\n') {
						if(j == 0 && *(*(lines+i)+j) =='\0'){
							i = HEIGHT;
							j = WIDTH;
							break;
						}
						linenumber += 1;
						j = WIDTH;
						continue;
					}else if(j == WIDTH-1){
						linenumber += 1;
					}else{
						continue;
					}
				}
			}

		if(linenumber == HEIGHT) // special case.line is Full
			return EDIT;

		for(i=0; i < linenumber-*y+1;i++){
			line_swap(lines[*y+1],lines[*y+1+i]); // line is DOWN	
		}
	 
		for(i=0;i<WIDTH;i++){
			lines[*y+1][i] = '\0'; //line is clear
		}
		
		
		if(lines[*y+1][0] == '\0') 
			lines[*y+1][0] = ' ';
		if(lines[*y][0] == '\0')
			lines[*y][0] = ' ';

		*y += 1; //Cursor is DOWN
		*x = 0;  
	
		return EDIT;
	}
	char jud = 0;
	if(c == 'i'){
		/*
		 * command 'i' : change Mode. Editor mode
		 */
		return EDIT;
	}else if(c == ':'){
		/*
		 * command ':' --> change Mode. Ex_command_mode
		 */
		if(*status == SEAR) return SEARR;
		else return EXCO;
	}else if(c == 'h' || c == 'j' || c == 'k' || c == 'l' || c == 'w' || c ==
			'b' || c == '$' || c == '0' || c == '^'){     
		/*
		 * Cursor Move Command
		 */
		Cursor(lines,x,y,c);
		return COMA;
	}
	else if(c =='/'){
	draw(lines, *x, *y,vimrcset);
	
	printf("[NORMAL] > ./%s > [Cursor]%d:%d\n",filename,*y,*x);

		printf("/");
		scanf("%s",pattern);

		for(i=*y;i<HEIGHT;i++){
			for(j=0;j<WIDTH;j++){
				if(i == *y && j <= *x) continue;
				if(lines[i][j] == '\0') break;
				if(lines[i][j] == ' ' || lines[i][j] == '\t' || lines[i][j]
						=='\0') continue;


				
				if(strlen(pattern) == 1){
					if(pattern[0] == lines[i][j]){
						*x = j;
						*y = i;
						return SEAR;
					}
					continue;
				}else if(strlen(pattern) == 2){
					if(pattern[0] == lines[i][j]){
						if(pattern[1] == lines[i][j]){
							*x = j;
							*y = i;
							return SEAR;
						}
					}else
						continue;
				}else if(i == HEIGHT-1 && j == WIDTH-1) break;
			
				if(pattern[0] == lines[i][j]){
						for(k=1;k<strlen(pattern);k++){
							
							if(pattern[k] == lines[i][j+k]){

								if(k == strlen(pattern)-1) {
									*x = j+k-strlen(pattern)+1;
									*y = i;
									jud = 1;
									i = HEIGHT;
									j = WIDTH;
									break;
								}else
								   continue;
							}
							else{ 
								break;
							}


						}
						if(jud == 1) {
							i = HEIGHT;
							j = WIDTH;
							break;;
						}
						else continue;
					}

				}
	 		}
		if(jud == 1)
		return SEAR;
		else return SEAR;
	}
	else{
		// not command
		return COMA;
	}
}

void Cursor(char **lines, int *x, int *y,int value){
    int i;
    int flag = 0; //cursor status
    int  tmp;
    
	if( *(*(lines+*y)+*x) == ' ' || lines[*y][*x] == '\t' || lines[*y][*x] ==
			'\0' ){
        flag = 0; //flag 0 : if lines[*y][*x] is not word
    }else{
        flag = 1; //flag 1 : if lines[*y][*x] is word
    }


    if(value == 'h'){ 
		/*
		  * 'h' command : Cursor left
		  */
		
		/*
        if(*x == 0 && *y > 0){ 
           
            if( *(*(lines+*y)+WIDTH-1) != '\n' &&  *(*(lines+*y)+WIDTH-1) !=
                    '\0'){
                *x = WIDTH-1;
                return;
            }
			
            for(i=WIDTH-1;i >= 0; i--){
                if(*(*(lines+*y)+i) == '\n'){
                    *x = i;
                    if(*x < 0)
                        *x = 0;

                    return;
                }
            }
        }
		*/

        *x -= 1;
        if(*x < 0)
            *x = 0;

    }else if(value == 'j'){
		/*
		 * 'j' command : Cursor Down 
		 */

        if(*(*(lines+*y+1)+0) != '\0'){ 
     
			if(*(*(lines+*y+1)+*x) != '\0'){ //standard case
                *y += 1;
				
                return;
        	}else{ 
				  /*
				   * special case. cursor move last
				   */
				
                for(i=WIDTH-1;i >= 0; i--){
                    if(*(*(lines+*y+1)+i) != '\0'){
                        *x = i;
                        *y += 1;
                        return;
                    }
                }

            }
			
        }else{
			 for(i=WIDTH-1;i >= 0; i--){
                    if(*(*(lines+*y+1)+i) != '\0'){
                        *x = i;
                        *y += 1;
                        return;
                    }
                }

		}

    }else if(value == 'k'){ 
		
		 /*
		  * 'k' command : Cursor UP
		  */

		if(*y == 0) //special case. not up
			return;

        if(*(*(lines+*y-1)+0) != '\0'){
			if(lines[*y-1][*x+1] == '\0')
				{
				 for(i=WIDTH-1;i >= 0; i--){
                    if(*(*(lines+*y-1)+i) != '\0'){
                        *x = i;
                        *y -= 1;
                        return;
                    }
                }

	
				}
			else if(*(*(lines+*y-1)+*x) != '\0'){
                *y -= 1;
				
                return;
            }else{
                for(i=WIDTH-1;i >= 0; i--){
                    if(*(*(lines+*y-1)+i) != '\0'){
                        *x = i;
                        *y -= 1;
                        return;
                    }
                }

            }
        }else{
			 for(i=WIDTH-1;i >= 0; i--){
                    if(*(*(lines+*y-1)+i) != '\0'){
                        *x = i;
                        *y -= 1;
                        return;
                    }
                }


        }
    }else if(value == 'l'){
		/*
		 * 'l' command : Cursor RIGHT
		 */

        if(*(*(lines+*y)+*x) != '\n' && *(*(lines+*y)+*x) != '\0'){
            *x += 1;
        }else{
           
                return;
            
        }
    }else  if(value == '0'){
         
		/*
		 * '0' command : Cursor move *x --> 0
		 */

		*x = 0;
        return;
     }else if(value == '^'){
		 
		  /*
		   * '^' command : Cursor move first word
		   */

        for(i=0;i<WIDTH;i++){
             if(*(*(lines+*y)+i) != ' ' && lines[*y][i] != '\t'){
                 *x = i;
                 return;
             }
        }
     }else if(value == 'w' ){
		 /*
		  * 'w' command : Cursor move to next word
		  */

        if(flag == 0){

			 /*  flag 0 
			  *  Mechanism : if find word , break and cursor stop. 
			  *  ex) hello   <cursor>  ffworld , if command 'w', result_cursor is 'f'
			  *  pattern : <c>'s next word is first word. 
			  */
			
			if(*x == 0 && lines[*y][*x] == '\0') return ; //special case. 
          
			for(i=*x;i<WIDTH;i++){ //FIND WORD loop
				if(lines[*y][i] == '\0'){ 
					/*
					 * special case. now line is not have next word. cursor move
					 * next line and execute 'w' command
					 */
					*y += 1;
					*x = 0;
					if(lines[*y][*x] == ' ' || lines[*y][*x] =='\t')
						Cursor(lines,x,y,'w');
					else return; // if lines[nextline][0] is word.
					return;
				}
                if( *(*(lines+*y)+i) == ' ' || lines[*y][i] == '\t'){ 
					//skip space and tab
                    continue;
                }
                if(*(*(lines+*y)+i) != ' ' && *(*(lines+*y)+i) != '\0' &&
                        *(*(lines+*y)+i) != '\t' ){
					/*
					 * Standard Mechanism. find word.and stop Cursor
					 */

                    *x = i;
                    return;
                }
				
                if( !(('a'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <= 'z') ||
                            ('A'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <=
                             'Z'))){
					/*
					 * Special case. if word is Special Character , stop Cursor
					 */
					if('0' <= lines[*y][i] && lines[*y][i] <= '9')
						continue;
					if(lines[*y][i] == '\0'){
						*y += 1;
						*x = 0;
					}else{	
                    *x = i;
					}
                    return;
                }
			        
           }
        }else{
			/* flag 1
			 * Mechanism : if word(flag==1) --> space(flag==0)---->word(flag==1):result_cursor
			 * ex) <cursor>ello world world, if cursor move space && if after move word, and cursor stop 
			 * pattern : after <cursor> move space, <cursor> move word. it is result word
			 */
				if(*x == 0 && lines[*y][*x] == '\0') return; //special case
	
            for(i=*x;i<WIDTH;i++){
				if(lines[*y][i] == '\0') {
					
					/*
					 * special case. if now line is not have next word
					 */

					*y += 1;
					*x = 0;

					if(lines[*y][*x] == ' ' || lines[*y][*x] =='\t')
						Cursor(lines,x,y,'w');
					else return; // lines[nextline][0] is word

					return;
				}
				
                if(*(*(lines+*y)+i) == ' '){ 
                    flag = 0;
                    continue;
                }

                if(flag == 0 && *(*(lines+*y)+i) != ' ' && *(*(lines+*y)+i) !=
						'\0' && lines[*y][i] != '\t' ){ 
					//space ---> word. stop	cursor
                    *x = i;
					return;
                }
             if( !(('a'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <= 'z') ||
                            ('A'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <=
                             'Z'))){ 
				 //special case. special char
				 	if('0' <= lines[*y][i] && lines[*y][i] <= '9'){
						flag = 1;
						continue;
					}
				 if(i == *x){
					 flag = 0;
					 continue;
					}
                    *x = i;
                    return;
                }

            }
			return;
		}	

    }else if(value == 'b'){
		/*
		 * 'b' command : Cursor move before word
		 *
		 */

		if(*x == 0 && *y == 0) return; //special case. not able move

		tmp = 0; 

            if(flag == 1){
                 /*  flag 1
				  *  Mechanism : 
				  *  case1. cursor is word's first index.  word(flag==1) ----> space(flag == 0) ---->  word(flag == 1) ----> space(flag == 0)
				  *  case2. cursor is not word's first index. word(flag==1) --->space(flag==0) 
				  */

				if(*x != 0 && (*(*(lines+*y)+*x-1) == ' ' || lines[*y][*x-1] ==
						'\t')){  
					/*
					 * case1. cursor is word's first index
					 *
					 *
					 *
					 */

                    for(i=*x;i>=0;i--)
					{
                        if(( *(*(lines+*y)+i) == ' '|| lines[*y][i] == '\t') && flag == 1){
							// It is case that word ---> space
                            tmp += 1;
                        }
                        
						if(tmp == 2){
							//find result_cursor
                            tmp = 0;
                            *x = i+1;
							
							printf("#success\n");
                            return;
                        }
						
						 /*
						  * set flag
						  */

                        if( *(*(lines+*y)+i) == ' ' || lines[*y][i] == '\t'){
						
                            flag = 0; 
                        }else{
							
                            flag = 1;
                        }
						/*
						 * special case check.
						 */
                        if(i == 0){ 
							
							if(tmp == 1 && (lines[*y][*x] != ' ' ||
										lines[*y][*x] != '\t')){
										*x = 0;
										return;
							}

                          //  *x = WIDTH-1;
							if(*y != 0){
								//not find result_cursor
								*x = WIDTH -1;
								*y -= 1;
							}
							else{ 
								//not able move cursor.
								*x = 0;
								return;
							}

							if(tmp == 1 && (lines[*y][*x] != ' ' ||
										lines[*y][*x] != '\t')){
										*x = 0;
										return;
							}

							Cursor(lines,x,y,'b'); //re_searching
                            return;
                        }

                     if( !(('A'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <= 'Z') ||
                            ('a'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <=
                             'z')) && flag == 1){
							//special case. lines[*x][i] is special character
						 	if('0' <= lines[*y][i] && lines[*y][i]<= '9'){
								continue;
							}
						 	if(*x == i){
								flag = 0;
								tmp +=1;
								continue;
							}
                             *x = i;
                             return;
					}
				  }

                }else{
				      /*
				       *    case2. cursor is not word's first index. word(flag==1) --->space(flag==0) 
				       *
				       */
                    for(i = *x;i>= 0;i--){

                        if( *(*(lines+*y)+i) == ' ' || lines[*y][i] == '\t'){
							//word ---> space , find result_cursor
							*x = i+1;
                            return;
                        }
                        
						if( i == 0){
							//now 
							if(*y != 0){
                             //re Searching
							*x = WIDTH-1;
							*y -= 1;

							Cursor(lines,x,y,'b'); 

							}else{
								//not able move
								*y = 0;
								*x = 0;
								return;
							}
						//	Cursor(lines,x,y,'b');

                            return;
                       }  
                        
                        if( !(('A'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <= 'Z') ||
                            ('a'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <=
                             'z'))){
							//special case. lines[][] is special chararactor
							if(lines[*y][i] <= '9' && '0' <= lines[*y][i])
								continue;

							if(*x == i)
								continue;

                                *x = i;
                                return;
                        }

                    }
                }
            }else{  
				
				  /*  flag 0
				   *  Mechanism : space(flag == 0) ---> word(flag == 1) ---> space(flag == 0)
				   *  
 				   *
				   */

                for(i=*x;i>=0;i--){

                    if(*(*(lines+*y)+i) == ' ' || lines[*y][i] == '\t' ||
							lines[*y][i] == '\0'){
                        if(flag == 1){
							// word ---> space , find result_cursor		
                            *x = i + 1;
                            return;
                        }
						
                        flag = 0;
                    }else{
						// lines[][] is word
                        flag = 1;
                    } 

                    if(i == 0){
						//special case. 
                        if(*(*(lines+*y)+i) == ' ' || lines[*y][i] == '\t'){
							if(*y != 0){
								*y -= 1;
								*x = WIDTH -1;
								Cursor(lines,x,y,'b'); //re_searching
								return;
							}else{
								return;
							}

                            return;
                        }
                        *x = i;
                        return;
                    }
                    if( !(('a'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <= 'z') ||
                            ('A'<= *(*(lines+*y)+i) && *(*(lines+*y)+i) <=
                             'Z') || *(*(lines+*y)+i) == ' ' || lines[*y][i]
							=='\t')){
						//special charactor
					if('0' <= lines[*y][i] && lines[*y][i] <= '9')
						continue;

                    if(lines[*y][i] == '\0') continue;
					*x = i;
                    return;
                    }
					
                }
            }
    }else if(value == '$'){
		/*
		 *
		 * '$' command : Cursor move now line last word's last index 
		 *
		 */
        for(i = WIDTH - 1;i >= 0; i--){
            if(i == WIDTH - 1 && *(*(lines+*y)+i) != ' ' && *(*(lines+*y)+i) !=
                    '\0' && *(*(lines+*y)+i) != '\t'){
                *x = i-1;
                break;
            }
            if( *(*(lines+*y)+i) == ' ' || *(*(lines+*y)+i) == '\0' ||
					lines[*y][i] == '\t'){
                *x = i;
                continue;
            }else{
                *x = *x - 1;
                break;
            }
        }
    }

}
