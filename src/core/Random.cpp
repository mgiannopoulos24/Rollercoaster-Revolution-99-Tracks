#include "Random.h"
#include <algorithm>

namespace rr99 {

uint64_t Random::s[4] = {0, 0, 0, 0};

uint64_t Random::rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

void Random::seed(uint64_t seed) {
    // Simple SplitMix64 to seed Xoshiro256**
    auto splitmix64 = [](uint64_t& state) -> uint64_t {
        uint64_t z = (state += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    };

    uint64_t state = seed;
    s[0] = splitmix64(state);
    s[1] = splitmix64(state);
    s[2] = splitmix64(state);
    s[3] = splitmix64(state);
}

uint64_t Random::next() {
    const uint64_t result = rotl(s[1] * 5, 7) * 9;

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;
    s[3] = rotl(s[3], 45);

    return result;
}

uint32_t Random::next32() {
    return static_cast<uint32_t>(next() >> 32);
}

float Random::nextFloat() {
    return static_cast<float>(next32()) / static_cast<float>(0xFFFFFFFF);
}

int32_t Random::nextInt(int32_t min, int32_t max) {
    if (min >= max) return min;
    return min + static_cast<int32_t>(next() % static_cast<uint64_t>(max - min + 1));
}

} // namespace rr99
