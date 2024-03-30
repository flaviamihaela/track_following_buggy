#ifndef MOTORS_H // Preprocessor directive to ensure this file is only included once in a single compilation
#define MOTORS_H

#define PI 3.14159265358979323846

class Motors {
protected:
    // PWM outputs for controlling motor speed
    PwmOut pwm1; // Controls speed of Motor A
    PwmOut pwm2; // Controls speed of Motor B
    DigitalOut control1; // Sets motor operation mode.
    DigitalOut control2;
    DigitalOut enable; // Enables/disables motor operation

    float frequency; // Frequency of PWM signal
    float period; // Period of PWM signal
    float speed_A; // Speed setting for Motor A
    float speed_B; // Speed setting for Motor B

public:
    Motors(PinName PWM1, PinName PWM2, PinName ctrl1, PinName ctrl2, PinName ena)
    : pwm1(PWM1), pwm2(PWM2), control1(ctrl1), control2(ctrl2), enable(ena) { // Initialize the pins.
        control1 = 1; // Set motor driver to Bipolar Mode of operation for control pin 1
        control2 = 1; // Set motor driver to Bipolar Mode of operation for control pin 2
        enable = 1;   // Enable motor driver
        pwm1.write(0.5); // Initialize PWM1 with a 50% duty cycle - stops motor if it's centered around 0
        pwm2.write(0.5); 
    }

    // Function to set frequency of PWM signals controlling the motors
    void setFreq(float freq) {
        frequency = freq;
        period = 1 / frequency; // Calculate the period from the frequency.
        pwm1.period(period);
        pwm2.period(period);
    }  

    // Function to set speed of motors by adjusting duty cycle of PWM signals.
    void setSpeed(float dr1, float dr2) {
        pwm1.write(dr1); 
        pwm2.write(dr2); 
    }
};

#endif // End of the preprocessor conditional 
