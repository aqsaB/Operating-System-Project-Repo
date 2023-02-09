/* Global variable for producer consumer */
extern int n; /* this is just declaration */
//extern sid32 sem_con;
//extern sid32 sem_prod;
extern sid32 run_sem;
/* Function Prototype */
void consumer(int count,sid32 sem_prod, sid32 sem_con);
void producer(int count, sid32 sem_prod, sid32 sem_con);
