//
//  DemoServer.cpp
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "DemoServer.h"
#include "../common/LogPrinter.h"
#include "LinkManager.h"
#include "DBManager.h"
#include "RoomManager.h"

#include "vnLog.h"
#include "vnVariableScript.h"
#include "vnMath.h"

#include <iostream>

IMPL_SERVER_APP(DemoServer)

bool DemoServer::initialize(int argc, const char *argv[]) {
    if (!_load_config()) {
        return false;
    }
    if (!m_console_mode) {
        Log::instance().printer(vnnew LogPrinterFile("logs/demo_svr"));
    }
	VN_LOG_INFO("m_db_threads" << m_db_threads);
	VN_LOG_INFO("m_db_server" << m_db_server.c_str());
	VN_LOG_INFO(" m_db_user" << m_db_user.c_str());
	VN_LOG_INFO(" m_db_passwd" << m_db_passwd.c_str());

    if (!DBManager::instance().startup(m_db_threads, m_db_server.c_str(), m_db_user.c_str(), m_db_passwd.c_str())) {
        return false;
    }
    
    if (!_init_link_pool()) {
        return false;
    }
    VN_LOG_INFO("start..");
    
    if (m_console_mode) {
        m_console_thread.reset(new std::thread([this](){
            str8 cmd;
            for (;;) {
                std::getline(std::cin, cmd);
                if (cmd == "exit") {
                    this->quit();
                    break;
                } else if (!cmd.empty()) {
                    this->post(std::bind(&DemoServer::_console_input, this, std::move(cmd)));
                }
            }
        }));
    }
    
    return true;
}

void DemoServer::update(f32 deltaTime) {
    
}

void DemoServer::finalize() {
    if (m_console_thread) {
        m_console_thread->join();
    }
    if (m_link_pool) {
        LinkManager::instance().closeAll();
        m_public_listen.close();
        m_link_pool->release();
    }
    RoomManager::instance().destroyAll();
    DBManager::instance().cleanup();
    VN_LOG_INFO("stop.");
}

bool DemoServer::_load_config() {
    VariableScript vs;
    auto root = vs.parse({0, "/home/leave/workspace_tankwar/demo_svr/run/demo_svr_config.var"});
    if (!root) {
        VN_LOG_FATAL("can NOT load demo_svr_config.var!");
        return false;
    }
    
    m_console_mode = root->queryAttributeBool("console_mode");
    m_link_pool_threads = root->queryAttributeInt32("link_pool_threads");
    m_public_port = (u16)root->queryAttributeInt32("public_port", 32001);
    
    m_db_threads = root->queryAttributeInt32("db_threads");
    m_db_server = root->queryAttributeString("db_server");
    m_db_user = root->queryAttributeString("db_user");
    m_db_passwd = root->queryAttributeString("db_passwd");
    
    return true;
}

bool DemoServer::_init_link_pool() {
    m_link_pool = LinkPool::create(m_link_pool_threads);
    
    if (!m_link_pool->listen([this](){ return &m_public_listen; }, {0, m_public_port}, true)) {
        VN_LOG_FATAL("can NOT listen *:" << m_public_port);
        return false;
    }
    
    return true;
}

void DemoServer::_console_input(str8 cmd) {
    
    
}
