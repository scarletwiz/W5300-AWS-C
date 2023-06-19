/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "timer_interface.h"
#include "mqtt_transport_interface.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Timeout */
#define MQTT_PUB_PERIOD (1000 * 10) // 10 seconds
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
static uint8_t g_mqtt_pub_msg_buf[MQTT_BUF_MAX_SIZE] = {
    0,
};
/* USER CODE END PM */

osThreadId_t dhcpTaskHandle;
const osThreadAttr_t dhcp_attributes = {
  .name = "dhcpTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t awsPublishTaskHandle;
const osThreadAttr_t awsPublish_attributes = {
  .name = "awsPublishTask",
  .stack_size = 1204*2, //128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void aws_mqtt_publish_task(void *argument);
void dhcp_task(void *argument);
void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void);

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  awsPublishTaskHandle = osThreadNew(aws_mqtt_publish_task, NULL, &awsPublish_attributes);
  //dhcpTaskHandle = osThreadNew(dhcp_task, NULL, &dhcp_attributes);

}

/* USER CODE BEGIN Header_StartDefaultTask */

void dhcp_task(void *argument){
	dhcp_loop();
}

void aws_mqtt_publish_task(void *argument){
  int retval = 0;
  uint32_t tick_start = 0;
  uint32_t tick_end = 0;
  uint8_t pub_cnt=0;

  tick_start = millis();
  while(1)
  {
    retval = mqtt_transport_yield(MQTT_YIELD_TIMEOUT);

    if (retval != 0)
    {
      printf(" Failed, mqtt_transport_yield returned %d\n", retval);
      while (1);
    }

    tick_end = millis();
    if (tick_end > tick_start + MQTT_PUB_PERIOD)
    {
      tick_start = millis();

      sprintf(g_mqtt_pub_msg_buf, "{\"message\":\"Hello, World!\", \"publish count\":\"%d\"}\n", pub_cnt++);
      mqtt_transport_publish(MQTT_PUB_TOPIC, g_mqtt_pub_msg_buf, strlen(g_mqtt_pub_msg_buf), 0);
    }
  }
}
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

