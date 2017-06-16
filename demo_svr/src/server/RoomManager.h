//
//  RoomManager.h
//  demo_svr
//
//  Created by Wayne on 7/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__RoomManager__
#define __demo_svr__RoomManager__

#include "Room.h"

#include "vnSingleton.h"
#include "vnReadWriteLock.h"
#include "vnRunLoop.h"

#include <map>
#include <set>
#include <memory>
#include <thread>

class RoomManager : public Singleton<RoomManager> {
public:
    RoomManager();
    ~RoomManager();
    
    void post(std::function<void()> fn);
    
    void destroyAll();
    
    RoomPtr find(const str8 &name);
    RoomPtr create(const str8 &name);
    void destroy(const str8 &name);
	
	void enter(RoomPtr room, PlayerPtr player, PublicLinkPtr link);
private:
    RunLoop m_loop;
    std::map<str8, Room *> m_roomIndex;
    ReadWriteLock m_roomIndexLock;
    
    std::set<RoomPtr> m_rooms;
    
    std::unique_ptr<std::thread> m_thread;
    
    void _update(f32 deltaTime);
};

#endif /* defined(__demo_svr__RoomManager__) */
