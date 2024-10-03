#include "oled_lib/oled_128x32.h"
#include "oled_lib/font.h"

#define OLED_128X32

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;

// @var array Chache memory Lcd 4 * 128 = 512
uint8_t oled_buffer_array[MEM_SIZE] = {SSD1306_DATA_STREAM};
uint8_t _counter = 0;


uint8_t init_ssd1306[] = {         
  0x00,
  SSD1306_DISPLAY_OFF,                                            // 0xAE = Set Display OFF
  SSD1306_COMMAND,                                                 // send command

  SSD1306_SET_MUX_RATIO,
   0x1F,                                                           // 0xA8 - 0x3F for 128 x 64 version (64MUX)  
                                                                  //  - 0x1F for 128 x 32 version (32MUX)
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
  0xF0,                                                           // 0x81, 0x7F - reset value (max 0xFF)
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
  0xA4,                                                           //follow RAM content A5 set entire display ON
  SSD1306_DISPLAY_ON,                                             // 0xAF = Set Display ON
};


oled_128x32_possition_t oled_128x32_possition = {0, 0};

uint8_t init_oled_array = (sizeof(init_ssd1306)/sizeof(init_ssd1306[0]));

oled_status I2C_DMA_Transmit(uint8_t *data_ptr, uint16_t size) {
   uint8_t return_status = 0;

   while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY){}
   if (HAL_I2C_Master_Transmit_DMA(&hi2c1, OLED_I2C_ADDRESS, data_ptr, size) == HAL_OK){
      return_status = SSD1306_OK;
   }
   else{
      return_status = SSD1306_ERROR;
   }

   return return_status;
}
oled_status oled_128x32_Init(uint8_t *data, uint16_t size){
   uint8_t return_status = SSD1306_UNDEFINED;

   while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY){}
   if (HAL_I2C_Master_Transmit_DMA(&hi2c1, OLED_I2C_ADDRESS, data, size) == HAL_OK){
      return_status = SSD1306_OK;
   }
   else{
      return_status = SSD1306_ERROR;
   }

   return return_status;
  
}

oled_status oled_128x32_Update(uint8_t *data){
     return I2C_DMA_Transmit(data, MEM_SIZE);
}

oled_status oled_128x32_Set_Position(oled_128x32_possition_t *position, uint8_t posx, uint8_t posy){
   uint8_t status = SSD1306_UNDEFINED;

   if ((posx > MAX_X) || (posy > MAX_Y)) {                               // if out of range
      return status = SSD1306_ERROR;                                         // out of range
   }
   else{
      position->x = posx;
      position->y = posy;
      status = SSD1306_OK;   
   }

   return status;
}

oled_status oled_128x32_Set_Pixel(uint8_t x, uint8_t y){

  uint8_t page = 0;
  uint8_t pixel = 0;
  static uint32_t index =0;
  oled_128x32_Set_Position(&oled_128x32_possition, x, y);
  page = y >> 3;                                                  // find page (y / 8)
  pixel = 1 << (y - (page << 3));                                 // which pixel (y % 8)
  index = x + (page << 7) + 1;                                        // update counter
  oled_buffer_array[index++] |= pixel;                            // save pixel

   return SSD1306_OK;
}

oled_status oled_128x32_Clear(void){
   uint8_t status = SSD1306_UNDEFINED;
   memset((uint8_t *)oled_buffer_array, 0x00, MEM_SIZE);
   status = SSD1306_OK;

   return status;
}

// inline uint8_t read_byte(const uint8_t* font_array_ptr){
//    uint8_t byte_to_return = '\0';
//    byte_to_return = *font_array_ptr;
//    return byte_to_return;
// }


inline oled_status oled_128x32_DrawChar(char character, uint8_t fonts){
  uint8_t status = SSD1306_UNDEFINED;
  uint8_t columns_number = 0;
  uint8_t char_cols_lenth = 0;
  static uint16_t array_position = 1;
  uint8_t *chose_font = NULL;

    // Check if character is valid
    if (character < 32 || character > 126){
        status = SSD1306_ERROR;
    }
    else
    {
      switch (fonts)
       {
       case FONT_5x8_:
             char_cols_lenth = CHARS_COLS_LENGTH_5x8;
             chose_font = &FONT_5x8[character - 32][columns_number];
          break;
       case FONT_6x8_:
             char_cols_lenth = CHARS_COLS_LENGTH_6x8;
             chose_font = &FONT_6x8[character - 32][columns_number];
          break;
         default:
         break;
      }
      while(columns_number < char_cols_lenth){
         oled_buffer_array[array_position] = chose_font[columns_number];
         columns_number++;
         array_position++;
      }
      status = SSD1306_OK;
    }

   return status;
}

void oled_128x32_DrawString (char *str, uint8_t fonts)
{
  uint32_t i = 0;
  while (str[i] != '\0') {
    oled_128x32_DrawChar (str[i++], fonts);
  }
}