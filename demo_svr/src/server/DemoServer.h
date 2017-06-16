//
//  DemoServer.h
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__DemoServer__
#define __demo_svr__DemoServer__

#include "../common/ServerApp.h"
#include "PublicListen.h"
#include "vnLink.h"
#include <thread>
#include <memory>

class DemoServer : public ServerApp {
    DECL_SERVER_APP()
public:
    virtual bool initialize(int argc, const char *argv[]);
    virtual void update(f32 deltaTime);
    virtual void finalize();
    
private:
    
    bool _load_config();
    void _console_input(str8 cmd);
    bool _init_link_pool();
    
    
    bool m_console_mode = false;

    std::unique_ptr<std::thread> m_console_thread;
    
    size_t m_link_pool_threads = 0;
    size_t m_db_threads = 0;
    str8 m_db_server;
    str8 m_db_user;
    str8 m_db_passwd;
    
    LinkPool *m_link_pool = nullptr;
    
    PublicListen m_public_listen;
    u16 m_public_port = 0;
};

#endif /* defined(__demo_svr__DemoServer__) */
