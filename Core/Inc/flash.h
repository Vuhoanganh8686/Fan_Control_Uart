/*
 * flash.h
 *
 *  Created on: Aug 12, 2023
 *      Author: Acer
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "message_handle.h"
#include "stm32f3xx_hal.h"

extern TIM_HandleTypeDef htim1;

/* Base address of the Flash sectors */
#define ADDR_FLASH_PAGE_255    ((uint32_t)0x0807F800) /* Base @ of Sector 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_254    ((uint32_t)0x0807F000) /* Base @ of Sector 1, 2 Kbytes */
#define ADDR_FLASH_PAGE_253    ((uint32_t)0x0807E800) /* Base @ of Sector 2, 2 Kbytes */
//#define ADDR_FLASH_PAGE_3    ((uint32_t)0x0800C000) /* Base @ of Sector 3, 2 Kbytes */
//#define ADDR_FLASH_PAGE_4    ((uint32_t)0x08010000) /* Base @ of Sector 4, 2 Kbytes */
//#define ADDR_FLASH_PAGE_5    ((uint32_t)0x08020000) /* Base @ of Sector 5, 2 Kbytes */
//#define ADDR_FLASH_PAGE_6    ((uint32_t)0x08040000) /* Base @ of Sector 6, 2 Kbytes */
//#define ADDR_FLASH_PAGE_7    ((uint32_t)0x08060000) /* Base @ of Sector 7, 2 Kbytes */

void flash_erase();
void mutate_power(POWER power);
void mutate_control_mode(CONTROL_MODE control_mode);
void mutate_wind_mode(WIND_MODE wind_mode);
//void wind_control(TIM_HandleTypeDef tim_handler, WIND_MODE wind_mode);
uint8_t get_power();
uint8_t get_control_mode();
uint8_t get_wind_mode();
void wind_control(TIM_HandleTypeDef tim_handle, WIND_MODE wind_mode);
void check_and_fix_variables();
void restart_before_state();


#endif /* INC_FLASH_H_ */
