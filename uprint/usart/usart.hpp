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

// ****** FUNCTIONS && STRUCT FOR C++ ******

#ifdef __cplusplus
}

struct UpArg {
    up_arg_t arg;

    UpArg(const char ch) {
        arg.type = CHAR;
        arg.data = (uint16_t)ch;
    }
    UpArg(const char *str) {
        arg.type = STR;
        arg.str = str;
    }
    UpArg(const int num, arg_type type = DEC) {
        arg.type = type;
        arg.data = num;
    }
};

static inline UpArg bin(uint16_t arg) { return UpArg(arg, BIN); }
static inline UpArg hex(uint16_t arg) { return UpArg(arg, HEX); }

// ****** UPRINT TEMPLATE ******

template<typename ... Args>
void uprint(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for uprint.");
    int dummy[] = {0, (uprint_write(UpArg(args).arg), 0)...};
    (void)dummy;
}

template<typename ... Args>
void uprintln(const Args& ... args) {
    uprint(args...);
    uput_ch('\n');
}

#endif // __cplusplus

#endif //USARTPRINT_H_INCLUDED