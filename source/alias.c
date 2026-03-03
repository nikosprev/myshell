
#include "../headers/exec.h"


int create_alias(char **argv,int argc,Aliases *als){
  int i=0;


  if(als->count==MAX_ALIASES){                                              // If the list of aliases is full
    printf("No space left for more aliases, please delete an existing one.\n");
    return -1;
  }
  else{

    if(als->count>0)                                                          // Check if the name is taken
        for(i=0; i<als->count; i++){

            if(strcmp(als->list[i].name,argv[1])==0) {
              printf("Alias name already exist, delete existing alias or provide a different name.\n");
              return -1;
            }
          }



    als->list[als->count].command=malloc(sizeof((argc-2)*sizeof(char)));                      // Allocate memory for the command
  for(i=0; i<argc-2; i++ ){

    als->list[als->count].command[i]=strdup(argv[i+2]);                                                  // Allocate memory and copy the command words
  }


        als->list[als->count].name=strdup(argv[1]);                                       // Allocate memory and copy the name



    if(&(als->list[als->count])==NULL){ perror("Strdup failed"); exit(1);}

    als->count++;


    return 0;
  }



}

int destroy_alias(char *name,Aliases *als){
      int i=0;
      int found=0;

      for(i=0; i<als->count; i++){
        if(strcmp(als->list[i].name,name) ==0){
          found=1;
          break;
        }
      }
      if(found==0){
        printf("Alias not found!\n");
        return -1;
      }
      else{
        if(i<als->count-1)                // If we dont want to delete the last alias on the list replace it with the last
           memcpy(&(als->list[i]),&(als->list[als->count-1]),sizeof(Alias));
        free(&(als->list[als->count-1]));
        als->count--;                                          // Else free the last
          if(&(als->list[i])==NULL){ perror("Memcpy failed"); return -1;}
          return 0;
      }


}
char ** check_alias(char **argv,Aliases *als){
    int i=0;
      int found=0;
    if(als->count>0){


      for(i=0; i<als->count; i++){


        if(strcmp(als->list[i].name,argv[0]) ==0){

          found=1;
          break;
        }

      }

      if(found=1){

        return als->list[i].command;
      }

    }



      return NULL;

}


void free_alias(Aliases *als){
    int i;
    for(i=0; i<als->count; i++)

        free(als->list->name);
        free(als->list->command);
}