#ifndef _CRiver_H_
#define _CRiver_H_
#include "GameStaticEntity.h"

class CRiver : public GameStaticEntity
{
public:
	CRiver(Game *_game, const vector2f &_pos, const vector2i & _radius, u32 _hp);
	
	CRiver(Game *_game);

	virtual void init();
	
	virtual void   fini();
	
	virtual void snapshot(pkt::Game &game);
	
	virtual void Init(TiXmlElement* e);
protected:
	u32 m_hp;
};

#endif