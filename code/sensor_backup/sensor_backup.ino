#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a
#define DEFAULT_ADDR                                0x29

#define NUMBER_OF_SENSORS   32
#define NUMBER_OF_MOTORS    8
#define CHUNKS              1

#define PROGRAM_DELAY       30431
#define MEASURE_DELAY       30

#define XSHUT_PIN           9
#define trigPin             11
#define echoPin             10  

byte gbuf[16];
uint8_t target_addr = DEFAULT_ADDR;
uint16_t distances[NUMBER_OF_SENSORS];
byte statuses[NUMBER_OF_SENSORS];
byte output[4];
uint8_t offset=0, part=0;
long timer, elapsed, ultra_distance=9999;

void setup() {
  Serial.begin(57600);
  while (! Serial) {delay(1);}
  
  pinMode(XSHUT_PIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  digitalWrite(XSHUT_PIN, LOW);
  digitalWrite(trigPin, LOW); 
  
  //DDRB |= (1 << XSHUT_PIN) | (1 << trigPin) | (0 << echoPin);
  //PORTB |= (0 << XSHUT_PIN) | (0 << trigPin);
  Wire.begin();
  delay(500); //wait for sensors to boot
  
  //reprogram all the addresses as the XSHUT signal propagates
  digitalWrite(XSHUT_PIN, HIGH); delay(15);
  //PORTB |= (1 << XSHUT_PIN); delay(15);
  for(int i=1; i<=NUMBER_OF_SENSORS; i++)
  {
    distances[i-1] = 9999;
    target_addr = DEFAULT_ADDR;
    set_address(DEFAULT_ADDR + i);
    //fast_mode(DEFAULT_ADDR + i);
    delayMicroseconds(PROGRAM_DELAY);
  }
}

void loop() {
  //read first group
  trigger_part(0, 14, 1);
  //trigger_15();
  
  //read the ultrasonic measurement and determine measurement time
  timer = millis();
  //read_ultrasonic();
  elapsed = millis() - timer;

  //wait for the rest of the measure delay, if needed
  if(elapsed < MEASURE_DELAY)
    delay(MEASURE_DELAY - (millis() - timer));
  read_part(0, 14, 1);
  //read_15();

/*
  //read second group
  trigger_part(15, 29, 1);
  delay(MEASURE_DELAY);
  read_part(15, 29, 1);
*/

  /*
  for(int i=0; i<NUMBER_OF_SENSORS; i++)
  {
    Serial.write(uint8_t((distances[i] >> 8)));
    Serial.write(uint8_t(distances[i]));
  }
  */
  

  
  //fill in and send the output array
  calculate_output2();
  Serial.write(output[0]); 
  Serial.write(output[1]); 
  Serial.write(output[2]); 
  Serial.write(output[3]);
  
  



  /*
  //for testing two sensors with bluetooth
  output[0] = 0; output[1] = 0; output[2] = 0; output[3] = 0;
  output[0] |= (0x0F & distances[0]); output[1] |= (0xF0 & (distances[1] << 4));
  */



  /*
  //for one sensor and the ultrasonic
  //if(distances[0] < ultra_distance)
  
  //for two sensors and the ultrasonic
  if(distances[0] < distances[1])

  //for one sensor
  //if(distances[0] > 500)
  
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
  */
}

//*********************************************************//
// updates the address of the sensor on the DEFAULT_ADDR
//  to the input "addr"
//*********************************************************//
void set_address(uint8_t addr){
  write_byte_data_at(VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, addr);
}

//*********************************************************//
// triggers NUMBER_OF_SENSORS sensors to start measuring
// starts at DEFAULT_ADDR, then increments address from there
//*********************************************************//
void trigger_all(){
  target_addr = DEFAULT_ADDR + 1;
  
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);
    
    target_addr++;
    delayMicroseconds(10);
  }
}
void trigger_part(uint8_t first, uint8_t last, uint8_t step_size){
  target_addr = DEFAULT_ADDR + 1 + first;
  
  for(int i=first; i<=last; i+=step_size){
    write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);
    
    target_addr += step_size;
    delayMicroseconds(10);
  }
}
void trigger_15()
{
  uint8_t addrs[] = {2, 5, 7, 9, 11, 13, 16, 17, 18, 19, 20, 22, 24, 26, 29};
  for(uint8_t i=0; i < 15; i++)
  {
    target_addr = DEFAULT_ADDR + 1 + addrs[i];
    write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);
  }
}


//*********************************************************//
// asks NUMBER_OF_SENSORS sensors to return their measurements
// starts at DEFAULT_ADDR, then increments address from there
// adds distance data to the "distances" array
// adds status data to the "statuses" array
//*********************************************************//
void read_all(){
  uint16_t meas;
  target_addr = DEFAULT_ADDR + 1;
  
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    read_block_data_at(VL53L0X_REG_RESULT_RANGE_STATUS, 12);
    
    meas = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
    if(meas > 20 && meas < 1200) distances[i] = meas;
    else distances[i] = 9999;
    
    statuses[i] = ((gbuf[0] & 0x78) >> 3);
    
    target_addr++;
    delayMicroseconds(10);
  }
}
void read_part(uint8_t first, uint8_t last, uint8_t step_size){
  uint16_t meas;
  target_addr = DEFAULT_ADDR + 1 + first;
  
  for(int i=first; i<=last; i+=step_size){
    read_block_data_at(VL53L0X_REG_RESULT_RANGE_STATUS, 12);
    
    meas = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
    if(meas > 20 && meas < 1200) distances[i] = meas;
    else distances[i] = 9999;
    
    statuses[i] = ((gbuf[0] & 0x78) >> 3);
    
    target_addr += step_size;
    delayMicroseconds(10);
  }
}
void read_15()
{
  uint16_t meas;
  uint8_t addrs[] = {2, 5, 7, 9, 11, 13, 16, 17, 18, 19, 20, 22, 24, 26, 29};
  
  for(uint8_t i=0; i < 15; i++)
  {
    target_addr = DEFAULT_ADDR + 1 + addrs[i];
    read_block_data_at(VL53L0X_REG_RESULT_RANGE_STATUS, 12);
    
    meas = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
    if(meas > 20 && meas < 1200) distances[addrs[i]] = meas;
    else distances[i] = 9999;
    
    statuses[i] = ((gbuf[0] & 0x78) >> 3);
  }
}

