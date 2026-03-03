
#include "../headers/exec.h"


int wild_check(char * str){
  if(strchr(str,'?') || strchr(str,'*')) return 1;
  else return 0;
}
