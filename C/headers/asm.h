#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ __volatile__ ("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outd(uint16_t port, uint32_t value) {
    __asm__ __volatile__ ("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outq(uint16_t port, uint64_t value) {
    __asm__ __volatile__ ("outq %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint32_t ind(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint64_t inq(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void invlpg(uint64_t virtual_adress) {
    __asm__ __volatile__ ("invlpg (%0)" :: "r" (virtual_adress) : "memory");
}