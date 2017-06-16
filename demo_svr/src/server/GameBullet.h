//
//  GameBullet.h
//  demo_svr
//
//  Created by Wayne on 7/24/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GameBullet__
#define __demo_svr__GameBullet__

#include "GameEntity.h"
class CProtectMask;
class GameBullet : public GameObject 
{
public:
	GameBullet(Game *_game, u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32 _speed, game::Direction _dir, u32 _power);
    
    u32 ownerId() const { return m_ownerId; }
    
    const vector2f & position() const { return m_position; }
    const vector2i & radius() const { return m_radius; }
    
    virtual void init();
    virtual void fini();    
    virtual void update(f32 deltaTime);
    virtual void snapshot(pkt::Game &game);
	virtual void Init(TiXmlElement* e);
protected:
    u32 m_ownerId;
    vector2f m_position;
    vector2i m_radius;
    f32 m_speed;
    game::Direction m_direction;
    vector2f m_velocity;
	u32 m_power;
    bool m_destroyed = false;
};

#endif /* defined(__demo_svr__GameBullet__) */
