#ifndef __camconfig_h__
#define __camconfig_h__

#include "drivers.h"

#define CAMCONFIG_BASE 		(0xFFFFFE60)


#define CAMCONFIG_WRITE		(*(volatile uint32_t *const)(CAMCONFIG_BASE+4))
#define CAMCONFIG_READ		(*(volatile uint32_t *const)(CAMCONFIG_BASE+8))
#define CAMCONFIG_RESULT	(*(volatile uint32_t *const)(CAMCONFIG_BASE+12))
#define CAMCONFIG_STATUS	(*(volatile uint8_t *const)(CAMCONFIG_BASE+16))

#define CAM_ID_READ		(0xBB000000)
//#define CAM_ID_READ		(0xAB000000)
#define CAM_ID_WRITE 	(0xBA000000)

uint32_t read_cam(uint8_t address);
uint8_t write_cam(uint8_t address, uint16_t data);

#endif // __camconfig_h__

