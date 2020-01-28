#include <Wire.h>

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a
#define DEFAULT_ADDR                                0x29

#define NUMBER_OF_SENSORS   30
#define DELAY_MS            30
#define XSHUT_PIN           10

byte gbuf[16];
uint8_t target_addr;
uint16_t distances[NUMBER_OF_SENSORS];
long timer;

void setup() {
  Serial.begin(115200);
  while (! Serial) {delay(1);}
  pinMode(XSHUT_PIN, OUTPUT);

  //reprogram all the addresses as the XSHUT signal propagates
  digitalWrite(XSHUT_PIN, HIGH);
  for(int i=0; i<NUMBER_OF_SENSORS; i++)
  {
    set_address(0x29 + i);
    delay(DELAY_MS);
  }
}

void loop() {
  timer = millis();
  trigger_all();
  Serial.print("Trigger Time: ");
  Serial.print(millis() - timer);
  Serial.println("ms");
  
  delay(100);
  
  timer = millis();
  read_all();
  Serial.print("Read Time: ");
  Serial.print(millis() - timer);
  Serial.println("ms");

  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    Serial.print(i); Serial.print(": "); Serial.println(distances[i]); 
    Serial.println("\n---\n");
  }
}


void set_address(uint8_t addr){
  write_byte_data_at(VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, addr*2);
}


void trigger_all(){
  target_addr = DEFAULT_ADDR;
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);
    target_addr++;
  }
}


void read_all(){
  target_addr = DEFAULT_ADDR;
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    read_block_data_at(VL53L0X_REG_RESULT_RANGE_STATUS, 12);
    distances[i] = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
    byte DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);
    target_addr++;
  }
}


// ************************************************************* //
// write_byte_data_at
// writes the data byte to the given target_addr
void write_byte_data_at(byte reg, byte data) {
  // write data word at target_addr and register
  Wire.beginTransmission(target_addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}
// ************************************************************* //


// ************************************************************* //
// read_block_data_at
// reads "sz" bytes from the given register
void read_block_data_at(byte reg, int sz) {
  Wire.beginTransmission(target_addr);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(target_addr, sz);
  for (int i=0; i<sz; i++) {
    while (Wire.available() < 1) delay(1);
    gbuf[i] = Wire.read();
  }
}
// ************************************************************* //

