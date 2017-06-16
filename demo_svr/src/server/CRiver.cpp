#include "CRiver.h"
#include "Game.h"
#include "tinyxml.h"
CRiver::CRiver(Game *_game, const vector2f &_pos, const vector2i & _radius, u32 _hp)
: GameStaticEntity(_game, _pos, _radius), m_hp(_hp)
{

}

CRiver::CRiver(Game *_game)
: GameStaticEntity(_game)
{

}


//�ȵ���addObjectȻ�����init();
void CRiver::init()
{
	//������Χ��cell��Ϣ
	m_game->cell_set(m_position, m_radius, this);
}

void CRiver::Init(TiXmlElement* e)
{
	e->QueryFloatAttribute("x", &m_position.x);
	e->QueryFloatAttribute("y", &m_position.y);
	e->QueryIntAttribute("w", &m_radius.x);
	e->QueryIntAttribute("h", &m_radius.y);
	//e->QueryUnsignedAttribute("hp", &m_hp);
}


void CRiver::snapshot(pkt::Game &game)
{
	game.rivers.push_back({});
	auto &info = game.rivers.back();
	info.objectId = m_objectId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}

void  CRiver::fini()
{
	//������Χ��cell��Ϣ
	m_game->cell_set(m_position, m_radius, (GameEntity *)nullptr);
}