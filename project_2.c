#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024
#define ALPHA_OFFSET 97 // ascii code for "a"
#define LETTERS 26 // letters in the alphabet
#define NUMBER_OF_MAPPERS 4
#define NUMBER_OF_RECEIVERS 26
#define READ 0
#define WRITE 1
//FILE *input_file;

int chalk;
int mapper_pipes[NUMBER_OF_MAPPERS][2];
int receiver_pipes[NUMBER_OF_RECEIVERS][2];
pid_t mapper_ids[NUMBER_OF_MAPPERS];
pid_t receiver_ids[NUMBER_OF_RECEIVERS];
bool process_type[3] = {false, false, false};

void make_mapper_pipes(){
	int j;
	for(j = 0; j < NUMBER_OF_MAPPERS; j++){
                 if (pipe(mapper_pipes[j])< 0) {
                 perror("pipe");
                 exit(EXIT_FAILURE);
                }
		//printf("mapper pipe has been made\n");
         }

}

void make_reducer_pipes(){
	int j;
	for(j = 0; j < NUMBER_OF_RECEIVERS; j++){
                 if (pipe(receiver_pipes[j]) < 0) {
                 perror("pipe");
                 exit(EXIT_FAILURE);
                }
	//	printf("reducer pipe has been made: %d\n", j);
        }
}

void make_mappers(int parent_id){
	int i;
	int tempID;
	for(i=0; i<NUMBER_OF_MAPPERS; i++){
		//check that its in the parent process
		if(getpid() == parent_id){
			tempID = fork();
		//	printf("this is the pid of the mapper: %d", tempID);
			//check that the forking actually worked
			if(tempID < 0){
				perror("failed to make a mapper");
				exit(1);
			}
			 //add it to the mapper id array
			mapper_ids[i] = tempID;
			if(getpid() != parent_id){
			//	printf("mapper id is added: %d\n", i);
				//printf("this is the pid of the mapper: %d", tempID);
				//store the number of the mapper and if its main/
				process_type[1] = true;
			//	for(i = 0; i< 3; i++){
			//		printf("bool[%d]:%d\n", i, process_type[i]);
			//	}
				chalk = 1;
//               			close(mapper_pipes[i][WRITE]);
//				close(receiver_pipes[i][READ]);
			}
		}
		
	} 
//	for(i = 0; i<NUMBER_OF_MAPPERS; i++){
//		printf("process id for mapper: %d is %d\n", i, mapper_ids[i]);
//	}
}

void make_reducers(int parent_id){
	int i;
	int tempID;
	for(i=0; i<NUMBER_OF_RECEIVERS; i++){
                //check that its in the parent process
                if(getpid() == parent_id){
                        tempID = fork();
                //      printf("this is the pid of the mapper: %d", tempID);
                        //check that the forking actually worked
                        if(tempID < 0){
                                perror("failed to make a reducer");
                                exit(1);
                        }
                         //add it to the reducer id array
                        receiver_ids[i] = tempID;
                        if(getpid() != parent_id){
                        	process_type[2] = true;
				chalk = 0;
			}
                }

        }
	
}

void close_parent_pipes(){
	//need a switch statement to see when to close what sides
	int i;
	for(i = 0; i<NUMBER_OF_MAPPERS; i++){
		close(mapper_pipes[i][READ]);
		printf("closed the pipes\n");	
	}	
	for(i = 0; i<NUMBER_OF_RECEIVERS; i++){
                close(receiver_pipes[i][READ]);
        }
	//parent: need to close the read ends of both the mapper and the reducer pipes
	//mapper: need to close the write of the mapper and the read of the reducer
	//reducer: need to close the write ends of mapper and reducers and read of mapper
}

void move_through_file(){
	//another switch statement that moves through the file
	//parents: writes each line to alternating mappers, closes the write ends of mappers and reducers
	//mapper: read from the mapper pipe character by character and write to the appropriate reducer pipe
	//reducer: read from the reducer pipe and increment the count based on if the character is the correct letter
	if(process_type[0]== true){
		//parent_move();	
	}
	else if(process_type[1] == true){
		//mapper_move();{
	}
	else{
		//reducer_move();
	}
}

void parent_move(FILE *input_file){
	printf("in the actual parent move method\n");
	char buffer[BUFFER_SIZE];
  	int i;
	int map;	
  	for(i = 0; (fgets(buffer,BUFFER_SIZE, input_file) != NULL); i++)
  	{
		printf("in the for loop\n");
   		printf("%d:%d\n",i, mapper_pipes[i][WRITE]);
		map = mapper_pipes[i% NUMBER_OF_MAPPERS][WRITE];
		 write(map, buffer,strlen(buffer));
		printf("after mapper pipes");
		 fputs(buffer, stdout);
	}

}

void mapper_move(){
}

void reducer_move(){
}



