//
//  Game.h
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__Game__
#define __demo_svr__Game__

#include "GamePlayer.h"
#include "GameBullet.h"
#include "vnPacket.h"
#include "CProtectMask.h"
#include <map>
#include <list>
#include <vector>
#include "CPropertyFactory.h"
class CProperty;
class Room;
class Game : public AllocatedObject {
public:

	friend class CPropertyFactory;
	explicit Game(Room *room, f32 _playTime=120.0f);
    u32 generateObjectId();
    
    void removeObject(u32 objectId);
    
    GameObjectPtr findObject(u32 objectId);
    
    void update(f32 deltaTime);
    
    f32 time_stamp() const { return m_time_stamp; }
    
    void snapshot(pkt::Game &game);
    
    
	GamePlayerPtr createPlayer(const vector2f &_pos, game::Direction _dir, f32 _speed, size_t _playerSeat, u32 _hp);
	u32 createBullet(u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32 _speed, game::Direction _dir, u32 _power);
	void createProtectMask(u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32  _survivalTime);
	void createWall(const vector2f &_pos, const vector2i & _radius, u32 _hp);
    
	
	
	f32 broadcastPeriod() const { return 0.25f; }
    
    const vector2i & size() const { return m_size; }
    
    std::vector<PacketPtr> & broadcastPackets() { return m_broadcastPackets; }
    
    struct Cell 
	{
        GameEntity *entity = nullptr;
        GameBullet *bullet = nullptr;
		CProtectMask *pMask = nullptr;
		CProperty *pProperty = nullptr;
    };
    
    Cell * cell(const vector2i &_pos);
    
	bool cell_check(const vector2i &_pos, const vector2i &_radius, GameEntity *except = nullptr) const;


	bool any_check(const vector2i &_pos, const vector2i &_radius) const;
	CProperty* property_check(const vector2i &_pos, const vector2i &_radius) const;


	bool mask_check(const vector2i &_pos, const vector2i &_radius, u32 ownId) const;
	//bool river_check(const vector2i &_pos, const vector2i &_radius, u32 ownId) const;

	//void cell_set(const vector2i &_pos, const vector2i &_radius);
	void cell_set(const vector2i &_pos, const vector2i &_radius, GameEntity *entity);
    void cell_set(const vector2i &_pos, const vector2i &_radius, GameBullet *bullet);
	void cell_set(const vector2i &_pos, const vector2i &_radius, CProtectMask *mask);
	void cell_set(const vector2i &_pos, const vector2i &_radius, CProperty  *pProperty);
	GameBullet * cell_getBullet(const vector2i &_pos, const vector2i &_radius, GameBullet *except = nullptr) const;
	bool _addObject(GameObjectPtr object);
protected:
	
    void _update(f32 deltaTime);

	f32 m_propertyTimer = 20.0f;
	f32 m_propertyTime = 20.0f;

	u32 m_lastId = 0;
    
	Room *m_room;
    
    struct ObjectIndexBlock;
    struct ObjectBlock;
    
    struct ObjectIndexBlock {
        std::list<ObjectBlock>::iterator it;
        std::list<ObjectBlock> *list = nullptr;
    };
    
    struct ObjectBlock  {
        GameObjectPtr obj;
		
        std::map<u32, ObjectIndexBlock>::iterator it;
        
        ObjectBlock() = default;
        ObjectBlock(const ObjectBlock &) = default;
        ObjectBlock(ObjectBlock &&v) : obj(std::move(v.obj)), it(std::move(v.it)) {}
        ObjectBlock(GameObjectPtr &&_obj) : obj(std::move(_obj)) {}
    };

    
    std::map<u32, ObjectIndexBlock> m_objectIndex;

    std::list<ObjectBlock> m_objects, m_incomings;


    std::vector<std::list<ObjectBlock>::iterator> m_removeds;
    
    bool m_updating = false;
    f32 m_updateTimer = 0;

    f32 m_time_stamp = 0;
    
    std::vector<PacketPtr> m_broadcastPackets;
    
    std::vector<Cell> m_cells;
    vector2i m_size {40, 40};

	f32 m_playTime;

};

#endif /* defined(__demo_svr__Game__) */
