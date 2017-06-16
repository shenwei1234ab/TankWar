//
//  Game.cpp
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "Game.h"
#include "vnLog.h"
#include "Room.h"
#include "CWall.h"
#include "CProtectMask.h"
#include "CPropertyFactory.h"
#include "CProperty.h"
Game::Game(Room *room, f32 _playTime)
: m_room(room),
m_playTime(_playTime)
{
    m_broadcastPackets.reserve(32);
    m_cells.resize((size_t)m_size.area());
}


u32 Game::generateObjectId() {
    return ++m_lastId;
}

bool Game::_addObject(GameObjectPtr object) 
{
    auto ret = m_objectIndex.insert({object->objectId(), {}});
	//insert failed
    if (!ret.second) {
        return false;
    }
    auto &idx = ret.first->second;
    if (m_updating) 
	{
        idx.list = &m_incomings;
    } 
	else 
	{
        idx.list = &m_objects;
    }
    auto obj = object.release();
    idx.it = idx.list->insert(idx.list->end(), {obj});
    idx.it->it = ret.first;
    obj->init();


	//object->grab();
	//idx.it = idx.list->insert(idx.list->end(), { object.get() });
	//idx.it->it = ret.first;
	//object->init();
    return true;
}



void Game::removeObject(u32 objectId) {

    auto it = m_objectIndex.find(objectId);
    if (it == m_objectIndex.end()) {
        return ;
    }
    
    it->second.it->obj->fini();
    if (m_updating) {
        if (it->second.list == &m_incomings) {
            m_incomings.erase(it->second.it);
        } else {
            m_removeds.push_back(it->second.it);
        }
    } else {
        m_objects.erase(it->second.it);
    }
    m_objectIndex.erase(it);
}

GameObjectPtr Game::findObject(u32 objectId) 
{
    auto it = m_objectIndex.find(objectId);
    if (it == m_objectIndex.end()) 
	{
        return nullptr;
    }
    return it->second.it->obj;
}






GamePlayerPtr Game::createPlayer(const vector2f &_pos, game::Direction _dir, f32 _speed, size_t _playerSeat, u32 _hp)
{
    auto p = vnnew GamePlayer(this, _pos, _dir, _speed, _playerSeat,_hp);
    _addObject({p, true});
    return p;
}




u32 Game::createBullet(u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32 _speed, game::Direction _dir,u32 _power)
{
    auto p = vnnew GameBullet(this, ownerId, _pos, _radius, _speed, _dir,_power);
    _addObject(p);
    return p->objectId();
}



void Game::createProtectMask(u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32  _survivalTime)
{
	auto p = vnnew CProtectMask(this, ownerId, _pos, _radius,   _survivalTime);
	_addObject(p);
}




void Game::createWall(const vector2f &_pos, const vector2i & _radius, u32 _hp)
{
	auto p = vnnew CWall(this, _pos, _radius, _hp);
	_addObject(p);
}


void Game::update(f32 deltaTime) 
{
	if (m_playTime <= 0)
	{
		VN_LOG_INFO("Game Time is Over");
		//结束游戏
		m_room->async_endDemoGame();
		return;
	}
	m_playTime -= deltaTime;
	if (m_propertyTimer <= 0)
	{
		CProperty *pNewProperty = CPropertyFactory::getInstance().produceProperty(this);
		if (pNewProperty)
		{
			//发送包
			auto pkt = vnnew pkt::res_createProperty();
			pkt->data.property = { pNewProperty->getPropertyName(), pNewProperty->objectId(), pNewProperty->getPosition().x, pNewProperty->getPosition().y };
			broadcastPackets().push_back(pkt);
		}
		m_propertyTimer = m_propertyTime;
		
	}
	m_propertyTimer -= deltaTime;

	if (!m_broadcastPackets.empty()) {
		m_room->_game_broadcast(m_broadcastPackets);
		m_broadcastPackets.clear();
	}

    m_updateTimer += deltaTime;
    const f32 kDT = 1.f / 128;
    while (m_updateTimer >= kDT) {
        m_updateTimer -= kDT;
        m_time_stamp += kDT;
        _update(kDT);
        if (!m_broadcastPackets.empty()) {
            m_room->_game_broadcast(m_broadcastPackets);
            m_broadcastPackets.clear();
        }

    }
}


void Game::snapshot(pkt::Game &game) {
    game.timeStamp = m_time_stamp;
    for (auto &i : m_objects) {
        i.obj->snapshot(game);
    }
}

Game::Cell * Game::cell(const vector2i &_pos) {
    if (!(_pos >= vector2i::kZero && _pos < m_size)) {
        return nullptr;
    }
    return m_cells.data() + _pos.y * m_size.x + _pos.x;
}


//碰撞到了任何物体或者出界返回false;
bool Game::any_check(const vector2i &_pos, const vector2i &_radius) const
{
	vector2i _min = _pos - _radius;
	if (_min.x < 0 || _min.y < 0)
	{
		return false;
	}
	vector2i _max = _pos + _radius;
	if (_max.x > m_size.x || _max.y > m_size.y)
	{
		return false;
	}
	const Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
	for (; _min.y < _max.y; ++_min.y, p += m_size.x)
	{
		auto q = p;
		for (auto i = _min.x; i < _max.x; ++i, ++q) 
		{
			if (q->entity || q->bullet || q->pMask || q->pProperty)
			{
				return false;
			}
		}
	}
	return true;
}




