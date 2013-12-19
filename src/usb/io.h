#include <stdint.h>
static inline void outl(uint32_t value, uint16_t port);
static inline uint32_t inl(uint16_t port);
static inline void outb(uint8_t value, uint16_t port);
static inline uint8_t inb(uint16_t port);
