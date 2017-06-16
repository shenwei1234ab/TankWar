//
//  GameBullet.cpp
//  demo_svr
//
//  Created by Wayne on 7/24/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "GameBullet.h"
#include "vnLog.h"
#include "Game.h"
#include "CProtectMask.h"
#include "CWall.h"
#include "GameTank.h"
#include "GamePlayer.h"
#include "tinyxml.h"
#include "CRiver.h"
GameBullet::GameBullet(Game *_game, u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32 _speed, game::Direction _dir, u32 _power)
: GameObject(_game)
, m_ownerId(ownerId)
, m_position(_pos)
, m_radius(_radius)
, m_speed(_speed)
, m_direction(_dir)
, m_power(_power)
{

}

void GameBullet::init() {
    switch (m_direction) {
        case game::kD_PX:
            m_velocity.set(m_speed, 0);
            break;
            
        case game::kD_PY:
            m_velocity.set(0, m_speed);
            break;
            
        case game::kD_NX:
            m_velocity.set(-m_speed, 0);
            break;
            
        case game::kD_NY:
            m_velocity.set(0, -m_speed);
            
        default:
            break;
    }
    m_game->cell_set(m_position, m_radius, this);
}

 
void GameBullet::Init(TiXmlElement* e)
{

}



void GameBullet::fini() {
    m_game->cell_set(m_position, m_radius, (GameBullet *)nullptr);
}

void GameBullet::snapshot(pkt::Game &game) {
    game.bullets.push_back({});
    auto &info = game.bullets.back();
    info.objectId = m_objectId;
    info.ownerId = m_ownerId;
    info.positionX = m_position.x;
    info.positionY = m_position.y;
    info.direction = (u32)m_direction;
    info.speed = m_speed;
}





//void GameBullet::update(f32 deltaTime) 
//{
//	if (m_destroyed) {
//		return;
//	}
//	vector2f newPos = m_position + m_velocity * deltaTime;
//
//	vector2i _pos(newPos);
//
//	if (_pos == vector2i(m_position)) {
//		m_position = newPos;
//		return;
//	}
//
//	vector2i _min = _pos - m_radius, _max = _pos + m_radius;
//	if (!(_min >= vector2i::kZero && _max < m_game->size()))
//	{
//		// out bound.
//		auto pkt = vnnew pkt::res_bulletOutOfMap();
//		pkt->data.objectId = m_objectId;
//		m_game->broadcastPackets().push_back(pkt);
//		m_destroyed = true;
//		//Destory
//		m_game->removeObject(m_objectId);
//		return;
//	}
//	auto pitch = m_game->size().x;	//40
//	auto cell = m_game->cell(_min);
//	
//	
//////////////////////////////////////////
//	GameBullet *bullet = nullptr;
//	auto p = cell;
//	for (auto i = _min.y; i < _max.y; ++i, p += pitch) {
//		auto q = p;
//		for (auto j = _min.x; j < _max.x; ++j, ++q) {
//			if (q->bullet && q->bullet != this) {
//				bullet = q->bullet;
//				goto l_bullet;
//			}
//		}
//	}
//l_bullet:
//	if (bullet) 
//	{
//		// collided with another bullet.
//		auto pkt = vnnew pkt::res_bulletCollideBullet();
//		pkt->data.objectId = m_objectId;
//		pkt->data.bulletId = bullet->m_objectId;
//		pkt->data.positionX = newPos.x;
//		pkt->data.positionY = newPos.y;
//		m_game->broadcastPackets().push_back(pkt);
//		m_destroyed = true;
//		bullet->m_destroyed = true;
//		m_game->removeObject(m_objectId);
//		m_game->removeObject(bullet->m_objectId);
//		return;
//	}
//	
//	
/////////////////////////////////////////碰撞到实体
//	GameEntity *entity = nullptr;
//	p = cell;
//	for (auto i = _min.y; i < _max.y; ++i, p += pitch) {
//		auto q = p;
//		for (auto j = _min.x; j < _max.x; ++j, ++q) {
//			VN_LOG_INFO("ShootEnemy Tank ");
//			if (q->entity && q->entity->objectId() != m_ownerId) 
//			{
//				entity = q->entity;
//				goto l_entity;
//			}
//		}
//	}
//l_entity:
//	if (entity) 
//	{
//		auto pkt = vnnew pkt::res_bulletCollideEntity();
//		pkt->data.objectId = m_objectId;
//		pkt->data.entityId = entity->objectId();
//		pkt->data.positionX = newPos.x;
//		pkt->data.positionY = newPos.y;
//		m_game->broadcastPackets().push_back(pkt);
//		m_destroyed = true;
//		m_game->removeObject(m_objectId);
//		GameTank *pTank = dynamic_cast<GameTank*>(entity);
//		if (pTank)
//		{
//			VN_LOG_DEBUG("resetPosition");
//			VN_LOG_DEBUG("old Pos is " << pTank->position().x << pTank->position().y);
//			pTank->resetPosition();
//			VN_LOG_DEBUG("new Pos is " << pTank->position().x << pTank->position().y);
//			VN_LOG_DEBUG("collider enemy's tank");
//		}
//		return;
//	}
//////////////////////////////////////////
//	CProtectMask *pMask = nullptr;
//	p = cell;
//	for (auto i = _min.y; i < _max.y; ++i, p += pitch) {
//		auto q = p;
//		for (auto j = _min.x; j < _max.x; ++j, ++q) {
//			
//			if (q->pMask && q->pMask->getownid()!= m_ownerId) {
//				VN_LOG_DEBUG("collider enemy's mask");
//				pMask = q->pMask;
//				goto l_Mask;
//			}
//		}
//	}
//l_Mask:
//	if (pMask) {
//		// collided with entity.
//		auto pkt = vnnew pkt::res_bulletCollideMask();
//		pkt->data.objectId = m_objectId;
//		pkt->data.maskId = pMask->objectId();
//		pkt->data.positionX = newPos.x;
//		pkt->data.positionY = newPos.y;
//		m_game->broadcastPackets().push_back(pkt);
//		m_destroyed = true;
//		m_game->removeObject(m_objectId);
//		return;
//	}
//
//
//
//////////////////////////////////////////
//	m_game->cell_set(m_position, m_radius, (GameBullet *)nullptr);
//	m_position = newPos;
//	p = cell;
//	for (auto i = _min.y; i < _max.y; ++i, p += pitch) {
//		auto q = p;
//		for (auto j = _min.x; j < _max.x; ++j, ++q) {
//			q->bullet = this;
//		}
//	}
//}








