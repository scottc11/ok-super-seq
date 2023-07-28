#include "task_handles.h"

TaskHandle_t th_main;
TaskHandle_t th_interrupt_handler;
TaskHandle_t th_sequence_handler;

QueueHandle_t qh_interrupt_queue;
QueueHandle_t qh_sequence_queue;