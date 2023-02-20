/* xsh_run.c - xsh_run */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <shprototypes.h>
//#include <prodcons_bb.h> 
/*------------------------------------------------------------------------
 * xsh_run - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
sid32 run_sem;
shellcmd xsh_run(int nargs, char *args[]) {
	run_sem = semcreate(0);
	// Print list of available functions
	 if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {
	 printf("hello\n");
	 printf("list\n");
	 printf("prodcons\n");
	 printf("prodcons_bb\n");
       	 return OK;
       	}

	else if(strncmp(args[1], "hello", 5) == 0) {
		  /* create a process with the function as an entry point. */
		  resume(create((void *) xsh_hello, 4096, 2, "hello function", 2, nargs - 1, &(args[1])));
	}
	 else if(strncmp(args[1], "prodcons_bb", 11) == 0){
	      resume(create((void *) xsh_prodcons_bb, 4096, 2, "prodcons_bb function", 2, nargs - 1, &(args[1])));
	 } 

	else if(strncmp(args[1], "prodcons", 8) == 0) {
		  /* create a process with the function as an entry point. */
		  resume(create((void *) xsh_prodcons, 4096, 2, "prodcons function", 2, nargs - 1, &(args[1])));
	} 
	 else{
		printf("Unknown command. Please try again");
	     }
	wait(run_sem);
	return 0;
}
