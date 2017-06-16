//
//  Room.h
//  demo_svr
//
//  Created by Wayne on 7/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__Room__
#define __demo_svr__Room__

#include "Player.h"
#include "Game.h"

#include <memory>

using namespace vn;

class RoomManager;

class Room : public AsyncObject {
    friend class RoomManager;
public:
    enum State {
        kNotInit = 0,
        kWaiting,
        kFighting,
    };
    
    virtual ~Room();

    virtual void async(std::function<void()> fn);
    
    const str8 & name() { return m_name; }
    
    void async_initDemo(PlayerPtr owner, PublicLinkPtr link);
    void async_endDemoGame();
    
    void async_relogin(PlayerPtr player, PublicLinkPtr link);
	void async_exit(size_t seat);
    void async_disconnected(size_t seat);
    
    void async_startGame();
    void async_moveTank(size_t seat, u32 direction);
    void async_stopTank(size_t seat);
    void async_fireCommonAttack(size_t seat, u32 direction);
	void async_firePowerAttack(size_t seat, u32 direction);
    void async_backGame(PlayerPtr player, PublicLinkPtr link);
    
    void _game_broadcast(const std::vector<PacketPtr> &packets);
private:
    Room(const str8 &_name);
    void _update(f32 deltaTime);
	void _enter(PlayerPtr player, PublicLinkPtr link);
    void _destroy();
    
    void _endDemoGame();
    
    str8 m_name;
    
    State m_state = kNotInit;
	
	struct Info {
		PlayerPtr player;
		PublicLinkPtr link;
        GamePlayerPtr tank;
	};
	
    std::array<Info, 4> m_players;
    size_t m_player_count = 0;
    
    std::unique_ptr<Game> m_game;
};

typedef RefCountedPtr<Room> RoomPtr;

#endif /* defined(__demo_svr__Room__) */
