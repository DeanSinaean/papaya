#include <stdint.h>

#define CFG_CLASSCODE 0x08
#define CFG_USBBASE   0x10
#define CFG_HCCR   	  0x34
//#define CFG_EECP   	  0x34
#define CFG_USBLEGSUP 0x00 // offset to EECP
#define CFG_USBLEGSUP_OSSEM 24 
#define CFG_USBLEGSUP_BIOSSEM 16 
#define CFG_USBLEGCTLSTS 0x04 //offset to EECP

#define CFG_INT 0x3C
#define CFG_MM_BASE 0x10

inline uint32_t cfgIORead32( uint32_t base, uint32_t offset);
inline uint8_t cfgIORead8( uint32_t base, uint32_t offset);
inline void cfgIOWrite32( uint32_t base, uint32_t offset,uint32_t value);
inline void cfgIOWrite8( uint32_t base, uint32_t offset,uint8_t value);
