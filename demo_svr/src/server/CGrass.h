#ifndef _CGrass_H_
#define _CGrass_H_
#include "GameEntity.h"
class CGrass : public GameObject
{
public:
	CGrass(Game *_game, const vector2f &_pos, const vector2i & _radius);

	CGrass(Game *_game);

	virtual void init();

	virtual void   fini();

	virtual void snapshot(pkt::Game &game);

	virtual void Init(TiXmlElement* e);

protected:
	vector2f m_position;
	vector2i m_radius;
};

#endif