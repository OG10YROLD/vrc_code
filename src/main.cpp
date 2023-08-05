#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// (Port number, Cartridge, Clockwise=0 Anticlockwise=1, Unit to use with the motor)
	pros::Motor left_back_mtr(7, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor left_front_mtr(6, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor right_back_mtr(3, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor right_front_mtr(8, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_clockwise(12, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_anticlockwise(14, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor_Group catapult({catapult_clockwise, catapult_anticlockwise});
	pros::Motor_Group left({left_back_mtr, left_front_mtr});
	pros::Motor_Group right({right_back_mtr, right_front_mtr});
	left.set_brake_modes(MOTOR_BRAKE_HOLD);
	right.set_brake_modes(MOTOR_BRAKE_HOLD);

	left.move_relative(1800, 200);
	right.move_relative(-1800, 200);
	while (true) {
		if (left.is_stopped() && right.is_stopped()) {
			left.move_relative(3600, 200);
			right.move_relative(3600, 200);
			break;
		}
		pros::delay(20);
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	// (Port number, Cartridge, Clockwise=0 Anticlockwise=1, Unit to use with the motor)
	pros::Motor left_back_mtr(7, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor left_front_mtr(6, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor right_back_mtr(3, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor right_front_mtr(8, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_clockwise(12, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_anticlockwise(14, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor_Group catapult({catapult_clockwise, catapult_anticlockwise});
	catapult.set_brake_modes(MOTOR_BRAKE_HOLD);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);
		int r2_press = master.get_digital_new_press(DIGITAL_R2);
		int up = master.get_digital(DIGITAL_UP);

		left_back_mtr = left;
		left_front_mtr = left;
		right_back_mtr = right;
		right_front_mtr = right;
		
		if ((r2_press || up) && catapult.is_stopped()) {
			catapult.move_relative(840, 200);
			catapult.brake();
		}

		pros::delay(20);
	}
}
