#include "CSpeedUpProperty.h"
#include "Game.h"
#include "vnLog.h"
#include "GameTank.h"
#include "CPropertyFactory.h"
//static CSpeedUpProperty s_speedProperty;

REGISETER_PROPERTY(CSpeedUpProperty)
CSpeedUpProperty::CSpeedUpProperty(Game *_game, const vector2f &_pos, const vector2i &_radius, f32 _survivalTime) :
CProperty(_game,_pos,_radius,_survivalTime)
{
	m_propertyName = "CSpeedUpProperty";
}

//CSpeedUpProperty::CSpeedUpProperty()
//{
//	VN_LOG_DEBUG("CSpeedUpProperty");
//	CPropertyFactory::getInstance().registFun(std::bind(&CSpeedUpProperty::create, this, std::placeholders::_1));
//}

void CSpeedUpProperty::snapshot(pkt::Game &game)
{
	game.properties.push_back({});
	auto &info = game.properties.back();
	info.propertyName = "CSpeedUpProperty";
	info.objectId = m_objectId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}





void CSpeedUpProperty::produceEffect(GameTank *ptank)const
{
	ptank->setSeed(ptank->speed() + m_speedUp);
}


//
//CProperty*  CSpeedUpProperty::create(Game *pGame)
//{
//	return new CSpeedUpProperty(pGame, { 0, 0 });
//}