// Open-loop bench test. Confirm driver wiring before uploading.
// Accelerometer tilt is not a calibrated joint-angle measurement.
#include <Wire.h>
#include <MPU9250_WE.h>

#define MPU9250_ADDR 0x68
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

//might change depending of driver
const int pwmPin1 = 9;
const int pwmPin2 = 10;
const int STEP_PWM = 120; //Adjustable
const unsigned long totalT = 3000;
const unsigned long delayT = 500;

unsigned long t0;
bool stepON = false;

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  if (!myMPU9250.init())
  {
    Serial.println("MPU9250 does not respond");
    while(1);
  }
  else
  {
    Serial.println("MPU9250 is connected");
  }

  delay(1000);
  myMPU9250.autoOffsets();
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);
  myMPU9250.setSampleRateDivider(5);


  pinMode(pwmPin1, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  analogWrite(pwmPin1, 0);
  analogWrite(pwmPin2, 0);

  delay(200);
  t0 = millis();


}

void loop() 
{
  unsigned long t = millis()-t0;
  if (t >= totalT)
  {
    analogWrite(pwmPin1, 0);
    analogWrite(pwmPin2, 0);
    while (true) {} // One-shot test: reset to repeat.
  }

  if (t >= delayT && !stepON)
  {
    analogWrite(pwmPin1, STEP_PWM);
    analogWrite(pwmPin2, 0);
    stepON = true;
  }

  //IMU
  xyzFloat gValue = myMPU9250.getGValues();
  float angleDeg = atan2(gValue.y, gValue.z) * 180.0/PI;

  //send data to MATLAB
  Serial.print(t/1000.0, 3); Serial.print(",");
  Serial.print(angleDeg, 3); Serial.print(",");
  Serial.println(stepON ? STEP_PWM :0);


}
