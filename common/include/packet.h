#pragma once

#include <Common-cpp/inc/defines.h>

enum class PacketType : nByte { kInput = 0, kLeft, kRight, kJump, kShoot };

enum class PacketKey : nByte { kInput = 0, kLeft, kRight, kJump, kShoot };