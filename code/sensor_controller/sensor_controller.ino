#include <Wire.h>

#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a
#define DEFAULT_ADDR                                0x29

#define NUMBER_OF_SENSORS   30

#define PROGRAM_DELAY       30431
#define MEASURE_DELAY       35

#define XSHUT_PIN           PB1 //9
#define LPPin               PD2 //2
#define trigPin             PD4 //4
#define echoPin             PD5 //5
#define debugPin1           PB2 //10
#define debugPin2           PB3 //11
#define debugPin3           PB4 //12

byte gbuf[16];
uint8_t target_addr = DEFAULT_ADDR;
uint16_t distances[NUMBER_OF_SENSORS];
uint16_t prev[NUMBER_OF_SENSORS];
byte statuses[NUMBER_OF_SENSORS];
byte output[4];
long ultra_distance=9999;
uint8_t LP = 1;

void setup() {
  Serial.begin(57600);
  while (! Serial) {delay(1);}
  DDRB |= (1 << XSHUT_PIN) | (1 << debugPin1) | (1 << debugPin2) | (1 << debugPin3);
  DDRD |= (1 << trigPin);
  PORTB = 0;
  Wire.begin();
  //Wire.setClock(200000L);
  delay(500); //wait for sensors to boot
  
  //reprogram all the addresses as the XSHUT signal propagates
  PORTB |= (1 << XSHUT_PIN); delay(15);
  for(int i=1; i<=NUMBER_OF_SENSORS; i++)
  {
    distances[i-1] = 9999;
    prev[i-1] = 9999;
    target_addr = DEFAULT_ADDR;
    set_address(DEFAULT_ADDR + i);
    delayMicroseconds(PROGRAM_DELAY);
  }
}

void loop() {
  //LP off, pin high, enable LP
  if(LP == 1 && (PIND & 1<<LPPin)) LP = 2;
  //LP on, pin low, disable LP
  else if(LP == 2 && !(PIND & 1<<LPPin)) LP = 1;

  //trigger sensors
  PORTB |= (1 << debugPin1);
  trigger_part(0, NUMBER_OF_SENSORS-3, 1);
  
  //read the ultrasonic measurement and determine measurement time
  long timer = millis();
  read_ultrasonic();
  long elapsed = millis() - timer;

  //wait for the rest of the measure delay, if needed
  if(elapsed < LP*MEASURE_DELAY)
    delay(LP*MEASURE_DELAY - elapsed);
 
  PORTB |= (1 << debugPin2);
  read_part(0, NUMBER_OF_SENSORS-3, 1);
  PORTB |= (1 << debugPin3);
  
  //fill in and send the output array
  calculate_output2();
  distances[NUMBER_OF_SENSORS-1] = ultra_distance;
  
  //send output data
  send_output();
  //send_data();
  //send_status();

  PORTB &= ~((1 << debugPin1) | (1 << debugPin2) | (1 << debugPin3));
  for(uint8_t i=0; i<NUMBER_OF_SENSORS; i++)
    prev[i] = distances[i];
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
void trigger_part(uint8_t first, uint8_t last, uint8_t step_size){
  target_addr = DEFAULT_ADDR + 1 + first;
  
  for(int i=first; i<=last; i+=step_size){
    write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);
    target_addr += step_size;
  }
}


