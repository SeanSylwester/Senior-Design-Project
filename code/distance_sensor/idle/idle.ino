#include "Adafruit_VL53L0X.h"

#define NORMAL 0
#define HIGH_RANGE 1
#define HIGH_ACCURACY 2
#define HIGH_SPEED 3

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_Dev_t *pMyDevice = lox.getDevicePointer();
VL53L0X_Error Status = VL53L0X_ERROR_NONE;
int address = 0x29;

void setup() {
  Serial.begin(115200);
  while (! Serial) {delay(1);}
  
  if (!lox.begin(0x29, true)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  while(1); //spin forever before taking a measurement
  
  Status = set_mode(NORMAL);
  //Status = set_mode(HIGH_RANGE);
  //Status = set_mode(HIGH_SPEED);
  //Status = set_mode(HIGH_ACCURACY);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  int i;
    
  Serial.print("Reading a measurement at address "); Serial.print(address, HEX); Serial.print(": ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4){ // && measure.RangeMilliMeter > 100 && measure.RangeMilliMeter < 900) {  // phase failures have incorrect data
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.print("Failed: ");
    Serial.println(measure.RangeStatus);
  }

/*
  //address switching
  delay(1000);
  if(address == 0x29) address = 0x26;
  else address = 0x29;
  lox.setAddress(address);
  delay(50);

8/}

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

