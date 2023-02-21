/* xsh_prodcons_bb.c - xsh_prodcons_bb */

#include <xinu.h>
#include <prodcons_bb.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int arr_q[5];                 // Definition for global circular array n
int head = 0;
int tail = 0;
int total_consumers = 0;
sid32 sem_prod; //producer semaphore
sid32 sem_con; //consumer semaphore

/*------------------------------------------------------------------------
 * xsh_prodcons_bb - create producer and consumer that will print elements 
 * upto the value of count.
 *------------------------------------------------------------------------
 */
shellcmd xsh_prodcons_bb(int nargs, char *args[]) {

	// Argument verifications and validation
	/* Check argument count */
	/* perform error checking for -- too many arguments
	 * incorrect argument type i.e. check if each char in the argument is an integer
	 * negative values */
	if (nargs > 5) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		signal(run_sem);
		return 1;
	}
	if(nargs < 5){
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		signal(run_sem);
		return 1;
	} 
	int n,m,i,j;

	n = atoi(args[1]);
	m = atoi(args[2]);
	i = atoi(args[3]);
	j = atoi(args[4]);
	total_consumers = m;
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
	}
	if(nargs == 5){
		if(n*i != m*j){
			flag = false;
			printf("Iteration Mismatch Error: the number of producer(s) iteration does not match the consumer(s) iteration\n");
		signal(run_sem);
		return 1;
		}
	}
	if (flag){
		sem_prod = semcreate(1);
		sem_con = semcreate(0);
		for(int p = 0; p<n;p++){
			resume(create(producer_bb,1024,1,"Producer",4,p,i,sem_prod,sem_con));
		}
		for(int q=0; q<m; q++){
			resume(create(consumer_bb,1024,1,"Consumer",4,q,j,sem_prod, sem_con));
		}
	}
	return 0;
}
