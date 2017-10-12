#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* now declare (and allocate space for) some kernel objects,
 like the two threads we will use */
cyg_thread thread_s[2]; /* space for two thread objects */

char stack[2][4096]; /* space for two 4K stacks */

/* now the handles for the threads */
cyg_handle_t simple_threadA, simple_threadB;

/* and now variables for the procedure which is the thread */
cyg_thread_entry_t simple_program;

/* and now a mutex to protect calls to the C library */
#if 1
cyg_mutex_t cliblock;
#else
extern cyg_mutex_t cliblock;
#endif

/* we install our own startup routine which sets up threads */
void test_ecos_simple_program(void)
{
    printf("Entering twothreads' cyg_user_start() function\n");

#if 1
    cyg_mutex_init(&cliblock);
#endif

    cyg_thread_create(4, simple_program, (cyg_addrword_t) 0,
            "Thread A", (void *) stack[0], 4096,
            &simple_threadA, &thread_s[0]);
    cyg_thread_create(4, simple_program, (cyg_addrword_t) 1,
            "Thread B", (void *) stack[1], 4096,
            &simple_threadB, &thread_s[1]);

    cyg_thread_resume(simple_threadA);
    cyg_thread_resume(simple_threadB);
}

/* this is a simple program which runs in a thread */
void simple_program(cyg_addrword_t data)
{
    int message = (int) data;
    int delay;

    cyg_mutex_lock(&cliblock);
    printf("Beginning execution; thread data is %d\n", message);
    cyg_mutex_unlock(&cliblock);

    cyg_thread_delay(200);

    for (;;) {
        /* delay = 1000 + (rand() % 50); */
        delay = 5000*(1+message) + (rand() % 50);

        /* note: printf() must be protected by a
           call to cyg_mutex_lock() */
        cyg_mutex_lock(&cliblock);

        {
            cyg_tick_count_t tick;
            tick = cyg_current_time();
            #if 0
            printf("(%llu) Thread %d: and now a delay of %d clock ticks\n",
                        tick, message, delay);
            #else
//            printf("(%llu) Thread %d\n", tick, message);
            #endif

        }

        cyg_mutex_unlock(&cliblock);
        cyg_thread_delay(delay);
    }
}

