#include <xinu.h>
#include <prodcons_bb.h>

void consumer_bb(int q, int j, sid32 sem_prod, sid32 sem_con) {
  // TODO: implement the following:
  //read elements from tail if queue has elements
 
  int d;
  for(d = 0; d<j; d++){
	wait(sem_con);
	int element = arr_q[tail];
	tail++;
	if(tail==4){
		tail = 0;
	}
	printf("name : consumer_%d, read : %d\n",q,element);
        signal(sem_prod);
  }
  if(head==tail && (q+1)==total_consumers){
  	signal(run_sem);
}
}
