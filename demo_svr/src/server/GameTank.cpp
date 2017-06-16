//
//  GameTank.cpp
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "GameTank.h"

#include "Game.h"
#include "vnLog.h"
#include "tinyxml.h"
#include "CProperty.h"
GameTank::GameTank(Game *_game, const vector2f &_pos, game::Direction _dir, f32 _speed, u32 _hp)
: GameDynamicEntity(_game, _pos, { 2, 2 }), m_speed(_speed), m_hp(_hp)
{
    _setDirection(_dir);
	
}


GameTank::GameTank(Game *_game) :GameDynamicEntity(_game)
{

}




void GameTank::Init(TiXmlElement* e)
{
	//读入xml的信息

}

void GameTank::init()
{
	GameEntity::init();
	m_initPosition = m_position;
	m_initHp = m_hp;
}

void GameTank::move(game::Direction _dir, f32 _time) {
    m_state = kMoving;
    m_movingTimer = _time;
    if (m_direction != _dir) {
        _setDirection(_dir);
    }
}

void GameTank::stop() {
    if (m_state != kMoving) {
        return ;
    }
    m_state = kIdle;
    m_movingTimer = 0;
    if (m_broadcastState == kIdle) {
        return ;
    }
    m_broadcastTimer = 0;
    m_broadcastState = kIdle;
    
    auto pkt = vnnew pkt::res_tankStop();
    pkt->data.objectId = m_objectId;
    pkt->data.positionX = m_position.x;
    pkt->data.positionY = m_position.y;
    m_game->broadcastPackets().push_back(pkt);
}




void GameTank::resetPosition()
{
	fini();
	m_position = m_initPosition;
	init();
}






void GameTank::firePowerAttack(game::Direction _dir) {
	if (m_powerAttackTimer) {
		return;
	}
	m_powerAttackTimer = m_powerAttackInterval;

	if (m_direction != _dir) {
		_setDirection(_dir);
	}

	vector2f bulletPos = m_position;
	switch (m_direction) {
	case game::kD_PX:
		bulletPos.x += 1;
		break;

	case game::kD_PY:
		bulletPos.y += 1;
		break;

	case game::kD_NX:
		bulletPos.x -= 1;
		break;

	case game::kD_NY:
		bulletPos.y -= 1;
		break;

	default:
		break;
	}
	f32 bulletSpeed = 16;
	auto pkt = vnnew pkt::res_tankPowerAttack();
	pkt->data.tankId = m_objectId;
	pkt->data.tankPosX = m_position.x;
	pkt->data.tankPosY = m_position.y;
	pkt->data.bulletId = m_game->createBullet(m_objectId, bulletPos, { 1, 1 }, bulletSpeed, m_direction,2);
	pkt->data.bulletPosX = bulletPos.x;
	pkt->data.bulletPosY = bulletPos.y;
	pkt->data.speed = bulletSpeed;
	pkt->data.direction = m_direction;
	m_game->broadcastPackets().push_back(pkt);
}










void GameTank::fireCommonAttack(game::Direction _dir) {
    if (m_commonAttackTimer) {
        return ;
    }
	m_commonAttackTimer = m_commonAttackInterval;
    
    if (m_direction != _dir) {
        _setDirection(_dir);
    }
    
    vector2f bulletPos = m_position;
    switch (m_direction) {
        case game::kD_PX:
            bulletPos.x += 1;
            break;
            
        case game::kD_PY:
            bulletPos.y += 1;
            break;
            
        case game::kD_NX:
            bulletPos.x -= 1;
            break;
            
        case game::kD_NY:
            bulletPos.y -= 1;
            break;
            
        default:
            break;
    }
    f32 bulletSpeed = 16;
    auto pkt = vnnew pkt::res_tankCommonAttack();
    pkt->data.tankId = m_objectId;
    pkt->data.tankPosX = m_position.x;
    pkt->data.tankPosY = m_position.y;
    pkt->data.bulletId = m_game->createBullet(m_objectId, bulletPos, {1, 1}, bulletSpeed, m_direction,1);
    pkt->data.bulletPosX = bulletPos.x;
    pkt->data.bulletPosY = bulletPos.y;
    pkt->data.speed = bulletSpeed;
    pkt->data.direction = m_direction;
    m_game->broadcastPackets().push_back(pkt);
}


