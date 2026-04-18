#ifndef USARTINPUT_HPP_INCLUDED
#define USARTINPUT_HPP_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h> // IWYU pragma: keep

#define BUFFER_SIZE 32

// ****** FUNCTIONS C ******

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sys_buffer sys_buffer;

sys_buffer* buffer_init(void);

bool buffer_reset(sys_buffer *buf);
bool buffer_sendch(sys_buffer *buf, char ch);
bool buffer_readch(sys_buffer *buf, char *ch);

void input_timer_init(void);

bool uset_ch(sys_buffer *buf, char *copy);
bool uset_line(sys_buffer *buf);

bool uread_word(sys_buffer *buf, char *word_buf, uint8_t len);
bool uread_num(sys_buffer *buf, uint16_t *num);


#ifdef __cplusplus
}

// ****** FUNCTIONS && CLASS C++ ******

class Input {
private:
    static inline sys_buffer *buf;
public:
    static void uinput_init() {
        buf = buffer_init();
        input_timer_init();
    }
    
    static bool buf_reset()                        { return buffer_reset(buf); }
    static bool buf_sendch(char ch)                { return buffer_sendch(buf, ch); }
    static bool buf_readch(char &ch)               { return buffer_readch(buf, &ch); }

    static bool set_ch()                           { return uset_ch(buf, NULL); }
    static bool set_line()                         { return uset_line(buf); }

    static bool read_word(char *word, uint8_t len) { return uread_word(buf, word, len); }
    static bool read_num(uint16_t &num)            { return uread_num(buf, &num); }

    Input() = delete;
};

// ****** USART SCAN ******

typedef enum { CH, STR } specs;

namespace uscan_sys {
    inline bool uscan_check(specs spec) {
        switch (spec) {
        case CH: Input::set_ch();    return true;
        case STR: Input::set_line(); return true;
        }
        return false;
    }

    template<typename T>
    inline void uscan_check(const T others) { static_assert(sizeof(T) == 0, "Invalid specifier."); }
}

template<typename ... Args>
bool uscan(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for usan.");
    return (uscan_sys::uscan_check(args) && ...);
}

// ****** USART READ ******

struct Word { char *word; uint8_t len; };
struct Num  { uint16_t &num; };

template<uint8_t L>
inline Word s_word(char (&word)[L]) { return {word, sizeof(word)}; }
inline Num s_num(uint16_t &num)     { return {num}; }

namespace uread_sys {
    inline bool uread_accept(Word word)     { return Input::read_word(word.word, word.len); }
    inline bool uread_accept(Num num)       { return Input::read_num(num.num); }

    template<typename T>
    inline void uread_accept(const T others) { static_assert(sizeof(T) == 0, "Invalid specifier."); }
};

template<typename ... Args>
bool uread(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for uread.");
    return (uread_sys::uread_accept(args) && ...);
}

#endif // __cplusplus

#endif //USARTINPUT_HPP_INCLUDED