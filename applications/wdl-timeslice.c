#include <rtthread.h>

#define THREAD_STACK_SIZE       1024
#define THREAD_PRIORITY         20
#define THREAD_TIMESLICE        10

static void thread_entry(void *parameter)
{
    rt_uint32_t name_num;
    rt_uint32_t cout=0;

    name_num=(rt_uint32_t)parameter;
    while(1)
    {
        if(cout%5==0)
        {
            rt_kprintf("thread %d is running,cout is %d\n",name_num,cout);
            if(cout>200)return;
        }
        cout++;
    }
}

int creat_thread_timeslice(){
    rt_thread_t flag;
    flag=rt_thread_create("wdl-timeslice_1",thread_entry,(void*)1,THREAD_STACK_SIZE,THREAD_PRIORITY,THREAD_TIMESLICE);
    if(flag!=NULL)
        rt_thread_startup(flag);
    else 
        return -1;
    flag=rt_thread_create("wdl-timeslice_2",thread_entry,(void*)2,THREAD_STACK_SIZE,THREAD_PRIORITY,THREAD_TIMESLICE-5);
    if(flag!=NULL)
        rt_thread_startup(flag);
    else 
        return -1;//创建失败返回信号
    return 0;
}