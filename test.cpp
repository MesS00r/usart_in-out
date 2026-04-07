#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <usart.hpp>

int main(void) {
    ubegin(9600);

    uprint("Hello ");
    uprintln("world");

    uprint("NUMS -> ", 123, 7, 12, 1111, '\n');

    // Max 20 args!!!
    uprint("fws", 's', 123, "fws", 23, "fws", 's', 123, "fws", 23, "fws", 's', 123, "fws", 23, "fws", 's', 123, "fws", 23);
    uprintln("fws", 's', 123, "fws", 23, "fws", 's', 123, "fws", 23, "fws", 's', 123, "fws", 23, "fws", 's', 123, "fws", 23);

    return 0;
}