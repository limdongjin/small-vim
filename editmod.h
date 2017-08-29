void swap(char *a,char *b){
	//swap Function
    
	char tmp = *a;
    *a = *b;
    *b = tmp;

}


void shift_left(char *a, int x, int y){ 
	// a shift left 
	int i;
	char tmp; 
	int xx;
	int yy;
	
	for(i=WIDTH;i>= x-1;i--){
		swap(&(a[WIDTH-1]), &(a[i]));
		printf("#Swap %c %c ---> %c %c\n",a[i], a[WIDTH-1], a[WIDTH-1],a[i]);
	}



}

void shift_pp_left(char **lines, int *x,int *y){
	int i;
	char tmp; 
	int xx;
	int yy;
	
	for(i=WIDTH;i>= *x-1;i--){
//		swap(a
	}


}                                  



void shift_right(char *a,int index){
    //a shift right
	
	int i;
	
    for(i=0;i<WIDTH-index;i++)
        swap(a,a+i);
}
void line_swap(char *a, char *b){
	// line swap

	char *tmp;
	int i,j;
	
	tmp = (char*)malloc(sizeof(char)*WIDTH);
	
	for(i=0;i<WIDTH;i++){
		tmp[i] = b[i];
	}
	
	for(i=0;i<WIDTH;i++){
		b[i] = a[i];
	}
	for(i=0;i<WIDTH;i++){
		a[i] = tmp[i];
	}
	free(tmp);
	return;
}
void shift_pp_right(char **lines,int *x, int *y,char c){
	int i;
	char tmp;
	int xx;
	int yy;
	
	if(*x >= WIDTH && *y >= HEIGHT)
		return;
	
	for(i=0;i<WIDTH-*x;i++)
		swap(*(lines+*y)+*x,*(lines+*y)+*x+i);
	
	tmp = *(*(lines+*y)+*x);
	*(*(lines+*y)+*x) = c;
	c = tmp;
	
	xx = *x; yy = *y;
	xx = 0; yy += 1;

	if(*(*(lines+*y)+WIDTH-1) == '\n' || *(*(lines+*y)+WIDTH-1) == '\0'){
		shift_right((*(lines+yy)+xx),xx);
		
		*(*(lines+yy)+xx) = c;
		return;
	}else{
		if(c == '\0')
			return;
		shift_pp_right(lines,&xx,&yy,c);
	}
	return;
	
}
void Edit_ch(char **lines, int *x, int *y,char c){
	/*
	 *
	 * Insert and Enter and TAB and and BACKSPACE 
	 *
	 */

	char tmp;
//	int xx = *x,yy =*y;
    int linenumber = 0;
	int i,j;
	int flag = 0;
	int ExistWord = 0; 
	int lastindex = WIDTH - 1;
	int* wordlast;  //line check, final index and save 
	
	wordlast = (int *)malloc(sizeof(int)*40);
	
	/*
	 * FIND linenumber 
	 */

	for(i=HEIGHT - 1;i>= 0 ; i--){
				if(lines[i][0] != '\0'){
					linenumber = i;
					break;
				}
		
		}
     //		printf("#line numer is %d\n",linenumber);

	/*
	 * find wordlast[i] , final index
	 */
	
	for(i=0;i<HEIGHT;i++){
		for(j=WIDTH-1;j>= 0; j--){
			if(lines[i][j] != '\0' && lines[i][j] != ' '){
				wordlast[i] = j;
				break;
			}
		}
	}

	if(c == BACKSPACE){
		
		/*
		 * Standard Backspace
		 */
		if(*x > 0){
			for(i=*x;i<WIDTH;i++){
				//shift left lines[*y]
				lines[*y][i-1] = lines[*y][i];
			}
			*x -= 1;
			return;
			}
			
		/*
		 * Special case.
		 */
		if(*x == 0){

			//not yet solve 

			if(lines[*y][*x+1] == '\0' || wordlast[*y] <= *x){
				for(i=*y; i <= linenumber; i++){
						//line UP
						for(j=0;j<WIDTH;j++){
							lines[i][j] = lines[i+1][j];
							
					}
				}
				}else{
					
				}
			if(*y != 0){
				*x = wordlast[*y-1];
				*y -= 1;
			}
			else{
				*x = 0;
			}
			return;
			}

	}
    if(c == ENTER){
			
		/*
		 * Find linenumber
		 */
		for(i=0;i<HEIGHT;i++){
				for(j=0;j<WIDTH;j++){
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
	//	printf("#line numer is %d\n",linenumber);

		if(linenumber == HEIGHT ) // overflow check
			return;

		/*
		 *  line DOWN
		 */
		
		for(i=0; i < linenumber-*y+1;i++){
			line_swap(lines[*y+1],lines[*y+1+i]);	
		}
			
		for(i=0;i<WIDTH;i++){ 
			//next line clear
			lines[*y+1][i] = '\0';
		}
		lines[*y+1][0] = ' '; 
		/*
		 *  Copy Enter line
		 */
		for(i=0;i<WIDTH-*x;i++){
			if(lines[*y][*x+i] == '\0')
				break;
			lines[*y+1][i] = lines[*y][*x+i]; 
		}
		for(i=0;i<WIDTH-*x;i++){
			//enter line clear
			lines[*y][*x+i] = '\0';
		}

		if(lines[*y+1][0] == '\0')
			lines[*y+1][0] = ' ';
		if(lines[*y][0] == '\0')
			lines[*y][0] = ' ';
		//*(*(lines+*y)+*x) = ' ';
		
		/*
		 * Corser Move
		 */

		*y += 1;
		*x = 0;
		//*(*(lines+*y)+*x) = '\n'; 
		
		return;
    }
    else{ 
		
		/*
		 *  Insert 
		 */
        

		/* special case. overflow
	     */
		if(*y == HEIGHT) 
			return; 
		
		if(*x == WIDTH-1){
			if(lines[*y][*x] == ' ' || lines[*y][*x] == '\0'){
				*(*(lines+*y)+*x) = c;
				*x = 0;
				*y += 1;
				return;
			}else{
				lines[*y][*x] = c;
				*x = 0;
				*y += 1;
				return;
			}
		}
		/* 
		 * Standard Insert  
		 */
		if(lines[*y][WIDTH-1] != '\0' && lines[*y][WIDTH-1] != '\n'){
			
			if(*x == 0 && lines[*y][*x] == ' ' && lines[*y][*x+1] == '\0')
				lines[*y][*x] = '\0';

			shift_pp_right(lines,x,y,c);		
			*x += 1;
		}else{
			if(*x == 0 && lines[*y][*x] == ' ' && lines[*y][*x+1] == '\0' )
				lines[*y][*x] = '\0';

			shift_right(*(lines+*y)+*x,*x);
			*(*(lines+*y)+*x) = c;
			*x += 1;
			return;
		}
	}

}
int Editor_mode(char **lines,int *x, int *y,char *filename,char **vimrcset){
		char c;

		draw(lines, *x, *y,vimrcset);
		
		printf("[INSERT] > ./%s > [Cursor]%d:%d\n",filename,*y,*x);
		printf("-- INSERT --\n");
		/*
		 * command read. 
		 * command : insert , Enter , TAB, Backspace
		 *
		 */
		c = getch();
		
		
		if( c != ESC){
			/*
			 *  Command execute
			 */
			Edit_ch(lines, x, y, c);
			return EDIT;
		}else{
			/*
			 * change mode, Command_mode
			 */
			return COMA;
		}	
}


