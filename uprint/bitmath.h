#ifndef BITMATH_H_INCLUDED
#define BITMATH_H_INCLUDED

#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

// ****** DIV && REM 10 ******

/**
 * @defgroup dec_div_mod Функции-операторы (десятичные).
 * @brief Делят число на 10 или находят остаток от деления числа на 10.
 * @{
 */

/**
 * @brief Делит число на 10.
 * @param num Число 2 байта.
 * @return uint16_t Число поделенное на 10.
 */
static inline uint16_t divu10(uint16_t num) {
    uint16_t quot, rem;

    quot = (num >> 1) + (num >> 2);
    quot += (quot >> 4);
    quot += (quot >> 8);
    quot >>= 3;
    rem = num - (((quot << 2) + quot) << 1);

    return quot + (rem > 9);
}

/**
 * @brief Находит остаток от деления числа на 10.
 * @param num Число 2 байта.
 * @return uint16_t Остаток от деления числа на 10.
 */
static inline uint16_t modu10(uint16_t num) {
    uint16_t quot, rem;

    quot = (num >> 1) + (num >> 2);
    quot += (quot >> 4);
    quot += (quot >> 8);
    quot >>= 3;
    rem = num - (((quot << 2) + quot) << 1);

    return rem > 9 ? rem - 10 : rem;
}

/**
 * @brief Делит число на 10 в степени n.
 * @param num Число 2 байта.
 * @param deg Степень десяти (n).
 * @warning Максимальная степень (n) ограничена 4.
 * @return uint16_t Число поделенное на 10 в степени n.
 */
static inline uint16_t divu10d(uint16_t num, uint8_t deg) {
    deg = deg > 4 ? 4 : deg;

    for (uint8_t i = 0; i < deg; i++) {
        num = divu10(num);
    }
    return num;
}

/**
 * @brief Находит остаток от деления числа на 10 в степени n.
 * @param num Число 2 байта.
 * @param deg Степень десяти (n).
 * @warning Максимальная степень (n) ограничена 4.
 * @return uint16_t Остаток от деления числа на 10 в степени n.
 */
static inline uint16_t modu10d(uint16_t num, uint8_t deg) {
    uint16_t rem = 0;
    uint16_t mul = 1;

    deg = deg > 4 ? 4 : deg;

    for (uint8_t i = 0; i < deg; i++) {
        rem += modu10(num) * mul;
        mul *= 10;
        num = divu10(num);
    }
    return rem;
}
/** @} dec_div_mod */

// ****** DIV && REM 2 ******

/**
 * @defgroup bin_div_mod Функции-операторы (двоичные).
 * @brief Делят число на 2 или находят остаток от деления числа на 2.
 * @{
 */

/**
 * @brief Делит число на 2.
 * @param num Число 2 байта.
 * @return uint16_t Число поделенное на 2.
 */
static inline uint16_t divu2(uint16_t num) {
    return num >> 1;
}

/**
 * @brief Находит остаток от деления числа на 2.
 * @param num Число 2 байта.
 * @return uint16_t Остаток от деления числа на 2.
 */
static inline uint16_t modu2(uint16_t num) {
    return num & 0x01;
}

/**
 * @brief Делит число на 2 в степени n.
 * @param num Число 2 байта.
 * @param deg Степень десяти (n).
 * @warning Максимальная степень (n) ограничена 16.
 * @return uint16_t Число поделенное на 2 в степени n.
 */
static inline uint16_t divu2d(uint16_t num, uint8_t deg) {
    deg = deg > 16 ? 16 : deg;
    return num >> deg;
}

/**
 * @brief Находит остаток от деления числа на 2 в степени n.
 * @param num Число 2 байта.
 * @param deg Степень десяти (n).
 * @warning Максимальная степень (n) ограничена 16.
 * @return uint16_t Остаток от деления числа на 2 в степени n.
 */
static inline uint16_t modu2d(uint16_t num, uint8_t deg) {
    uint16_t mask = 0x00;

    deg = deg > 16 ? 16 : deg;

    if (deg == 1)       mask = 0x01;
    else if (deg <= 4)  mask = 0x0f;
    else if (deg <= 8)  mask = 0xff;
    else if (deg <= 12) mask = 0x0fff;
    else if (deg <= 16) mask = 0xffff;

    return num & mask;
}
/** @} bin_div_mod */

// ****** DIV && REM 16 ******

/**
 * @defgroup hex_div_mod Функции-операторы (шестнадцатеричные).
 * @brief Делят число на 16 или находят остаток от деления числа на 16.
 * @{
 */

/**
 * @brief Делит число на 16.
 * @param num Число 2 байта.
 * @return uint16_t Число поделенное на 16.
 */
static inline uint16_t divu16(uint16_t num) {
    return num >> 4;
}

/**
 * @brief Находит остаток от деления числа на 16.
 * @param num Число 2 байта.
 * @return uint16_t Остаток от деления числа на 16.
 */
static inline uint16_t modu16(uint16_t num) {
    return num & 0x0f;
}

/**
 * @brief Делит число на 16 в степени n.
 * @param num Число 2 байта.
 * @param deg Степень десяти (n).
 * @warning Максимальная степень (n) ограничена 4.
 * @return uint16_t Число поделенное на 16 в степени n.
 */
static inline uint16_t divu16d(uint16_t num, uint8_t deg) {
    deg = deg > 4 ? 4 : deg;
    return num >> (4 * deg);
}

/**
 * @brief Находит остаток от деления числа на 16 в степени n.
 * @param num Число 2 байта.
 * @param deg Степень десяти (n).
 * @warning Максимальная степень (n) ограничена 4.
 * @return uint16_t Остаток от деления числа на 16 в степени n.
 */
static inline uint16_t modu16d(uint16_t num, uint8_t deg) {
    uint16_t mask = 0x00;

    deg = deg > 4 ? 4 : deg;

    switch (deg) {
    case 1: mask = 0x0f;   break;
    case 2: mask = 0xff;   break;
    case 3: mask = 0x0fff; break;
    case 4: mask = 0xffff; break;
    }

    return num & mask;
}
/** @} hex_div_mod */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BITMATH_H_INCLUDED