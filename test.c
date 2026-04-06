#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>

int main(void) {
    ubegin(9600);

    uprint("ssss");

    return 0;
}