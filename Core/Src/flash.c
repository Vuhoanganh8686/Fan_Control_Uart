/*
 * flash.c
 *
 *  Created on: Aug 12, 2023
 *      Author: Acer
 */

#include "flash.h"
#include "message_handle.h"

uint32_t power_page = 0;
uint32_t control_mode_page = 1;
uint32_t wind_mode_page = 2;
uint32_t p_error_page;



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
	flash_erase_power_init.PageAddress = ADDR_FLASH_PAGE_0;

	flash_erase_control_mode_init.TypeErase = FLASH_TYPEERASE_PAGES ;
	flash_erase_control_mode_init.NbPages = 1;
	flash_erase_control_mode_init.PageAddress = ADDR_FLASH_PAGE_1;

	flash_erase_wind_mode_init.TypeErase = FLASH_TYPEERASE_PAGES;
	flash_erase_wind_mode_init.NbPages = 1;
	flash_erase_wind_mode_init.PageAddress = ADDR_FLASH_PAGE_2;
}

void mutate_power(POWER power){
	/*
	 * @brief Mutate power variable
	 * @param power set value
	 */
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_1;
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_2;
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_0;
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
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADDR_FLASH_PAGE_0, power);
		HAL_FLASH_Lock();
	}
}

void mutate_control_mode(CONTROL_MODE control_mode){
	/*
	 * @brief mutate control mode variable
	 * @control_mode set value
	 */
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_1;
	if(*p_control_mode != control_mode){
		HAL_FLASH_Unlock();
		HAL_FLASHEx_Erase(&flash_erase_control_mode_init, &p_error_page);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADDR_FLASH_PAGE_1, control_mode);
		HAL_FLASH_Lock();
		if(control_mode == MANUAL){
			uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_2;
			wind_control(htim1, *p_wind_mode);
		}
	}
}

void mutate_wind_mode(WIND_MODE wind_mode){
	/*
	 * @brief mutate wind mode variable
	 * @wind_mode set value
	 */

	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_2;
	if(*p_wind_mode != wind_mode){
		HAL_FLASH_Unlock();
		HAL_FLASHEx_Erase(&flash_erase_wind_mode_init, &p_error_page);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADDR_FLASH_PAGE_2, wind_mode);
		HAL_FLASH_Lock();
		wind_control(htim1, wind_mode);
	}
}

uint8_t get_power(){
	/*
	 * @return power
	 */
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_0;
	return *p_power;
}

uint8_t get_control_mode(){
	/*
	 * @return control mode
	 */
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_1;
	return *p_control_mode;
}

uint8_t get_wind_mode(){
	/*
	 * @return wind mode
	 */
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_2;
	return *p_wind_mode;
}

void wind_control(TIM_HandleTypeDef htim1, WIND_MODE wind_mode){
	/*
	 * @brief Control PWM
	 * @param tim_handler timer handler
	 * @param wind_mode wind level to be controlled
	 */
	switch(wind_mode){
	case LEVEL_0:
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		break;
	case LEVEL_1:
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 599);
		break;
	case LEVEL_2:
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 799);
		break;
	}
}

void check_and_fix_variables(){
	/*
	 *@brief check correctness of the flash memory storing state variables and fix
	 */
	uint32_t* p_power = (uint32_t*) ADDR_FLASH_PAGE_0;
	uint32_t* p_control_mode = (uint32_t*) ADDR_FLASH_PAGE_1;
	uint32_t* p_wind_mode = (uint32_t*) ADDR_FLASH_PAGE_2;
	if(*p_power != ON && *p_power != OFF){
		mutate_power(OFF);
	}
	if(*p_control_mode != AUTOMATIC && *p_control_mode != MANUAL){
		mutate_control_mode(AUTOMATIC);
	}
	if(*p_wind_mode != LEVEL_0 && *p_wind_mode != LEVEL_1 && *p_wind_mode != LEVEL_2){
		mutate_wind_mode(LEVEL_0);
	}
}




