#include <Wire.h>

#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a
#define DEFAULT_ADDR                                0x29

#define NUMBER_OF_SENSORS   2
#define PROGRAM_DELAY       34
#define MEASURE_DELAY       60
#define XSHUT_PIN           2
#define LED_PIN             13

byte gbuf[16];
uint8_t target_addr = DEFAULT_ADDR;
uint16_t distances[NUMBER_OF_SENSORS];
byte statuses[NUMBER_OF_SENSORS];

void setup() {
  Serial.begin(38400);
  while (! Serial) {delay(1);}
  pinMode(XSHUT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(XSHUT_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
 
  Wire.begin();
  delay(500);
  
  //reprogram all the addresses as the XSHUT signal propagates
  digitalWrite(XSHUT_PIN, HIGH); delay(2);
  for(int i=1; i<=NUMBER_OF_SENSORS; i++)
  {
    target_addr = DEFAULT_ADDR;
    set_address(DEFAULT_ADDR + i);
    delay(PROGRAM_DELAY);
  }
}

void loop() {
  //trigger all the laser sensors
  trigger_all();
  delay(MEASURE_DELAY);
  read_all();  

  //turns on the LED if the first sensor's distance is larger than the second's
  if((NUMBER_OF_SENSORS == 2 && distances[0] > distances[1]))
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);    
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
  target_addr = DEFAULT_ADDR;
  
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    target_addr++;
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
  target_addr = DEFAULT_ADDR;
  
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    target_addr++;
    read_block_data_at(VL53L0X_REG_RESULT_RANGE_STATUS, 12);
    distances[i] = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
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
