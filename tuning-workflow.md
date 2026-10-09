# Step-test and PID tuning workflow

Adapted from the project's original Step Test notes.

1. Confirm the sensor, motor driver, supply, mechanical limits and board.
2. Connect and check the sensor and driver on a secured bench setup.
3. Apply a known PWM step with the Arduino step-test sketch.
4. Record commanded PWM and accelerometer tilt with MATLAB.
5. Inspect the response and fit a first- or second-order model where appropriate.
6. Derive discrete PID gains for the chosen sample period.
7. Implement calibrated feedback and motor-driver adapters in the PID sketch.
8. Validate direction, timing, limits and response before extending the controller.

Model fitting and gain calculation are planned steps; no implementation, fitted model or tuned gains are included here. PWM is the input command, not measured torque. Accelerometer tilt requires calibration and is affected by motion; it should not automatically be treated as joint angle.
