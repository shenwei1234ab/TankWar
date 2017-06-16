//
//  PublicListen.h
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__PublicListen__
#define __demo_svr__PublicListen__

#include "vnLink.h"

using namespace vn;

class PublicListen : public Link_TCP_Listen {
public:
    
protected:
    
    virtual void _init();
    virtual void _destroy();
    virtual Link_TCP_Connection * _accepted(const NetAddress &peer);
    //virtual void _error_accepting(int err) {}
};

#endif /* defined(__demo_svr__PublicListen__) */
