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

	pros::ADIAnalogIn limitSwitch('A');
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
	/*pros::Controller master(pros::E_CONTROLLER_MASTER);
	// (Port number, Cartridge, Clockwise=0 Anticlockwise=1, Unit to use with the motor)
	pros::Motor left_back_mtr(8, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor left_front_mtr(7, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor right_back_mtr(3, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor right_front_mtr(2, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor intake(4, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_clockwise(6, MOTOR_GEAR_RED, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_anticlockwise(5, MOTOR_GEAR_RED, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor_Group catapult({catapult_clockwise, catapult_anticlockwise});
	pros::Motor mexico_clockwise(19, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor mexico_anticlockwise(12, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor_Group mexico({mexico_clockwise, mexico_anticlockwise});
	pros::Motor_Group left({left_back_mtr, left_front_mtr});
	pros::Motor_Group right({right_back_mtr, right_front_mtr});
	left.set_brake_modes(MOTOR_BRAKE_HOLD);
	right.set_brake_modes(MOTOR_BRAKE_HOLD);

	left.move_relative(720, 200);
	right.move_relative(-720, 200);
	pros::delay(1000);

	while (true) {
		if (master.get_digital(DIGITAL_A)) {
			left.brake();
			right.brake();
			break;
		}
		if (left_back_mtr.is_stopped() && left_front_mtr.is_stopped() && right_back_mtr.is_stopped() && right_front_mtr.is_stopped()) {
			left.move_relative(360, 200);
			right.move_relative(360, 200);
			break;
		}
		pros::delay(20);
	}
	while (true) {
		if (master.get_digital(DIGITAL_A)) {
			left.brake();
			right.brake();
			break;
		}
		pros::delay(20);
	}*/
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
	pros::Motor left_back_mtr(8, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor left_front_mtr(7, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor right_back_mtr(3, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor right_front_mtr(2, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor intake(4, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_clockwise(6, MOTOR_GEAR_RED, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor catapult_anticlockwise(5, MOTOR_GEAR_RED, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor_Group catapult({catapult_clockwise, catapult_anticlockwise});
	pros::Motor mexico_clockwise(19, MOTOR_GEAR_GREEN, 0, MOTOR_ENCODER_DEGREES);
	pros::Motor mexico_anticlockwise(12, MOTOR_GEAR_GREEN, 1, MOTOR_ENCODER_DEGREES);
	pros::Motor_Group mexico({mexico_clockwise, mexico_anticlockwise});
	//pros::ADIAnalogIn limitSwitch('A');

	catapult.set_brake_modes(MOTOR_BRAKE_HOLD);
	intake.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left_back_mtr.set_brake_mode(MOTOR_BRAKE_COAST);
	left_front_mtr.set_brake_mode(MOTOR_BRAKE_COAST);
	right_back_mtr.set_brake_mode(MOTOR_BRAKE_COAST);
	right_front_mtr.set_brake_mode(MOTOR_BRAKE_COAST);

	bool cataStationary = true;
	bool settingPosition = false;
	bool bToggle = false;
	catapult.set_zero_position(0);

	bool mexicoOut = false;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);
		int r2 = master.get_digital(DIGITAL_R2);
		int r1 = master.get_digital(DIGITAL_R1);
		int up = master.get_digital(DIGITAL_UP);
		int l1 = master.get_digital(DIGITAL_L1);
		int l2 = master.get_digital(DIGITAL_L2);
		int b_press = master.get_digital_new_press(DIGITAL_B);
		int x_press = master.get_digital_new_press(DIGITAL_X);

		left_back_mtr = left >= 10 ? pow((left / 11.27), 2) : (left <= -10 ? -pow((-left / 11.27), 2) : 0);
		left_front_mtr = left >= 10 ? pow((left / 11.27), 2) : (left <= -10 ? -pow((-left / 11.27), 2) : 0);
		right_back_mtr = right >= 10 ? pow((right / 11.27), 2) : (right <= -10 ? -pow((-right / 11.27), 2) : 0);
		right_front_mtr = right >= 10 ? pow((right / 11.27), 2) : (right <= -10 ? -pow((-right / 11.27), 2) : 0);

		if (r1) {
			catapult.move_velocity(30);
			settingPosition = true;
			cataStationary = true;
		}
		else if (settingPosition) {
			catapult.brake();
			catapult.set_zero_position(0);
			settingPosition = false;
		}
		else if (r2 || bToggle) {
			catapult.move_velocity(((int)catapult.get_positions()[0] % 180) < 150 ? 63 : 31);
			cataStationary = false;
		}
		else if (!cataStationary) {
			catapult.move_relative(180 - ((int)catapult.get_positions()[0] % 180), 127);
			cataStationary = true;
		}
		if (b_press) {
			bToggle = !bToggle;
		}

		/*if (limitSwitch.get_value() > 25 && cataStationary) {
			catapult.move(63);
		}
		if (limitSwitch.get_value() < 15 && cataStationary) {
			catapult.brake();
		}*/
		
//		if (catapult_clockwise.is_stopped() || catapult_anticlockwise.is_stopped()) {
//			catapult.brake();
//		}

		if (l1) {
			intake.move(127);
		}
		else if (l2) {
			intake.move(-127);
		}
		else {
			intake.brake();
		}

		if (x_press && !mexicoOut) {
			mexico.move_absolute(90, 100);
			mexicoOut = true;
		}

		if (x_press && mexicoOut) {
			mexico.move_absolute(0, 100);
			mexicoOut = false;
		}

		pros::delay(20);
	}
}
