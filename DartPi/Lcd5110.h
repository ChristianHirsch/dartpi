#ifndef LCD5110_H
#define LCD5110_H
// Access from ARM Running Linux

static const unsigned short ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e .
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f .
};

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

// reset input (active low)
#define RES	4
// chip enable (active low)
#define SCE	22
// data / -command
#define DC	9
// serial data in
#define SDIN	10
// serial clock
#define SCLK	11
// light
#define	LIGHT	17


#define LCD_WIDTH 84
#define LCD_HEIGHT 48

#define LCD_C	0
#define LCD_D	1

void setup_io();
void initLcdPorts();
void initLcdScreen();
void sendByteToLcd(bool, unsigned char);
void clearLcdScreen();
void writeCharToLcd(char);
void writeStringToLcd(char *);
void writeStringToLcdXY(unsigned char, unsigned char, char*);
void drawBWImageToLcd(char *);

/*
int main(int argc, char ** argv)
{
	initLcdPorts();
	initLcdScreen();

	sendByteToLcd(LCD_C, 0x0d);
	drawBWImageToLcd("logo.txt");
	sleep(2);

	sendByteToLcd(LCD_C, 0x0c);

	clearLcdScreen();
	writeStringToLcdXY(0, 0, "BrewPi SysInfo");
	writeStringToLcdXY(0, 1, "--------------");
	for(;;)
	{
		struct sysinfo sys_info;
		if(sysinfo(&sys_info) != 0)
			break;

		char uptimeinfo[14];
		unsigned long uptime = sys_info.uptime / 60;
		sprintf(uptimeinfo, "Uptime %lu:%02lu h", uptime/60, uptime%60);

		char cpuinfo[10];
		unsigned long avgCpuLoad = sys_info.loads[0] / 1000;
		sprintf(cpuinfo, "CPU %3lu %%", avgCpuLoad);
		if(avgCpuLoad > 80)
			GPIO_CLR = 1 << LIGHT;
		else
			GPIO_SET = 1 << LIGHT;

		char raminfo[10];
		unsigned long totalRam = (sys_info.totalram - sys_info.freeram) / 1024 / 1024;
		sprintf(raminfo, "RAM %3lu MB", totalRam);

		char procinfo[14];
		sprintf(procinfo, "Processes %3hu", sys_info.procs);

		writeStringToLcdXY(0, 2, cpuinfo);
		writeStringToLcdXY(0, 3, raminfo);
		writeStringToLcdXY(0, 4, procinfo);
		writeStringToLcdXY(0, 5, uptimeinfo);

		sleep(5);
	}

	clearLcdScreen();

	GPIO_SET = 1 << SCE;

	return 0;
}
*/

void initLcdPorts()
{
        // Set up gpi pointer for direct register access
        setup_io();

        // Switch GPIOs for LCD to output mode

        /************************************************************************\
         * You are about to change the GPIO settings of your computer.          *
         * Mess this up and it will stop working!                               *
         * It might be a good idea to 'sync' before running this program        *
         * so at least you still have your code changes written to the SD-card! *
        \************************************************************************/

	int pins[6] = {RES, SCE, DC, SDIN, SCLK, LIGHT};

        // Set GPIO pins to output
        for (int g=0; g<7; g++)
        {
                INP_GPIO(pins[g]); // must use INP_GPIO before we can use OUT_GPIO
                OUT_GPIO(pins[g]);
        }
}

void initLcdScreen()
{
	// set GPIOs
	GPIO_CLR = 1 << SCE;
	GPIO_CLR = 1 << SCLK;
	GPIO_CLR = 1 << RES;
	usleep(2);
	GPIO_SET = 1 << RES;

	// init LCD
        sendByteToLcd(LCD_C, 0x21);	// LCD Extended Commands
        sendByteToLcd(LCD_C, 0xb1);	// Set LCD Cop (Contrast).	//0xb1
        sendByteToLcd(LCD_C, 0x04);	// Set Temp coefficent.		//0x04
        sendByteToLcd(LCD_C, 0x14);	// LCD bias mode 1:48. 		//0x13
        sendByteToLcd(LCD_C, 0x0c);	// LCD in normal mode. 0x0d inverse mode
        sendByteToLcd(LCD_C, 0x20);
        sendByteToLcd(LCD_C, 0x0c);

	clearLcdScreen();
}

void writeCharToLcd(char data)
{
	//sendByteToLcd(LCD_C, 0x0);
	for(int i=0; i < 5; i++)
		sendByteToLcd(LCD_D, ASCII[data-0x20][i]);
	sendByteToLcd(LCD_D, 0x00);
}

void writeStringToLcd(char *data)
{
	while(*data)
		writeCharToLcd(*data++);
}

void writeStringToLcdXY(unsigned char x, unsigned char y, char *data)
{
	sendByteToLcd(LCD_C, 0x80 | x);
	sendByteToLcd(LCD_C, 0x40 | y);

	writeStringToLcd(data);
}

void clearLcdScreen()
{
	sendByteToLcd(LCD_C, 0x80);
	sendByteToLcd(LCD_C, 0x40);
	for(int i=0; i < LCD_WIDTH * LCD_HEIGHT / 8; i++)
		sendByteToLcd(LCD_D, 0x00);
}

void sendByteToLcd(bool cd, unsigned char data)
{
	if(cd)
		GPIO_SET = 1 << DC;
	else
		GPIO_CLR = 1 << DC;

	unsigned char pattern = 0b10000000;
	for(int i=0; i < 8; i++)
	{
		GPIO_CLR = 1 << SCLK;
		if(data & pattern)
			GPIO_SET = 1 << SDIN;
		else
			GPIO_CLR = 1 << SDIN;
		usleep(1);
		GPIO_SET = 1 << SCLK;
		usleep(1);
		pattern >>= 1;
	}
}

void drawBWImageToLcd(char *file)
{
	FILE *fp = fopen(file, "r");
	if(fp==NULL)
		return;

	bool img[LCD_WIDTH][LCD_HEIGHT];

	int x=0, y=0;
	char c;

	for(y = 0; y < LCD_HEIGHT; y++)
	{
		for(x = 0; x < LCD_WIDTH; x++)
		{
			c = fgetc(fp);
			if(c != '0' && c != '1'){ continue;}
			img[x][y] = (c == '1');
			//printf("%d", img[x][y]);
			if(c == EOF) break;
		}
		//printf("\n");
	}
	fclose(fp);

	sendByteToLcd(LCD_C, 0x80);
	sendByteToLcd(LCD_C, 0x40);

	for(y=0; y < LCD_HEIGHT; y+=8)
	{
		for(x=0; x < LCD_WIDTH; x++)
		{
			unsigned char val = 0;
			for(int i=0; i < 8; i++)
				if(img[x][y+i])
					val += 0b00000001<<i;
			sendByteToLcd(LCD_D, val);
		}
	}
}

//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = (char *)mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;

} // setup_io

#endif