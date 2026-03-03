#ifndef ALIAS_H
#define ALIAS_H




#define MAX_ALIASES 10

typedef struct alias{                      // Assuming aliases are not permanent and are deleted when we exit the shell
   char *name;
   char **command;                               // This holds the commands + arguements specified
}Alias;



typedef struct aliases{
  Alias list[MAX_ALIASES];                     // This struct stores all the aliases created
  int count;
}Aliases;


int create_alias(char **,int ,Aliases *);            // Creates an alias
int destroy_alias(char *,Aliases *);                    // Destroys an alias
char ** check_alias(char **,Aliases *);                 // Check if a word is the name of any stored alias
void free_alias(Aliases *);                               // Frees memory in the list of aliases

#endif