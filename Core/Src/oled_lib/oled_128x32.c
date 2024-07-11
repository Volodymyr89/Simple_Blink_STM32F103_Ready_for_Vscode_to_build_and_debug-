#include "oled_lib/oled_128x32.h"



void oled_128x32_init(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength){

    HAL_DMA_Start_IT(hdma, SrcAddress, DstAddress, DataLength);
}