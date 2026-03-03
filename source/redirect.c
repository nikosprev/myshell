
#include "../headers/exec.h"

int which_red(char *str){

   if(strcmp(">",str)==0 || strcmp(">>",str)==0){

        return 1;
      }
    else if(strcmp("<",str)==0){
      return 2;
      }
    else return 0;
}



int redirect(char **argv,int flag, int *index, int argc, int back){
    pid_t pid;

    int fd_in=-1,fd_out=-1, status = 0;

    pid = fork();
    if (pid == 0) {
        // Child process

        if (flag==1) {  // Output mode
            if(strcmp(argv[index[0]],">>")==0)  fd_out = open(argv[index[0]+1],  O_WRONLY | O_APPEND,0666);       // ">>" shouldnt be a special case but work the same as ">" with the diffence of appending
            else fd_out = open(argv[index[0]+1], O_CREAT |  O_WRONLY,0666);
            if(fd_out < 0)
            perror("File error");
        }
        else if(flag == 2){   // Input mode
             fd_in = open(argv[index[1]+1], O_RDONLY,0666);
             if(fd_in < 0)
                fprintf(stderr, "File error");
        }
        else {                                               // Both
              fd_out = open(argv[index[0]+1], O_CREAT | O_WRONLY,0666 );

              fd_in = open(argv[index[1]+1], O_RDONLY,0666);
              if(fd_in < 0 || fd_out<0)
                    perror("File");
        }



            if(flag==1){
            dup2(fd_out, STDOUT_FILENO);   // Redirect input or output
              argv[index[0]] = NULL;
              argv[index[0]+1] = NULL;

              close(fd_out);          // Close the corresponding pointer so child process can use it
            }


             else if(flag==2){
            dup2(fd_in, STDIN_FILENO);
            argv[index[1]] = NULL;
            argv[index[1]+1] = NULL;
              close(fd_in);

             }

             else{
                 dup2(fd_out, STDOUT_FILENO);   // Redirect output
                 argv[index[0]] = NULL;
                 argv[index[0]+1] = NULL;

                 dup2(fd_in, STDIN_FILENO);          // Redirect input
                 argv[index[1]] = NULL;
                 argv[index[1]+1] = NULL;

                  close(fd_out);
                  close(fd_in);


             }

            if (execvp(argv[0], argv) == -1) {
                perror("SHELL");
            }
            exit(EXIT_FAILURE);

    }
    else if (pid < 0) { // Error forking process
        perror("SHELL");
    }
    else {
        // Parent process. Wait till it finishes execution
        if(back==0) waitpid(pid,&status,0);                    // Wait for child to end if not background
        if(flag==1)  dup2(STDOUT_FILENO, fd_out);
         if(flag==2) dup2(STDIN_FILENO, fd_in);
        else{
            dup2(STDOUT_FILENO, fd_out);
            dup2(STDIN_FILENO, fd_in);
        }
                                        // Reset standard output

        return 0;
    }

}