void GameBullet::update(f32 deltaTime)
{
	if (m_destroyed) {
		return;
	}
	vector2f newPos = m_position + m_velocity * deltaTime;

	vector2i _pos(newPos);

	if (_pos == vector2i(m_position)) {
		m_position = newPos;
		return;
	}

	vector2i _min = _pos - m_radius, _max = _pos + m_radius;
	if (!(_min >= vector2i::kZero && _max < m_game->size()))
	{
		// out bound.
		auto pkt = vnnew pkt::res_bulletOutOfMap();
		pkt->data.objectId = m_objectId;
		m_game->broadcastPackets().push_back(pkt);
		m_destroyed = true;
		//Destory
		m_game->removeObject(m_objectId);
		return;
	}
	auto pitch = m_game->size().x;	//40
	auto cell = m_game->cell(_min);


	////////////////////////////////////////
	GameBullet *bullet = nullptr;
	GameEntity *entity = nullptr;
	CProtectMask *pMask = nullptr;

	auto p = cell;
	for (auto i = _min.y; i < _max.y; ++i, p += pitch) {
		auto q = p;
		for (auto j = _min.x; j < _max.x; ++j, ++q) 
		{
			if (q->bullet && q->bullet != this) 
			{
				bullet = q->bullet;
				goto l_bullet;
			}
			else if (q->entity && q->entity->objectId() != m_ownerId)
			{
				entity = q->entity;
				goto l_entity;
			}
			else if (q->pMask && q->pMask->getownid() != m_ownerId)
			{
				pMask = q->pMask;
				goto l_Mask;
			}
		}
	}
	//撞到其他的子弹
l_bullet:
	if (bullet)
	{
		VN_LOG_INFO("Touch other bullet");
		// collided with another bullet.
		auto pkt = vnnew pkt::res_bulletCollideBullet();
		pkt->data.objectId = m_objectId;
		pkt->data.bulletId = bullet->m_objectId;
		pkt->data.positionX = newPos.x;
		pkt->data.positionY = newPos.y;
		m_game->broadcastPackets().push_back(pkt);
		m_destroyed = true;
		bullet->m_destroyed = true;
		m_game->removeObject(m_objectId);
		m_game->removeObject(bullet->m_objectId);
		return;
	}


l_entity:
	if (entity)
	{
		VN_LOG_DEBUG("touch entity");
		if (dynamic_cast<CRiver *>(entity))
		{
			goto l_Nothing;
		}
		//撞到墙
		CWall *pWall = dynamic_cast<CWall *>(entity);
		if (pWall)
		{
			u32 hp = pWall->getHP() - m_power;
			pWall->setHP(hp);
			if (hp <= 0)
			{
				//墙被销毁
				auto pkt = vnnew pkt::res_bulletdestoryObject();
				VN_LOG_INFO("wall destory");
				pkt->data.bulletId = m_objectId;
				pkt->data.objectId = pWall->objectId();
				VN_LOG_INFO("bullet id is " << pkt->data.bulletId);
				VN_LOG_INFO("wall id is " << pkt->data.objectId);
				m_game->broadcastPackets().push_back(pkt);
				m_destroyed = true;
				m_game->removeObject(pWall->objectId());
				m_game->removeObject(m_objectId);
				return;
			}
		}

		//撞到其他的坦克
		GameTank *pTank = dynamic_cast<GameTank*>(entity);
		if (pTank)
		{
			VN_LOG_DEBUG("touch other tank");
			//添加自己的分数
			GamePlayer *pPlayer = (GamePlayer *)(m_game->findObject(m_ownerId).get());
			if (pPlayer)
			{
				pPlayer->addScore(1);
			}
			//坦克的血量减少
			pTank->setHP(pTank->getHP() - m_power);
			auto pkt = vnnew pkt::res_bulletCollideTank();
			pkt->data.bulletId = m_objectId;
			pkt->data.tankId = pTank->objectId();
			pkt->data.score = pPlayer->getScore();
			pkt->data.hp = pTank->getHP();
			m_game->broadcastPackets().push_back(pkt);
			m_destroyed = true;
			m_game->removeObject(m_objectId);
			if (pTank->getHP() <= 0)
			{
				pTank->resetHp();
				pTank->resetPosition();
			}
			return;
		}


		//
		auto pkt = vnnew pkt::res_bulletCollideEntity();
		pkt->data.objectId = m_objectId;
		pkt->data.entityId = entity->objectId();
		pkt->data.positionX = newPos.x;
		pkt->data.positionY = newPos.y;
		m_game->broadcastPackets().push_back(pkt);
		m_destroyed = true;
		m_game->removeObject(m_objectId);
		return;
	}



////////////////////////////////////////
l_Mask:
	//撞到敌人的防护罩
	if (pMask) 
	{
		VN_LOG_DEBUG("touch other mask");
		/*u32 hp = pMask->getHP() - 1;
		pMask->setHP(hp);
		if (hp <= 0)
		{
			auto pkt = vnnew pkt::res_bulletdestoryObject();
			pkt->data.bulletId = m_objectId;
			pkt->data.objectId = pMask->objectId();
			m_game->removeObject(m_objectId);
			m_game->removeObject(pMask->objectId());
			m_game->broadcastPackets().push_back(pkt);
			m_destroyed = true;
			return;
		}
		auto pkt = vnnew pkt::res_bulletCollideMask();
		pkt->data.objectId = m_objectId;
		pkt->data.maskId = pMask->objectId();
		pkt->data.positionX = newPos.x;
		pkt->data.positionY = newPos.y;
		m_game->broadcastPackets().push_back(pkt);
		m_destroyed = true;*/
		//摧毁子弹
		auto pkt = vnnew pkt::res_destoryObject();
		pkt->data.objectId = m_objectId;
		m_game->broadcastPackets().push_back(pkt);
		m_destroyed = true; 
		m_game->removeObject(m_objectId);
		return;
	}

////////////////////////////////////////
l_Nothing:
	//touch nothing then move
	m_game->cell_set(m_position, m_radius, (GameBullet *)nullptr);
	m_position = newPos;
	p = cell;
	for (auto i = _min.y; i < _max.y; ++i, p += pitch) {
		auto q = p;
		for (auto j = _min.x; j < _max.x; ++j, ++q) {
			q->bullet = this;
		}
	}
}
