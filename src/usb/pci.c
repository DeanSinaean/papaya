#include <stdint.h>
#include "io.h"
inline uint32_t cfgIORead32( uint32_t base, uint32_t offset)
{
	outl(base+offset,0xcf8);
	return inl(0xcfc);
}
inline void cfgIOWrite32( uint32_t base, uint32_t offset,uint32_t value)
{
	outl(base+offset,0xcf8);
	outl(value,0xcfc);
}
inline uint8_t cfgIORead8( uint32_t base, uint32_t offset)
{
	outb(base+offset,0xcf8);
	return inb(0xcfc);
}
inline void cfgIOWrite8( uint32_t base, uint32_t offset,uint8_t value)
{
	outb(base+offset,0xcf8);
	outb(value,0xcfc);
}
