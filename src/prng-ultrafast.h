/*

  Ultra Fast Random Number Generator for 8, 16, and 32-Bit Devices

  From https://www.electro-tech-online.com/threads/ultra-fast-pseudorandom-number-generator-for-8-bit.124249/
  With additional edits by jeblad@gmail.com

  This is a small PRNG, experimentally verified to have at least a 50 million byte period for 8 bits
  by generating 50 million bytes and observing that there were no overlapping sequences and repeats.
  This generator passes serial correlation, entropy, Monte Carlo Pi value, arithmetic mean,
  And many other statistical tests. This generator may have a period of up to 2^32, but this has
  not been verified.

  This generator is not suitable for cryptography due to its short period (by  cryptographic standards)
  and simple construction. No attempt was made to make this generator suitable for cryptographic use.

  Due to the use of a constant counter, the generator should be resistant to latching up.
  A significant performance gain is had in that the x variable is only ever incremented.

  Only 4 bytes of ram (in 8 bit mode) are needed for the internal state, and generating a result
  requires 3 XORs, 2 ADDs, one bit shift right, and one increment. Difficult or slow operations like
  multiply, etc were avoided for maximum speed on ultra low power devices.

*/

#pragma once

#include <concepts>
#include <optional>

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

        UltraFast(T seed)
        {
            x = seed;
        }

        UltraFast(unsigned long long seed) {
            x = static_cast<T>(seed);
            a = static_cast<T>(seed % v3);
            b = static_cast<T>(seed % v2);
            c = static_cast<T>(seed % v1);
        }

        void seed(T seed,
                std::optional<T> p1 = std::nullopt,
                std::optional<T> p2 = std::nullopt,
                std::optional<T> p3 = std::nullopt)
        {
            x = seed;
            if ( p3.has_value()) {
                a = static_cast<T>(p3.value());
            }
            if ( p2.has_value()) {
                b = static_cast<T>(p2.value());
            }
            if ( p1.has_value()) {
                c = static_cast<T>(p1.value());
            }
        }

        void addEntropy(std::optional<T> p1 = std::nullopt,
                std::optional<T> p2 = std::nullopt,
                std::optional<T> p3 = std::nullopt)
        {
            if ( p3.has_value()) {
                a ^= static_cast<T>(p3.value());
            }
            if ( p2.has_value()) {
                b ^= static_cast<T>(p2.value());
            }
            if ( p1.has_value()) {
                c ^= static_cast<T>(p1.value());
            }
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
