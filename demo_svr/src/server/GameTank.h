//
//  GameTank.h
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GameTank__
#define __demo_svr__GameTank__

#include "GameDynamicEntity.h"

class GameTank : public GameDynamicEntity {
public:
    enum State {
        kIdle = 0,
        kMoving,
    };
	GameTank(Game *_game);
	GameTank(Game *_game, const vector2f &_pos, game::Direction _dir, f32 _speed,u32 _hp);
    
    game::Direction direction() const { return m_direction; }
    
    f32 speed() const { return m_speed; }
	
	void setSeed(f32 _speed);
    
	void move(game::Direction _dir, f32 _time = 0.25f);
    
	void stop();



	void firePowerAttack(game::Direction _dir);
    void fireCommonAttack(game::Direction _dir);
	
	virtual void init();
	
	virtual void update(f32 deltaTime);
	
	virtual bool canMove(const vector2f &newPos);
	
	void resetPosition();
	
	virtual void Init(TiXmlElement* e);
	
	u32 getHP()const
	{
		return  m_hp;
	}

	void setHP(u32 _hp)
	{
		m_hp = _hp;
		if (m_hp < 0)
		{
			m_hp = 0;
		}
	}

	void resetHp()
	{
		m_hp = m_initHp;
	}

protected:
	f32 m_maxSpeed = 12;
	u32 m_initHp;
	vector2f m_initPosition;
    game::Direction m_direction = game::kD_PX;
    State m_state = kIdle;
	f32 m_speed = 5;
    f32 m_broadcastState = kIdle;
	u32 m_hp=4;
	f32 m_commonAttackInterval = 0.5f;
	f32 m_powerAttackInterval = 1.0f;
//timer
	f32 m_movingTimer = 0;
	f32 m_commonAttackTimer = 0;
	f32 m_powerAttackTimer = 0;
	f32 m_broadcastTimer = 0;
//funtion
	void _setDirection(game::Direction _dir);
	void _snapshot(pkt::GameTank &tank);
};

#endif /* defined(__demo_svr__GameTank__) */
