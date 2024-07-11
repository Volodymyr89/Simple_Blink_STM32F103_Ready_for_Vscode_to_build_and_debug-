#ifndef _128_32_OLED_
#define _128_32_OLED_

#include "stdint.h"
#include "stm32f1xx_hal_dma.h"

 // Address definition
  // ------------------------------------------------------------------------------------
  #define SSD1306_ADDR              0x3C

  // Command definition
  // ------------------------------------------------------------------------------------
  #define SSD1306_COMMAND           0x80  // Continuation bit=1, D/C=0; 1000 0000
  #define SSD1306_COMMAND_STREAM    0x00  // Continuation bit=0, D/C=0; 0000 0000
  #define SSD1306_DATA              0xC0  // Continuation bit=1, D/C=1; 1100 0000
  #define SSD1306_DATA_STREAM       0x40  // Continuation bit=0, D/C=1; 0100 0000

  #define SSD1306_SET_MUX_RATIO     0xA8  // Set MUX ratio to N+1 MUX, N=A[5:0] : from 16MUX to 64MUX
  #define SSD1306_DISPLAY_OFFSET    0xD3  // Set Display Offset
  #define SSD1306_DISPLAY_ON        0xAF  // Display ON in normal mode
  #define SSD1306_DISPLAY_OFF       0xAE  // Display OFF (sleep mode)
  #define SSD1306_DIS_ENT_DISP_ON   0xA4  // Entire Display ON, Output ignores RAM content
  #define SSD1306_DIS_IGNORE_RAM    0xA5  // Resume to RAM content display, Output follows RAM content
  #define SSD1306_DIS_NORMAL        0xA6  // Normal display, 0 in RAM: OFF in display panel, 1 in RAM: ON in display panel
  #define SSD1306_DIS_INVERSE       0xA7  // Inverse display, 0 in RAM: ON in display panel, 1 in RAM: OFF in display panel
  #define SSD1306_DEACT_SCROLL      0x2E  // Stop scrolling that is configured by command 26h/27h/29h/2Ah
  #define SSD1306_ACTIVE_SCROLL     0x2F  // Start scrolling that is configured by the scrolling setup commands:26h/27h/29h/2Ah
  #define SSD1306_SET_START_LINE    0x40  // Set Display Start Line
  #define SSD1306_MEMORY_ADDR_MODE  0x20  // Set Memory, Addressing Mode
  #define SSD1306_SET_COLUMN_ADDR   0x21  // Set Column Address
  #define SSD1306_SET_PAGE_ADDR     0x22  // Set Page Address 
  #define SSD1306_SEG_REMAP         0xA0  // Set Segment Re-map, X[0]=0b column address 0 is mapped to SEG0
  #define SSD1306_SEG_REMAP_OP      0xA1  // Set Segment Re-map, X[0]=1b: column address 127 is mapped to SEG0
  #define SSD1306_COM_SCAN_DIR      0xC0  // Set COM Output, X[3]=0b: normal mode (RESET) Scan from COM0 to COM[N –1], e N is the Multiplex ratio
  #define SSD1306_COM_SCAN_DIR_OP   0xC8  // Set COM Output, X[3]=1b: remapped mode. Scan from COM[N-1] to COM0, e N is the Multiplex ratio
  #define SSD1306_COM_PIN_CONF      0xDA  // Set COM Pins Hardware Configuration, 
                                          // A[4]=0b, Sequential COM pin configuration, A[4]=1b(RESET), Alternative COM pin configuration
                                          // A[5]=0b(RESET), Disable COM Left/Right remap, A[5]=1b, Enable COM Left/Right remap                                        
  #define SSD1306_SET_CONTRAST      0x81  // Set Contrast Control, Double byte command to select 1 to 256 contrast steps, increases as the value increases
  #define SSD1306_SET_OSC_FREQ      0xD5  // Set Display Clock Divide Ratio/Oscillator Frequency
                                          // A[3:0] : Define the divide ratio (D) of the  display clocks (DCLK): Divide ratio= A[3:0] + 1, RESET is 0000b (divide ratio = 1) 
                                          // A[7:4] : Set the Oscillator Frequency, FOSC. Oscillator Frequency increases with the value of A[7:4] and vice versa. RESET is 1000b
  #define SSD1306_SET_CHAR_REG      0x8D  // Charge Pump Setting, A[2] = 0b, Disable charge pump(RESET), A[2] = 1b, Enable charge pump during display on 
                                          // The Charge Pump must be enabled by the following command:
                                          // 8Dh ; Charge Pump Setting
                                          // 14h ; Enable Charge Pump
                                          // AFh; Display ON
  #define SSD1306_SET_PRECHARGE     0xD9  // Set Pre-charge Period
  #define SSD1306_VCOM_DESELECT     0xDB  // Set VCOMH Deselect Leve
  #define SSD1306_NOP               0xE3  // No operation
  #define SSD1306_RESET             0xE4  // Maybe SW RESET, @source https://github.com/SmingHub/Sming/issues/501

#define INIT_ARRAY_SIZE      (sizeof(INIT_SSD1306[])/sizeof(INIT_SSD1306[0]))

const uint8_t INIT_SSD1306[] = {            
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

void oled_128x32_init(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);

#endif