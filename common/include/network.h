#pragma once

#include <Common-cpp/inc/Logger.h>
#include <LoadBalancing-cpp/inc/Client.h>

#include "rollback.h"
#include "Renderer.h"
#include "packet.h"

class Network final : public ExitGames::LoadBalancing::Listener {
 public:
  // network funcs
  Network(const ExitGames::Common::JString& appID,
          const ExitGames::Common::JString& appVersion, Game* game,
          Renderer* renderer, Rollback* rollback);
  void Connect();
  void Disconnect();
  void Service();

  bool IsConnected() const noexcept { return is_connected_; };

  void JoinRandomOrCreateRoom() noexcept;
  void RaiseEvent(bool reliable, PacketType type,
                  const ExitGames::Common::Hashtable& event_data) noexcept;
  void ReceiveEvent(int player_nr, PacketType type,
                    const ExitGames::Common::Hashtable& event_content) noexcept;

  // listener funcs

  void debugReturn(int debugLevel,
                   const ExitGames::Common::JString& string) override;

  void connectionErrorReturn(int errorCode) override;
  void clientErrorReturn(int errorCode) override;
  void warningReturn(int warningCode) override;
  void serverErrorReturn(int errorCode) override;

  void joinRoomEventAction(
      int playerNr, const ExitGames::Common::JVector<int>& playernrs,
      const ExitGames::LoadBalancing::Player& player) override;

  void leaveRoomEventAction(int playerNr, bool isInactive) override;

  void customEventAction(
      int playerNr, nByte eventCode,
      const ExitGames::Common::Object& eventContent) override;

  void connectReturn(int errorCode,
                     const ExitGames::Common::JString& errorString,
                     const ExitGames::Common::JString& region,
                     const ExitGames::Common::JString& cluster) override;

  void disconnectReturn() override;

  void leaveRoomReturn(int errorCode,
                       const ExitGames::Common::JString& errorString) override;
  void joinRandomOrCreateRoomReturn(int, const ExitGames::Common::Hashtable&,
                                    const ExitGames::Common::Hashtable&, int,
                                    const ExitGames::Common::JString&) override;

 private:
  bool is_connected_ = false;
  ExitGames::LoadBalancing::Client load_balancing_client_;
  ExitGames::Common::Logger
      mLogger;  // name must be mLogger because it is accessed by EGLOG()
  Game* game_;
  Renderer* renderer_;
  Rollback* rollback_;
};