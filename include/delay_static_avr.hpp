/*
 * This file is part of the Peripheral Template Library project.
 *
 * Copyright (c) 2012 Paul Sokolovsky <pfalcon@users.sourceforge.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <inline.hpp>

ALWAYS_INLINE void __delay_cycles2(unsigned long delay)
{
    if (__builtin_constant_p(delay)) {
        if (delay <= 262140U) {
            uint16_t d = delay >> 2;
            asm volatile(
            "ldi    r24, %0 \n"
            "ldi    r25, %1 \n"
            "1: \n"
#ifdef AVR_HAS_SBIW
            "sbiw   r24, 1 \n" //2
#else
            "subi   r24, 1 \n" //1
            "sbci   r25, 0 \n" //1
#endif
            "brne   1b \n"
            : : "M" (d & 0xff), "M" ((d >> 8) & 0xff) : "r24", "r25"
            );
        } else {
            uint32_t d = delay / 6;
            asm volatile(
            "ldi    r24, %0 \n"
            "ldi    r25, %1 \n"
            "ldi    r26, %2 \n"
            "ldi    r27, %3 \n"
            "1: \n"
            "subi   r24, 1 \n" //1
            "sbci   r25, 0 \n" //1
            "sbci   r26, 0 \n" //1
            "sbci   r27, 0 \n" //1
            "brcc   1b \n"     //2
            : : "M" (d & 0xff), "M" ((d >> 8) & 0xff), "M" ((d >> 16) & 0xff), "M" ((d >> 24) & 0xff) : "r24", "r25", "r26", "r27"
            );
        }
    }
//            : : "M" ((uint8_t)delay), "M" ((uint8_t)(delay >> 8)) : "r24", "r25"

#if 0
    if (delay <= 262140U) {
        uint16_t d = delay >> 2;
#if 1 //working_non_optimal
        asm volatile(
            "1: \n"
            "sbiw   %0, 1 \n" //2
            "brne   1b \n"    //2
            : "=&w" (d) : "0" (d)
        );
#endif
    } else {
        uint32_t d = delay / 6;
        asm volatile(
            "1: \n"
            "subi   %A0, 1 \n" //1
            "sbci   %B0, 0 \n" //1
            "sbci   %C0, 0 \n" //1
            "sbci   %D0, 0 \n" //1
            "brcc   1b \n"     //2
            : "=&w" (d) : "0" (d)
        );
    }

#if rather_should_work
    asm volatile(
            "1: \n"
            "sbiw   %0, 1 \n"
            "brne   1b \n"
            : : "w" (d): "0"
    );
#endif


#if var_1
    asm volatile(
            "1: \n"
            "sbiw   %0, 1 \n"
            "brne   1b \n"
            : : "w" ((uint16_t)delay) : "r24", "r25", "r26", "r27", "r28", "r29"//, "r30", "r31"
    );
#endif
#endif
}


class StaticDelay
{
public:
    ALWAYS_INLINE static void delay(long cycles)
    {
        __delay_cycles2(cycles);
    }
};
