/*
 * sensor_fcn.h
 *
 *  Created on: Mar 23, 2026
 *      Author: Antonin Putala
 */

#ifndef INC_SENSOR_FCN_H_
#define INC_SENSOR_FCN_H_

/* Includes */
#include "main.h"

/* Extern global variables */
extern sensor_data_t measured_data;
extern settings_t    set_data;

/* Function prototypes */
void sensor_init(void);
void sensor_read(void);

#endif /* INC_SENSOR_FCN_H_ */
