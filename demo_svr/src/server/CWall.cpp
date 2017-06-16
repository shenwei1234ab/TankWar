#include "CWall.h"
#include "Game.h"
#include "tinyxml.h"
CWall::CWall(Game *_game, const vector2f &_pos, const vector2i & _radius,u32 _hp)
: GameStaticEntity(_game, _pos, _radius), m_hp(_hp)
{
	
}

CWall::CWall(Game *_game)
: GameStaticEntity(_game)
{

}


//先调用addObject然后调用init();
void CWall::init()
{
	//设置周围的cell信息
	m_game->cell_set(m_position, m_radius, this);
}

void CWall::Init(TiXmlElement* e)
{
	e->QueryFloatAttribute("x", &m_position.x);
	e->QueryFloatAttribute("y", &m_position.y);
	e->QueryIntAttribute("w", &m_radius.x);
	e->QueryIntAttribute("h", &m_radius.y);
	e->QueryUnsignedAttribute("hp", &m_hp);
}


void CWall::snapshot(pkt::Game &game)
{
	game.walls.push_back({});
	auto &info = game.walls.back();
	info.objectId = m_objectId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}

void  CWall::fini()
{
	//设置周围的cell信息
	m_game->cell_set(m_position, m_radius, (GameEntity *)nullptr);
}