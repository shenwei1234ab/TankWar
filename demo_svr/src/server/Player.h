//
//  Player.h
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__Player__
#define __demo_svr__Player__

#include "DBManager.h"

#include "PublicLink.h"

class Player : public AsyncObject {
public:
    Player(u32 pid) : m_playerId(pid) {}
    
    virtual void async(std::function<void()>);
    
    u32 playerId() const { return m_playerId; }
    const str8 & playerName() const { return m_playerName; }
    
    void async_loginNew(RefCountedPtr<PublicLink> link);
    void async_login(RefCountedPtr<PublicLink> link);
    void async_disconnected();
    void async_setName(str8 name);
    void async_sendPlayerBaseInfo();
    void async_createDemoRoom();
	void async_enterRoom(str8 roomName);
    void async_backGame();
	
	void async_notify_enterRoom(RefCountedPtr<Room> room);
	void async_notify_exitRoom();
	void async_notify_destroyRoom();
	void async_notify_destroyRoomNoMsg();
    void async_notify_reloginRoom(bool ok);
protected:
    u32 m_playerId;
    str8 m_playerName;
    bool m_initialized = false;
    bool m_isInRoom = false;
    
    PublicLinkPtr m_link;
    
    RefCountedPtr<Room> m_room;
};

typedef RefCountedPtr<Player> PlayerPtr;

#endif /* defined(__demo_svr__Player__) */