//*********************************************************//
// asks NUMBER_OF_SENSORS sensors to return their measurements
// starts at DEFAULT_ADDR, then increments address from there
// adds distance data to the "distances" array
// adds status data to the "statuses" array
//*********************************************************//
void read_part(uint8_t first, uint8_t last, uint8_t step_size){
  uint16_t meas;
  target_addr = DEFAULT_ADDR + 1 + first;
  
  for(int i=first; i<=last; i+=step_size){
    read_block_data_at(VL53L0X_REG_RESULT_RANGE_STATUS, 12);
    statuses[i] = ((gbuf[0] & 0x78) >> 3);
    
    meas = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
    if(meas > 20 && meas < 1500)
//      distances[i] = meas;
      if(prev[i] != 9999) distances[i] = meas;
      else distances[i] = 9998;
    else distances[i] = 9999;
    
    target_addr += step_size;
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
// triggers and reads the ultrasonic sensor measurement
// and stores it into the ultra_distance global
//*********************************************************//
void read_ultrasonic()
{
  PORTD |= (1 << trigPin);
  delayMicroseconds(10); 
  PORTD &= ~(1 << trigPin);
  
  //long duration = pulse_in(echoPin, 1, MEASURE_DELAY*1000);
  long duration = pulseIn(echoPin, 1);
  ultra_distance = (duration/2) / 29.1 * 10;      // distance calculation in mm
}



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
  
  output[0] |= (0x0F & calc_range(min_dists(0, 2))); //back
  output[0] |= (0xF0 & (calc_range(min_dists(3,  5)) << 4)); //back-left
  output[1] |= (0x0F & calc_range(min_dists(6,  9))); //left
  output[1] |= (0xF0 & (calc_range(min_dists(11, 13)) << 4)); //front-left
  output[2] |= (0x0F & calc_range(min(min_dists(14, 16), ultra_distance))); //front
  //output[2] |= (0x0F & calc_range(min_dists(14,  16))); //front no ultrasonic
  output[2] |= (0xF0 & (calc_range(min_dists(17, 19)) << 4)); //front-right
  output[3] |= (0x0F & calc_range(min_dists(20, 24))); //right
  output[3] |= (0xF0 & (calc_range(min_dists(25, 27)) << 4)); //back-right
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
  
  if(dist <= 650) out = 4;
  else if(dist <= 1200) out = 2;
  else if(dist <= 2000) out = 1;
  //else if(dist <= 3000) out = 2;
  //else if(dist <= 5000) out = 1;
  else out = 0;

  return out;
}

//*********************************************************//
// serially sends all sensor data
//*********************************************************//
void send_data()
{
  for(uint8_t i=0; i<NUMBER_OF_SENSORS; i++)
  {
    Serial.write(0x00FF & distances[i]);
    Serial.write((0xFF00 & distances[i]) >> 8);
  }
}

//*********************************************************//
// serially sends all sensor data
//*********************************************************//
void send_status()
{
  for(uint8_t i=0; i<NUMBER_OF_SENSORS; i++)
  {
    Serial.write(0x00FF & statuses[i]);
    Serial.write((0xFF00 & statuses[i]) >> 8);
  }
}

//*********************************************************//
// serially sends output bytes
//*********************************************************//
void send_output()
{
  Serial.write(0xff);
  Serial.write(output[0]); 
  Serial.write(output[1]); 
  Serial.write(output[2]); 
  Serial.write(output[3]);
}


unsigned long pulse_in(uint8_t pin, uint8_t state, unsigned long timeout)
{
  // cache the port and bit of the pin in order to speed up the
  // pulse width measuring loop and achieve finer resolution.  calling
  // digitalRead() instead yields much coarser resolution.
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  uint8_t stateMask = (state ? bit : 0);
  unsigned long width = 0; // keep initialization out of time critical area
  
  // convert the timeout from microseconds to a number of times through
  // the initial loop; it takes 16 clock cycles per iteration.
  unsigned long numloops = 0;
  unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;
  
  // wait for any previous pulse to end
  while ((*portInputRegister(port) & bit) == stateMask)
    if (numloops++ == maxloops)
      return 0;
  
  // wait for the pulse to start
  while ((*portInputRegister(port) & bit) != stateMask)
    if (numloops++ == maxloops)
      return 0;
  
  // wait for the pulse to stop
  while ((*portInputRegister(port) & bit) == stateMask) {
    if (numloops++ == maxloops)
      return 0;
    width++;
  }

  // convert the reading to microseconds. The loop has been determined
  // to be 20 clock cycles long and have about 16 clocks between the edge
  // and the start of the loop. There will be some error introduced by
  // the interrupt handlers.
  return clockCyclesToMicroseconds(width * 21 + 16); 
}

