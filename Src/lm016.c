#include "lm016.h"

int x,y = 0;

void send_command(lcd_t * lcd, uint8_t cmd){
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);
	if(lcd->mode == _8_BIT){
		for(int i=0; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], !!(cmd & (1<<i)) ? GPIO_PIN_RESET : GPIO_PIN_SET );
		}
	}
	else if(lcd->mode == _4_BIT){
		
		for(int i=4; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], !!(cmd & (1<<i)) ? GPIO_PIN_RESET : GPIO_PIN_SET );
		}
		
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		for(int i=4; i<8 ; i++){
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], !!(cmd & (1<<(i-4))) ? GPIO_PIN_RESET : GPIO_PIN_SET );
		}
		
	}
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
}
void lcd_shift_cursor_R(lcd_t * lcd){
	x++;
	if (x == 16){
		y++;
		x = 0;
		if(y == 2){
			y = 0;
		}
	}
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[0], lcd->data_pins[0], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[1], lcd->data_pins[1], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[2], lcd->data_pins[2], GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->data_ports[3], lcd->data_pins[3], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[4], lcd->data_pins[4], GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->data_ports[5], lcd->data_pins[5], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[6], lcd->data_pins[6], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[7], lcd->data_pins[7], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
}

void lcd_shift_cursor_L(lcd_t * lcd){
	x--;
	if (x == -1){
		y--;
		x = 15;
		if(y == -1){
			y = 1;
		}
	}
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[0], lcd->data_pins[0], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[1], lcd->data_pins[1], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[2], lcd->data_pins[2], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[3], lcd->data_pins[3], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[4], lcd->data_pins[4], GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->data_ports[5], lcd->data_pins[5], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[6], lcd->data_pins[6], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[7], lcd->data_pins[7], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
}

void lcd_init(lcd_t * lcd)
{
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(lcd->data_ports[0], lcd->data_pins[0], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[1], lcd->data_pins[1], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[2], lcd->data_pins[2], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[3], lcd->data_pins[3], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[4], lcd->data_pins[4], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[5], lcd->data_pins[5], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[6], lcd->data_pins[6], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[7], lcd->data_pins[7], GPIO_PIN_RESET);
}

void lcd_putchar(lcd_t * lcd, uint8_t character)
{
	HAL_GPIO_WritePin(lcd->en_port,lcd->en_pin, GPIO_PIN_SET);
	HAL_Delay(1);
	if(lcd->mode){
			
		HAL_GPIO_WritePin(lcd->data_ports[0], lcd->data_pins[0], !!(character & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[1], lcd->data_pins[1], !!(character & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[2], lcd->data_pins[2], !!(character & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[3], lcd->data_pins[3], !!(character & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[4], lcd->data_pins[4], !!(character & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[5], lcd->data_pins[5], !!(character & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[6], lcd->data_pins[6], !!(character & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[7], lcd->data_pins[7], !!(character & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
	HAL_Delay(1);
	HAL_GPIO_WritePin(lcd->en_port,lcd->en_pin, GPIO_PIN_RESET);
	
	x++;
	if (x == 16){
		y++;
		x = 0;
		if(y == 2){
			y = 0;
		}
	}
}

void lcd_set_curser(lcd_t * lcd, uint16_t row, uint16_t col)
{
	if (col == y){
		if(row > x){
			while(row - x > 0){
				lcd_shift_cursor_R(lcd);
			}
		}
		else if(row < x){
			while(x - row > 0){
				lcd_shift_cursor_L(lcd);
			}
		}
	}
	else if (col > y){
		while (16*(col - y) + row - x > 0){
			lcd_shift_cursor_R(lcd);
		}
	}
	else{
		while (16*(y - col) + x - row > 0){
			lcd_shift_cursor_L(lcd);
		}
	}
}

void lcd_clear(lcd_t * lcd)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);  //selecting command register
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(lcd->en_port,lcd->en_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[0], lcd->data_pins[0], GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->data_ports[1], lcd->data_pins[1], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[2], lcd->data_pins[2], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[3], lcd->data_pins[3], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[4], lcd->data_pins[4], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[5], lcd->data_pins[5], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[6], lcd->data_pins[6], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->data_ports[7], lcd->data_pins[7], GPIO_PIN_RESET);
		
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_SET);
	
}

void lcd_puts(lcd_t * lcd, char *str)
{
	while(*str != '\0')
	{
		lcd_putchar(lcd,*str);
		str++;
	}
}
