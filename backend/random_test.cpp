uint32_t shuffle_bit(uint32_t v, uint32_t n);
uint32_t calculate_trx_hash(char* buf, int size);
uint32_t calculate_trx_hash2();
uint32_t calculate_trx_hash3(uint32_t data);
// get_key2()
// uint32_t tapos_block_prefix();

uint32_t last_checksum; // = 0 initally
uint32_t last_hash; // = 0 initially

random_val begin_random(const playerv2 &value) {
    uint32_t seed = value.seed;
    if (seed == 0) {
        seed = seed_identity(value.owner);
    }

    seed ^= get_key2(value.owner);
    auto hash2 = calculate_trx_hash2();
    int strength1 = (last_checksum % 7) + (last_hash % 11) + (hash2 % 13);
    // int strength2 = (last_checksum % 11) + (last_trx_hash % 13) + (hash2 % 7);
    seed = shuffle_bit(seed, strength1);
    seed ^= shuffle_bit(last_hash, strength2);
    // seed ^= tapos_block_prefix();
    seed ^= hash2;

    // Or, return seed to get an integer
    auto rval = random_val(seed, 0);
    return rval;
}

void end_random(playerv2 &value, const random_val &val) {
    uint32_t seed = val.seed ^ get_key2(value.owner);
    value.seed = seed;
}

void checksum_gateway(name from, uint32_t block, uint32_t checksum) {
    last_checksum = checksum;
    if (checksum & 0x80000000) {
        test_checksum_v2(from, block, checksum);
    } else {
        test_checksum_v3(from, block, checksum);
    }
}

void test_checksum_v2(name from, uint32_t block, uint32_t checksum) {
    int32_t num = time_util::now_shifted();
    int32_t v0 = block ^ get_checksum_key(from);
    int32_t hash = calculate_trx_hash3(v0) | 0x80000000;

    assert_true((num + 60) > v0, "check your system time. it's too fast. (checksum failure)");
    assert_true((num - v0) < 90, "check your system time it's too slow. (checksum failure)");
    assert_true(hash == checksum, "invalid checksum");

    // save_checksum(from, block, checksum, v0);
}

void test_checksum_v3(name from, uint32_t block, uint32_t checksum) {
    int32_t num = time_util::now_shifted();
    int32_t v0 = block ^ get_checksum_key(from);
    int32_t hash = calculate_trx_hash3(block) & 0x7FFFFFFF;

    assert_true((num + 60) > v0, "check your system time. it's too fast. (checksum failure)");
    assert_true((num - v0) < 90, "check your system time it's too slow. (checksum failure)");
    assert_true(hash == checksum, "invalid checksum");

    // save_checksum(from, block, checksum, v0);
}