void GameTank::setSeed(f32 _speed)
{
	if (_speed < 0)
	{
		_speed = 0;
	}
	if (_speed > m_maxSpeed)
	{
		_speed = m_maxSpeed;
	}
	m_speed = _speed;
}

bool GameTank::canMove(const vector2f &newPos)
{
	//判断是否撞到实体
	bool bRet = GameDynamicEntity::canMove(newPos);
	//不能移动
	if (!bRet)
	{
		VN_LOG_DEBUG("collider enemy or wall");
		return bRet;
	}
	//判断非实体的碰撞
	//在判断是否碰撞到敌人的mask
	bRet = m_game->mask_check(newPos, m_radius, m_objectId);
	if (!bRet)
	{
		VN_LOG_DEBUG("collider enemy's mask");
	}
	return bRet;
}


//void GameTank::update(f32 deltaTime) {
//	if (m_commonAttackTimer > 0) {
//		if (m_commonAttackTimer > deltaTime) {
//			m_commonAttackTimer -= deltaTime;
//		}
//		else {
//			m_commonAttackTimer = 0;
//		}
//	}
//	if (m_state == kMoving) {
//		if (m_broadcastTimer > 0) {
//			m_broadcastTimer = math::max_(m_broadcastTimer - deltaTime, 0.f);
//		}
//		if (m_movingTimer > 0) {
//			f32 time;
//			if (m_movingTimer > deltaTime) {
//				m_movingTimer -= deltaTime;
//				time = deltaTime;
//			}
//			else {
//				time = m_movingTimer;
//				m_movingTimer = 0;
//			}
//			碰撞检测
//			if (_updateMovement(time))
//			{
//				没有碰撞到其他cell
//				if (m_broadcastTimer == 0)
//				{
//					重新计时
//					m_broadcastTimer = m_game->broadcastPeriod();
//					m_broadcastState = kMoving;
//					发送广播包
//					auto pkt = vnnew pkt::res_tankMove();
//					pkt->data.objectId = m_objectId;
//					pkt->data.positionX = m_position.x;
//					pkt->data.positionY = m_position.y;
//					pkt->data.direction = (u32)m_direction;
//					m_game->broadcastPackets().push_back(pkt);
//				}
//			}
//			else if (m_broadcastState == kMoving)
//			{
//				m_broadcastState = kIdle;
//				m_broadcastTimer = 0;
//				 broadcast stop.
//				auto pkt = vnnew pkt::res_tankStop();
//				pkt->data.objectId = m_objectId;
//				pkt->data.positionX = m_position.x;
//				pkt->data.positionY = m_position.y;
//				m_game->broadcastPackets().push_back(pkt);
//			}
//		}
//	}
//}


