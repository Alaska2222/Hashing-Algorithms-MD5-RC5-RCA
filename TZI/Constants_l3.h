#pragma once
#include <cmath>
#include <cstdint>

const int w = 32;  // Word size in bits 
const int r = 8;   // Number of rounds
const int b = 32;  // The number of 8-bit bytes (octets) in the secret key K
const uint32_t P = 0xB7E15163;
const uint32_t Q = 0x9E3779B9;
const long long module = pow(2, w);
const uint32_t size_ = w * 2 / 8;
const int bSize = w * 2;

