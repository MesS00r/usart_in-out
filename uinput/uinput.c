#include "uinput.hpp"

// ****** USART BUFFER STRUCT ******

struct sys_buffer {
    char buffer[BUFFER_SIZE];
    uint8_t first, last;
};

sys_buffer* buffer_init(void) {
    static struct sys_buffer global = {
        .buffer = {0},
        .first = 0, .last = 0
    };

    return &global;
};

// ****** USART BUFFER METHODS ******

bool buffer_reset(sys_buffer *buf) {
    if (!buf) return false;

    memset(buf->buffer, 0, sizeof(buf->buffer));
    buf->first = 0;
    buf->last = 0;

    return true;
}

bool buffer_sendch(sys_buffer *buf, char ch) {
    if (!buf) return false;

    if (buf->last >= BUFFER_SIZE) {
        buffer_reset(buf);
    }

    buf->buffer[buf->last] = ch;
    buf->last++;

    return true;
}

bool buffer_readch(sys_buffer *buf, char *ch) {
    if (!buf) return false;
    if (!ch) return false;

    if (buf->first == 0 && buf->last == 0) return false;
    else if (buf->first == buf->last) {
        buffer_reset(buf);
        return false;
    }

    *ch = buf->buffer[buf->first];
    buf->first++;

    return true;
}

// ****** TIMER 1 ******

void input_timer_init(void) {
    TCCR1B = (1 << CS11) | (1 << CS10);
    TCNT1 = 0;
}

static inline bool _uget_ch_timeout(uint16_t time_tick, char *ch) {
    if (!ch) return false;

    uint16_t start = TCNT1;
    uint16_t now = 0, elapsed = 0;

    while (true) {
        now = TCNT1;

        if (UCSR0A & (1 << RXC0)) {
            *ch = UDR0;
            return true;
        }

        if (now >= start) elapsed = now - start;
        else elapsed = (65536 - start) + now;

        if (elapsed >= time_tick) return false;
    }
}

// ****** USART SCAN CHAR ******

static inline char _uget_ch_nobuf(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

bool uset_ch(sys_buffer *buf, char *copy) {
    if (!buf) return false;

    static char next = 0;
    if (next != 0) {
        if (copy) *copy = next;

        buffer_sendch(buf, next);
        next = 0;

        return true;
    }
    
    char ch = _uget_ch_nobuf();

    if (ch == '\n') ch = '\0';
    else if (ch == '\r') {
        if (!_uget_ch_timeout(1000, &next)) next = 0;
        else if (next == '\n') next = 0;

        ch = '\0';
    }

    if (copy) *copy = ch;

    buffer_sendch(buf, ch);
    return true;
}

// ****** USART SCAN STR ******

bool uset_line(sys_buffer *buf) {
    if (!buf) return false;

    char ch = 0;

    for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
        if (!uset_ch(buf, &ch)) return false;
        if (ch == '\0') return true;
    }

    buffer_sendch(buf, '\0');
    return true;
}

// ****** USART READ STR ******

bool uread_word(sys_buffer *buf, char *word_buf, uint8_t len) {
    if (!buf) return false;
    if (!word_buf) return false;
    if (len == 0) return false;

    char ch = 0;

    for (uint8_t i = 0; i < len - 1; i++) {
        if (!buffer_readch(buf, &ch)) return false;
        word_buf[i] = ch;

        if (ch == '\0') return true;
    }

    word_buf[len - 1] = '\0';

    while (buffer_readch(buf, &ch)) {
        if (ch == '\0') break;
    }

    return true;
}

// ****** USART READ NUMBER ******

bool uread_num(sys_buffer *buf, uint16_t *num) {
    if (!buf) return false;
    if (!num) return false;

    const uint8_t len = 5;
    uint8_t digit = 0;
    uint16_t val = 0;

    char num_buf[len];
    memset(num_buf, 0, len);

    if (!uread_word(buf, num_buf, sizeof(num_buf))) return false;
    num_buf[sizeof(num_buf) - 1] = '\0';

    *num = 0;

    for (int8_t i = 0; i < len; i++) {
        if (num_buf[i] == '\0') break;
        if (num_buf[i] > '9' || num_buf[i] < '0') return false;
        
        digit = num_buf[i] - '0';

        val *= 10;
        val += digit;
    }

    *num = val;
    return true;
}