//
//  DBManager.h
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__DBManager__
#define __demo_svr__DBManager__

#include "vnSingleton.h"
#include "vnWorkPool.h"

#include "AsyncObject.h"

using namespace vn;

class DBManager : public Singleton<DBManager> 
{
public:
    
    struct PlayerInfo {
        str8 playerName;
        PlayerInfo() = default;
        PlayerInfo(const PlayerInfo &) = default;
        PlayerInfo(PlayerInfo &&rhs): playerName(std::move(rhs.playerName)) {}
    };
    
    bool startup(size_t threads, const c8 *server, const c8 *user, const c8 *passwd);
    void cleanup();
    
    void getPlayerId(str8 ppId, AsyncObjectPtr obj, std::function<void(bool _new, u32 playerId)> fn);
    void loadPlayerInfo(u32 playerId, AsyncObjectPtr obj, std::function<void(bool result, PlayerInfo &info)> fn);
    void setPlayerName(u32 playerId, str8 name, AsyncObjectPtr obj, std::function<void(bool result, str8 &name)> fn);
private:
    WorkPool m_pool;
};


#endif /* defined(__demo_svr__DBManager__) */
