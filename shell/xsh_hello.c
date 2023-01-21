/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tprints hello <string> \n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}

	/* Check argument count */

	if (nargs > 2) {
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

	return 0;
}
