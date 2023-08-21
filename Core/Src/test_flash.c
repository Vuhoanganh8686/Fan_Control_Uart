/*
 * test_flash.c
 *
 *  Created on: Aug 15, 2023
 *      Author: Acer
 */

#include "test_flash.h"

void print_state(uint8_t state){
	printf("Actual:   %d\n", state);
}
void test_get(){
	// Test case 1
	modify_power(OFF);
	printf("Expected: 0\n");
	print_state(get_power());
	printf("\n");

	// Test case 2
	modify_power(ON);
	printf("Expected: 1\n");
	print_state(get_power());
	printf("\n");

}
void test_mutate(){
	//Test case 1
	modify_power(OFF);
	printf("Expected: 0\n");
	print_state(get_power());
	printf("\n");

	//Test case 2
	modify_power(ON);
	printf("Expected: 1\n");
	print_state(get_power());
	printf("\n");

	// Test case 3
	modify_control_mode(AUTOMATIC);
	printf("Expected: 0\n");
	print_state(get_control_mode());
	printf("\n");

	// Test case 4
	modify_control_mode(MANUAL);
	printf("Expected: 1\n");
	print_state(get_control_mode());
	printf("\n");

	// Test case 5
	modify_wind_mode(LEVEL_0);
	printf("Expected: 0\n");
	print_state(get_wind_mode());
	printf("\n");

	//Test case 6
	modify_wind_mode(LEVEL_1);
	printf("Expected: 1\n");
	print_state(get_wind_mode());
	printf("\n");

	//Test case 7
	modify_wind_mode(LEVEL_2);
	printf("Expected: 2\n");
	print_state(get_wind_mode());

}

