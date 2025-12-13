/*
  Testing Procedures for Ultra Fast Random Number Generator
*/

#include "prng-ultrafast.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
/*
constexpr uint8_t operator""_U8(unsigned long long int v)
{
    return static_cast<uint8_t>(v);
}

constexpr uint16_t operator""_U16(unsigned long long int v)
{
    return static_cast<uint16_t>(v);
}

constexpr uint32_t operator""_U32(unsigned long long int v)
{
    return static_cast<uint32_t>(v);
}
*/

#define PRIME1 18446744073709551557ULL
#define PRIME2 12345678910987654321ULL


TEST_CASE("testing the random generator with 8 bit and default constructor") {
    prng::UltraFast<uint8_t> rand{};
    CHECK(rand.draw() == 124);
    CHECK(rand.draw() == 240);
    CHECK(rand.draw() == 223);
    CHECK(rand.draw() == 175);
}

TEST_CASE("testing the random generator with 8 bit") {
    prng::UltraFast<uint8_t> rand{PRIME1};
    CHECK(rand.draw() == 144);
    CHECK(rand.draw() == 229);
    CHECK(rand.draw() ==  53);
    CHECK(rand.draw() == 205);
}

TEST_CASE("testing the random generator with 16 bit") {
    prng::UltraFast<uint16_t> rand{PRIME1};
    CHECK(rand.draw() == 3292);
    CHECK(rand.draw() == 4833);
    CHECK(rand.draw() == 11571);
    CHECK(rand.draw() == 21547);
}

TEST_CASE("testing the random generator with 32 bit") {
    prng::UltraFast<uint32_t> rand{PRIME1};
    CHECK(rand.draw() == 2482706013);
    CHECK(rand.draw() == 1146759350);
    CHECK(rand.draw() == 3421366543);
    CHECK(rand.draw() == 3893964988);
}

TEST_CASE("testing the random generator with explicit state") {
    prng::UltraFast<uint8_t, 239U, 241U, 251U> rand{PRIME1};
    CHECK(rand.draw() == 134);
    CHECK(rand.draw() ==  63);
    CHECK(rand.draw() == 112);
    CHECK(rand.draw() ==  11);
}

// this should not be equal to the previous, due to the additional entropy
TEST_CASE("testing the random generator with additional entropy") {
    prng::UltraFast<uint8_t, 239U, 241U, 251U> rand{PRIME1};
    rand.addEntropy(PRIME2);
    CHECK(rand.draw() ==  82);
    CHECK(rand.draw() == 215);
    CHECK(rand.draw() == 120);
    CHECK(rand.draw() == 210);
}

TEST_CASE("testing the random generator with newly seeded state") {
    prng::UltraFast<uint8_t, 241U, 239U, 251U> randA{};
    randA.seed(PRIME2);
    prng::UltraFast<uint8_t, 241U, 239U, 251U> randB{PRIME1};
    randB.seed(PRIME2);
    CHECK(randA.draw() == randB.draw());
    CHECK(randA.draw() == randB.draw());
    CHECK(randA.draw() == randB.draw());
    CHECK(randA.draw() == randB.draw());
}
