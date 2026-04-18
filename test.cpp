#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <uprint.hpp>
#include <uinput.hpp>

int main(void) {
    ubegin(9600);
    Input::uinput_init();

    uprint("Hello ");
    uprintln("world");

    uscan(STR, STR);

    char word[8] = {0};
    uint16_t num = 0;

    uread(s_word(word), s_num(num));

    uprintln(word);
    uprintln(num);

    return 0;
}