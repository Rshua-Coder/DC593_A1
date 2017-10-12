#include <stdio.h>
#include <stdlib.h>
#include "SysKer.h"

#define PAUSE_FOR_UITRON_DEBUG 0

void cyg_user_start(void)
{
#if PAUSE_FOR_UITRON_DEBUG
    static int a=0;
#endif

    printf("Hello, eCos world!\n");

#if PAUSE_FOR_UITRON_DEBUG
    while(!a)
    {
        a=a+a;
    }
#endif

#if 0
    cyg_user_start_test_ecos();
#endif

#if 1
    _main();
#endif
}