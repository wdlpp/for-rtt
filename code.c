#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;
struct rt_semaphore sem_F, sem_S, sem_T;
static rt_thread_t SemF;
static rt_thread_t SemS;
static rt_thread_t SemT;

static void Sem_Func_1(void *param)
{
	rt_uint32_t count = 0;
	while(1)
	{
		rt_sem_take(&sem_T, RT_WAITING_FOREVER);
		count++;								
		rt_kprintf("The Sem First has done! The number is %d\n",count);
		rt_sem_release(&sem_F);
		if(count == 10)
			break;
		rt_thread_mdelay(10);
	}
	rt_kprintf("The Sem First has over!\n");
    
}


static void Sem_Func_2(void *param)
{
	rt_uint32_t count = 0;
	while(1)
	{
		rt_sem_take(&sem_F, RT_WAITING_FOREVER);
		count++;								
		rt_kprintf("The Sem Second has done! The number is %d\n",count);
		rt_sem_release(&sem_S);
		if(count == 10)
			break;
		rt_thread_mdelay(50);
	}
	rt_kprintf("The Sem Second has over!\n");
    
}


static void Sem_Func_3(void *param)
{
	rt_uint32_t count = 0;
	while(1)
	{
		rt_sem_take(&sem_S, RT_WAITING_FOREVER);
		count++;								
		rt_kprintf("The Sem Third has done! The number is %d\n",count);
		rt_sem_release(&sem_T);
		if(count == 10)
			break;
		rt_thread_mdelay(20);
	}
	rt_kprintf("The Sem Third has over!\n");
    
}


static rt_uint32_t set, get;
int Sem_thread(void)
{
	rt_sem_init(&sem_F, "Sem1",   set, RT_IPC_FLAG_FIFO);
  rt_sem_init(&sem_S, "Sem2",   get, RT_IPC_FLAG_FIFO);
	rt_sem_init(&sem_T, "Sem3",   5, RT_IPC_FLAG_FIFO);

	
	SemF = rt_thread_create("First",
                          Sem_Func_1,
													RT_NULL,
                          THREAD_STACK_SIZE,
                          THREAD_PRIORITY + 1,
													THREAD_TIMESLICE);
	
	SemS = rt_thread_create("Second",
                          Sem_Func_2,
													RT_NULL,
                          THREAD_STACK_SIZE,
                          THREAD_PRIORITY,
													THREAD_TIMESLICE);
	
	SemT = rt_thread_create("Third",
                          Sem_Func_3,
													RT_NULL,
                          THREAD_STACK_SIZE,
                          THREAD_PRIORITY - 1,
													THREAD_TIMESLICE);
													
	rt_thread_startup(SemF);
	rt_thread_startup(SemS);
	rt_thread_startup(SemT);
	
	return 0;
}