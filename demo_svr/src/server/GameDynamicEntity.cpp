//
//  GameDynamicEntity.cpp
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "GameDynamicEntity.h"
#include "Game.h"




//false:碰撞到其他cell不移动
//true:没有碰撞到其他cell物体

bool GameDynamicEntity::_updateMovement(f32 deltaTime) {
    auto newPos = m_position + m_velocity * deltaTime;
    //检查是否碰撞到其他gameEntity,或者越界。
    if (!m_game->cell_check(vector2i(newPos), m_radius, this)) {
        return false;
    }
    m_game->cell_set(vector2i(m_position), m_radius, (GameEntity*)nullptr);
    
    m_position = newPos;
    
    m_game->cell_set(vector2i(m_position), m_radius, this);
    
    return true;
}



bool GameDynamicEntity::canMove(const vector2f &newPos)
{
	//碰撞到了实体或者出界
	if (!m_game->cell_check(vector2i(newPos), m_radius, this)) 
	{
		return false;
	}
	return true;
}



void GameDynamicEntity::_Move(const vector2f &newPos)
{
	m_game->cell_set(vector2i(m_position), m_radius, (GameEntity*)nullptr);

	m_position = newPos;

	m_game->cell_set(vector2i(m_position), m_radius, this);
}


