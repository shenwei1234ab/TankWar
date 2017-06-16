//
//  PlayerManager.cpp
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "PlayerManager.h"
#include "Player.h"
#include "Room.h"

#include "vnLog.h"

#include <mutex>


PlayerManager::PlayerManager()
: m_thread([this](){
    m_loop.run();
}){
    
}

PlayerManager::~PlayerManager() {
    m_loop.post_quit();
    m_thread.join();
}

void PlayerManager::post(std::function<void()> fn) {
    m_loop.post(std::move(fn));
}

PlayerPtr PlayerManager::get(u32 playerId) {
    std::unique_lock<SpinLock> lck(m_lock);
    auto &ptr = m_players.insert({playerId, nullptr}).first->second;
    if (!ptr) {
        VN_LOG_DEBUG("[PlayerManager] create player #" << playerId);
        ptr = vnnew Player(playerId);
    }
    return ptr;
}

void PlayerManager::remove(u32 playerId) {
    std::unique_lock<SpinLock> lck(m_lock);
    m_players.erase(playerId);
    VN_LOG_DEBUG("[PlayerManager] remove player #" << playerId);

}