#include "TB6612FNG.h"
#include <driver/ledc.h>
#include <cmath>
#include <algorithm>

// TB6612FNG has maximum PWM switching frequency of 100kHz.
#define DEFAULT_LEDC_FREQ 20000
#define DEFAULT_LEDC_RANGE LEDC_TIMER_10_BIT

/* ************************************************************************** */

Tb6612fngLedc::Tb6612fngLedc(int pin, int chan, int freq, int range)
    : pin(pin), chan(chan), freq(freq), range(range), rangeMax((1 << range) - 1) {}

void Tb6612fngLedc::begin() {
  ledcSetup(chan, freq, range);
  ledcAttachPin(pin, chan);
}

void Tb6612fngLedc::write(float value) {
  const auto v = std::max(0.0f, std::min(1.0f, value));
  ledcWrite(chan, v * rangeMax);
}

/* ************************************************************************** */

Tb6612fngMotor::Tb6612fngMotor(int in1, int in2, int pwm)
    : Tb6612fngMotor(in1, in2, Tb6612fngLedc(pwm, LEDC_CHANNEL_0, DEFAULT_LEDC_FREQ, DEFAULT_LEDC_RANGE)) {}

Tb6612fngMotor::Tb6612fngMotor(int in1, int in2, Tb6612fngLedc ledc) : in1(in1), in2(in2), ledc(ledc) {}

void Tb6612fngMotor::begin() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  ledc.begin();
}

void Tb6612fngMotor::drive(float velocity) {
  direct(velocity >= 0, velocity <= 0);
  ledc.write(std::abs(velocity));
}

void Tb6612fngMotor::brake() { direct(true, true); }

void Tb6612fngMotor::coast() { direct(false, false); }

void Tb6612fngMotor::direct(bool in1, bool in2) {
  digitalWrite(this->in1, in1 ? HIGH : LOW);
  digitalWrite(this->in2, in2 ? HIGH : LOW);
}

/* ************************************************************************** */

Tb6612fng::Tb6612fng(int standby, int in1A, int in2A, int pwmA)
    : Tb6612fng(standby, in1A, in2A, Tb6612fngLedc(pwmA, LEDC_CHANNEL_0, DEFAULT_LEDC_FREQ, DEFAULT_LEDC_RANGE)) {}

Tb6612fng::Tb6612fng(int standby, int in1A, int in2A, Tb6612fngLedc pwmA)
    : Tb6612fng(standby, new Tb6612fngMotor(in1A, in2A, pwmA)) {}

Tb6612fng::Tb6612fng(int standby, Tb6612fngMotor *motorA) : Tb6612fng(standby, motorA, nullptr) {}

Tb6612fng::Tb6612fng(int standby, int in1A, int in2A, int pwmA, int in1B, int in2B, int pwmB)
    : Tb6612fng(standby, in1A, in2A, Tb6612fngLedc(pwmA, LEDC_CHANNEL_0, DEFAULT_LEDC_FREQ, DEFAULT_LEDC_RANGE), in1B,
                in2B, Tb6612fngLedc(pwmB, LEDC_CHANNEL_1, DEFAULT_LEDC_FREQ, DEFAULT_LEDC_RANGE)) {}

Tb6612fng::Tb6612fng(int standby, int in1A, int in2A, Tb6612fngLedc pwmA, int in1B, int in2B, Tb6612fngLedc pwmB)
    : Tb6612fng(standby, new Tb6612fngMotor(in1A, in2A, pwmA), new Tb6612fngMotor(in1B, in2B, pwmB)) {}

Tb6612fng::Tb6612fng(int standby, Tb6612fngMotor *motorA, Tb6612fngMotor *motorB)
    : standby(standby), motorA(motorA), motorB(motorB) {}

Tb6612fng::~Tb6612fng() {
  if (motorA)
    delete motorA;
  if (motorB)
    delete motorB;
}

void Tb6612fng::begin() {
  pinMode(standby, OUTPUT);
  if (motorA)
    motorA->begin();
  if (motorB)
    motorB->begin();
  enable(true);
}

void Tb6612fng::enable(bool value) { digitalWrite(standby, value ? HIGH : LOW); }

void Tb6612fng::drive(float velocityA, float velocityB, int duration, bool stop) {
  if (motorA)
    motorA->drive(velocityA);
  if (motorB)
    motorB->drive(velocityB);
  if (duration) {
    delay(duration);
    if (stop)
      brake();
  }
}

void Tb6612fng::brake() {
  if (motorA)
    motorA->brake();
  if (motorB)
    motorB->brake();
}

void Tb6612fng::coast() {
  if (motorA)
    motorA->coast();
  if (motorB)
    motorB->coast();
}

/* ************************************************************************** */
