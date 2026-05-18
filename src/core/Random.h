#ifndef RR99_CORE_RANDOM_H
#define RR99_CORE_RANDOM_H

#include <cstdint>

namespace rr99 {

class Random {
public:
    static void seed(uint64_t seed);
    static uint64_t next();
    static uint32_t next32();
    static float nextFloat();
    static int32_t nextInt(int32_t min, int32_t max);

private:
    static uint64_t s[4];
    static uint64_t rotl(const uint64_t x, int k);
};

} // namespace rr99

#endif // RR99_CORE_RANDOM_H
