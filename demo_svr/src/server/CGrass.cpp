#include "CGrass.h"
#include "Game.h"
#include "tinyxml.h"
CGrass::CGrass(Game *_game, const vector2f &_pos, const vector2i & _radius)
: GameObject(_game),
m_position(_pos),
m_radius(_radius)
{

}

CGrass::CGrass(Game *_game)
: GameObject(_game)
{

}


//先调用addObject然后调用init();
void CGrass::init()
{
	//设置周围的cell信息

}

void CGrass::Init(TiXmlElement* e)
{
	e->QueryFloatAttribute("x", &m_position.x);
	e->QueryFloatAttribute("y", &m_position.y);
	e->QueryIntAttribute("w", &m_radius.x);
	e->QueryIntAttribute("h", &m_radius.y);
	//e->QueryUnsignedAttribute("hp", &m_hp);
}


void CGrass::snapshot(pkt::Game &game)
{
	game.grasses.push_back({});
	auto &info = game.grasses.back();
	info.objectId = m_objectId;
	info.positionX = m_position.x;
	info.positionY = m_position.y;
}

void  CGrass::fini()
{
	//设置周围的cell信息
}