#pragma once

#include <cstdint>
#include <string>
#include <vector>

// base typedefs
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;
typedef uintptr_t uptr;


// animation typedef
typedef double time_type;

// Rename std libs for if we want to remove them
using String = std::string;

template<class T>
using Array = std::vector<T>;


typedef u16 HasCompBitMask;
