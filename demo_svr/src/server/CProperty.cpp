#include "CProperty.h"
#include "Game.h"
#include "GameTank.h"
#include "vnLog.h"
#include "tinyxml.h"
CProperty::CProperty(Game *_game, const vector2f &_pos, const vector2i &_radius, f32 _survivalTime) :
GameObject(_game),
m_position(_pos),
m_radius(_radius),
m_survivalTime(_survivalTime)
{

}





void CProperty::init()
{
	//valid input Check
	m_game->cell_set(m_position, m_radius, this);
}





void CProperty::update(f32 deltaTime)
{
	if (m_survivalTime <= 0)
	{
		//Ïú»Ù
		auto pkt = vnnew pkt::res_destoryObject();
		pkt->data.objectId = m_objectId;
		m_game->removeObject(m_objectId);
		m_game->broadcastPackets().push_back(pkt);
	}
	m_survivalTime -= deltaTime;
}



void CProperty::snapshot(pkt::Game &game)
{
	game.properties.push_back({});
	auto &info = game.properties.back();
	info.propertyName = "CProperty";
	info.objectId = m_objectId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}






void CProperty::fini()
{
	m_game->cell_set(m_position, m_radius, (CProperty *)nullptr);
}