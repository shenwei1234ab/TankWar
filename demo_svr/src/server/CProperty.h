#ifndef _CProperty_H_
#define _CProperty_H_
#include "GameEntity.h"
class GameTank;
class CProperty : public GameObject
{
public:
	CProperty(){};
	CProperty(Game *_game, const vector2f &_pos, const vector2i & _radius, f32  _survivalTime = 10.0f);
	
	//CProperty* create(){};

	virtual void init();
	
	virtual void fini();
	

	virtual void update(f32 deltaTime);

	virtual void snapshot(pkt::Game &game);

	
	vector2i getRadius()const
	{
		return m_radius;
	}

	void setPosition(const vector2f &_pos)
	{
		m_position = _pos;
	}

	vector2f getPosition()const
	{
		return m_position; 
	}

	virtual void produceEffect(GameTank *ptank)const=0;

	str8  getPropertyName()const
	{
		return m_propertyName;
	}

	virtual CProperty*   create(Game *pGame) = 0;
	
protected:
	str8 m_propertyName;
	
	vector2f m_position;
	
	vector2i m_radius;
	
	f32 m_survivalTime;
};



#define REGISETER_PROPERTY(PROPERTY)\
	static PROPERTY s_PROPERTY;\
PROPERTY::PROPERTY(){\
	CPropertyFactory::getInstance().registFun(std::bind(&PROPERTY::create, this, std::placeholders::_1)); \
}\
CProperty*  PROPERTY::create(Game *pGame){\
	return new PROPERTY(pGame, { 0, 0 }); \
}\

#endif