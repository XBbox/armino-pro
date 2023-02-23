#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>
#include "hal_button.h"


#define main
beken_thread_t main_thread_h;

void user_app_main(void){
    hal_button_thread();

    // 2.create task
    // bk_err_t err = rtos_create_thread()
}

int main(void)
{
	// extern int app_main(void);
	bk_set_printf_sync(true);
	shell_set_log_level(BK_LOG_WARN);
	// legacy_init();

	user_app_main();

	return 0;
}