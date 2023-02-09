/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <prodcons.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int n;                 // Definition for global variable 'n'
sid32 sem_prod; //producer semaphore
sid32 sem_con; //consumer semaphore
/* Now global variable n is accessible by all the processes i.e. consume and produce */

/*------------------------------------------------------------------------
 * xsh_prodcons - create producer and consumer that will print elements 
 * upto the value of count.
 *------------------------------------------------------------------------
 */
shellcmd xsh_prodcons(int nargs, char *args[]) {

	// Argument verifications and validations
	int count = 200;    // local varible to hold count

	/* Check argument count */
	/* perform error checking for -- too many arguments
	 * incorrect argument type i.e. check if each char in the argument is an integer
	 * negative values */
	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		return 1;
	}
	bool flag = true;
	if (nargs == 2){
		int i;
		if (args[1][0] == '-'){
			printf("Negative values for count are not allowed\n");
			flag = false;
		}
		else{
			for (i=0; i<strlen(args[1]);i++){
				if(!isdigit(args[1][i])){
					printf("Incorrect argument type. Please enter a valid number\n");
					flag = false;
					break;
				}
			}
		}
		if (flag){
		count = atoi(args[1]);
		}
	}
	if (flag){
		sem_prod = semcreate(1);
		sem_con = semcreate(0);
		resume(create(producer,1024,1,"Producer",3,count,sem_prod, sem_con));
		resume(create(consumer,1024,1,"Consumer",3, count,sem_prod, sem_con));
	}
	return 0;
}
