


#include "../headers/exec.h"
#include <signal.h>



void prompt(void){
    printf("\033[0;35m");          //Set the text to purple
    printf("in-mysh-now:");             //Display prompt
    printf("\033[0m");                  // Reset color

}






int main (void){
    char *input=NULL,*token_copy=NULL,*token=NULL;
    char *lim=";\n";
    char* rest = NULL;
    size_t s=MAX_INPUT;
    int commands=0,status;






    Aliases *als=malloc(sizeof(Aliases));                   // Create struct for aliases and history
    als->count=0;

    History *myhis=malloc(sizeof(History));
    myhis->count=0;


    while(1){


        input=NULL;



      prompt();

      if(getline(&input,&s,stdin)==-1) return -1;                       // Read a line


      if(input[0]!='\n'){

        token=strtok_r(input,lim,&rest);                            // First divide the input at semicolons , in tokenise() we divide at spaces

      while(token){

          token_copy=strdup(token);                                       // Copy token so it can potentially be stored in history
           if(tokenise(token,als,myhis)==0) add_history(myhis,token_copy);                   // Only store in history if the calling of the command was successful
            free(token_copy);

        token=strtok_r(NULL,lim,&rest);

      }

    free(token);

    free(input);
      }


    }
    free_alias(als);
    free_his(myhis);
    free(als);
    free(myhis);

}



