		
		
		 #include <stdio.h>
		 #include <unistd.h>
		 #include <errno.h>
		 #include <stdlib.h>
		 #include <sys/types.h>
		 #include <string.h>
		 #include <fcntl.h>
		 #include <signal.h>
		 #include <string.h>
		 #include <math.h>
		 #include "kufs.h"
		

/*
Authors:
Cem Uyuk 
Oguzhan Uz
*/
		#define MAX_LINE  100 
		/* Length of a command line entry*/
		
		int read(char inputBuffer[], char *args[],int *background);
		
		
		void rm(char* label, int thick){
		  int check = 0;
		  int test=0;
		  
		  // if label is null, gives error asking for a valid file name
		  if(label!=NULL){
		    char mood;
		    int blocks[3];
		    char buff[1024];
		    _directory_entry _directory_entries[4];
		    int i,j;
		    int e_inode;
		    mood = _inode_table[thick].TT[0];
		    blocks[0] = stoi(_inode_table[thick].XX,2);
		    blocks[1] = stoi(_inode_table[thick].YY,2);
		    blocks[2] = stoi(_inode_table[thick].ZZ,2);
		    if (mood=='F') {
		      printf("Fatal Error: Terminating.\n");
		      exit(1);
		    }
		
			//checks for each block the inode entries and the directory entries with the given name.
			//If everything is succesfull, sets the check that denotes found or not to 1.
		    for (i=0; i<3; i++) {
		      if (blocks[i]==0) continue;
		      readKUFS(blocks[i],(char *)_directory_entries);
		      for (j=0; j<4; j++) {
		        if (_directory_entries[j].F=='0') continue;
		        e_inode = stoi(_directory_entries[j].MMM,3);
		      if (_inode_table[e_inode].TT[0]=='D')  {
		        rm(label, e_inode);
		      }
		      if(strncmp(_directory_entries[j].fname, label, 252)!=0  && thick==CD_INODE_ENTRY){
				  test=1;
			  }
		      else if(strncmp(_directory_entries[j].fname, label, 252)==0 || thick!=CD_INODE_ENTRY){
				  
		        if(stoi(_inode_table[e_inode].XX, 2)!=0){
		        returnBlock(stoi(_inode_table[e_inode].XX, 2));
		      }
		      
		      if(stoi(_inode_table[e_inode].YY, 2)!=0){
		        returnBlock(stoi(_inode_table[e_inode].YY, 2));
		      }
		      
		      if(stoi(_inode_table[e_inode].ZZ, 2)!=0){
		        returnBlock(stoi(_inode_table[e_inode].ZZ, 2));
		      }
		      
		        strncpy(_inode_table[e_inode].TT,"00",2);
		        
		        strncpy(_inode_table[e_inode].XX,"00",2);
		        
		        strncpy(_inode_table[e_inode].YY,"00",2);
		        
		        strncpy(_inode_table[e_inode].ZZ,"00",2);
		        
		        _directory_entries[j].F = '0';
		        _inode_bitmap[e_inode] = '0';
		        
		        writeKUFS(BLOCK_INODE_BITMAP, (char *)_inode_bitmap);
		        writeKUFS(BLOCK_INODE_TABLE, (char *)_inode_table);
		        writeKUFS(blocks[i], (char *)_directory_entries);
		        check=1;;
		      }
		    }
		  }
		
		// If the label, name, is null. Asks to enter again.
		  }else if(label==NULL){
		    printf("Please write correctly: rm <filename>\n");
		    return;
		  }else{
			  test=0;
		  }
		  
		  //If the file is not found, then says there is no such file.
		  if(check!=1 && thick==CD_INODE_ENTRY){
		    printf("There is no such file %s.\n", label);
		  }
		  return;
		}
		
		void display(char* label){
		  int check = 0;
		  int test=0;
		  
		    // if label is null, gives error asking for a valid file name
		  if(label!=NULL){
		    char mood;
		    int blocks[3];
		    char buffer[1024];
		    
		    
		    _directory_entry _directory_entries[4];
		    
		    
		    int i,j;
		    int e_inode;
		    mood = _inode_table[CD_INODE_ENTRY].TT[0];
		    
		    
		    blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
		    blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
		    blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);
		    if (mood=='F') {
		      printf("Fatal Error: Terminating!.\n");
		      exit(1);
		    }
		
				//checks for each block the inode entries and the directory entries with the given name.
			//If everything is succesfull, sets the check that denotes found or not to 1.
		    for (i=0; i<3; i++) {
		      if (blocks[i]==0) continue;
		      readKUFS(blocks[i],(char *)_directory_entries);
		      for (j=0; j<4; j++) {
		        if (_directory_entries[j].F=='0') continue;
		        e_inode = stoi(_directory_entries[j].MMM,3);
		        if(_inode_table[e_inode].TT[0]!='F'){
					test=1;
					}
				else if (_inode_table[e_inode].TT[0]=='F')  {
					
		        if(strncmp(_directory_entries[j].fname, label, 252)==0){
		         
		          if(stoi(_inode_table[e_inode].XX, 2)!=0){
		          readKUFS(stoi(_inode_table[e_inode].XX, 2), (char *)buffer);
		          printf("%s\n", buffer);
		        }
		        
		        if(stoi(_inode_table[e_inode].YY, 2)!=0){
		          readKUFS(stoi(_inode_table[e_inode].YY, 2), (char *)buffer);
		          printf("%s\n", buffer);
		        }
		        
		        if(stoi(_inode_table[e_inode].ZZ, 2)!=0){
		          readKUFS(stoi(_inode_table[e_inode].ZZ, 2), (char *)buffer);
		          printf("%s\n", buffer);
		        }
		        check=1;
		        
		        }
		
		      }
		    }
		  }
		
		// If the label, name, is null. Asks to enter again.
		  }else if(label==NULL){
		    printf("Please write correctly: display <fname>\n");
		    return;
		  }else{
			  test=0;
		  } 
		    //If the file is not found, then says there is no such file.
		  if(check!=1){
		    printf("There is no such file %s.\n", label);
		  }
		  return;
		}
		
		void create(char* label){
		  if(label!=NULL){
		    char mood;
		    int blocks[3];
		    _directory_entry _directory_entries[4];
		
		    int i,j;
		
		    int free_dir_block=-1,free_dir_entry=-1;
		    int free_i_entry;
		
		
		
		    // if we dont have free inode entries
		    if (free_inode_entries == 0) {
		      printf("Error! Inode table has no free space.\n");
		      return;
		    }
		
		    //reads inode entry
		    mood = _inode_table[CD_INODE_ENTRY].TT[0];
		    blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
		    blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
		    blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);
		
		    // in a directory, if the itype is F then this shouldnt happen. We must terminate.
		    if (mood=='F') {
		      printf("Fatal Error: Terminating!\n");
		      exit(1);
		    }
		
		    // checks whether the name exists
		    for (i=0; i<3; i++) {
		      if (blocks[i]==0) { 	
		        if (free_dir_block==-1) free_dir_block=i; 
		        continue;
		      }
		
				// reads a directory entry with casting
		      readKUFS(blocks[i],(char *)_directory_entries); 
		
		      // 4 possible directory entry, so need to check all of them
		      for (j=0; j<4; j++) {
				  // for an unused input
		        if (_directory_entries[j].F=='0') { 
		          if (free_dir_entry==-1) { free_dir_entry=j; free_dir_block=i; } 
		          continue;
		        }
		
				// comparing with user input name
		        if (strncmp(label,_directory_entries[j].fname, 252) == 0) {
		            printf("%.252s: Given name already exists. \n",label);
		            return;
		        }
		      }
		    }
		  
		
		    //checks if there are no space for making a directory
		    if (free_dir_entry==-1 && free_dir_block==-1) {
		      printf("Error! There is no space for a directory. \n");
		      return;
		    }
		    else { // otherwise
				// didn't find an empty entry but not all three blocks have been used
		      if (free_dir_entry == -1) { 
		        free_dir_entry=0;
		
		        if ((blocks[free_dir_block] = getBlock())==-1) {  
		          printf("Error! There is no space in the disk.\n");
		          return;
		        }
		
		        writeKUFS(blocks[free_dir_block],NULL);	
		
		        switch(free_dir_block) {	
					// updating the inode of the directory to let now we are using a new block
		          case 0: itos(_inode_table[CD_INODE_ENTRY].XX,blocks[empty_dblock],2); break;
		          case 1: itos(_inode_table[CD_INODE_ENTRY].YY,blocks[empty_dblock],2); break;
		          case 2: itos(_inode_table[CD_INODE_ENTRY].ZZ,blocks[empty_dblock],2); break;
		        }
		      }
		
		
		
			// getting an empty place
		      free_i_entry = getInode();	
		
		      readKUFS(blocks[free_dir_block],(char *)_directory_entries);	
		      _directory_entries[free_dir_entry].F = '1';			// setting it as used
		      strncpy(_directory_entries[free_dir_entry].fname,label,252);	// putting the name in
		      itos(_directory_entries[free_dir_entry].MMM,empty_ientry,3);	
		      writeKUFS(blocks[free_dir_block],(char *)_directory_entries);	// now writing the block on the disk
		
		
		
		      strncpy(_inode_table[free_i_entry].TT,"FI",2);
		      strncpy(_inode_table[free_i_entry].XX,"00",2);
		      strncpy(_inode_table[free_i_entry].YY,"00",2);
		      strncpy(_inode_table[free_i_entry].ZZ,"00",2);
		
		
		      char entry[3072];
		      
		      char sub_entry1[1024];
		      char sub_entry2[1024];
		      char sub_entry3[1024];
		      char uyuk;
		      int bytes = 0;
		      
		      
		      while((uyuk= getchar())!=27){
		        entry[bytes]=uyuk;
		        bytes++;
		      }
		      if(strlen(entry)>0){
		        strncpy(sub_entry1, entry, 1024);
		        itos(_inode_table[free_i_entry].XX, getBlock(), 2);
		        writeKUFS(stoi(_inode_table[free_i_entry].XX, 2), (char *)sub_entry1);
		        if(strlen(entry)>1024){
		          strncpy(sub_entry2, entry+1024, 1024);
		          itos(_inode_table[free_i_entry].YY, getBlock(), 2);
		          writeKUFS(stoi(_inode_table[free_i_entry].YY, 2), (char *)sub_entry2);
		        }
		        if(strlen(entry)>2048){
		          strncpy(sub_entry3, entry+2048, 1024);
		          itos(_inode_table[free_i_entry].ZZ, getBlock(), 2);
		          writeKUFS(stoi(_inode_table[free_i_entry].ZZ, 2), (char *)sub_entry3);
		        }
		      }
		      writeKUFS(BLOCK_INODE_TABLE, (char *)_inode_table);
		    }
		  }
		}
		
		
		      /*
				After exec, fork() will run. If backg==1 parent will wait, whilst the child
				* will be processing.
		       */
		int main(void)
		{
		  mountKUFS();
		  char inputBuffer[MAX_LINE]; 	        
		  int backg;             	        /* 0/1 bool to detect & */
		  char *args[MAX_LINE/2 + 1];	        
		  pid_t child;            		/* pid of the child */
		  int status;           		
		  int shouldrun = 1;
		
		  int i, upper;
		
		
		  while (shouldrun){            		
		    backg = 0;
		
			/* with read function, reads the next command  line entry */
		    shouldrun = read(inputBuffer,args,&backg);       
		
		    if (strncmp(inputBuffer, "exit", 4) == 0)
		      shouldrun = 0;     /* Exit from kush with exit commandh*/
		
		
		    if (shouldrun) {
				//Checking the entered command line among the possible commands listed below
		      if(strcmp(args[0], "cd")==0){
		        if(args[1]!=NULL){
		          cd(args[1]);
		        }else{
		          printf("cd must be followed by some directory name\n");
		        }
		      }
		      else if(strcmp(args[0], "ls")==0){
		        ls();
		      }
		      else if(strcmp(args[0], "rd")==0){
		        rd();
		      }
		      else if(strcmp(args[0], "md")==0){
					if(args[1]!=NULL){
					md(args[1]);
					}else{
					printf("md command must be followed by other entries. not enough inputs\n");
					}
				
		      }else if(strcmp(args[0], "stats")==0){
		        stats();
		      }
		      else if(strcmp(args[0], "display")==0){
		        display(args[1]);
		      }
		      else if(strcmp(args[0], "create")==0){
		        create(args[1]);
		      }
		      else if(strcmp(args[0], "rm")==0){
		        rm(args[1], CD_INODE_ENTRY);
		      }
		
		    }
		  }
		  return 0;
		}
		
		/**
		 * read, just reads command-line then breaks entry into pieces. It helps us read.
		 */
		
		int read(char inputBuffer[], char *args[],int *background)
		{
		    
		    /* number of characters in the line */
		    int size
		    
		      /* inputBuffer array index */
		      int i;
		      
		      /* beginning of command parameter */
		     int start;	
		     
		     /* index of where to place the next parameter into args[] */
		     int count;
		     
		     int command_number;	
		     
		     int check;
		     check=1;
		
		    count = 0;
		
		    /* read what the user enters on the command line */
		    
		    
		    do {
			  printf("kush>");
			  fflush(stdout);
			  length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
		    }
		    while (inputBuffer[0] == '\n');
		    start = -1;
		    if (size == 0)
		      exit(0);         
	
	
			//If there is an error reding the line, terminates with -1.
		
		    if ( (length < 0) && (errno != EINTR) ) {
		      perror("Reading error!");
		      exit(-1);           
		    }
	
			
			if(check==1){
				
		    for (i=0;i<length;i++) {
		      /* Checking characters in the buffer*/
		
		      switch (inputBuffer[i]){
		      case ' ':
		      case '\t' :               
			if(start != -1){
			  args[count] = &inputBuffer[start];    
			  count++;
			} else{
				check=1;
			}
			
			inputBuffer[i] = '\0'; 
			start = -1;
			break;
		
		      case '\n':              
			if (start != -1){
			  args[count] = &inputBuffer[start];
			  count++;
			}
			
			inputBuffer[i] = '\0';
			args[count] = NULL; 
			break;
		
		      default :           
			if (start == -1)
			  start = i;
			if (inputBuffer[i] == '&') {
			  *background  = 1;
			  inputBuffer[i-1] = '\0';
			}
		      } 
		    }    
		
	
		    if (*background)
		      args[--count] = NULL;
		
		    args[count] = NULL;
		
		    return 1;
		}
		} 
		
	
