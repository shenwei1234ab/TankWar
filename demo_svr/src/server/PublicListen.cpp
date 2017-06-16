//
//  PublicListen.cpp
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "PublicListen.h"
#include "PublicLink.h"

#include "vnLog.h"


void PublicListen::_init() {
    
}

void PublicListen::_destroy() {
    
}

Link_TCP_Connection * PublicListen::_accepted(const NetAddress &peer) {
    VN_LOG_INFO("[public] client " << peer.to_string() << " connected");
    return vnnew PublicLink();
}