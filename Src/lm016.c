#include "lm016.h"

void send_command(lcd_t * lcd, uint8_t cmd){
	
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);
	
	if(lcd->mode == _8_BIT){
		for(int i=0; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (cmd & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET );
		}
	}
	else if(lcd->mode == _4_BIT){
		
		for(int i=4; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (cmd & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET );
		}
		
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		for(int i=4; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (cmd & (1<<(i-4))) ? GPIO_PIN_SET : GPIO_PIN_RESET );
		}
		
	}
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void lcd_init(lcd_t * lcd)
{
	__GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef LCD_O;
	LCD_O.Mode = GPIO_MODE_OUTPUT_PP;
	LCD_O.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	LCD_O.Pull  = GPIO_NOPULL;
	
	LCD_O.Pin = lcd->rs_pin;
	HAL_GPIO_Init(lcd->rs_port, &LCD_O);
	
	LCD_O.Pin = lcd->en_pin;
	HAL_GPIO_Init(lcd->en_port, &LCD_O);
	
	for( uint8_t i=0; i<8; i++)
	{
		LCD_O.Pin  = lcd->data_pins[i];
		HAL_GPIO_Init(lcd->data_ports[i], &LCD_O);
	}
	
  HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	HAL_Delay(40);
	if(lcd->mode == _8_BIT)
		send_command(lcd, 0x38);
	else
		send_command(lcd, 0x28);
	
	
	send_command(lcd, 0x0E); //display on, cursor blinking
	send_command(lcd, 0x01); //clear LCD
	
}

void lcd_putchar(lcd_t * lcd, uint8_t character)
{
	HAL_GPIO_WritePin(lcd->rs_port,lcd->rs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->en_port,lcd->en_pin, GPIO_PIN_SET);
	
	
	if(lcd->mode == _8_BIT){
		for(int i=0; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (character & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET );
		}
	}
	else if(lcd->mode == _4_BIT){
		
		for(int i=4; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (character & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET );
		}
		
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		for(int i=4; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (character & (1<<(i-4))) ? GPIO_PIN_SET : GPIO_PIN_RESET );
		}
		
	}
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void lcd_set_curser(lcd_t * lcd, uint16_t row, uint16_t col)
{
	if(col < 16){
		if(row == 0)
			send_command(lcd,0x80+col);
		else if(row == 1)
			send_command(lcd,0xC0+col);
	}
}

void lcd_clear(lcd_t * lcd)
{
	send_command(lcd, 0x01);
}

void lcd_puts(lcd_t * lcd, char *str)
{
	while(*str != '\0')
	{
		lcd_putchar(lcd,*str);
		str++;
	}
}
