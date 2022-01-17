/*
 * StepperMotor.h
 *
 *  Created on: Mar 12, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATE_STEPPERMOTOR_H_
#define INC_SINTMATE_STEPPERMOTOR_H_

#define		OC_HOME_VALUE			13000
#define		OC_START_VALUE			2000
#define		OC_END_VALUE			1000
#define		OC_ACCELERATION_FACTOR	8
#define		OC_PULSES_PER_ROTATION	200
#define		OC_TABLE_SIZE			100
#define		OC_HOMING_STEPS			16
#define		OC_HOMING_TABLE_SIZE	OC_HOMING_STEPS
#define		STEP_IDLE				0
#define		STEP_ACCELERATION		1
#define		STEP_STEADY				2
#define		STEP_DECELERATION		3
#define		STEP_HOMING				4
#define		STEP_SPEED_RPM			84
#define		MAX_STEP_SPEED_RPM		120
#define		MIN_STEP_SPEED_RPM		50

#define		HOMING_TIMEOUT			10000

extern	uint32_t	step_phase_state;

void StepperInit(void);
void SintMate_do_step_table(void);
void SintMate_do_rotations(uint32_t rotation_number);
void SintMate_stop_rotations(void);
uint32_t SintMate_do_homing_steps(void);


#endif /* INC_SINTMATE_STEPPERMOTOR_H_ */
