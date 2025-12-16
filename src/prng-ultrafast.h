/*

    Ultra Fast Random Number Generator for 8, 16, and 32-Bit Devices

    From https://www.electro-tech-online.com/threads/ultra-fast-pseudorandom-number-generator-for-8-bit.124249/
    With additional edits by jeblad@gmail.com

    This is a small PRNG, experimentally verified to have at least a 50 million byte period for 8 bits
    by generating 50 million bytes and observing that there were no overlapping sequences and repeats.
    This generator passes serial correlation, entropy, Monte Carlo Pi value, arithmetic mean,
    And many other statistical tests. This generator may have a period of up to 2^32 with 8 bits, but
    this has not been verified.

    This generator is not suitable for cryptography due to its short period (by  cryptographic standards)
    and simple construction. No attempt was made to make this generator suitable for cryptographic use.

    Due to the use of a constant counter, the generator should be resistant to latching up.
    A significant performance gain is had in that the x variable is only ever incremented.

    Only 4 bytes of ram (in 8 bit mode) are needed for the internal state, and generating a result
    requires 3 XORs, 2 ADDs, one bit shift right, and one increment. Difficult or slow operations like
    multiply, etc were avoided for maximum speed on ultra low power devices.

    It is most useful when you need a pseudo random sequence that can be repeated for subsequent runs,
    and when the rather limited period is not an issue.

    Copyright (c) 2025 John Erling Blad <jeblad@gmail.com> with additional (c) as appropriate.

    Distributed under the GNU General Public License v3.
    See accompanying file LICENSE or the page at
    https://www.gnu.org/licenses/

    The documentation can be found at the library's page:
    https://github.com/jeblad/hash-fnv1a/blob/main/README.md

*/

#pragma once

#include <concepts>

namespace prng{

    template<std::unsigned_integral T,
            std::unsigned_integral auto v1=static_cast<T>(4294967291U), // prime less than largest UInt32 value
            std::unsigned_integral auto v2=static_cast<T>(3999999919U), // prime
            std::unsigned_integral auto v3=static_cast<T>(4100000231U)> // prime
    class UltraFast {
        private:
        T x{0}, a{static_cast<T>(v3)}, b{static_cast<T>(v2)}, c{static_cast<T>(v1)};

        public:

        UltraFast() {}

        UltraFast(unsigned long long val) {
            seed(val);
        }

        void seed(unsigned long long val)
        {
            x = 0;
            a = static_cast<T>(val % v3);
            b = static_cast<T>(val % v2);
            c = static_cast<T>(val % v1);
        }

        void addEntropy(unsigned long long val)
        {
            a ^= static_cast<T>(val % v3);
            b ^= static_cast<T>(val % v2);
            c ^= static_cast<T>(val % v1);
        }

        T draw()
        {
            x++;                 //x is incremented every round and is not affected by any other variable

            a = (a^c^x);         //note the mix of addition and XOR
            b = (b+a);           //and the use of very few instructions
            c = ((c+(b>>1))^a);  //the right shift is to ensure that high-order bits from b can affect  
                                 //low order bits of other variables
            return(c);
        }
    };

}
