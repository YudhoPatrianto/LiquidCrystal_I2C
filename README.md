# LiquidCrystal_I2C_Revision

LiquidCrystal Arduino And ESP32 library for I2C LCD displays 

what's changed?:
  - `Add Support Multiple LCD (WARNING! Must Different I2C Addresses)`

**How To Use?**
```c++
#include "LiquidCrystal_I2C.h"

// Properties LCD1
const int lcdA_sda = 27;
const int lcdA_scl = 26;
LiquidCrystal_I2C lcdA(0x27, 20, 4, lcdA_sda, lcdA_scl, 0); // (Address lcd, column, rows, sda, scl, number wire)
// Properties LCD2
const int lcdB_sda = 25;
const int lcdB_scl = 33;
LiquidCrystal_I2C lcdB(0x26, 16, 2, lcdB_sda, lcdB_scl, 1); // (Address lcd, column, rows, sda, scl, number wire)

void setup() {
  lcdA.begin();
  lcdA.backlight();
  lcdA.setCursor(0,0);
  lcdA.print("Hello LCD1");

  lcd2.begin();
  lcd2.backlight();
  lcd2.setCursor(0,0);
  lcd2.print("Hello LCD2");
}

void loop() {

}
```