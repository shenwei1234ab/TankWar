//
//  RoomManager.cpp
//  demo_svr
//
//  Created by Wayne on 7/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "RoomManager.h"

#include <mutex>

RoomManager::RoomManager() {
    m_thread.reset(new std::thread([this](){
        m_loop.run(1.f / 64, std::bind(&RoomManager::_update, this, std::placeholders::_1));
    }));
}

RoomManager::~RoomManager() {
    destroyAll();
}

void RoomManager::post(std::function<void()> fn) {
    m_loop.post(std::move(fn));
}

void RoomManager::destroyAll() {
    m_roomIndexLock.lock_write();
    m_roomIndex.clear();
    m_roomIndexLock.unlock();
    if (m_thread) {
        m_loop.post([this](){
            for (auto &i : m_rooms) {
                i->_destroy();
            }
            m_rooms.clear();
        });
        m_loop.post_quit();
        m_thread->join();
        m_thread.reset();
    }
}

RoomPtr RoomManager::find(const str8 &name) {
    std::unique_lock<ReadWriteLock> lck(m_roomIndexLock);
    auto it = m_roomIndex.find(name);
    if (it == m_roomIndex.end()) {
        return nullptr;
    }
    return {it->second, true};
}

RoomPtr RoomManager::create(const str8 &name) {
    m_roomIndexLock.lock_write();
    auto &ptr = m_roomIndex.insert({name, nullptr}).first->second;
    if (ptr) {
        m_roomIndexLock.unlock();
        return nullptr;
    }
    ptr = vnnew Room(name);
    m_roomIndexLock.unlock();
    
    m_loop.post(std::bind([this](RoomPtr room){
        m_rooms.insert(std::move(room));
    }, RoomPtr(ptr, true)));
    
    return ptr;
}

void RoomManager::destroy(const str8 &name) {
    m_roomIndexLock.lock_write();
    auto it = m_roomIndex.find(name);
    if (it == m_roomIndex.end()) {
        m_roomIndexLock.unlock();
        return ;
    }
    RoomPtr room(it->second, true);
    m_roomIndex.erase(it);
    m_roomIndexLock.unlock();
    
    m_loop.post(std::bind([this](RoomPtr room){
        m_rooms.erase(std::move(room));
    }, std::move(room)));
}

void RoomManager::enter(RoomPtr room, PlayerPtr player, PublicLinkPtr link) {
	m_loop.post(std::bind([](const RoomPtr &room, PlayerPtr &player, PublicLinkPtr &link){
		room->_enter(std::move(player), std::move(link));
	}, std::move(room), std::move(player), std::move(link)));
}

void RoomManager::_update(f32 deltaTime) {
    for (auto &i : m_rooms) {
        i->_update(deltaTime);
    }
}

