#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <string_view>
#include <cstring>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <numeric>
#include <assert.h>
#include <chrono>
#include <math.h>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

//namespace tbb = Concurrency;

#if (defined __linux__) || (defined __APPLE__) 
#include <tbb/tbb.h>
#include "cryptopp/cryptlib.h"
#include "cryptopp/hex.h"
#include "cryptopp/crc.h"
#include "cryptopp/keccak.h"
#include "cryptopp/sha3.h"
#include "cryptopp/ripemd.h"
#include "cryptopp/md5.h"

#elif _WIN32
#include <ppl.h>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
namespace tbb = Concurrency;
#include "cryptlib.h"
#include "hex.h"
#include "crc.h"
#include "keccak.h"
#include "sha3.h"
#include "ripemd.h"
#include "md5.h"
#else
#endif

#include "version.h"
#include "utility.string.h"

#include "hasher.h"
#include "mhasher.h"
#include "hashes.h"
#include "address.h"
#include "bytes.external.h"
#include "mhasher.external.h"
#include "mhasher.external.test.h"
// TODO: reference additional headers your program requires here
