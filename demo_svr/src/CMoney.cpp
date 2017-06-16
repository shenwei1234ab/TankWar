#include "CMoney.h"
#include "Game.h"
#include "vnLog.h"
#include "GameTank.h"
#include "CPropertyFactory.h"

//static CMoney s_money;
CMoney::CMoney(Game *_game, const vector2f &_pos, const vector2i &_radius, f32 _survivalTime) :
CProperty(_game, _pos, _radius, _survivalTime)
{
	m_propertyName = "CMoney";
}

REGISETER_PROPERTY(CMoney)
//CMoney::CMoney()
//{
//	CPropertyFactory::getInstance().registFun(std::bind(&CMoney::create, this, std::placeholders::_1));
//}

void CMoney::snapshot(pkt::Game &game)
{
	game.properties.push_back({});
	auto &info = game.properties.back();
	info.propertyName = m_propertyName;
	info.objectId = m_objectId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}





void CMoney::produceEffect(GameTank *ptank)const
{
	//GamePlayer *pPlayer = (GamePlayer *)(ptank);

	GamePlayer *pPlayer = dynamic_cast<GamePlayer *>(ptank);
	if (pPlayer)
	{
		pPlayer->addScore(1);
	}
}


#CProperty*  CMoney::create(Game *pGame)
#{
#	return new CMoney(pGame, { 0, 0 });
#}

