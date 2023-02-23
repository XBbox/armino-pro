#ifndef __HAL_BUTTON_H__
#define __HAL_BUTTON_H__

#define BUTTON_H_PRIORITY   5
#define BUTTON_H_SIZE       1024

typedef void (*hal_button_isr)(void);

typedef struct hal_button
{
    /* data */
    char *model;
    void (*curModelShow)(void);
}hal_button_t;

typedef struct
{
	hal_button_isr next_cb;
	hal_button_isr prev_cb;
} hal_button_cb;


void hal_button_thread(void);

#endif