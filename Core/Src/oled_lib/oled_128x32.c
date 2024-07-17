#include "oled_lib/oled_128x32.h"

const uint8_t INIT_SSD1306[] = {        
  SSD1306_ADDR,                                                   // send address    
  SSD1306_COMMAND,                                                 // send command
  SSD1306_DISPLAY_OFF,                                            // 0xAE = Set Display OFF

  SSD1306_SET_MUX_RATIO,
  0x1F,                                                           // 0xA8 - 0x3F for 128 x 64 version (64MUX)
                                                                  //      - 0x1F for 128 x 32 version (32MUX)
  SSD1306_MEMORY_ADDR_MODE,
  0x00,                                                           // 0x20 = Set Memory Addressing Mode
                                                                  // 0x00 - Horizontal Addressing Mode
                                                                  // 0x01 - Vertical Addressing Mode
                                                                  // 0x02 - Page Addressing Mode (RESET)
  SSD1306_SET_START_LINE,                                         // 0x40
  SSD1306_DISPLAY_OFFSET,
  0x00,                                                           // 0xD3
  SSD1306_SEG_REMAP_OP,                                           // 0xA0 / remap 0xA1
  SSD1306_COM_SCAN_DIR_OP,                                        // 0xC0 / remap 0xC8
  SSD1306_COM_PIN_CONF,
  0x02,                                                           // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
                                                                  // 0x12 - for 128 x 64 version
                                                                  // 0x02 - for 128 x 32 version
  SSD1306_SET_CONTRAST,
  0x7F,                                                           // 0x81, 0x7F - reset value (max 0xFF)
  SSD1306_DIS_ENT_DISP_ON,                                        // 0xA4
  SSD1306_DIS_NORMAL,                                             // 0xA6
  SSD1306_SET_OSC_FREQ,
  0x80,                                                           // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
  SSD1306_SET_PRECHARGE, 
  0xc2,                                                           // 0xD9, higher value less blinking
                                                                  // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
  SSD1306_VCOM_DESELECT,
  0x20,                                                           // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
  SSD1306_SET_CHAR_REG,
  0x14,                                                           // 0x8D, Enable charge pump during display on
  SSD1306_DEACT_SCROLL,                                           // 0x2E
  SSD1306_DISPLAY_ON,                                             // 0xAF = Set Display ON
};

void oled_128x32_init(I2C_TypeDef *I2Cx, uint8_t data){

    LL_I2C_GenerateStartCondition(I2C1);
    LL_I2C_TransmitData8(I2C1, data);
}