#ifndef _CMoney_H_
#define _CMoney_H_
#include "CProperty.h"
class Game;
class CMoney : public CProperty
{
public:
	CMoney(Game *_game, const vector2f &_pos, const vector2i &_radius = { 1, 1 }, f32  _survivalTime = 60.0f);

	CMoney();


	CProperty*  create(Game *pGame);


	virtual void snapshot(pkt::Game &game);


	virtual void produceEffect(GameTank *ptank)const;
protected:


};

#endif