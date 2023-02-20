#include <xinu.h>
#include <prodcons_bb.h>

void producer_bb(int p, int i,sid32 sem_prod, sid32 sem_con) {
  // TODO: implement the following:
  // - Iterates from 0 to count (count including)
  //   - setting the value of the global variable 'n' each time
  //   - print produced value (new value of 'n'), e.g.: "produced : 8"
  int c;
  for( c=0; c<i; c++){
  	wait(sem_prod);
	arr_q[head] = c;
	head++;
	if(head==4){
		head = 0;
	}
	printf("name : producer_%d, write : %d\n", p,c);
        signal(sem_con);	
  
  }
}
