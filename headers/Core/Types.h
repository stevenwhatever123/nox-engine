#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// base typedefs
typedef uint8_t  u8;
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

// ECS typedef
typedef u16 HasCompBitMask;

// Rename std libs for if we want to remove them
using String = std::string;
template<class T> using Array = std::vector<T>;
template<class T, class U> using Map = std::map<T, U>;

// Math & GLM
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;
typedef glm::ivec3 ivec3;

// Audio
typedef u32 DspId;
typedef u32 ChannelId;
