//
//  LinkManager.cpp
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "LinkManager.h"

void LinkManager::add(Link_TCP_Connection *link) {
    m_lock.lock();
    m_links.insert(link);
    m_lock.unlock();
}

void LinkManager::remove(Link_TCP_Connection *link) {
    m_lock.lock();
    m_links.erase(link);
    m_lock.unlock();
}

void LinkManager::closeAll() {
    m_lock.lock();
    for (auto p : m_links) {
        p->close();
    }
    m_links.clear();
    m_lock.unlock();
}