# track_following_buggy
 
This C++ code provides motor control capabilities to a team-developed robot buggy and was specifically designed to manage and control the operations of two Permanent Magnet Direct Current (PMDC) motors through PWM (Pulse Width Modulation) signals. The system is capable of adjusting the speed and operation mode of each motor independently.

## Control Algorithm

Proportional Integral and Derivative Controller: proportional control calculates the error of the measured variable against the desired set point. The correction that is applied is proportional to this error, i.e. more severe correction for a larger error and less so for a smaller error. The integral and derivative terms help to reduce the steady-state error and oscillation present in the simple proportional control.  
 - P: component that is proportional to the current error. Position corrected with respect to this proportion; i.e. larger correction for larger error. 
 - I: integral term that considers all past values of the cross-track error and uses this to correct the residual steady-state error left by proportional control.  
 - D: temporal derivative of the cross-track error, used to eliminate the overshooting effect present in proportional control. It is effectively an estimate of the future trend of the error; that   
   is when the buggy has turned to reduce the error, D tells the controller that the error has already reduced, and the counter-steering will soften, helping converge the output to the target 
   trajectory. 

PID is used with analogue inputs.

## Key aspects:
- Control two motors with adjustable speeds.
- Set motor operation modes through digital control signals.
- Configure PWM signal frequency to suit motor specifications.
- Easy integration with various microcontroller units (MCUs) supporting the mbed framework.




