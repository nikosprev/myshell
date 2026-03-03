

#include "../headers/exec.h"


int pipe_check(char *str){
  if(strcmp("|",str)==0) return 1;
  else return 0;
}

void pipe_exec(char *input, Aliases *als,History *myhis){             // Find index of the pipe character, break input into 2 and give it to tokenise()
    int count=0,i=0,index;
    int pipefd[2];

    char *command_1 ;                  // First command
    char *command_2;        // Second command

    for(i=0; i<strlen(input); i++){
                                   // Find index
      if(input[i]=='|'){
        index=i;
        break;
      }
    }


    command_1=malloc(index);
    command_2=malloc(strlen(input)-1-index);

    strncpy(command_1,input,index);
    strncpy(command_2,input+index+1,strlen(input)-1-index);

    if(pipe(pipefd) == -1) {  //        Create pipe
        perror("Pipe creating failed");
        exit(-1);
    }

    if(fork() == 0)   // First Child - run first command through tokenize
    {
        dup2(pipefd[1], STDOUT_FILENO);   // Redirect STDOUT to output part of pipe
        close(pipefd[0]);      //close pipe read
        close(pipefd[1]);      //close write pipe
        tokenise(command_1,als,myhis);
        free(command_1);

        exit(0);                             // Tokenise will eventually call exec() which will fork again so we need to terminate here
    }

    if(fork() == 0)   // child 2
    {
        dup2(pipefd[0], STDIN_FILENO);   // Redirect STDIN to Input part of pipe
        close(pipefd[1]);       //closing pipe write
        close(pipefd[0]);       //close read pipe
        tokenise(command_2,als,myhis);

        free(command_2);

        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(0);   // Wait for child 1 to finish
    wait(0);   // Wait for child 2
}