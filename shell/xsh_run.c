/* xsh_run.c - xsh_run */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <shprototypes.h>

/*------------------------------------------------------------------------
 * xsh_run - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_run(int nargs, char *args[]) {

	/* Output info for '--help' argument */

	// Print list of available functions
	 if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {
	 printf("hello\n");
	 printf("prodcons\n");
       	 return OK;
       	}

	else if(strncmp(args[1], "hello", 5) == 0) {
		  /* create a process with the function as an entry point. */
		  resume(create((void *) xsh_hello, 4096, 2, "hello function", 2, nargs - 1, &(args[1])));
	}

	else if(strncmp(args[1], "prodcons", 8) == 0) {
		  /* create a process with the function as an entry point. */
		  resume(create((void *) xsh_prodcons, 4096, 2, "prodcons function", 2, nargs - 1, &(args[1])));
	}
	 else{
		printf("Unknown command. Please try again");
	     }

	



	/* Check argument count */

	/*if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	if (nargs < 2) {
		                fprintf(stderr, "%s: too less arguments\n", args[0]);
				                fprintf(stderr, "Try '%s --help' for more information\n",
								                        args[0]);
						                return 1;
	}
	//ascdate(now, datestr);
	printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]); 
*/
	return 0;
}