//碰撞到了实体或者出界返回false;
bool Game::cell_check(const vector2i &_pos, const vector2i &_radius, GameEntity *except) const 
{
    vector2i _min = _pos - _radius;
    if (_min.x < 0 || _min.y < 0) 
	{
        return false;
    }
    vector2i _max = _pos + _radius;
    if (_max.x > m_size.x || _max.y > m_size.y) 
	{
        return false;
    }
    const Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
    for (; _min.y < _max.y; ++_min.y, p += m_size.x) 
	{
        auto q = p;
        for (auto i = _min.x; i < _max.x; ++i, ++q) {
            if (q->entity && q->entity != except) {
                return false;
            }
        }
    }
    return true;
}



//碰撞到其他的mask返回 false;
//否则返回true;
bool Game::mask_check(const vector2i &_pos, const vector2i &_radius, u32 ownId) const
{
	vector2i _min = _pos - _radius;
	vector2i _max = _pos + _radius;
	const Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
	for (; _min.y < _max.y; ++_min.y, p += m_size.x)
	{
		auto q = p;
		for (auto i = _min.x; i < _max.x; ++i, ++q) {
			//碰到敌人的防护罩
			if (q->pMask && q->pMask->getownid() != ownId) {
				return false;
			}
		}
	}
	return true;
}



CProperty*  Game::property_check(const vector2i &_pos, const vector2i &_radius) const
{

	vector2i _min = _pos - _radius;
	vector2i _max = _pos + _radius;
	const Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
	for (; _min.y < _max.y; ++_min.y, p += m_size.x)
	{
		auto q = p;
		for (auto i = _min.x; i < _max.x; ++i, ++q) 
		{
			//碰到pPropery
			if (q->pProperty) 
			{
				return q->pProperty;
			}
		}
	}
	return nullptr;
}









void Game::cell_set(const vector2i &_pos, const vector2i &_radius, GameEntity *entity) {
    vector2i _min(math::max_(_pos.x - _radius.x, 0), math::max_(_pos.y - _radius.y, 0));
    vector2i _max(math::min_(_pos.x + _radius.x, m_size.x), math::min_(_pos.y + _radius.y, m_size.y));
    Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
    for (; _min.y < _max.y; ++_min.y, p += m_size.x) {
        auto q = p;
        for (auto i = _min.x; i < _max.x; ++i, ++q) {
            q->entity = entity;
        }
    }
}


void Game::cell_set(const vector2i &_pos, const vector2i &_radius, CProperty  *pProperty)
{
	vector2i _min(math::max_(_pos.x - _radius.x, 0), math::max_(_pos.y - _radius.y, 0));
	vector2i _max(math::min_(_pos.x + _radius.x, m_size.x), math::min_(_pos.y + _radius.y, m_size.y));
	Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
	for (; _min.y < _max.y; ++_min.y, p += m_size.x) {
		auto q = p;
		for (auto i = _min.x; i < _max.x; ++i, ++q) {
			q->pProperty= pProperty;
		}
	}
}


void Game::cell_set(const vector2i &_pos, const vector2i &_radius, GameBullet *bullet) {
    vector2i _min(math::max_(_pos.x - _radius.x, 0), math::max_(_pos.y - _radius.y, 0));
    vector2i _max(math::min_(_pos.x + _radius.x, m_size.x), math::min_(_pos.y + _radius.y, m_size.y));
    Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
    for (; _min.y < _max.y; ++_min.y, p += m_size.x) {
        auto q = p;
        for (auto i = _min.x; i < _max.x; ++i, ++q) {
            q->bullet = bullet;
        }
    }
}




void Game::cell_set(const vector2i &_pos, const vector2i &_radius, CProtectMask *mask) {
	vector2i _min(math::max_(_pos.x - _radius.x, 0), math::max_(_pos.y - _radius.y, 0));
	vector2i _max(math::min_(_pos.x + _radius.x, m_size.x), math::min_(_pos.y + _radius.y, m_size.y));
	Cell *p = m_cells.data() + _min.y * m_size.x + _min.x;
	for (; _min.y < _max.y; ++_min.y, p += m_size.x) {
		auto q = p;
		for (auto i = _min.x; i < _max.x; ++i, ++q) {
			q->pMask = mask;
		}
	}
}


GameBullet * Game::cell_getBullet(const vector2i &_pos, const vector2i &_radius, GameBullet *except) const {
    vector2i _min(math::max_(_pos.x - _radius.x, 0), math::max_(_pos.y - _radius.y, 0));
    vector2i _max(math::min_(_pos.x + _radius.x, m_size.x), math::min_(_pos.y + _radius.y, m_size.y));
    auto p = m_cells.data() + _min.y * m_size.x + _min.x;
    for (; _min.y < _max.y; ++_min.y, p += m_size.x) {
        auto q = p;
        for (auto i = _min.x; i < _max.x; ++i, ++q) {
            if (q->bullet && q->bullet != except) {
                return q->bullet;
            }
        }
    }
    return nullptr;
}

void Game::_update(f32 deltaTime) {
    m_updating = true;
    for (auto &i : m_objects) {
        i.obj->update(deltaTime);
    }
    for (auto &i : m_incomings) {
        auto &idx = i.it->second;
        idx.list = &m_objects;
        idx.it = m_objects.insert(m_objects.end(), std::move(i));
    }
    m_incomings.clear();
    for (auto &i : m_removeds) {
        m_objects.erase(i);
    }
    m_removeds.clear();
    m_updating = false;
}




