//
//  GameDynamicEntity.h
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GameDynamicEntity__
#define __demo_svr__GameDynamicEntity__

#include "GameEntity.h"

class GameDynamicEntity : public GameEntity {
public:
    using GameEntity::GameEntity;
    
    const vector2f & velocity() const { return m_velocity; }
    void setVelocity(const vector2f &v) { m_velocity = v; }
	virtual bool canMove(const vector2f &newPos);
protected:
	void _Move(const vector2f &newPos);
    bool _updateMovement(f32 deltaTime);
    
    vector2f m_velocity;
};

#endif /* defined(__demo_svr__GameDynamicEntity__) */
