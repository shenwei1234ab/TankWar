//
//  GamePlayer.h
//  demo_svr
//
//  Created by Wayne on 7/22/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GamePlayer__
#define __demo_svr__GamePlayer__

#include "GameTank.h"

class GamePlayer : public GameTank
{
public:
	GamePlayer(Game *_game, const vector2f &_pos, game::Direction _dir, f32 _speed, size_t _playerSeat,u32 _hp);

	virtual void snapshot(pkt::Game &game);

	size_t playerSeat() const { return m_playerSeat; }


	unsigned int getScore()const
	{
		return m_score;
	}

	void setScore(unsigned int _score)
	{
		m_score = _score;
	}

	void addScore(unsigned int _score)
	{
		m_score += _score;
	}

protected:
    size_t m_playerSeat = 0;
	unsigned int m_score = 0;
};

typedef RefCountedPtr<GamePlayer> GamePlayerPtr;

#endif /* defined(__demo_svr__GamePlayer__) */
