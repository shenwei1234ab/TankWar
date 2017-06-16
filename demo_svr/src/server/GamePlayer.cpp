//
//  GamePlayer.cpp
//  demo_svr
//
//  Created by Wayne on 7/22/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "GamePlayer.h"

GamePlayer::GamePlayer(Game *_game, const vector2f &_pos, game::Direction _dir, f32 _speed, size_t _playerSeat, u32 _hp)
: GameTank(_game, _pos, _dir, _speed, _hp)
, m_playerSeat(_playerSeat) {
    
}

void GamePlayer::snapshot(pkt::Game &game) {
    game.players.push_back({});
    auto &player = game.players.back();
    player.seat = (unsigned int)m_playerSeat;
    this->_snapshot(player.tank);
}