void GameTank::update(f32 deltaTime) {
    if (m_commonAttackTimer > 0) 
	{
        if (m_commonAttackTimer > deltaTime) 
		{
            m_commonAttackTimer -= deltaTime;
        } 
		else 
		{
            m_commonAttackTimer = 0;
        }
    }
	if (m_powerAttackTimer > 0)
	{
		if (m_powerAttackTimer > deltaTime)
		{
			m_powerAttackTimer -= deltaTime;
		}
		else
		{
			m_powerAttackTimer = 0;
		}
	}
    if (m_state == kMoving) 
	{
        if (m_broadcastTimer > 0) 
		{
            m_broadcastTimer = math::max_(m_broadcastTimer - deltaTime, 0.f);
        }
        if (m_movingTimer > 0) 
		{
            f32 time;
            if (m_movingTimer > deltaTime) 
			{
                m_movingTimer -= deltaTime;
                time = deltaTime;
            } 
			else 
			{
                time = m_movingTimer;
                m_movingTimer = 0;
            }
			auto newPos = m_position + m_velocity * time;
			if (canMove(newPos))
			{
				_Move(newPos);

				//碰到道具了
				CProperty *pProperty = m_game->property_check(m_position, m_radius);
				if (pProperty)
				{
					pProperty->produceEffect(this);
					auto pkt = vnnew pkt::res_getProperty();
					pkt->data.property = { pProperty->getPropertyName(), pProperty->objectId(), pProperty->getPosition().x, pProperty->getPosition().y };
					pkt->data.tankId = m_objectId;
					m_game->broadcastPackets().push_back(pkt);
					m_game->removeObject(pProperty->objectId());
				}


				if (m_broadcastTimer == 0)
				{
					VN_LOG_INFO("Move broadcast" );
					//重新计时
					m_broadcastTimer = m_game->broadcastPeriod();
					m_broadcastState = kMoving;
					//发送广播包
					auto pkt = vnnew pkt::res_tankMove();
					pkt->data.objectId = m_objectId;
					pkt->data.positionX = m_position.x;
					pkt->data.positionY = m_position.y;
					pkt->data.direction = (u32)m_direction;
					m_game->broadcastPackets().push_back(pkt);
		        }
			}
			else
			{
				//VN_LOG_INFO("can not move");
				if (m_broadcastState == kMoving)
				{
					m_broadcastState = kIdle;
					m_broadcastTimer = 0;
					// broadcast stop.
					auto pkt = vnnew pkt::res_tankStop();
					pkt->data.objectId = m_objectId;
					pkt->data.positionX = m_position.x;
					pkt->data.positionY = m_position.y;
					m_game->broadcastPackets().push_back(pkt);
				}
			}
			



			//没有碰撞到其他cell就移动
   //         if (_updateMovement(time)) 
			//{
   //             if (m_broadcastTimer == 0) 
			//	{
			//		//重新计时
   //                 m_broadcastTimer = m_game->broadcastPeriod();
   //                 m_broadcastState = kMoving;
   //                 //发送广播包
   //                 auto pkt = vnnew pkt::res_tankMove();
   //                 pkt->data.objectId = m_objectId;
   //                 pkt->data.positionX = m_position.x;
   //                 pkt->data.positionY = m_position.y;
   //                 pkt->data.direction = (u32)m_direction;
   //                 m_game->broadcastPackets().push_back(pkt);
   //             }
   //         }
			////碰撞到其他cell
			//else if (m_broadcastState == kMoving) 
			//{
   //             m_broadcastState = kIdle;
   //             m_broadcastTimer = 0;
			//	// broadcast stop.
   //             auto pkt = vnnew pkt::res_tankStop();
   //             pkt->data.objectId = m_objectId;
   //             pkt->data.positionX = m_position.x;
   //             pkt->data.positionY = m_position.y;
   //             m_game->broadcastPackets().push_back(pkt);
   //         }
        }
    }
}






void GameTank::_snapshot(pkt::GameTank &tank) {
    tank.objectId = m_objectId;
    tank.direction = (unsigned int)m_direction;
    tank.positionX = m_position.x;
    tank.positionY = m_position.y;
    tank.speed = m_speed;
}

void GameTank::_setDirection(game::Direction _dir) {
    m_direction = _dir;
    switch (_dir) {
        case game::kD_PX:
            this->setVelocity({m_speed, 0});
            break;
            
        case game::kD_PY:
            this->setVelocity({0, m_speed});
            break;
            
        case game::kD_NX:
            this->setVelocity({-m_speed, 0});
            break;
            
        case game::kD_NY:
            this->setVelocity({0, -m_speed});
            break;
            
        default:
            break;
    }
}
