#ifndef USARTINPUT_HPP_INCLUDED
#define USARTINPUT_HPP_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h> // IWYU pragma: keep

#define BUFFER_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sys_buffer sys_buffer;

sys_buffer* buffer_init(void);

bool buffer_reset(sys_buffer *buf);
bool buffer_sendch(sys_buffer *buf, char ch);
bool buffer_readch(sys_buffer *buf, char *ch);

void input_timer_init(void);

bool uset_ch(sys_buffer *buf, uint8_t *last, char *copy);
bool uset_line(sys_buffer *buf, uint8_t len, uint8_t *word_i);

bool uread_word(sys_buffer *buf, char *word_buf, uint8_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //USARTINPUT_HPP_INCLUDED