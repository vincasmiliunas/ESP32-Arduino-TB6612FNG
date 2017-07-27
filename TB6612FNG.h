#pragma once

#include <Arduino.h>

/* ************************************************************************** */

class Tb6612fngLedc {
public:
  Tb6612fngLedc(int pin, int chan, int freq, int range);
  void begin();
  void write(float value);

private:
  int pin, chan, freq, range, rangeMax;
};

/* ************************************************************************** */

class Tb6612fngMotor {
public:
  Tb6612fngMotor(int in1, int in2, int pwm);
  Tb6612fngMotor(int in1, int in2, Tb6612fngLedc ledc);
  void begin();

  // Velocity: -1.0..+1.0. Motor duty cycle with positive/negative direction.
  void drive(float velocity);

  // Braking enables low resistance circuit across the motor coil (short).
  void brake();

  // Coasting enables high resistance circuit across the motor coil (standby).
  void coast();

protected:
  void direct(bool in1, bool in2);

private:
  int in1, in2;
  Tb6612fngLedc ledc;
};

/* ************************************************************************** */

class Tb6612fng {
public:
  Tb6612fng(int standby, int in1A, int in2A, int pwmA);
  Tb6612fng(int standby, int in1A, int in2A, Tb6612fngLedc pwmA);
  Tb6612fng(int standby, Tb6612fngMotor *motorA);
  Tb6612fng(int standby, int in1A, int in2A, int pwmA, int in1B, int in2B, int pwmB);
  Tb6612fng(int standby, int in1A, int in2A, Tb6612fngLedc pwmA, int in1B, int in2B, Tb6612fngLedc pwmB);
  Tb6612fng(int standby, Tb6612fngMotor *motorA, Tb6612fngMotor *motorB);
  ~Tb6612fng();
  void begin();

  Tb6612fngMotor &getA() { return *motorA; }
  Tb6612fngMotor &getB() { return *motorB; }
  void enable(bool value);

  // Velocity: -1.0..+1.0. Motor duty cycle with positive/negative direction.
  // Duration: a delay of ms after setting motor speed.
  // Stop: Braking after a delay.
  void drive(float velocity, int duration = 0, bool stop = true) { drive(velocity, velocity, duration, stop); }
  void drive(float velocityA, float velocityB, int duration = 0, bool stop = true);

  // Braking enables low resistance circuit across the motor coil (short).
  void brake();

  // Coasting enables high resistance circuit across the motor coil (standby).
  void coast();

private:
  int standby;
  Tb6612fngMotor *motorA, *motorB;
};
