#ifndef _128_32_OLED_
#define _128_32_OLED_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include <string.h> 

#define MAX_X                (uint8_t)127
#define MAX_Y                (uint8_t)31
#define MEM_SIZE             (uint16_t)513
#define OLED_I2C_ADDRESS     (uint8_t)(0x3C<<1)


  // Command definition
  // ------------------------------------------------------------------------------------
  #define SSD1306_COMMAND           (uint8_t)0x80  // Continuation bit=1, D/C=0; 1000 0000
  #define SSD1306_COMMAND_STREAM    (uint8_t)0x00  // Continuation bit=0, D/C=0; 0000 0000
  #define SSD1306_DATA              (uint8_t)0xC0  // Continuation bit=1, D/C=1; 1100 0000
  #define SSD1306_DATA_STREAM       (uint8_t)0x40  // Continuation bit=0, D/C=1; 0100 0000
  #define SSD1306_SET_MUX_RATIO     (uint8_t)0xA8  // Set MUX ratio to N+1 MUX, N=A[5:0] : from 16MUX to 64MUX
  #define SSD1306_DISPLAY_OFFSET    (uint8_t)0xD3  // Set Display Offset
  #define SSD1306_DISPLAY_ON        (uint8_t)0xAF  // Display ON in normal mode
  #define SSD1306_DISPLAY_OFF       (uint8_t)0xAE  // Display OFF (sleep mode)
  #define SSD1306_DIS_ENT_DISP_ON   (uint8_t)0xA4  // Entire Display ON, Output ignores RAM content
  #define SSD1306_DIS_IGNORE_RAM    (uint8_t)0xA5  // Resume to RAM content display, Output follows RAM content
  #define SSD1306_DIS_NORMAL        (uint8_t)0xA6  // Normal display, 0 in RAM: OFF in display panel, 1 in RAM: ON in display panel
  #define SSD1306_DIS_INVERSE       (uint8_t)0xA7  // Inverse display, 0 in RAM: ON in display panel, 1 in RAM: OFF in display panel
  #define SSD1306_DEACT_SCROLL      (uint8_t)0x2E  // Stop scrolling that is configured by command 26h/27h/29h/2Ah
  #define SSD1306_ACTIVE_SCROLL     (uint8_t)0x2F  // Start scrolling that is configured by the scrolling setup commands:26h/27h/29h/2Ah
  #define SSD1306_SET_START_LINE    (uint8_t)0x40  // Set Display Start Line
  #define SSD1306_MEMORY_ADDR_MODE  (uint8_t)0x20  // Set Memory, Addressing Mode
  #define SSD1306_SET_COLUMN_ADDR   (uint8_t)0x21  // Set Column Address
  #define SSD1306_SET_PAGE_ADDR     (uint8_t)0x22  // Set Page Address 
  #define SSD1306_SEG_REMAP         (uint8_t)0xA0  // Set Segment Re-map, X[0]=0b column address 0 is mapped to SEG0
  #define SSD1306_SEG_REMAP_OP      (uint8_t)0xA1  // Set Segment Re-map, X[0]=1b: column address 127 is mapped to SEG0
  #define SSD1306_COM_SCAN_DIR      (uint8_t)0xC0  // Set COM Output, X[3]=0b: normal mode (RESET) Scan from COM0 to COM[N –1], e N is the Multiplex ratio
  #define SSD1306_COM_SCAN_DIR_OP   (uint8_t)0xC8  // Set COM Output, X[3]=1b: remapped mode. Scan from COM[N-1] to COM0, e N is the Multiplex ratio
  #define SSD1306_COM_PIN_CONF      (uint8_t)0xDA  // Set COM Pins Hardware Configuration, 
                                          // A[4]=0b, Sequential COM pin configuration, A[4]=1b(RESET), Alternative COM pin configuration
                                          // A[5]=0b(RESET), Disable COM Left/Right remap, A[5]=1b, Enable COM Left/Right remap                                        
  #define SSD1306_SET_CONTRAST      (uint8_t)0x81  // Set Contrast Control, Double byte command to select 1 to 256 contrast steps, increases as the value increases
  #define SSD1306_SET_OSC_FREQ      (uint8_t)0xD5  // Set Display Clock Divide Ratio/Oscillator Frequency
                                          // A[3:0] : Define the divide ratio (D) of the  display clocks (DCLK): Divide ratio= A[3:0] + 1, RESET is 0000b (divide ratio = 1) 
                                          // A[7:4] : Set the Oscillator Frequency, FOSC. Oscillator Frequency increases with the value of A[7:4] and vice versa. RESET is 1000b
  #define SSD1306_SET_CHAR_REG      (uint8_t)0x8D  // Charge Pump Setting, A[2] = 0b, Disable charge pump(RESET), A[2] = 1b, Enable charge pump during display on 
                                          // The Charge Pump must be enabled by the following command:
                                          // 8Dh ; Charge Pump Setting
                                          // 14h ; Enable Charge Pump
                                          // AFh; Display ON
  #define SSD1306_SET_PRECHARGE     (uint8_t)0xD9  // Set Pre-charge Period
  #define SSD1306_VCOM_DESELECT     (uint8_t)0xDB  // Set VCOMH Deselect Leve
  #define SSD1306_NOP               (uint8_t)0xE3  // No operation
  #define SSD1306_RESET             (uint8_t)0xE4  // Maybe SW RESET, @source https://github.com/SmingHub/Sming/issues/501

typedef enum status{
  SSD1306_UNDEFINED,
   SSD1306_OK,
   SSD1306_ERROR
}oled_status;


typedef struct position{
   uint8_t x;
   uint8_t y;
}oled_128x32_possition_t;

typedef enum fonts{
   FONT_5x8_ = (uint8_t)0,
   FONT_6x8_,
   FONT_8x8_
}fonts_t;

extern uint8_t oled_buffer_array[MEM_SIZE];
extern uint8_t init_ssd1306[];
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern uint8_t init_oled_array;
extern oled_128x32_possition_t oled_128x32_possition;

oled_status oled_128x32_Init(uint8_t *data, uint16_t size);
oled_status oled_128x32_Set_Pixel(uint8_t x, uint8_t y);
oled_status oled_128x32_Update(uint8_t *data);
oled_status oled_128x32_DrawChar(char character, uint8_t fonts);
oled_status oled_128x32_Set_Position(oled_128x32_possition_t *position, uint8_t posx, uint8_t posy);
void oled_128x32_DrawString (char *str, uint8_t fonts);
oled_status I2C_DMA_Transmit(uint8_t *data_ptr, uint16_t size);
#endif