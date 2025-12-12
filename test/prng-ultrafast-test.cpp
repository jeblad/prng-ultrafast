/*
  Testing Procedures for Ultra Fast Random Number Generator
*/

#include "prng-ultrafast.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

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

TEST_CASE("testing the random generator with 8 bit and default constructor") {
    prng::UltraFast<uint8_t> rand{};
    CHECK(rand.draw() == 124);
    CHECK(rand.draw() == 240);
    CHECK(rand.draw() == 223);
    CHECK(rand.draw() == 175);
}

TEST_CASE("testing the random generator with 8 bit") {
    prng::UltraFast<uint8_t> rand{1_U8};
    CHECK(rand.draw() == 127);
    CHECK(rand.draw() == 244);
    CHECK(rand.draw() == 198);
    CHECK(rand.draw() == 158);
}

TEST_CASE("testing the random generator with 16 bit") {
    prng::UltraFast<uint16_t> rand{1_U16};
    CHECK(rand.draw() == 63743);
    CHECK(rand.draw() == 436);
    CHECK(rand.draw() == 4326);
    CHECK(rand.draw() == 8462);
}

TEST_CASE("testing the random generator with 32 bit") {
    prng::UltraFast<uint32_t> rand{1_U32};
    CHECK(rand.draw() == 1989933311);
    CHECK(rand.draw() == 3475407284);
    CHECK(rand.draw() == 1369198822);
    CHECK(rand.draw() == 3148497166);
}

TEST_CASE("testing the random generator with explicit state") {
    prng::UltraFast<uint8_t, 239U, 241U, 251U> rand{1_U8};
    CHECK(rand.draw() == 228);
    CHECK(rand.draw() == 145);
    CHECK(rand.draw() == 219);
    CHECK(rand.draw() == 92);
}

// this should not be equal to the previous, due to the additional entropy
TEST_CASE("testing the random generator with additional entropy") {
    prng::UltraFast<uint8_t, 239U, 241U, 251U> rand{1_U8};
    rand.addEntropy(251U, 241U, 239U);
    CHECK(rand.draw() == 23);
    CHECK(rand.draw() == 53);
    CHECK(rand.draw() == 115);
    CHECK(rand.draw() == 234);
}

TEST_CASE("testing the random generator with newly seeded state") {
    prng::UltraFast<uint8_t, 241U, 239U, 251U> randA{1_U8};
    randA.seed(2, 239U, 251U, 241U);
    prng::UltraFast<uint8_t, 251U, 241U, 239U> randB{1_U8};
    randB.seed(2, 239U, 251U, 241U);
    CHECK(randA.draw() == randB.draw());
    CHECK(randA.draw() == randB.draw());
    CHECK(randA.draw() == randB.draw());
    CHECK(randA.draw() == randB.draw());
}
