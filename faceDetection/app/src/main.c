#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "detectFace.h"
#include "test.h"

#ifdef __SPEAR32__
	#include "sdram.h"
	#include "svga.h"
	#include "dis7seg.h"
	#include "aluext.h"
	#include "getframe.h"
	#include "camconfig.h"
	
	extern uint8_t *reg;
	extern module_handle_t counterHandle;
#endif // __SPEAR32__

#define CLKPERIOD 20
#define PRESCALER 1

int main(int argc, char **argv)
{	
	uint32_t fps_c,i,y,fps_mean;
	
	#ifdef __SPEAR32__
		// initialize HW modules
		dis7seg_init();
		sdram_init();
		svga_init();
		test_init();
		
	#endif
	
	#ifdef TEST
		{
			image_t image;
			rect_t face;
			
			test_receiveImage(&image, argv[1]);
						
			face = faceDetection(&image);
			image_paintRectangle(&image, face);
			svga_outputImage(&image);
			test_sendImage(&image, argv[2]);
			
		}
	#else
				
		dis7seg_hex(0x01);
		
		//pause mode
		write_cam(0x0B,1<<1|1);
		
		// invert pixe clock
		write_cam(0x0A, (1<<15)|1);
		
		// column size
		write_cam(0x04, 2559);
		
		// row size
		write_cam(0x03, 1919);
		
		// shutter width lower
		//write_cam(0x09, 1200);
		
		// GAIN
		
		//write_cam(0x35,0x19C);
		
		//write_cam(0x2C,0x9A);
		//write_cam(0x2D,0x19C);
		//write_cam(0x2B,0x13);
		//write_cam(0x2E,0x13);
		
		write_cam(0x2B, (0<<8)|(0<<6)|(0x13)); //Green
		write_cam(0x2E, (0<<8)|(0<<6)|(0x13)); //Green Reset
		write_cam(0x2D, (6<<8)|(0<<6)|(0xF)); //Red Reset
		write_cam(0x2C, (5<<8)|(0<<6)|(0xF)); //Blue Reset
		
		// ganz gut: write_cam(0x2B, (0<<8)|(0<<6)|(0x13)); //Green
		// ganz gut: write_cam(0x2E, (0<<8)|(0<<6)|(0x13)); //Green Reset
		// ganz gut: write_cam(0x2D, (6<<8)|(0<<6)|(0xF)); //Red Reset
		// ganz gut: write_cam(0x2C, (5<<8)|(0<<6)|(0xF)); //Blue Reset
		
		//write_cam(0x49,0x1A8);
		
		// row and column skiping => 640x480 res
		write_cam(0x22, 0x03);
		write_cam(0x23, 0x03);
		
		// testbild
		write_cam(0xA1,123); // Grün
		write_cam(0xA2,456); // Rot
		write_cam(0xA3,4000); // Blau
		//write_cam(0xA0,(0<<4)|(1));
		
		//write_cam(0xA4,123);
		write_cam(0xA0,0);
		
		//write_cam(
		
		// Grün => Grün
		// Rot => Blau
		// Blau => Grün
		
		// Grün => Blaurot
		// Rot => Blau
		// Blau => Rot
		
		
				
		// mirror der rows
		write_cam(0x20, (1<<15));
		//write_cam(0x20, 0);
		
		
		// restart cam		
		write_cam(0x0b,1);
		
		// wait for restart finished
		while(read_cam(0x0b)&0x01)
			asm("nop");
			
		
		//dis7seg_hex(read_cam(0xA0));
		
		/*y = 0;
		while(1) {
			*reg = (1 << COUNTER_CLEAR_BIT);
			*reg = (1 << COUNTER_COUNT_BIT);
			
			GETFRAME_CLEAR = 1;
			while (!GETFRAME_CLEAR)
				asm("nop");
			
			fps_c = counter_getValue(&counterHandle);
			fps_c *= CLKPERIOD * PRESCALER;
			fps_c /= 1000000;
			
			if (y == 10) {
				dis7seg_uint32(1000/fps_c);
				y = 0;
			}
			else {
				y++;
			}
		}*/
		
		y = 0;
		i = 0;
		fps_mean = 0;
		while (1) {

			*reg = (1 << COUNTER_CLEAR_BIT);
			*reg = (1 << COUNTER_COUNT_BIT);
			
			GETFRAME_START = 1;
			//i = 0;	
			while(!GETFRAME_RETURN ) {
				//dis7seg_uint32(GETFRAME_COUNTER);
				asm("nop");
			}		
			
			fps_c = counter_getValue(&counterHandle);		
			fps_c *= CLKPERIOD * PRESCALER;
			fps_c /= 1000000;
			
			fps_mean += 1000/fps_c;
			fps_mean /= 2;		
			
			if (y == 10) {
				
				dis7seg_uint32(fps_mean);
				y = 0;
			}
			else
				y++;
			i++;
		}
		
		printFrameBuffer(argv[2]);
		
	#endif
	dis7seg_hex(0xEEEEEEEE);
	#ifdef __SPEAR32__
		test_release();
		dis7seg_release();
	#endif
	
	return 0;
}
