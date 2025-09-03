#include "LiquidCrystal_I2C.h"
#include <Arduino.h>

/// @brief The constructor for the LiquidCrystal_I2C class.
/// @param lcd_addr The I2C address of the LCD (MUST DIFFRENT ADDRESS).
/// @param lcd_cols The number of columns for the LCD.
/// @param lcd_rows The number of rows for the LCD.
/// @param sda The SDA pin for I2C communication.
/// @param scl The SCL pin for I2C communication.
/// @param wire_id The Wire ID I2C Communication
LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows,uint8_t sda, uint8_t scl, uint8_t wire_id,uint8_t charsize) {
    _addr = lcd_addr;
    _cols = lcd_cols;
    _rows = lcd_rows;
    _charsize = charsize;
    _backlightval = LCD_BACKLIGHT;
    _sda = sda;
    _scl = scl;
    _wire_id = wire_id;

    _wire = new TwoWire(_wire_id);
}

void LiquidCrystal_I2C::begin() {
    if (!_wire_initialized) {
        _wire->begin(_sda, _scl);
        _wire_initialized = true;
    }

    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    if (_rows > 1) _displayfunction |= LCD_2LINE;
    if ((_charsize != 0) && (_rows == 1)) _displayfunction |= LCD_5x10DOTS;

    delay(50);
    expanderWrite(_backlightval);
    delay(1000);

    write4bits(0x03 << 4); delayMicroseconds(4500);
    write4bits(0x03 << 4); delayMicroseconds(4500);
    write4bits(0x03 << 4); delayMicroseconds(150);
    write4bits(0x02 << 4);

    command(LCD_FUNCTIONSET | _displayfunction);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();
    clear();

    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
    home();
    forceReset();
}

void LiquidCrystal_I2C::forceReset() {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0x00);
    delay(1000);
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(_backlightval);
    delay(1000);
    clear();
    home();
}

void LiquidCrystal_I2C::clear() {
    command(LCD_CLEARDISPLAY);
    delayMicroseconds(2000);
    _cursor_col = 0;
    _cursor_row = 0;
}

void LiquidCrystal_I2C::home() {
    command(LCD_RETURNHOME);
    delayMicroseconds(2000);
    _cursor_col = 0;
    _cursor_row = 0;
}

void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= _rows) row = _rows - 1;
    if (col >= _cols) col = _cols - 1;
    _cursor_col = col;
    _cursor_row = row;
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LiquidCrystal_I2C::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::scrollDisplayLeft() {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LiquidCrystal_I2C::scrollDisplayRight() {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LiquidCrystal_I2C::leftToRight() {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::rightToLeft() {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::autoscroll() {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::noAutoscroll() {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7;
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) write(charmap[i]);
}

void LiquidCrystal_I2C::noBacklight() {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void LiquidCrystal_I2C::backlight() {
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(0);
}

bool LiquidCrystal_I2C::getBacklight() {
    return _backlightval == LCD_BACKLIGHT;
}

inline void LiquidCrystal_I2C::command(uint8_t value) {
    send(value, 0);
}

inline size_t LiquidCrystal_I2C::write(uint8_t value) {
    if (_cursor_col >= _cols) return 0;
    send(value, Rs);
    _cursor_col++;
    return 1;
}

void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    write4bits(highnib | mode);
    write4bits(lownib | mode);
}

void LiquidCrystal_I2C::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void LiquidCrystal_I2C::expanderWrite(uint8_t data) {
    _wire->beginTransmission(_addr);
    _wire->write(data | _backlightval);
    _wire->endTransmission();
}

void LiquidCrystal_I2C::pulseEnable(uint8_t data) {
    expanderWrite(data | En);
    delayMicroseconds(1);
    expanderWrite(data & ~En);
    delayMicroseconds(50);
}

void LiquidCrystal_I2C::load_custom_character(uint8_t char_num, uint8_t *rows) {
    createChar(char_num, rows);
}

void LiquidCrystal_I2C::setBacklight(uint8_t new_val) {
    if (new_val) backlight();
    else noBacklight();
}

void LiquidCrystal_I2C::printstr(const char c[]) {
    print(c);
}
