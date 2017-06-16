#ifndef _CProtectMask_H_
#define _CProtectMask_H_
#include "GameEntity.h"
class CProtectMask : public GameObject
{
public:
	CProtectMask(Game *_game, u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32  _survivalTime=60.0f);
	virtual void update(f32 deltaTime);
	virtual void snapshot(pkt::Game &game);
	virtual void init();
	
	u32 getownid()const
	{
		return m_ownerId;
	}
	virtual void fini();
	virtual void Init(TiXmlElement* e);
protected:
	u32 m_ownerId;
	vector2f m_position;
	vector2i m_radius;
	f32 m_survivalTime;

};

#endif