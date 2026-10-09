# Prosthetic Leg Control

Arduino and MATLAB code for a Prosthetics Society project exploring prosthetic leg control for stair ascent and descent.

This repository contains a joint-angle PID controller scaffold and a motor step-test workflow for collecting response data before tuning.

## Files

| File | Purpose |
| --- | --- |
| [PID.ino](PID.ino) | PID calculation with integral and PWM limits |
| [stepTest.ino](stepTest.ino) | Open-loop motor step test with MPU9250 tilt measurements |
| [acquireStepData.m](acquireStepData.m) | Record serial data in MATLAB and plot the response |

## Setup and use

The step-test sketch uses an MPU9250 at I2C address `0x68`, the `MPU9250_WE` library, and motor-driver inputs on pins 9 and 10. Confirm the actual board, driver wiring and mechanical limits before a secured bench test.

1. Download the code. Place each Arduino sketch in its own matching folder: `PID/PID.ino` and `stepTest/stepTest.ino`. Open and upload only the sketch you intend to use.
2. Install `MPU9250_WE` in the Arduino IDE and select your board and port.
3. Upload `stepTest.ino`. It applies PWM 120 after 500 ms and stops at 3000 ms after test timing starts. It runs once per reset.
4. Close the Serial Monitor. In MATLAB, change to the folder containing `acquireStepData.m` and run:

```matlab
serialportlist("available")
[t, y, u] = acquireStepData("COM3", 10); % Replace COM3 with your Arduino port.
```

5. Reset the Arduino during capture to record the complete test. MATLAB saves `t` (seconds), `y` (accelerometer tilt in degrees) and `u` (commanded PWM) to `step_data.mat`, overwriting any previous capture.

## Tuning workflow

Collect a step response → fit an appropriate motor model → select discrete PID gains → implement calibrated angle feedback and motor-driver functions → validate the controller on the bench.

## Current status

The PID sketch has a nominal 1 ms update interval, integral clamping and output limiting. Gains are currently zero, and the sensor and motor functions are placeholders. The step-test measurement is accelerometer tilt, which requires calibration before use as joint-angle feedback.

Model fitting, tuned gains, stair trajectories and measured performance results are not included. The code has not been compiled or physically tested as part of this repository refinement and is not validated for use on a person.
