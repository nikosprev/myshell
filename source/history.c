

#include "../headers/exec.h"


void free_his(History *myhis){                                                       // Free commands stored in history
    int i;
    for(i=0; i<myhis->count; i++)
        free(myhis->commands[i]);
}


int add_history(History *myhis,char *input){
      int i;
      if(myhis->count==MAX_HISTORY){                               // If full


        for(i=0; i<MAX_HISTORY-1; i++){
                                                                    //Move stored commands down the list
          free(myhis->commands[i]);                                   //Free least recent
          myhis->commands[i]=strdup(myhis->commands[i+1]);                    // Allocate and copy

        if(myhis->commands[i]==NULL){
             perror("Strdup failed!\n");
             exit(1);
            }

        }

          myhis->commands[i]=strdup(input);                                   // Allocate and copy most recent command


         for(i=0; i<MAX_HISTORY; i++){
            if(myhis->commands[i]==NULL){
              perror("Memcpy failed!\n");
              exit(1);
            }
      }

      return 0;
      }

      else{                                                                             // If not foul
        myhis->commands[myhis->count]=strdup(input);

         myhis->count++;
           if(myhis->commands[myhis->count-1]==NULL){

             perror("Strdup failed!\n");
             exit(1);
            }

        return 0;
      }


}


char * print_history(History *myhis){
  int i,num=0;
  char *input=NULL;
  size_t s=2;
  printf("Last %d commands:\n",myhis->count);
  for(i=0; i<myhis->count; i++){                                        // Printing stored commands in reverse order (Least recent first)

    printf("  %d. %s\n",myhis->count-i,myhis->commands[i]);

  }
  while(1){
  printf("Press enter to exit myHistory, or choose a command by typing its number.\n");
   if(getline(&input,&s,stdin)==-1){printf("Error reading input."); exit(-1);}

    if(input[0]=='\n') return NULL;                         // If "enter" is pressed

      num=atoi(input);
     if(num<1 || num>myhis->count){
      printf("Invalid number entered.\n");
     }
     else{
        printf("\033[0;35m");          //Set the text to purple
        printf(">>");
        printf("\033[0m");                  // Reset color
        printf(" %s\n",myhis->commands[myhis->count-num]);
        return myhis->commands[myhis->count-num];                             // Return the stored command
     }

  }

}