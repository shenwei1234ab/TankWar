#include "CProtectMask.h"
#include "Game.h"
#include "vnLog.h"
#include "tinyxml.h"
CProtectMask::CProtectMask(Game *_game, u32 ownerId, const vector2f &_pos, const vector2i &_radius, f32 _survivalTime) :
GameObject(_game),
m_ownerId(ownerId),
m_position(_pos),
m_radius(_radius),
m_survivalTime(_survivalTime)
{
	VN_LOG_INFO("CProtectMask created " << this->m_objectId);
	VN_LOG_INFO("CProtectMask belong to  " << this->m_ownerId);
}

void CProtectMask::init()
{
	//valid input Check
	m_game->cell_set(m_position, m_radius, this);
}



void CProtectMask::Init(TiXmlElement* e)
{

}


void CProtectMask::update(f32 deltaTime)
{
	if (m_survivalTime <= 0)
	{
		//Ïú»Ù
		auto pkt = vnnew pkt::res_destoryObject();
		pkt->data.objectId = m_objectId;

		VN_LOG_DEBUG("destory ptotectMask id " << m_objectId);
		m_game->removeObject(m_objectId);
		m_game->broadcastPackets().push_back(pkt);
	}
	m_survivalTime -= deltaTime;

}



void CProtectMask::snapshot(pkt::Game &game)
{
	game.protectmasks.push_back({});
	auto &info = game.protectmasks.back();
	info.objectId = m_objectId;
	info.ownerId = m_ownerId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}

void CProtectMask::fini()
{
	m_game->cell_set(m_position, m_radius, (CProtectMask *)nullptr);
}