//
//  GameObject.cpp
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "GameObject.h"
#include "Game.h"
#include "tinyxml.h"
GameObject::GameObject(Game *_game)
: m_game(_game)
, m_objectId(_game->generateObjectId()) {
    
}

GameObject::GameObject(const GameObject &rhs)
: m_game(rhs.m_game)
, m_objectId(rhs.m_game->generateObjectId()) {
    
}


void GameObject::Init(TiXmlElement* e)
{
	
}
