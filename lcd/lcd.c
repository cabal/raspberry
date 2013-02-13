#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "font.h"
#include "lcd.h"
#include "logo.h"

#define I2C_COMMAND 0x3c
#define I2C_DATA 0x3d

int cmd_fd, data_fd;
unsigned char p_col, p_row;

void _lcdDataWrite(unsigned char data) {
	write( data_fd, &data, 1 );
}

void _lcdCommandWrite(unsigned char command) {
	write( cmd_fd, &command, 1 );
}

void _lcdChar(char c) {
	p_col += 6;
	if (p_col > 122) {
		p_row++;
		if (p_row > 7) {
			p_row = 0;
		}

		lcdGoTo(p_row, 0);
	}

	if (c < 32 || c > 128) {
		c = '?';
	}

	c -= 32;

	unsigned char i, j, o, n;
	for(i = 0; i < 5; i++) {
		o = font5x8[5 * c + i];

		for(n = j = 0; j < 8; j++) {
			n = (n << 1) + (o & 1);
			o >>= 1;
		}

		_lcdDataWrite(n);
	}

	_lcdDataWrite(0x00);
}

void lcdInit(void) {
	if ( ( cmd_fd = open( "/dev/i2c-1", O_RDWR ) ) < 0 ) {
		printf("Failed to open the bus.");
		exit(1);
	}

	if ( ( data_fd = open( "/dev/i2c-1", O_RDWR ) ) < 0 ) {
		printf("Failed to open the bus.");
		exit(1);
	}

	if ( ioctl( cmd_fd, I2C_SLAVE, 0x3c ) < 0 ) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		exit(1);
	}

	if ( ioctl( data_fd, I2C_SLAVE, 0x3d ) < 0 ) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		exit(1);
	}

	_lcdCommandWrite(LCD_SYSTEM_RESET);
	usleep(50);

	_lcdCommandWrite(LCD_SET_BIAS);
	_lcdCommandWrite(0x60);

	lcdClear();

	_lcdCommandWrite(LCD_ENABLE_DISPLAY);
}

void lcdClose(void) {
	close( data_fd );
	close( cmd_fd );
}

void lcdClear(void) {
	unsigned char row;
	unsigned char col;

	for (row = 0; row < 8; row ++) {
		lcdGoTo(row, 0);
		for (col = 0; col < 128; col++) {
			_lcdDataWrite(0x00);
		}
	}

	lcdGoTo(0, 0);
}

void lcdClearLine(unsigned char l) {
	unsigned char i;

	lcdGoTo(l, 0);
	for (i = 0; i < 128; i++) {
		_lcdDataWrite(0x00);
	}

	lcdGoTo(l, 0);
}

void lcdClearToEnd(void) {
	for (; p_col < 128; p_col++) {
		_lcdDataWrite(0x00);
	}
}

void lcdNewLine(void) {
	lcdGoTo(p_row == 7 ? 0 : p_row + 1, 0);
}

void lcdHome(void) {
	lcdGoTo(0, 0);
}

void lcdGoTo(unsigned char row, unsigned char col) {
	_lcdCommandWrite(LCD_SET_PAGE_ADDR | (7 - row));
	_lcdCommandWrite(LCD_SET_COLUMN_ADDR_LSB | ( col & 0x0f ));
	_lcdCommandWrite(LCD_SET_COLUMN_ADDR_MSB | ( col >> 4 ));

	p_row = row;
	p_col = col;
}

void lcdText(char* text) {
	while(*text != 0) {
		_lcdChar(*text);
		text++;
	}
}

void lcdWrite(unsigned char c) {
	_lcdChar(c);
}

void lcdLogo(void) {
	unsigned char r, c;
	for(r = 0; r < 7; r++) {
		lcdGoTo(r, 0);
		for(c = 0; c < 128; c++) {
			_lcdDataWrite( cabal_logo[128 * r + c] );
		}
	}
}
