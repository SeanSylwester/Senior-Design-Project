#include "Adafruit_VL53L0X.h"

#define NORMAL 0
#define HIGH_RANGE 1
#define HIGH_ACCURACY 2
#define HIGH_SPEED 3

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_Dev_t *pMyDevice = lox.getDevicePointer();
VL53L0X_Error Status = VL53L0X_ERROR_NONE;
int address = 0x29;
long timer;

void setup() {
  Serial.begin(115200);
  while (! Serial) {delay(1);}
  
  if (!lox.begin(0x29, true)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  Status = set_mode(NORMAL);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  int i, mode;

  if (Serial.available() > 0) {
    mode = Serial.read();
    if(mode == 48)
      Status = set_mode(NORMAL);
    else if(mode == 49)
      Status = set_mode(HIGH_RANGE);
    else if(mode == 50)
      Status = set_mode(HIGH_SPEED);
    else if(mode == 51)
      Status = set_mode(HIGH_ACCURACY);
  }
  
  Serial.print(address, HEX); Serial.print(" - ");
  timer = millis();
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  Serial.print(millis() - timer); Serial.print("ms - ");

  if (measure.RangeStatus != 4){// phase failures have incorrect data
    Serial.print(measure.RangeMilliMeter); Serial.println("mm ");
  } else {
    Serial.print("Failed: ");
    Serial.println(measure.RangeStatus);
  }
}

VL53L0X_Error set_mode(int mode)
{
  FixPoint1616_t signal_rate;
  FixPoint1616_t sigma;
  long time_budget = 60000;
  int vcsel_pre = 18;
  int vcsel_final = 14;

  if(mode == NORMAL)
  {
    signal_rate = 0.25*65536;
    sigma = 18*65536;
    time_budget = 33000;
    vcsel_pre = 18;
    vcsel_final = 14;
  }
  else if(mode == HIGH_RANGE)
  {
    signal_rate = 0.1*65536;
    sigma = 60*65536;
    time_budget = 33000;
    vcsel_pre = 18;
    vcsel_final = 14;
  }
  else if(mode == HIGH_ACCURACY)
  {
    signal_rate = 0.25*65536;
    sigma = 18*65536;
    time_budget = 200000;
    vcsel_pre = 18;
    vcsel_final = 14;
  }
  else if(mode == HIGH_SPEED)
  {
    signal_rate = 0.25*65536;
    sigma = 32*65536;
    time_budget = 20000;
    vcsel_pre = 18;
    vcsel_final = 14;
  }
  
  Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, signal_rate);
  Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, sigma);
  Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, time_budget);
  Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, vcsel_pre);
  Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, vcsel_final);
}

