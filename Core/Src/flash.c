/*
 * flash.c
 *
 *  Created on: Aug 12, 2023
 *      Author: Acer
 */

#include "flash.h"
#include "message_handle.h"
#include "stdint.h"


uint32_t p_error_page;

// volatile uint32_t ADDR_FLASH_PAGE_255  = 0x08033000;
// volatile uint32_t ADDR_FLASH_PAGE_254  = 0x08032800;
// volatile uint32_t ADDR_FLASH_PAGE_253  = 0x08032000;



FLASH_EraseInitTypeDef flash_erase_power_init ;
FLASH_EraseInitTypeDef flash_erase_control_mode_init;
FLASH_EraseInitTypeDef flash_erase_wind_mode_init;


void flash_erase()
{
    /*
	 * @brief init flash erase variables
	 */
	flash_erase_power_init.TypeErase =  FLASH_TYPEERASE_PAGES;
	flash_erase_power_init.NbPages = 1;
	flash_erase_power_init.PageAddress = ADDR_FLASH_PAGE_102;

	flash_erase_control_mode_init.TypeErase = FLASH_TYPEERASE_PAGES ;
	flash_erase_control_mode_init.NbPages = 1;
	flash_erase_control_mode_init.PageAddress = ADDR_FLASH_PAGE_101;

	flash_erase_wind_mode_init.TypeErase = FLASH_TYPEERASE_PAGES;
	flash_erase_wind_mode_init.NbPages = 1;
	flash_erase_wind_mode_init.PageAddress = ADDR_FLASH_PAGE_100;
}

void modify_power(POWER power){
	/*
	 * @brief Mutate power variable
	 * @param power set value
	 */
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_101;
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_100;
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_102;
	if(*p_power != power){
		if(power == OFF){
			wind_control(htim1, LEVEL_0);
		}
		else{
			if(*p_control_mode == MANUAL){
				wind_control(htim1, *p_wind_mode);
			}
		}
		HAL_FLASH_Unlock();
		HAL_FLASHEx_Erase(&flash_erase_power_init, &p_error_page);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADDR_FLASH_PAGE_102, power);
		HAL_FLASH_Lock();
	}
}

void modify_control_mode(CONTROL_MODE control_mode){
	/*
	 * @brief mutate control mode variable
	 * @control_mode set value
	 */
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_101;
	if(*p_control_mode != control_mode){
		HAL_FLASH_Unlock();
		HAL_FLASHEx_Erase(&flash_erase_control_mode_init, &p_error_page);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADDR_FLASH_PAGE_101, control_mode);
		HAL_FLASH_Lock();
		if(control_mode == MANUAL){
			uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_100;
			wind_control(htim1, *p_wind_mode);
		}
	}
}

void modify_wind_mode(WIND_MODE wind_mode){
	/*
	 * @brief mutate wind mode variable
	 * @wind_mode set value
	 */

	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_100;
	if(*p_wind_mode != wind_mode){
		HAL_FLASH_Unlock();
		HAL_FLASHEx_Erase(&flash_erase_wind_mode_init, &p_error_page);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADDR_FLASH_PAGE_100, wind_mode);
		HAL_FLASH_Lock();
		wind_control(htim1, wind_mode);
	}
}

uint8_t get_power(){
	/*
	 * @return power
	 */
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_102;
	return *p_power;
}

uint8_t get_control_mode(){
	/*
	 * @return control mode
	 */
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_101;
	return * p_control_mode;
}

uint8_t get_wind_mode(){
	/*
	 * @return wind mode
	 */
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_100;
	return *p_wind_mode;
}

void wind_control(TIM_HandleTypeDef tim_handle, WIND_MODE wind_mode){
	/*
	 * @brief Control PWM
	 * @param tim_handler timer handler
	 * @param wind_mode wind level to be controlled
	 */
	switch(wind_mode){
	case LEVEL_0:
		__HAL_TIM_SET_COMPARE(&tim_handle, TIM_CHANNEL_1, 0);
		break;
	case LEVEL_1:
		__HAL_TIM_SET_COMPARE(&tim_handle, TIM_CHANNEL_1, 599);
		break;
	case LEVEL_2:
		__HAL_TIM_SET_COMPARE(&tim_handle, TIM_CHANNEL_1, 799);
		break;
	}
}

void default_state(){
	/*
	 *@brief check correctness of the flash memory storing state variables and fix
	 */ 	
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_102;
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_101;
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_100;
	if(*p_power != ON && *p_power != OFF){
		modify_power(OFF);
	}
	if(*p_control_mode != AUTOMATIC && *p_control_mode != MANUAL){
		modify_control_mode(AUTOMATIC);
	}
	if(*p_wind_mode != LEVEL_0 && *p_wind_mode != LEVEL_1 && *p_wind_mode != LEVEL_2){
		modify_wind_mode(LEVEL_0);
	}
}

void restart_before_state(){
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_102;
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_101;
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_100;
	if(*p_power == ON && *p_control_mode == MANUAL){
		wind_control(htim1, *p_wind_mode);
	}
}




