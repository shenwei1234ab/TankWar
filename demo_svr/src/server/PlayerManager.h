//
//  PlayerManager.h
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__PlayerManager__
#define __demo_svr__PlayerManager__

#include "Player.h"

#include "vnSingleton.h"
#include "vnRunLoop.h"
#include "vnSpinLock.h"
#include <map>

using namespace vn;
class PlayerManager : public Singleton<PlayerManager> {
public:
    PlayerManager();
  
	~PlayerManager();
    
	void post(std::function<void()> fn);
 
    PlayerPtr get(u32 playerId);
    
	void remove(u32 playerId);

private:
 
	RunLoop m_loop;
    
	std::map<u32, PlayerPtr> m_players;
    
	SpinLock m_lock;
    
    std::thread m_thread;
};

#endif /* defined(__demo_svr__PlayerManager__) */