//*********************************************************//
// talks to the I2C device with the "target_addr" address
// writes "data" to the "reg" register
//*********************************************************//
void write_byte_data_at(byte reg, byte data) {
  Wire.beginTransmission(target_addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

//*********************************************************//
// talks to the I2C device with the "target_addr" address
// reads "sz" bytes from the "reg" register
// loads data into the global "gbuf" buffer
//*********************************************************//
void read_block_data_at(byte reg, uint8_t sz) {
  Wire.beginTransmission(target_addr);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(target_addr, sz);
  for (int i=0; i<sz; i++) {
    while (Wire.available() < 1) delay(1);
    gbuf[i] = Wire.read();
  }
}

//*********************************************************//
// calculates the output nibble for each motor
// writes to the global "output" array
// reads in the global constants and the "distances" array
//*********************************************************//
void calculate_output()
{
  uint16_t _min;
  uint8_t index = offset;
  uint8_t out;
  
  //initially clear the output bytes
  output[0] = 0;
  output[1] = 0;
  output[2] = 0;
  output[3] = 0;

  //determine closest object distance for each motor's group
  for(int i=0; i<NUMBER_OF_MOTORS; i++)
  {
    _min = 65535;
    //find the minimum distance for all the sensors in this motor's group
    //for(int j=0; j<(NUMBER_OF_SENSORS / NUMBER_OF_MOTORS); j++)
    for(int j=0; j<(NUMBER_OF_SENSORS / NUMBER_OF_MOTORS); j++)
    {
      //check ultrasonic sensor for index 0
      if(index % NUMBER_OF_SENSORS == 0)
        _min = min(_min, ultra_distance);

      //minimum laser sensor distance in this group
      _min = min(_min, distances[index % NUMBER_OF_SENSORS]);
      index++;
    }

    //convert to range intervals
    if(_min <= 200) out = 5;
    else if(_min <= 600) out = 4;
    else if(_min <= 1000) out = 3;
    else if(_min <= 3000) out = 2;
    else if(_min <= 5000) out = 1;
    else out = 0;

    //setup the output bytes
    //goes 0 0 1 1 2 2 3 3
    if(i%2 == 0)
      output[i/2] |= (0x0F & out);
    else
      output[i/2] |= (0xF0 & (out << 4));
  }
}

//*********************************************************//
// calculates the output nibble for each motor
// writes to the global "output" array
// reads in the global constants and the "distances" array
//*********************************************************//
void read_ultrasonic()
{
  long duration;
  digitalWrite(trigPin, HIGH);
  //PORTB |= (1 << trigPin);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  //PORTB |= (0 << trigPin);

  duration = pulseIn(echoPin, HIGH);
  ultra_distance = (duration/2) / 29.1 * 10;      // distance calculation in mm
}



//*********************************************************//
// sets the sensor at addres "addr" to the "fast" mode
//*********************************************************//
/*
void fast_mode(uint8_t addr)
{
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  VL53L0X_Dev_t *pMyDevice = lox.getDevicePointer();
  VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 32*65536);
  VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 20000); //VL53L0X_Error VL53L0X_set_measurement_timing_budget_micro_seconds()
}
*/



//*********************************************************//
// the three functions below fill in the output array
//*********************************************************//
void calculate_output2()
{
  uint16_t _min;
  uint8_t out;
  
  //initially clear the output bytes
  output[0] = 0;
  output[1] = 0;
  output[2] = 0;
  output[3] = 0;
  
  output[3] |= (0x0F & calc_range(min_dists(1,  3))); //back
  output[3] |= (0xF0 & (calc_range(min_dists(4,  5)) << 4)); //back-left
  output[0] |= (0x0F & calc_range(min_dists(6,  10))); //left
  output[0] |= (0xF0 & (calc_range(min_dists(11, 14)) << 4)); //front-left
  output[1] |= (0x0F & calc_range(min_dists(15, 18))); //front
  output[1] |= (0xF0 & (calc_range(min_dists(19, 21)) << 4)); //front-right
  output[2] |= (0x0F & calc_range(min_dists(22, 27))); //right
  output[2] |= (0xF0 & (calc_range(min_dists(28, 29)) << 4)); //back-right
}

//*********************************************************//
// finds the min in the given range in the distances array
//*********************************************************//
uint16_t min_dists(uint8_t first, uint8_t last)
{
  uint16_t _min = 65536;
  for(uint8_t i=first; i<last; i++)
  {
    _min = min(distances[i], distances[i+1]);
  }

  return _min;
}

//*********************************************************//
// converts a raw distance value to a distance range
//*********************************************************//
uint8_t calc_range(uint16_t dist)
{
  uint8_t out;
  
  if(dist <= 200) out = 5;
  else if(dist <= 600) out = 4;
  else if(dist <= 1000) out = 3;
  else if(dist <= 3000) out = 2;
  else if(dist <= 5000) out = 1;
  else out = 0;

  return out;
}


