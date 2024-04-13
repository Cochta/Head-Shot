#include "network.h"

Network::Network(const ExitGames::Common::JString& appID,
                 const ExitGames::Common::JString& appVersion)
    : load_balancing_client_(*this, appID, appVersion) {}

void Network::Connect() {
  // Connect() is asynchronous - the actual result arrives in the
  // Network::connectReturn() or the Network::connectionErrorReturn() callback
  if (!load_balancing_client_.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not Connect.");
}

void Network::Service() { load_balancing_client_.service(); }

void Network::JoinRandomOrCreateRoom() noexcept {
  const auto game_id = ExitGames::Common::JString();
  const ExitGames::LoadBalancing::RoomOptions room_options(true, true, 2);
  if (!load_balancing_client_.opJoinRandomOrCreateRoom(game_id, room_options))
    EGLOG(ExitGames::Common::DebugLevel::ERRORS,
          L"Could not join or create room.");
}

void Network::Disconnect() {
  load_balancing_client_
      .disconnect();  // Disconnect() is asynchronous - the actual result
                      // arrives in the Network::disconnectReturn() callback
}

void Network::CreateRoom(const ExitGames::Common::JString& roomName,
                         nByte maxPlayers) {
  if (load_balancing_client_.opCreateRoom(
          roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {
    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void Network::JoinRandomRoom(
    ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  load_balancing_client_.opJoinRandomRoom(expectedCustomRoomProperties);

  // is_master_ = load_balancing_client_.getLocalPlayer().getIsMasterClient();
  // std::cout << "is master: " << is_master_ << '\n';
}

void Network::RaiseEvent(bool reliable, PacketType type,
                         const ExitGames::Common::Hashtable& data) noexcept {
  if (!load_balancing_client_.opRaiseEvent(reliable, data,
                                           static_cast<nByte>(type))) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not raise event.");
  }
}

void Network::ReceiveEvent(int player_nr, PacketType type,
                           const ExitGames::Common::Hashtable& data) noexcept {
  // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive
  EGLOG(ExitGames::Common::DebugLevel::ALL,
        L"an event of type %d from player Nr %d with the following content has "
        L"just arrived: %ls",
        static_cast<nByte>(type), player_nr, data.toString(true).cstr());

  std::cout << "event content: " << data.toString().UTF8Representation().cstr()
            << '\n';

  switch (type) {
    case PacketType::kInput:
      break;
    case PacketType::kLeft:
      break;
    case PacketType::kRight:
      break;
    case PacketType::kJump:
      break;
    case PacketType::kShoot:
      break;
    default:
      break;
  }
}

void Network::debugReturn(int debugLevel,
                          const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void Network::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
}

void Network::clientErrorReturn(int errorCode) {
  std::cout << "client error code : " << errorCode << '\n';
}

void Network::warningReturn(int warningCode) {
  std::cout << "client warning code : " << warningCode << '\n';
}

void Network::serverErrorReturn(int errorCode) {
  std::cout << "server error code : " << errorCode << '\n';
}

void Network::joinRoomEventAction(
    int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) {
  std::cout << "Room state: player nr: " << playerNr
            << " player nrs size: " << playernrs.getSize() << " player userID: "
            << player.getUserID().UTF8Representation().cstr() << '\n';
}

void Network::leaveRoomEventAction(int playerNr, bool isInactive) {
  std::cout << "player nr: " << playerNr << " is inactive: " << isInactive
            << '\n';
}

void Network::customEventAction(int playerNr, nByte eventCode,
                                const ExitGames::Common::Object& eventContent) {
  if (eventContent.getType() != ExitGames::Common::TypeCode::HASHTABLE) {
    std::cerr << "Unsupported event content type \n";
    return;
  }

  const ExitGames::Common::Hashtable& data =
      ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent)
          .getDataCopy();
  ReceiveEvent(playerNr, static_cast<PacketType>(eventCode), data);
}

void Network::connectReturn(int errorCode,
                            const ExitGames::Common::JString& errorString,
                            const ExitGames::Common::JString& region,
                            const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';
}

void Network::disconnectReturn() { std::cout << "client disconnected\n"; }

void Network::leaveRoomReturn(int errorCode,
                              const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

void Network::joinRandomOrCreateRoomReturn(
    int i, const ExitGames::Common::Hashtable& hashtable,
    const ExitGames::Common::Hashtable& hashtable1, int i1,
    const ExitGames::Common::JString& string) {
  ExitGames::LoadBalancing::Listener::joinRandomOrCreateRoomReturn(
      i, hashtable, hashtable1, i1, string);
  std::cout << "Joined or created a room\n";
}