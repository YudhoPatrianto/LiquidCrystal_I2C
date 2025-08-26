# LiquidCrystal_I2C_Revision

LiquidCrystal Arduino And ESP32 library for I2C LCD displays 

what's changed?:
  - `Add Support Multiple LCD (WARNING! Must Different I2C Addresses)`

**How To Use?**
```c++
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

// Create Twowire
TwoWire my_lcd1 = TwoWire(0);
TwoWire my_lcd2 = TwoWire(1);

// Properties LCD1
const int lcd1_sda = 27;
const int lcd1_scl = 26;
LiquidCrystal_I2C lcd1(0x26, 16, 2); 

// Properties LCD2
const int lcd2_sda = 25;
const int lcd2_scl = 33;
LiquidCrystal_I2C lcd2(0x27, 20, 4); 

void setup() {
  my_lcd1.begin(lcd1_sda, lcd1_scl);
  lcd1.begin(&my_lcd1);
  lcd1.backlight();
  lcd1.setCursor(0,0);
  lcd1.print("Hello LCD1");

  my_lcd2.begin(lcd2_sda, lcd2_scl);
  lcd2.begin(&my_lcd2);
  lcd2.backlight();
  lcd2.setCursor(0,0);
  lcd2.print("Hello LCD2");
}

void loop() {

}
```