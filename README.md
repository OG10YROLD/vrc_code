# vrc_code
hello world

add auton selector on controller, and create functions for movement please

potential odom layout


#include "pros/vision.hpp"
#include "pros/motion.hpp"
#include "pros/adi.hpp"
#include "pros/api.hpp"
#include <cmath>

#define MAX_VELOCITY 150 //mm/s
#define TRACK_WIDTH 330 //mm (330 for E)

class OdometrySystem {
    public:
        OdometrySystem(int leftEncoderPort, int rightEncoderPort, pros::motor_gearset_e_t gearset);

        void resetEncoders();
        void updateOdometry();
        double getX();
        double getY();
        double getTheta();

    private:
        int leftEncoderPort;
        int rightEncoderPort;
        pros::motor_gearset_e_t gearset;
        double leftEncoderOffset;
        double rightEncoderOffset;
        double x;
        double y;
        double theta;
};

OdometrySystem::OdometrySystem(int leftEncoderPort, int rightEncoderPort, pros::motor_gearset_e_t gearset) {
    this->leftEncoderPort = leftEncoderPort;
    this->rightEncoderPort = rightEncoderPort;
    this->gearset = gearset;

    pros::motor_reset_encoder(leftEncoderPort);
    pros::motor_reset_encoder(rightEncoderPort);

    resetEncoders();
}

void OdometrySystem::resetEncoders() {
    leftEncoderOffset = pros::motor_get_position(leftEncoderPort);
    rightEncoderOffset = pros::motor_get_position(rightEncoderPort);

    x = 0;
    y = 0;
    theta = 0;
}

void OdometrySystem::updateOdometry() {
    double leftEncoderTicks = pros::motor_get_position(leftEncoderPort) - leftEncoderOffset;
    double rightEncoderTicks = pros::motor_get_position(rightEncoderPort) - rightEncoderOffset;

    double deltaL = (leftEncoderTicks * pros::motor_get_encoder_dpc(gearset)) / 360.0;
    double deltaR = (rightEncoderTicks * pros::motor_get_encoder_dpc(gearset)) / 360.0;

    double dL = deltaL / 2.0;
    double dR = deltaR / 2.0;

    double dTheta = (dR - dL) / TRACK_WIDTH;
    double dY = (dR + dL) / 2.0;

    x += dY * cos(theta);
    y += dY * sin(theta);
    theta += dTheta;
}

double OdometrySystem::getX() {
    return x;
}

double OdometrySystem::getY() {
    return y;
}

double OdometrySystem::getTheta() {
    return theta;
}

int main() {
    pros::Task odometryTask(OdometrySystem(7, 12, pros::E_MOTOR_GEARSET_36));
    pros::delay(5000);
    odometryTask.stop();
    return 0;
}
