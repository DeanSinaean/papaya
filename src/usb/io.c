#include <stdint.h>
static inline void outl(uint32_t value, uint16_t port)
{
	__asm__ __volatile__("outl %0, %1"::"a"(value),"d"(port));
}

static inline uint32_t inl(uint16_t port)
{
	uint32_t value;
	__asm__ __volatile__("inl %1, %0":"=a"(value):"d"(port));
	return value;
}
static inline void outb(uint8_t value, uint16_t port)
{
	__asm__ __volatile__("outb %0, %1"::"a"(value),"d"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;
	__asm__ __volatile__("inb %1, %0":"=a"(value):"d"(port));
	return value;
}
