/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convenient for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"

/**
 * You should add more #includes here
 */
//#include "okapi/api.hpp"
//#include "pros/api_legacy.h"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */

#ifdef __cplusplus
extern "C" {
#endif
void drive(double xCoord, double yCoord, double heading, double newXCoord, double newYCoord, double newHeading);
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
void opcontroldriver(void);
pros::Controller master(pros::E_CONTROLLER_MASTER);
// (Port number, Cartridge, Clockwise=0 Anticlockwise=1, Unit to use with the motor)
pros::Motor left_back_mtr(8, MOTOR_GEAR_BLUE, 1, MOTOR_ENCODER_DEGREES);
pros::Motor left_front_mtr(7, MOTOR_GEAR_BLUE, 0, MOTOR_ENCODER_DEGREES);
pros::Motor right_back_mtr(3, MOTOR_GEAR_BLUE, 0, MOTOR_ENCODER_DEGREES);
pros::Motor right_front_mtr(2, MOTOR_GEAR_BLUE, 1, MOTOR_ENCODER_DEGREES);
pros::Motor intake(4, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
pros::Motor catapult_clockwise(6, MOTOR_GEAR_RED, 0, MOTOR_ENCODER_DEGREES);
pros::Motor catapult_anticlockwise(5, MOTOR_GEAR_RED, 1, MOTOR_ENCODER_DEGREES);
pros::Motor_Group catapult({catapult_clockwise, catapult_anticlockwise});
pros::Motor hang(12, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
pros::Motor hang_down(13, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
pros::Motor_Group left({left_back_mtr, left_front_mtr});
pros::Motor_Group right({right_back_mtr, right_front_mtr});
bool driver = true;
double xCoord;
double yCoord;
double heading;
//pros::ADIAnalogIn limitSwitch('A');
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif  // _PROS_MAIN_H_
