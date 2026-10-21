#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <time.h>

#include <iostream> // input stream and output stream objects, e.g. std::cout, std::cin.
#include <stdio.h>  // input/output functionalities through the use of printf() and scanf().
#include <stdint.h>




void Temperature_Sensor_Init();
int Read_Temperature();
uint8_t alert();
uint8_t Set_Limit(int limit_temp);



#endif