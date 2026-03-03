
#include "../headers/exec.h"


int exec (char ** argv, int argc, int back){
  int status;
  pid_t id= fork();
     if(id<0){
        printf("Fork failed\n");
        exit(-1);
     }
     else if(id==0){     // If child

          execvp(argv[0],argv);
          printf("Command not found!\n");       // This will only be executed if execvp fails
          return -1;
     }
     else{
      if(back==0)                                      //If not background
        waitpid(id,&status,0);                    // Wait for child to end
        return 0;
     }
}


int tokenise(char *input, Aliases *als,History *myhis){
    char *input_copy=NULL,*input_copy_copy=NULL,*token=NULL;         // Need a copy because strtok will modify the orignal string
     char *lim=" \n";
     int argc=0,i=0,n=0;

      char **argv=NULL,** temp=NULL;

      int rd=0;                 // Flag for redirect chars , 0 = no redirect char found , 1=">" found, 2="<" found, 3= both
      int index[]={0,0};          // Array for indeces of redirect characters in the input
      int back=0;                     //Flag for background character
      int pipe=0;                         //Flag for pipe character
      int wild=0;                       //Flag for wildcard
      int files_num=0;
      int wild_index=0;

      char *rest1=NULL,*rest2=NULL;
       glob_t paths;

     input_copy=strdup(input);                      // Strdup also allocates the memory
    input_copy_copy=strdup(input);
      token=strtok_r(input,lim,&rest1);
      while(token){
                                              //Calculate number of arguements
        token=strtok_r(NULL,lim,&rest1);
        argc++;
      }
    argv=malloc((argc+1)*sizeof(char*));                       // argc + 1 words because argv must be null terminated
    token=strtok_r(input_copy,lim,&rest2);

    while(token){
        argv[i]=token;

        n=which_red(argv[i]);        // Check for redirections
        rd+=n;                                   // Add to flag

        if(n==1)  index[0]=i;                       // Index of >
        else if(n==2) index[1]=i;                   // Index of <


        n=pipe_check(argv[i]);       // Check for pipes
        if(n==1){ pipe=i;
          break;
        }
        n=background_check(argv[i]);        // Check for background character
        if(n==1){ back=1;
        break;
        }

        n=wild_check(argv[i]);                  // Check for wildcards
        if(n==1){
            wild=1;
            wild_index=i;
        }

        token=strtok_r(NULL,lim,&rest2);
        i++;
    }


     argv[i]=NULL;                          // Argv must be a NULL terminated array
     i=0;


    if(strcmp(argv[0],"createAlias")==0){
      if (argc<3){
        printf("Command not provided \n");
        return -1;
      }
      if(create_alias(argv,argc,als)==0){
        printf("Alias created! \n");
        return 0;
      }

    }


    if(strcmp(argv[0],"destroyAlias")==0){

      if(destroy_alias(argv[1],als)==0){
        printf("Alias destroyed! \n");
        return 0;
      }

    }

    if(strcmp(argv[0],"myHistory")==0){
      char *str=print_history(myhis);
      if(str!=NULL){                                                 // If a command was selected in myHistory recursively call for the selected command
        return tokenise(str,als,myhis);
      }
      else return 0;

    }

    temp=check_alias(argv,als);
                                         // If the command given is found in the alias list replace the argv
    if(temp) argv=temp;
                                                            // Note: Using alias and providing extra arguements is not supported right now, arguements have to be included when creating the alias

    if(wild>0){
            if (glob(argv[wild_index], 0, NULL, &paths) != 0) {                  // Get the names of the files using glob
               fprintf(stderr, "Error: glob failed\n");
                return 1;
            }
            files_num= paths.gl_pathc;                                  // Number of files found
    }
    else {
      files_num=1;                                               // If there are no wildcards the loop bellow must be executed once
    }

    for (i = 0; i < files_num ; i++) {
       if(wild>0){
        argv[wild_index]= strdup(paths.gl_pathv[i]);                        // Replace the arguement containing the wild character with a file name
       }
     if(rd>0){

      redirect(argv,rd,index,argc,back);                                    // Handle redirection
     }
      else if(pipe>0) pipe_exec(input_copy_copy,als,myhis);    // Prota pipe check             // Handle pipes


      else {

         exec(argv,argc,back);                                             // Execute commands that dont contain pipes or redirections


      }
}

    free(input_copy);
   free(input_copy_copy);

   free(argv);

    return 0;

}