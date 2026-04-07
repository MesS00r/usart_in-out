#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <bitmath.h>

// ****** FUNCTIONS C ******

#ifdef __cplusplus
extern "C" {
#endif

void ubegin(const uint16_t baud);

void uput_ch(const char ch);
void uput_str(const char *str);

void uprint_dec(const uint16_t num, bool neg);
void uprint_bin(const uint16_t num);
void uprint_hex(const uint16_t num);

#ifdef __cplusplus
}

// ****** FUNCTIONS && STRUCTS C++ ******

struct Bin { uint16_t val; };
struct Hex { uint16_t val; };
struct Neg { uint16_t val; };

static inline Bin bin(uint16_t arg) { return {arg}; }
static inline Hex hex(uint16_t arg) { return {arg}; }
static inline Neg neg(int16_t arg)  { return { (uint16_t)(arg < 0 ? -arg : arg)}; }

// ****** UPRINT TEMPLATE ******

namespace upsys {
    static inline void uprint_write(const char ch)      { uput_ch(ch); }
    static inline void uprint_write(const char *str)    { uput_str(str); }

    static inline void uprint_write(const int16_t num)  { uprint_dec(num, false); }
    static inline void uprint_write(const uint16_t num) { uprint_dec(num, false); }

    static inline void uprint_write(const Bin bin)      { uprint_bin(bin.val); }
    static inline void uprint_write(const Hex hex)      { uprint_hex(hex.val); }
    static inline void uprint_write(const Neg neg)      { uprint_dec(neg.val, true); }

    template<typename T>
    static inline void uprint_write(const T others)     { static_assert(sizeof(T) == 0, "Invalid type."); }
}

template<typename ... Args>
void uprint(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for uprint.");
    (upsys::uprint_write(args), ...);
}

template<typename ... Args>
void uprintln(const Args& ... args) {
    uprint(args...);
    uput_ch('\n');
}

#endif // __cplusplus

#endif //USARTPRINT_H_INCLUDED