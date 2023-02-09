#include <xinu.h>
#include <prodcons.h>

void producer(int count,sid32 sem_prod, sid32 sem_con) {
  // TODO: implement the following:
  // - Iterates from 0 to count (count including)
  //   - setting the value of the global variable 'n' each time
  //   - print produced value (new value of 'n'), e.g.: "produced : 8"
  int i;
  for( i=0; i<=count; i++){
	//wait(sem_prod);
	//int c = semcount(sem_prod);
	//printf("sem count = %d\n",c);
  	wait(sem_prod);
	n = i;
	printf("produced : %d\n", n);
        signal(sem_con);	
  
  }
}
