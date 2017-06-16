//
//  ServerApp.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "ServerApp.h"

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include <vector>
#include <iostream>
#include <list>
ServerApp * (*& ServerAppCreator::pfn())() {
    static ServerApp *(*s_pfn)() = nullptr;
    return s_pfn;
}

ServerApp * ServerAppCreator::create() {
    return pfn()();
}

void ServerAppCreator::destroy(ServerApp *p) {
    delete p;
}

ServerApp::~ServerApp() {
//    std::lock_guard<SpinLock> lck(m_timers_lock);
//    (void)lck;
//    for (auto p : m_timers) {
//        delete p;
//    }
}
#include <stdio.h>

class my_class
{
	private:
		int id;
	public:
		void setId(int id){id = id;}
}




void test()
{
	
	f.m_i = 1;
    
iet Tlist_Use_Right_Window=1 "让窗口显示在右边，0的话就是显示在左边
	std::list<int> myList;
    myList.push_back(1);
    myList.push_back(2);
    for(std::list<int>::iterator it = myList.begin();it!=myList.end();++it)
    {
      std::cout<< *it<<std::endl;
    }
    Father f;
    f.m_i = 1;

  
}
int ServerApp::main(int argc, const char *argv[]) {
    if (!this->initialize(argc, argv)) {
        this->finalize();
        return -1;
    }
    m_mainLoop.run(0.25f, [this](f32 dt) {
        //this->_updateTimers(dt);
        this->update(dt);
    });
    this->finalize();
    return 0;
}

void ServerApp::post(std::function<void ()> fn) {
    m_mainLoop.post(std::move(fn));
}

void ServerApp::quit() {
    m_mainLoop.quit();
}

//timer_t ServerApp::createTimer(f32 periods, std::function<void()> fn, u32 repeats) {
//    if (!fn) {
//        return nullptr;
//    }
//    
//    std::lock_guard<SpinLock> lck(m_timers_lock);
//    (void)lck;
//    
//    auto p = vnnew Timer(periods, repeats, std::move(fn));
//    m_timers.insert(p);
//    
//    return p;
//}
//
//void ServerApp::destroyTimer(timer_t timer) {
//    std::lock_guard<SpinLock> lck(m_timers_lock);
//    (void)lck;
//    auto it = m_timers.find((Timer *)timer);
//    if (it == m_timers.end()) {
//        return ;
//    }
//    m_timers.erase(it);
//    delete *it;
//}
//
//void ServerApp::_updateTimers(f32 dt) {
//    std::vector<Timer *> rised, dead;
//    m_timers_lock.lock();
//    for (auto p : m_timers) {
//        p->timer += dt;
//        if (p->timer >= p->periods) {
//            p->timer -= p->periods;
//            rised.push_back(p);
//            if (p->repeats != u32(-1)) {
//                if (!p->repeats--) {
//                    dead.push_back(p);
//                }
//            }
//        }
//    }
//    m_timers_lock.unlock();
//    
//    for (auto p : rised) {
//        p->fn();
//    }
//    
//    if (!dead.empty()) {
//        m_timers_lock.lock();
//        for (auto p : dead) {
//            auto it = m_timers.find(p);
//            if (it != m_timers.end()) {
//                delete p;
//                m_timers.erase(it);
//            }
//        }
//        m_timers_lock.unlock();
//    }
//}

void _my_exit(int) {
    ServerApp::instance().quit();
}

int main(int argc, const char * argv[]) {
    ::signal(SIGINT, _my_exit);
    ::signal(SIGTERM, _my_exit);
    ::signal(SIGPIPE, SIG_IGN);
	
	//test
    char link[10],path[100];
    sprintf(link,"/proc/%d/exe",getpid());
    readlink(link,path,sizeof(path));
    printf("%s\n",path);

    str8 s = argv[0];
    FILE *fp = fopen((s + ".pid").c_str(), "w");
    if (fp) {
        s = std::to_string(::getpid());
        fwrite(s.c_str(), s.length(), 1, fp);
        fclose(fp);
    }
    
    return ServerApp::instance().main(argc, argv);
}
