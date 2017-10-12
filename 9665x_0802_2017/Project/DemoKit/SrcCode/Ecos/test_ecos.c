#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdio.h>

//cyg_mutex_t cliblock;

/* we install our own startup routine which sets up threads */
void cyg_user_start_test_ecos(void)
{
//  cyg_mutex_init(&cliblock);

    //test_ecos_simple_program();
}

