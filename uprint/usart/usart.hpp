#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <bitmath.h>
#include <common.h>

// ****** FUNCTIONS && STRUCT FOR C ******

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { CHAR, STR, DEC, BIN, HEX } arg_type;

typedef struct up_arg_t {
    arg_type type;
    union {
        uint16_t data;
        const char* str;
    };
} up_arg_t;

void ubegin(const uint16_t baud);

void uput_ch(const char ch);
void uput_str(const char *str);

void uprint_dec(const uint16_t num);
void uprint_bin(const uint16_t num);
void uprint_hex(const uint16_t num);

void uprint_write(const up_arg_t arg);
// void uprint_write(uint8_t len, const UpArg *args);

#ifdef __cplusplus
}
#endif

// ****** FUNCTIONS && STRUCT FOR C++ ******

struct UpArg {
    up_arg_t arg = {};

    UpArg(const char ch) : arg({.type = CHAR, .data = (uint16_t)(ch)}) {}
    UpArg(const char *str) : arg({.type = STR, .str = str}) {}
    UpArg(uint16_t num, arg_type type = DEC) : arg({.type = type, .data = num}) {}
};

static inline UpArg bin(uint16_t arg) { return UpArg(arg, BIN); }
static inline UpArg hex(uint16_t arg) { return UpArg(arg, HEX); }

// ****** UPRINT TEMPLATE ******

template<typename ... Args>
void uprint(Args& ... args) {
    uint16_t dummy[] = {0, (uprint_write(args), 0)...};
    (void)dummy;
}

#endif //USARTPRINT_H_INCLUDED