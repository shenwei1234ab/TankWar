#ifndef _CSpeedUpProperty_H_
#define _CSpeedUpProperty_H_
#include "CProperty.h"

class Game;
class CSpeedUpProperty : public CProperty
{
public:
	CSpeedUpProperty();


	CSpeedUpProperty(Game *_game, const vector2f &_pos, const vector2i &_radius = {1,1}, f32  _survivalTime = 60.0f);
	
	
	virtual void snapshot(pkt::Game &game);
	

	virtual void produceEffect(GameTank *ptank)const;


	CProperty*  create(Game *pGame);
protected:
	f32 m_speedUp = 4.0f;
};
#endif