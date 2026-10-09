// Joint-angle PID scaffold: hardware adapter functions below are unfinished.
//Parameters (change based on MATLAB later)
float Kp = 0.0;
float Ki = 0.0;
float Kd = 0.0;

//Sampling
const float Ts = 0.001f; // 1 ms
const unsigned long Ts_us = (unsigned long)(Ts * 1e6f);


float targetDeg = 0.0f;
float integral  = 0.0f;
float prevErr   = 0.0f;
unsigned long lastTime = 0;

//Limit
const float Imax = 100.0f;   // Clamp accumulated error (not saturation-aware anti-windup)
const float Umax = 255.0f;   //PWM magnitude limit

void setup() 
{
  Serial.begin(115200);// can be changed
  setupSensor();
  setupMotor();

  targetDeg = readJointAngleDeg();
  lastTime = micros();
}

void loop() 
{
  //Fixed-rate
  unsigned long now = micros();
  if (now - lastTime >= Ts_us) 
  {
    lastTime += Ts_us;

    float currentDeg = readJointAngleDeg();
    float error = targetDeg - currentDeg;

    //PID
    integral = integral + error * Ts;
    if (integral >  Imax) integral =  Imax;
    if (integral < -Imax) integral = -Imax;

    float derivative = (error - prevErr) / Ts;
    float u = Kp * error + Ki * integral + Kd * derivative;

    //Actuate
    if (u >  Umax) u =  Umax;
    if (u < -Umax) u = -Umax;
    writeMotorPWM(u);

    prevErr = error;
  }

  // Debug
  // static unsigned long lastPrint = 0;
  // if (millis() - lastPrint >= 100) 
  // {
  //   lastPrint = millis();
  //   Serial.println();
  // }
}


void setupSensor() 
{
  // TODO: initialise the selected sensor. 

}
void setupMotor()  
{
  // TODO: initialise the confirmed motor driver at zero output. 

}

float readJointAngleDeg() 
{
  // TODO: return calibrated joint-angle feedback.
  return 0.0f;
}

void writeMotorPWM(float u) 
{
  // TODO: map signed PWM to the motor driver.

}
