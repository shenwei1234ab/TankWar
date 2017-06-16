//
//  GameEntity.h
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GameEntity__
#define __demo_svr__GameEntity__

#include "GameObject.h"

#include "vnVector2.h"

class GameEntity : public GameObject {
public:
	GameEntity(Game *_game, const vector2f &_position, const vector2i & _radius);
	GameEntity(Game *_game) :GameObject(_game)
	{

	}
    const vector2f & position() const { return m_position; }
    const vector2i & radius() const { return m_radius; }
    
    virtual void init();
    virtual void fini();
    
protected:
    vector2f m_position;
    vector2i m_radius;
};

#endif /* defined(__demo_svr__GameEntity__) */
