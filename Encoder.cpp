// Include C12832 LCD display library header file
#include "C12832.h" 

// Define wheel radius (meters)
#define WHEEL_RADIUS 0.040 

// Initialize LCD display by specifying pin connections
C12832 lcd(D11, D13, D12, D7, D10); 

class Encoder {
    protected:
        QEI* Encoder_R; // Pointer to QEI object representing right wheel encoder
        QEI* Encoder_L; // Pointer to QEI object representing left wheel encoder
        unsigned long count_R, count_L, oldR, oldL, pulses_R, pulses_L; // Variables to store encoder counts and pulses
        Timer t_R, t_L;  // Timers to measure intervals for speed calculations
        float dt_R, dt_L, speed_R, speed_L, trans_Speed, rot_Speed; // Variables for delta times, wheel speeds, and velocities

    public:
        Encoder(QEI* A, QEI* B): Encoder_R(A), Encoder_L(B) {
            Encoder_L->reset(); // Reset left wheel encoder count
            Encoder_R->reset(); // Reset right wheel encoder count
            count_R = count_L = 0; // Initialize counts to 0
        }

        // Calculate and return speed of right wheel
        float GetSpeedR(void) {
            oldR = count_R; // Store old count.
            count_R = abs(Encoder_R->getPulses()); // Get absolute value of current pulse count
            dt_R = t_R.read_us(); // Read elapsed time in microseconds since timer was last reset

            // Calculate speed based on change in pulse count and elapsed time
            if (Encoder_R->getPulses() < 0) {
                speed_R = -1 * (((count_R - oldR) / 1024.0) / (dt_R / 1000000.0)) * (2.0 * PI * WHEEL_RADIUS);
            } else if (Encoder_R->getPulses() > 0) {
                speed_R = (((count_R - oldR) / 1024.0) / (dt_R / 1000000.0)) * (2.0 * PI * WHEEL_RADIUS);
            }

            t_R.reset(); // Reset timer
            t_R.start(); // Start timer for next measurement
            return speed_R; // Return calculated speed
        }

        // Calculate and return speed of left wheel
        float GetSpeedL(void) {
            oldL = count_L;
            count_L = abs(Encoder_L->getPulses());
            dt_L = t_L.read_us();

            if (Encoder_L->getPulses() < 0) {
                speed_L = -1 * (((count_L - oldL) / 1024.0) / (dt_L / 1000000.0)) * (2.0 * PI * WHEEL_RADIUS);
            } else if (Encoder_L->getPulses() > 0) {
                speed_L = (((count_L - oldL) / 1024.0) / (dt_L / 1000000.0)) * (2.0 * PI * WHEEL_RADIUS);
            }

            t_L.reset();
            t_L.start();
            return speed_L;
        }

        // Calculate and display translational velocity (average of two wheel speeds).
        float GetTransVelocity(void) {
            trans_Speed = (GetSpeedR() + GetSpeedL()) / 2.0; // Average of right and left wheel speeds
            lcd.locate(0, 3); // Set cursor position on LCD
            lcd.printf("Translational: %f", trans_Speed); // Display translational speed on LCD
            return trans_Speed; // Return calculated translational speed
        }

        // Calculate and display angular velocity based on translational speed and wheel radius
        float GetAngularVelocity(void) {
            rot_Speed = (trans_Speed) / WHEEL_RADIUS; // Angular speed calculation
            lcd.locate(0, 14); // Set cursor position on LCD
            lcd.printf("Rotational: %f", rot_Speed); // Display rotational speed on LCD
            return rot_Speed; // Return calculated rotational speed
        }

        // Get pulse count for left wheel encoder
        long GetPulsesL(void) {
            return (Encoder_L->getPulses());
        }

        // Get pulse count for ight wheel encoder
        long GetPulsesR(void) {
            return (Encoder_R->getPulses());
        }

        // Reset both wheel encoders.
        void Reset(void) {
            Encoder_R->reset();
            Encoder_L->reset();
        }
};
