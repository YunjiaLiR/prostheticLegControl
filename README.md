# Prosthetic Leg Control

Arduino and MATLAB prototype code developed for a Prosthetics Society leg-control project. The intended application is prosthetic leg movement during stair ascent and descent. This repository contains the initial joint-angle PID scaffold and an open-loop motor step-test workflow for collecting data before controller tuning.

## Current status

**Development prototype.** The uploaded code does not yet implement stair trajectories, gait-phase detection or a complete hardware-integrated controller. No recorded datasets or measured performance results are included.

| Component | Implemented | Still needed |
| --- | --- | --- |
| PID scaffold | Nominal 1 ms update interval, PID calculation, integral clamp, signed PWM clamp | Sensor and motor adapters, tuned gains, target trajectory |
| Step test | MPU9250 setup, accelerometer tilt, PWM step, serial logging, timed stop | Confirmed board/driver wiring and bench validation |
| MATLAB capture | Serial parsing, data saving and response plots | Recorded responses, model fitting and gain selection |

## Repository contents

- [PID sketch](firmware/PID/PID.ino): joint-angle control scaffold; gains are zero and hardware functions are placeholders.
- [Step-test sketch](firmware/stepTest/stepTest.ino): one-shot open-loop motor test with MPU9250 accelerometer feedback.
- [MATLAB capture](matlab/acquireStepData.m): captures and plots time, tilt and commanded PWM.
- [Tuning workflow](docs/tuning-workflow.md): the original test plan converted into readable project notes.

## Hardware and software

The step-test sketch assumes an MPU9250 at I2C address `0x68` and two motor-driver inputs on PWM pins 9 and 10. The Arduino board and motor-driver model are not specified in the supplied files: confirm both before using these pin assignments.

Use the Arduino IDE with `Wire` and `MPU9250_WE`, and MATLAB with the `serialport` API. Board, library and MATLAB versions have not yet been recorded.

## Run the step-test workflow

1. Confirm wiring, motor-driver input semantics and mechanical travel limits. Secure the mechanism for a bench test. This prototype is not validated for use on a person.
2. Open `firmware/stepTest/stepTest.ino` in the Arduino IDE, select the actual board and port, and install the required library.
3. Check the test settings: PWM magnitude 120, step onset 500 ms and stop time 3000 ms after test timing starts. The sketch automatically starts after setup and runs once per reset.
4. Upload the sketch. Close the Arduino Serial Monitor before opening MATLAB.
5. Change MATLAB's working folder to `matlab`, find your port and start capture:

```matlab
serialportlist("available")
[t, y, u] = acquireStepData("COM3", 10); % Replace COM3 with your Arduino port.
```

6. Reset the Arduino while capture is running to record the complete response. Startup messages are ignored. Results are saved as `step_data.mat` in MATLAB's current folder; this file is overwritten on each successful capture.

Serial data format is `time_s,tilt_deg,commanded_pwm` at 115200 baud. The test has no explicit fixed logging interval; use the transmitted timestamps.

## PID scaffold

The controller computes:

```text
error = target angle - measured angle
output = Kp * error + Ki * integral(error) + Kd * derivative(error)
```

The accumulated error is clamped to ±100 and the output to ±255. The target initially holds the measured startup angle. The sensor currently returns zero and the motor output function is empty, so this sketch does not control hardware as supplied.

The 1 ms interval is a nominal software schedule, not a verified hardware rate. Integral clamping is not saturation-aware anti-windup, and the derivative has no filtering. Hardware timing, measurement calibration and tuning remain to be validated.

## Project scope

These files document the control-code portion of the society project. The repository does not currently include circuit schematics, mechanical designs, stair-control logic or hardware-test results. Those can be added when available.

## Validation

The upload includes a MATLAB syntax correction, configurable capture port, numeric-record filtering and serial cleanup. The step-test stop condition is checked before actuation/logging on each iteration. Runtime execution, board compilation and physical tests have not been performed for this repository update.
