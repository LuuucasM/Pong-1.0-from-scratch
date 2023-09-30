#pragma once

#include <cstdint>
#include <bitset>
#include <cassert>

using EntityID = std::uint64_t;
using ComponentType = std::uint64_t;

const uint64_t MAX_ENTITIES = 10;
const uint64_t MAX_COMPONENTS = 30;

using ComponentMask = std::bitset<MAX_COMPONENTS>;

typedef void (*CollisionFunc)(EntityID, EntityID);