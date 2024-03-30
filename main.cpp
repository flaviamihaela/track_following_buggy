#include "mbed.h" // Mbed library for hardware abstraction on ARM microcontrollers
#include "QEI.h" // Quadrature Encoder Interface library for reading encoder outputs
#include "Motors.h" 
#include "OneWire_Methods.h" // Library for OneWire communication
#include "ds2781.h" // Library for interfacing with the DS2781 battery fuel gauge IC
#include "Encoder.h"
#define PI 3.14159265358979323846 
#define CENTRE 0.153 
#define KT 0.007 // Torque constant for motors
#define GR 18.75 // Gear ratio for motor gearbox
#define WHEEL_RADIUS 0.040 // Radius of wheel in meters
#define PERIOD 0.00005 // Control loop period in seconds

// Initialize digital input/output for OneWire communication - battery monitoring
DigitalInOut one_wire_pin(PC_14); 
// Initialize motor controller
Motors AB(PB_13, PB_1, PC_10, PC_12, PC_11);
// Initialize encoders for each wheel - pins and settings
QEI* Enc_A = new QEI(PA_13, PA_14, NC, 256, QEI::X4_ENCODING);
QEI* Enc_B = new QEI(PC_8, PC_6, NC, 256, QEI::X4_ENCODING);
// Initialize Encoder object with the two QEI instances
Encoder Encoders(Enc_A, Enc_B);

// Function to calculate duty cycle for motor control based on desired speed and battery voltage
float calcDC (float speed) {
    int VoltageReading; 
    float VBatt;
    VoltageReading = ReadVoltage(); // Read battery voltage
    VBatt = VoltageReading * 0.00976; // Convert reading to actual voltage
    // Calculate and return duty cycle
    return ((KT * (speed / WHEEL_RADIUS) * GR) + VBatt) / (2.0 * VBatt);
}

// PID-like controller function to adjust motor speeds based on reference speeds and feedback
void Controller(float RefSpeedL, float RefSpeedR, float Kp, float Ki, float Kd) {
    // Variable initialization for control loop
    float outL, outR, currentL, currentR, errorL, errorR, dt, cumErrorL, cumErrorR, rateErrorL, rateErrorR, DCL = 0.5, DCR = 0.5, lastErrorL, lastErrorR, previousTime = 0, currentTime = 0;
    long count = 0;
    Timer t;
    t.start(); // Start timer for control loop timing

    // Control loop
    while (1) {  
        wait(PERIOD); // Wait for next control loop iteration
        
        // Read current speeds from encoders
        currentL = Encoders.GetSpeedL();
        currentR = Encoders.GetSpeedR();
        // Calculate speed errors
        errorL = RefSpeedL - currentL;
        errorR = RefSpeedR - currentR;
        
        // Time calculations for integral and derivative terms
        currentTime = t.read_us() / 1000000.0;
        dt = currentTime - previousTime;
        cumErrorL += errorL * dt;
        cumErrorR += errorR * dt;
        //rateErrorL = (errorL - lastErrorL) / dt;
        //rateErrorR = (errorR - lastErrorR) / dt;
        
        // PID control calculations for left and right motors
        outL = (Kp * errorL) + (Ki * cumErrorL) /* + (Kd * rateErrorL) */ + RefSpeedL;
        outR = (Kp * errorR) + (Ki * cumErrorR) /* + (Kd * rateErrorR) */ + RefSpeedR; 
        
        // Calculate duty cycles for both motors
        DCL = calcDC(outL);
        DCR = calcDC(outR);  
        
        // Anti-windup and saturation handling for integral term
        if(count > 10000) {
            count = 0;
            cumErrorL = 0;
            cumErrorR = 0;
        }
        
        // Limit duty cycle to valid range [0, 1]
        DCL = std::min(std::max(DCL, 0.0f), 1.0f);
        DCR = std::min(std::max(DCR, 0.0f), 1.0f);

        // Set motor speeds based on calculated duty cycles
        AB.setSpeed(DCL, DCR);
        
        // Update previous error and time for next loop iteration
        //lastErrorL = errorL;
        //lastErrorR = errorR;
        previousTime = currentTime;
    }
}

// Predefined maneuver functions (forward, turn90R, turn90L, turn180) are used to move the robot in specific patterns.
int main() {
    AB.setFreq(20000); // Set PWM frequency for motor control
    AB.setSpeed(0.5, 0.5); // Set initial speed for both motors
    Controller(0.7, -0.7, 0.2, 0.4, 0); // Start control loop with specified reference speeds and PID constants
};
