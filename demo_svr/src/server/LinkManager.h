//
//  LinkManager.h
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__LinkManager__
#define __demo_svr__LinkManager__

#include "vnSingleton.h"
#include "vnLink.h"
#include "vnSpinLock.h"

#include <set>

using namespace vn;

class LinkManager : public Singleton<LinkManager> 
{
public:
    
	void add(Link_TCP_Connection *link);
    
	void remove(Link_TCP_Connection *link);
    
    void closeAll();
    
private:
    std::set<Link_TCP_Connection *> m_links;
    SpinLock m_lock;
};

#endif /* defined(__demo_svr__LinkManager__) */
