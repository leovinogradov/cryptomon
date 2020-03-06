// EOS Blox - Pseudo-Random Number Generator
// https://github.com/EOSBlox/random
#ifndef EOS_BLOX_RANDOM_H
#define EOS_BLOX_RANDOM_H

#include <cassert>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <utility>
#include <vector>

#ifndef NO_EOSIO
#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>
#include <eosio/system.hpp>
#include <eosio/ignore.hpp>
//#define EOSIO_ASSERT(expr) eosio_assert(expr, "Failed: " # expr);
//#else
//#define EOSIO_ASSERT(expr) assert(expr);
#endif

namespace eosblox {

/// Pseudo-random number generator.
/** The 64-bit internal seed is expanded using splitmix64 [0] into a 128-bit state, which the
    xoroshiro128+ [1] generator is continually applied to for retrieving the next pseudo-random
    number in the sequence.

    If further values are given to accumulate the seed, splitmix64 is applied together with the next
    prime in an internal sequence.

    Operations for generating randomness:
    * \p next()
    * \p nextDouble()
    * \p nextInRange()
    * \p nextSample()
    * \p shuffle()
    * \p sample()

    [0]: https://dl.acm.org/citation.cfm?doid=2714064.2660195
    [1]: http://xoshiro.di.unimi.it
    */
class Random {
public:
#ifndef NO_EOSIO
  /// Initialize generator with a seed derived from the Transactions as Proof-of-Stake (TAPOS)
  /// block number and prefix.
  /** Notice that this is only to give _something_ as seed that changes depending on configuration.
      It is still _recommended_ to seed the generator with proper randomness of some kind. */
  Random()
  {
    accumSeedRange({std::abs(eosio::tapos_block_num()), std::abs(eosio::tapos_block_prefix())});
  }
#endif

  Random(const uint64_t seed) : seed_(seed)
  {
  }

  uint64_t seed() const
  {
    return seed_;
  }

  /// Accumulate seed with \p extra.
  void accumSeed(const uint64_t extra)
  {
    seed_ = splitmix64(seed_ ^ extra * nextPrime());
  }

#ifndef NO_EOSIO
/*
  void accumSeed(const eosio::checksum160 &extra)
  {
    accumSeedArray(extra.hash);
  }

  void accumSeed(const capi_checksum256 &extra)
  {
    accumSeedArray(extra.hash);
  }

  void accumSeed(const capi_checksum512 &extra)
  {
    accumSeedArray(extra.hash);
  }

  void accumSeed(const capi_signature &extra)
  {
    accumSeedArray(extra.data);
  }

  void accumSeed(const capi_public_key &extra)
  {
    accumSeedArray(extra.data);
  }
*/
#endif

  template <typename T>
  void accumSeedRange(std::initializer_list<T> &&extra)
  {
    accumSeedRange(extra);
  }

  template <typename Range>
  void accumSeedRange(Range &&extra)
  {
    for (auto it = extra.begin(); it != extra.end(); ++it) {
      accumSeed(*it);
    }
  }

  template <typename T, std::size_t N>
  void accumSeedArray(const T (&extra)[N])
  {
    for (int i = 0; i < N; ++i) {
      accumSeed(extra[i]);
    }
  }

  /// Next pseudo-random number.
  /** next() is based upon xoroshiro128+ at http://xoshiro.di.unimi.it */
  uint64_t next()
  {
    // If state is {0, 0} then use splitmix64 to expand 64-bit seed into the 128-bit state.
    if (state[0] == 0 && state[1] == 0) {
      state[0] = splitmix64(seed_);
      state[1] = splitmix64(state[0]);
    }

    const auto s0 = state[0];
    auto s1 = state[1];
    const auto res = s0 + s1;
    s1 ^= s0;
    state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    state[1] = rotl(s1, 37);
    return res;
  }

  /// Next double in [0, 1[
  double nextDouble()
  {
    constexpr auto max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(next()) / max;
  }

  /// Next number in [min, max[
  uint64_t nextInRange(const uint64_t min, const uint64_t max)
  {
    eosio::check(min < max, "error");
    if (min >= max) {
      return min;
    }
    return static_cast<uint64_t>(static_cast<double>(max - min) * nextDouble()) + min;
  }

  /// Next random sample from \p population.
  template <typename Container>
  auto nextSample(const Container &population)
  {
    const auto size = population.size();
    eosio::check(size > 0, "error");
    if (size <= 0) {
      return typename Container::value_type();
    }
    return population[nextInRange(0, size)];
  }

  /// Shuffles every element of container \p data around once.
  template <typename Container>
  void shuffle(Container &data)
  {
    const auto size = data.size();
    for (auto it = data.begin(); it != data.end(); ++it) {
      std::swap(*it, *(data.begin() + (next() % size)));
    }
  }

  /// Sample \p n values from \p population.
  template <typename Container>
  Container sample(const int n, const Container &population)
  {
    eosio::check(n > 0, "error");

    const auto size = population.size();
    EOSIO_ASSERT(size > 0);

    if (n <= 0 || size <= 0) {
      return {};
    }

    Container res;
    for (int i = 0; i < n; ++i) {
      res.push_back(nextSample(population));
    }
    return res;
  }

private:
  uint64_t nextPrime()
  {
    return primes[primeIndex++ % primes.size()];
  }

  inline uint64_t rotl(const uint64_t x, int k)
  {
    return (x << k) | (x >> (64 - k));
  }

  inline uint64_t splitmix64(const uint64_t input)
  {
    auto z = (input + uint64_t(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * uint64_t(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * uint64_t(0x94D049BB133111EB);
    return z ^ (z >> 31);
  }

  uint64_t seed_ = 1;
  uint64_t state[2] = {0, 0};
  int primeIndex = 0;
  const std::vector<uint64_t> primes = {
    7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333,
    7349, 7351, 7369, 7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499,
    7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591,
    7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723,
    7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841, 7853, 7867, 7873, 7877,
    7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017,
    8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111};
};

} // eosblox

#endif // EOS_BLOX_RANDOM_H