int main(void){

	int j;
	int count = 0;
	pid_t parentid = getpid();

//	FILE *input_file = fopen("input.txt", "r");

	//creates the mapper pipes and stores in the array
	make_mapper_pipes();
	//creates the reciever pipes and stores in the array
	make_reducer_pipes();

	//create the mappers
	int i;
        int tempID;
        for(i=0; i<NUMBER_OF_MAPPERS; i++){
                //check that its in the parent process
        //        if(getpid() == parent_id){
                        tempID = fork();
                //      printf("this is the pid of the mapper: %d", tempID);
                        //check that the forking actually worked
                        if(tempID < 0){
                                perror("failed to make a mapper");
                                exit(1);
                        }
                         //add it to the mapper id array
                       // mapper_ids[i] = tempID;
                        if(tempID == 0){
                        //      printf("mapper id is added: %d\n", i);
                                //printf("this is the pid of the mapper: %d", tempID);
                                //store the number of the mapper and if its main/
                                process_type[1] = true;
                        //      for(i = 0; i< 3; i++){
                        //              printf("bool[%d]:%d\n", i, process_type[i]);
                        //      }
                                chalk = 1;
//                                      close(mapper_pipes[i][WRITE]);
//                              close(receiver_pipes[i][READ]);
				int j;
				for(j = 0; j < NUMBER_OF_MAPPERS; j++){
                  			  close(mapper_pipes[j][READ]);
                  			printf("yo this works\n");
				}	
				 for(j = 0; j < 26; j++){
                 		  // close(reducer_pipes[r][PIPE_WRITE_END]);
                	   		 close(receiver_pipes[j][READ]);
		                  }
				char buffer[BUFFER_SIZE];
                  		read(mapper_pipes[i][READ], buffer,BUFFER_SIZE);
				for(j = 0; j< strlen(buffer); j++){
					if(buffer[j] >= ALPHA_OFFSET && buffer[j] < ALPHA_OFFSET + 26){//we know its a lowercase letter
                      			char input[2] = {buffer[j], '\0'};
                      			//printf("%s\n", input);
                      			write(receiver_pipes[buffer[j] - ALPHA_OFFSET][WRITE],input,1);		
					}
				}
				for(j=0; j< NUMBER_OF_MAPPERS; j++){
                     			 close(mapper_pipes[j][READ]);
                    		}
				for(j=0; j< NUMBER_OF_RECEIVERS; j++){
                     			 close(receiver_pipes[j][WRITE]);
                    		}
				exit(0);	
				}
				else{
				}
		}
		//create reducers
		for(i = 0 ; i< NUMBER_OF_RECEIVERS; j++){
			pid_t tempRedID = fork();
			if(tempRedID< 0){//check for error
          			perror("Error in forking reducers.");
          			exit(EXIT_FAILURE);
			}	
			else if(tempRedID ==0){
				sleep(1);
				int k;
         			 for(k=0; k<NUMBER_OF_MAPPERS; k++){
            				close(mapper_pipes[k][WRITE]);
            				close(mapper_pipes[k][READ]);
          			}
          			for(k=0; k<NUMBER_OF_MAPPERS; k++){
            				close(receiver_pipes[k][WRITE]);
          			}	
				 int count = 0;
          			char buffer[BUFFER_SIZE];
          			read(receiver_pipes[i][READ], buffer, BUFFER_SIZE);
          			//printf("%s\n",buffer);
          
         			 for(k=0; k<strlen(buffer);k++){
           				 if(buffer[k] == j + ALPHA_OFFSET){
             					 count++;
           				 }
          			}	
         			 printf("Count %c : %d\n",j+ALPHA_OFFSET  , count);
				for(k=0; k<26; k++){
         			 close(receiver_pipes[k][READ]);
         			 }

          			//exit
          			exit(0);
        			}else{//in parent
            				//do nothing
        			}
			}
		int h;
    		for(h = 0; h < NUMBER_OF_MAPPERS; h++){
     			 close(mapper_pipes[h][READ]);
    		}
		
		for(h=0; h < NUMBER_OF_RECEIVERS; h++){
     			 close(receiver_pipes[h][READ]);
     			 close(receiver_pipes[h][WRITE]);
    		}
		char readB[BUFFER_SIZE];
    		FILE *input_file = fopen("input.txt","r");
    		h=0;
      		while(fgets(readB, BUFFER_SIZE, input_file) > 0){
        		write(mapper_pipes[h][WRITE],readB, strlen(readB) + 1);
        		h++;
      		}
		
		//close write end of mapper pipes
    		for(h=0; h<4; h++){
      			close(mapper_pipes[h][WRITE]);
    		}

   		 //wait for mappers
    		for(h=0; h<4; h++){
      			wait(NULL);
    		}

    		//wait for reducers
    		for(h=0; h<26; h++){
      			wait(NULL);
    		}

   		 return 0;

}

