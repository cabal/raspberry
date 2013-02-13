#define LCD_SET_COLUMN_ADDR_LSB 0x00
#define LCD_SET_COLUMN_ADDR_MSB 0x10
#define LCD_SET_POWER_CTRL 0x28
#define LCD_SET_LINE_ADDR 0x40
#define LCD_SET_PAGE_ADDR 0xB0
#define LCD_SET_BIAS 0x81
#define LCD_ENABLE_DISPLAY 0xAF
#define LCD_ENABLE_ALL 0xA5
#define LCD_INVERT_DISPLAY 0xA7
#define LCD_SYSTEM_RESET 0xE2
#define LCD_SET_SCROLL 0x88

#define CHAR_LARROW 127
#define CHAR_RARROW 126
#define CHAR_DEG 128

void lcdClear(void);
void lcdClearLine(unsigned char);
void lcdClearToEnd(void);
void lcdNewLine(void);
void lcdGoTo(unsigned char, unsigned char);
void lcdHex(unsigned char);
void lcdHome(void);
void lcdInit(void);
void lcdText(char *);
void lcdWrite(unsigned char);
void lcdLogo(void);
void lcdClose(void);
