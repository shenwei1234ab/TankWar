//
//  GameCommon.h
//  demo_svr
//
//  Created by Wayne on 7/22/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GameCommon__
#define __demo_svr__GameCommon__

#include "vnPacket.h"

using namespace vn;

#include <vector>
#include <array>

namespace pkt {
#include "../autocode/demo.hpp"
}

namespace game {

    enum Direction {
        kD_PX = 0,
        kD_PY = 1,
        kD_NX = 2,
        kD_NY = 3,
        
        kD_MAX = 4
    };
    
}

#endif /* defined(__demo_svr__GameCommon__) */
