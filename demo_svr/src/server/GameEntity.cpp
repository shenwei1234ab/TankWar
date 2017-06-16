//
//  GameEntity.cpp
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "GameEntity.h"
#include "Game.h"

GameEntity::GameEntity(Game *_game, const vector2f &_position, const vector2i &_radius)
: GameObject(_game)
, m_position(_position)
, m_radius(_radius) {
    
}



void GameEntity::init() 
{
    m_game->cell_set(vector2i(m_position), m_radius, this);
}

void GameEntity::fini() {
    m_game->cell_set(vector2i(m_position), m_radius, (GameEntity *)nullptr);
}