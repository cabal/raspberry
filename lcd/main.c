#include <stdio.h>
#include <string.h>

#include "lcd.h"

int main(void) {
	lcdInit();

	lcdLogo();

	char *hw = "Hello world!";;
	lcdGoTo(7, (21 - strlen(hw)) * 3 + 1);
	lcdText(hw);

	lcdClose();
	return 0;
}
