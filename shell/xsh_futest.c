#include <xinu.h>
#include <future.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

extern sid32 print_sem;

syscall xsh_futest(int nargs, char *args[]) {
	print_sem = semcreate(1);
	future_t* f_exclusive;
	f_exclusive = future_alloc(FUTURE_EXCLUSIVE, sizeof(int), 1);
	/*if(f_exclusive->state == FUTURE_EMPTY){
		printf("Yes it works\n");
	}*/
	// First, try to iterate through the arguments and make sure they are all valid based on the requirements
	int i = 1;
	bool flag = true;
	if(nargs < 3){
		printf("Syntax: run futest (g|[0-9]+ )+\n");
		signal(run_sem);
		return OK;
	}
	while (i < nargs) {
	     // TODO: write your code here to check the validity of arguments
	     if (strcmp(args[i], "g") == 0){
	     	i++;
	     }
	     else{
		     for (int j=0; j<strlen(args[i]);j++){
		           if(!isdigit(args[i][j])){
		                  printf("Syntax: run futest [g ...] [VALUE ...]\n");
    			  	flag = false;
			  	break;								                                     		      }
		   	 
	     		}
			if(flag){
		     		i++;
			}
			else{
				break;}
	    
		}
	}
	if(!flag){
	//invalid arguments found
		signal(run_sem);
		return OK;
	}

        i = 1; // reseting the index

	// Iterate again through the arguments and create the following processes 
	// based on the passed argument ("g" or VALUE")
	while (i < nargs) {
		if (strcmp(args[i], "g") == 0) {
			char id[10];
			sprintf(id, "fcons%d",i);
			resume(create(future_cons, 2048, 20, id, 1, f_exclusive));
		}
		else {
			uint8 number = atoi(args[i]);
			resume(create(future_prod, 2048, 20, "fprod1", 2, f_exclusive, number));
			//printf("\nAfter creating consumers");
			sleepms(5);
		     }
		i++;
	    }
	sleepms(100);
	future_free(f_exclusive);
	//printf("signalling run semaphore");
	signal(run_sem);
	//printf("After signalling run semaphore");
	return OK;
}
