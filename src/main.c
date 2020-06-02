#include <stdint.h>
#include <pthread.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/Board.h>

extern void *mainThread(void *arg0);
extern void *recvThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

int main(void)
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    Board_init();
    pthread_attr_init(&attrs);

    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, mainThread, NULL);
    if (retc != 0) {
        while (1) {}
    }

    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, recvThread, NULL);
    if (retc != 0) {
        while (1) {}
    }
    BIOS_start();
    return (0);
}
