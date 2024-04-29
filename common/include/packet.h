#pragma once

#include <Common-cpp/inc/Containers/Hashtable.h>

enum class PacketType : nByte { kInput = 0, kFrame, kFrameConfirmation };

enum class PacketKey : nByte { kInput = 0, kFrame, kChecksum };

struct Packet {
  PacketType type{};
  ExitGames::Common::Hashtable data{};
};