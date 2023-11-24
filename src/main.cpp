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
	for (int i = 0; i < 25; i++) {
		int left = master.get_digital(DIGITAL_LEFT);
		if (left) {
			driver = false;
			break;
		}
		pros::delay(20);
	}
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
void competition_initialize() {

}

void drive(double newXCoord, double newYCoord, double newHeading) {
	const double PI = 3.141592653589793;
	const double DB_SEP = 12.4016; // inches between two sides of drivebase
	const double GR = 2; // Gear ratio of drivebase is GR:1 (GR is input)
	
	double xDif = newXCoord - xCoord;
	double yDif = newYCoord - yCoord;
	double travelRotation;
	if (xDif > 0 && yDif > 0) {
		travelRotation = 90-(atan(yDif/xDif)*180/PI)-heading;
	}
	else if (xDif < 0 && yDif > 0) {
		travelRotation = 270+(atan(yDif/-xDif)*180/PI)-heading;
	}
	else if (xDif > 0 && yDif < 0) {
		travelRotation = 90+(atan(-yDif/xDif)*180/PI)-heading;
	}
	else if (xDif < 0 && yDif < 0) {
		travelRotation = 270-(atan(-yDif/-xDif)*180/PI)-heading;
	}
	else {
		travelRotation = 0;
	}
	
	double travelRotationDistanceToTurn = (travelRotation*(PI/180))*DB_SEP; // contains amount for both sides to turn (half for each) to meet new heading, in inches
	double travelDistance = sqrt(pow(xDif, 2) + pow(yDif, 2));
	pros::lcd::set_text(1, std::to_string(xDif));
	pros::lcd::set_text(2, std::to_string(yDif));
	pros::lcd::set_text(3, std::to_string(travelRotation));
	pros::delay(50);
	left.move_relative(360*((travelRotationDistanceToTurn/2)/(4*PI))*GR, 300);
	right.move_relative(360*(-(travelRotationDistanceToTurn/2)/(4*PI))*GR, 300);
	pros::delay(50);
	while (left.get_target_positions()[0] - left.get_positions()[0] + 5 >= 10 && right.get_target_positions()[0] - right.get_positions()[0] + 5 >= 10) {
		pros::delay(50);
	}
	pros::delay(300);
	left.move_relative(360*((travelDistance*24)/(4*PI))*GR, 300); // 24 inches in a tile
	right.move_relative(360*((travelDistance*24)/(4*PI))*GR, 300);
	pros::delay(50);
	while (left.get_target_positions()[0] - left.get_positions()[0] + 5 >= 10 && right.get_target_positions()[0] - right.get_positions()[0] + 5 >= 10) {
		pros::delay(50);
	}
	pros::delay(100);
	double finalRotation = newHeading - (heading + travelRotation);
	double finalRotationDistanceToTurn = (finalRotation*(PI/180))*DB_SEP;
	left.move_relative(360*((finalRotationDistanceToTurn/2)/(4*PI))*GR, 300);
	right.move_relative(360*(-(finalRotationDistanceToTurn/2)/(4*PI))*GR, 300);
	pros::delay(50);
	while (left.get_target_positions()[0] - left.get_positions()[0] + 5 >= 10 && right.get_target_positions()[0] - right.get_positions()[0] + 5 >= 10) {
		pros::delay(50);
	}
	xCoord = newXCoord;
	yCoord = newYCoord;
	heading = newHeading;
}

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
	left.set_brake_modes(MOTOR_BRAKE_HOLD);
	right.set_brake_modes(MOTOR_BRAKE_HOLD);
	left.set_zero_position(0);
	right.set_zero_position(0);

	xCoord = 1;
	yCoord = 0;
	heading = 0;

	drive(1.5, 1, 0);
	//drive(1.5, 1, 90);

	// left.move(-100);
	// right.move(-100);
	// pros::delay(800);
	// left.move(80);
	// right.move(80);
	// pros::delay(400);
	// left.move(50);
	// right.move(-50);
	// pros::delay(500);
	// left.move(100);
	// right.move(100);
	// pros::delay(1300);
	// left.brake();
	// right.brake();

	// left.move_relative(720, 200);
	// right.move_relative(-720, 200);
	// pros::delay(1000);
	// while (true) {
	// 	if (master.get_digital(DIGITAL_A)) {
	// 		left.brake();
	// 		right.brake();
	// 		break;
	// 	}
	// 	if (left_back_mtr.is_stopped() && left_front_mtr.is_stopped() && right_back_mtr.is_stopped() && right_front_mtr.is_stopped()) {
	// 		left.move_relative(360, 200);
	// 		right.move_relative(360, 200);
	// 		break;
	// 	}
	// 	pros::delay(20);
	// }
	// while (true) {
	// 	if (master.get_digital(DIGITAL_A)) {
	// 		left.brake();
	// 		right.brake();
	// 		break;
	// 	}
	// 	pros::delay(20);
	// }
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
	if (driver) {
		opcontroldriver();
	}
	else {
		autonomous();
	}
}
void opcontroldriver() {

	catapult.set_brake_modes(MOTOR_BRAKE_HOLD);
	intake.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left.set_brake_modes(MOTOR_BRAKE_COAST);
	right.set_brake_modes(MOTOR_BRAKE_COAST);
	hang.set_brake_mode(MOTOR_BRAKE_HOLD);
	
	bool cataStationary = true;
	bool settingPosition = false;
	bool bToggle = false;
	catapult.set_zero_position(0);

	bool hangUp = false;
	hang.set_zero_position(0);
	
	bool turning = false;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left_stick = master.get_analog(ANALOG_LEFT_Y);
		int right_stick = master.get_analog(ANALOG_RIGHT_Y);
		int r2 = master.get_digital(DIGITAL_R2);
		int r1 = master.get_digital(DIGITAL_R1);
		int up = master.get_digital(DIGITAL_UP);
		int l1 = master.get_digital(DIGITAL_L1);
		int l2 = master.get_digital(DIGITAL_L2);
		int b_press = master.get_digital_new_press(DIGITAL_B);
		int x_press = master.get_digital_new_press(DIGITAL_X);
		int a_press = master.get_digital_new_press(DIGITAL_A);
		int y_press = master.get_digital_new_press(DIGITAL_Y);

		// Checks whether sticks indicate turning or not
		if (left_stick <= -10 && right_stick >= 10 || left_stick >= 10 && right_stick <= -10) {
			turning = true;
		}
		else {
			turning = false;
			// Sets positions to 0 so we can check if they are not the same when we go forward or backward
			left.set_zero_position(0);
			right.set_zero_position(0);
		}

		// Controls drivebase
		if (turning) {
			left = (left_stick >= 10 ? pow((left_stick / (14)), 2) : (left_stick <= -10 ? -pow((-left_stick / (14)), 2) : 0));
			right = (right_stick >= 10 ? pow((right_stick / (14)), 2) : (right_stick <= -10 ? -pow((-right_stick / (14)), 2) : 0));
		}
		else {
			//double amount = left_stick > right_stick ? left_stick : right_stick;
			left = (left_stick >= 10 ? pow((left_stick / (11.27)), 2) : (left_stick <= -10 ? -pow((-left_stick / (11.27)), 2) : 0));
			right = (right_stick >= 10 ? pow((right_stick / (11.27)), 2) : (right_stick <= -10 ? -pow((-right_stick / (11.27)), 2) : 0));
		}
		
		// Code to control catapult. Only one of these cases can happen at a time, only if all preceding cases didn't happen. Very simple.
		if (r1) { // Firstly, checks R1, which is used to set the catapult to any position so that it can be fired a fixed amount by other buttons.
			catapult.move_velocity(30);
			settingPosition = true;
			cataStationary = true;
		}
		else if (settingPosition) { // This may confuse you. Remember, this only runs if R1 isn't being pressed (see last case). Thus, this runs when you let go of R1
			catapult.brake();
			catapult.set_zero_position(0); // Sets the new position to 0 so that it can complete a multiple of however many degrees a turn has
			settingPosition = false; // Signifies that R1 is no longer being used.
		}
		else if (r2) { // Runs the catapult while the R2 is active
			catapult.move_velocity(((int)catapult.get_positions()[0] % 180) < 150 ? 63 : 31);
			cataStationary = false;
		}
		else if (bToggle) { // Runs the catapult while the B toggle is active
			catapult.move_velocity(((int)catapult.get_positions()[0] % 180) < 150 ? 45 : 30);
			cataStationary = false;
		}
		else if (!cataStationary) { // Runs when both R2 is let go and B toggle is not active, in order to return the catapult to a multiple of the number of degrees in a turn.
			catapult.move_relative(180 - ((int)catapult.get_positions()[0] % 180), 127);
			cataStationary = true;
		}

		if (b_press) { // Toggles the B toggle when B is pressed
			bToggle = !bToggle;
		}

		/*if (limitSwitch.get_value() > 25 && cataStationary) {
			catapult.move(63);
		}
		if (limitSwitch.get_value() < 15 && cataStationary) {
			catapult.brake();
		}*/

		if (l1) { // Intake clockwise
			intake.move(-127);
		}
		else if (l2) { // Intake anticlockwise
			intake.move(127);
		}
		else { // Finished using intake
			intake.brake();
		}

		if (x_press && !hangUp) { // Toggles hang forward
			hang.move_absolute(90, 100);
			hangUp = true;
		}
		else if (x_press && hangUp) { // Toggles hang back
			hang.move_absolute(0, 100);
			hangUp = false;
		}

		if (a_press) { // Toggles between coast and hold for catapult
			catapult.set_brake_modes(catapult.get_brake_modes()[0] == MOTOR_BRAKE_HOLD ? MOTOR_BRAKE_COAST : MOTOR_BRAKE_HOLD);
		}

		if (y_press) { // Toggles between coast and hold for drivebase
			left.set_brake_modes(left.get_brake_modes()[0] == MOTOR_BRAKE_HOLD ? MOTOR_BRAKE_COAST : MOTOR_BRAKE_HOLD);
			right.set_brake_modes(right.get_brake_modes()[0] == MOTOR_BRAKE_HOLD ? MOTOR_BRAKE_COAST : MOTOR_BRAKE_HOLD);
		}

		pros::delay(20); // Delay between frames (The code works basically like a game loop)
	}
}
