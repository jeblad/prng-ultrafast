# prng-ultrafast

![stability-unstable](https://img.shields.io/badge/stability-experimental-orange.svg?style=for-the-badge)
![GitHub issues](https://img.shields.io/github/issues-raw/jeblad/prng-ultrafast?style=for-the-badge)

## Scope

Ultra Fast Random Number Generator for 8, 16, and 32-Bit Devices

From a post [Ultra Fast Pseudorandom number generator for 8-bit](https://www.electro-tech-online.com/threads/ultra-fast-pseudorandom-number-generator-for-8-bit.124249/) by _EternityForest_ (Dec 31, 2011) with additional edits by [jeblad](mailto:jeblad@gmail.com) (Nov 20, 2025).

This is a small [pseudorandom number generator](https://en.wikipedia.org/wiki/Pseudorandom_number_generator) (PRNG), experimentally verified to have at least a 50 million byte period for 8 bits by generating 50 million bytes and observing that there were no overlapping sequences and repeats. It passes serial correlation, entropy, Monte Carlo Pi value, arithmetic mean, and many other statistical tests. It may have a period of up to 2³², but this has not been verified.

In this specific implementation the values for the state variables are bound during instantiation of the template, thus making it possible to create repeatable sequences.

This generator is not suitable for cryptography due to its short period (by cryptographic standards) and simple construction. No attempt has been made to make this generator suitable for cryptographic use.

Due to the use of a constant counter, the generator should be resistant to latching up. A significant performance gain is had in that the x variable is only ever incremented.

Only 4 bytes of ram (in 8 bit mode) are needed for the internal state, and generating a result requires 3 XORs, 2 ADDs, one bit shift right, and one increment. Difficult or slow operations like multiply, etc., were avoided for maximum speed on ultra low power devices.

## Usage

The _prng-ultrafast_ library is in a single header file. Simply grab the file and put it wherever it is needed, or pull the repo as a submodule.

```bash
wget https://raw.githubusercontent.com/jeblad/prng-ultrafast/refs/heads/main/prng-ultrafast.h
```

or

```bash
git submodule add git@github.com:jeblad/prng-ultrafast.git path-to-submodule
```

The _path-to-submodule_ would typically be something like `include/prng-ultrafast` if you're in the project folder.

If you're adding the _prng-ultrafast_ as a submodule, then pull an updated version.

The most common use is to set the library up to generate a byte sized random value

```C++
#include "prng-ultrafast.h"
// initialized with default state values
prng::UltraFast<uint8_t> rand{};
// initialized with explicit state values
prng::UltraFast<uint8_t, 239U, 241U, 251U> rand{};
// initialized with additional entropy
prng::UltraFast<uint32_t> rand{1234567};
// set new seed
rand.seed(1234567);
// add entropy
rand.addEntropy(1234567);
// draw a new pseudo random value
int8_t result = rand.draw();
```

It might start with explicitly set state when instantiated

```C++
#include "prng-ultrafast.h"
prng::UltraFast<uint8_t, 239U, 241U, 251U> rand{};
rand.draw();
```

Note that it must be instantiated with different state values to generate unique sequences.

Or it can be given an initial seed, a new seed, or a complete state

```C++
#include "prng-ultrafast.h"
prng::UltraFast<uint8_t> rand{};
rand.draw();
rand.seed(42, 251U, 239U, 241U);
rand.draw();
```

## Development

The testing library [doctest](https://github.com/doctest/doctest) is used during development, but it isn't part of the library as such. It is although necessary for running the tests. Likewise, the implementation in `prng-ultrafast-test.cpp` isn't necessary for non-development use, it is only used during tests.

The _doctest_ library is in a single header file. Simply grab the file and put it wherever it is needed, or pull the repo as a submodule.

```bash
wget https://raw.githubusercontent.com/doctest/doctest/refs/heads/master/doctest/doctest.h
```

or

```bash
git submodule add git@github.com:doctest/doctest.git path-to-submodule
```

The _path-to-submodule_ would typically be something like `include/doctest` if you're in the project folder.

If you're adding the _doctest_ as a submodule, then pull an updated version.

The usual development cycle is to compile the test file, and then run it

```bash
g++ -Wall -Wextra -Werror -std=c++20 -I ./include/ -I ./src/ -o prng-ultrafast-test test/prng-ultrafast-test.cpp
```

Then run the executable `prng-ultrafast-test` that has the actual tests.

```bash

./prng-ultrafast-test
```

The executable can take several arguments. Check the _doctest_ manual for examples.
