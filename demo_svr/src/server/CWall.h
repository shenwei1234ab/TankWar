#ifndef _CWall_H_
#define _CWall_H_
#include "GameStaticEntity.h"

class CWall : public GameStaticEntity
{
public:
	CWall(Game *_game, const vector2f &_pos, const vector2i & _radius,u32 _hp);
	CWall(Game *_game);
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
	virtual void init();
	virtual void   fini();
	virtual void snapshot(pkt::Game &game);
	virtual void Init(TiXmlElement* e);
protected:
	u32 m_hp;
};

#endif