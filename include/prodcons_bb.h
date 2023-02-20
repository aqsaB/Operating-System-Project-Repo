/* Global variable for producer consumer */
extern int arr_q[5]; /* this is just declaration */
//extern sid32 sem_con;
//extern sid32 sem_prod;
extern int head;
extern int tail;
extern sid32 run_sem;
/* Function Prototype */
void consumer_bb(int p, int i,sid32 sem_prod, sid32 sem_con);
void producer_bb(int q, int j, sid32 sem_prod, sid32 sem_con);
