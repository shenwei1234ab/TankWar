//
//  Room.cpp
//  demo_svr
//
//  Created by Wayne on 7/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "Room.h"
#include "RoomManager.h"
#include "vnLog.h"
#include "GamePlayer.h"
#include "stdafx.h"
#include "tinyxml.h"
#include "GameObjectManager.h"
#include "CWall.h"

Room::Room(const str8 &_name)
: m_name(_name) {
    VN_LOG_DEBUG("[Room] create \"" << _name << "\"");
}

Room::~Room() {
    VN_LOG_DEBUG("[Room] destroy \"" << m_name << "\"");
}

void Room::async(std::function<void()> fn) {
    RoomManager::instance().post(std::move(fn));
}

void Room::async_initDemo(PlayerPtr owner, PublicLinkPtr link) {
    RoomManager::instance().post(std::bind([this](PlayerPtr owner, PublicLinkPtr link){
        m_players[0].player = std::move(owner);
        m_players[0].link = std::move(link);
        m_player_count = 1;
        m_state = kWaiting;
    }, std::move(owner), std::move(link)));
}

void Room::async_endDemoGame() {
    RoomManager::instance().post(std::bind(&Room::_endDemoGame, this));
}

void Room::async_relogin(PlayerPtr player, PublicLinkPtr link) {
    auto fn = [this](PlayerPtr &player, PublicLinkPtr &link) {
        if (m_state == kNotInit) {
            player->async_notify_reloginRoom(false);
        } else if (m_state == kWaiting) {
            size_t seat = 0;
            for (auto &info : m_players) {
                if (info.player == player) {
                    info.player.reset();
                    info.link.reset();
                    --m_player_count;
                    if (seat) {
                        pkt::res_playerExitRoom pkt;
                        pkt.data.seat = (u8)seat;
                        for (auto &i : m_players) {
                            if (i.link) {
                                i.link->sendPacket(pkt);
                            }
                        }
                    } else {
                        for (auto &i : m_players) {
                            if (i.player) {
                                VN_LOG_DEBUG("[Room] \"" << i.player->playerName() << "\" leave \"" << m_name << "\"");
                                i.player->async_notify_destroyRoom();
                            }
                        }
                        m_state = kNotInit;
                        RoomManager::instance().destroy(m_name);
                    }
                    break;
                }
                ++seat;
            }
            player->async_notify_reloginRoom(false);
        }
		else if (m_state == kFighting) 
		{
            for (auto &info : m_players)
			{
                if (info.player == player) 
				{
                    info.link.reset();
                    break;
                }
            }
            player->async_notify_reloginRoom(true);
            for (auto &i : m_players) 
			{
                if (i.link) 
				{
                    return ;
                }
            }
            _endDemoGame();
        }
    };
    RoomManager::instance().post(std::bind(fn, std::move(player), std::move(link)));
}

void Room::async_exit(size_t seat) {
	RoomManager::instance().post([this, seat](){
		auto &info = m_players[seat];
		if (!info.player) {
			return ;
		}
		if (m_state != kWaiting) {
			if (info.link) {
				info.link->sendPacket(pkt::res_exitRoomFailed());
			}
			return ;
		}
        VN_LOG_DEBUG("[Room] \"" << info.player->playerName() << "\" leave \"" << m_name << "\"");
		info.player->async_notify_exitRoom();
		info.player.reset();
		info.link.reset();
		--m_player_count;
		if (seat) {
			pkt::res_playerExitRoom pkt;
			pkt.data.seat = (u8)seat;
			for (auto &i : m_players) {
				if (i.link) {
					i.link->sendPacket(pkt);
				}
			}
		} else {
            for (auto &i : m_players) {
				if (i.player) {
                    VN_LOG_DEBUG("[Room] \"" << i.player->playerName() << "\" leave \"" << m_name << "\"");
					i.player->async_notify_destroyRoom();
				}
			}
			m_state = kNotInit;
			RoomManager::instance().destroy(m_name);
		}
	});
}


void Room::async_disconnected(size_t seat) {
    RoomManager::instance().post([this, seat](){
        auto &info = m_players[seat];
		if (!info.player || !info.link) {
			return ;
		}
        info.link->setRoom(nullptr, 0);
        info.link.reset();
		if (m_state == kFighting) {
            for (auto &i : m_players) {
                if (i.link) {
                    return ;
                }
            }
            _endDemoGame();
			return ;
		}
        VN_LOG_DEBUG("[Room] \"" << info.player->playerName() << "\" leave \"" << m_name << "\"");
		info.player->async_notify_exitRoom();
		info.player.reset();
		--m_player_count;
		if (seat) {
			pkt::res_playerExitRoom pkt;
			pkt.data.seat = (u8)seat;
			for (auto &i : m_players) {
				if (i.link) {
					i.link->sendPacket(pkt);
				}
			}
		} else {
            for (auto &i : m_players) {
				if (i.player) {
                    VN_LOG_DEBUG("[Room] \"" << i.player->playerName() << "\" leave \"" << m_name << "\"");
					i.player->async_notify_destroyRoom();
				}
			}
			m_state = kNotInit;
			RoomManager::instance().destroy(m_name);
		}
    });
}








void Room::async_startGame() 
{
    RoomManager::instance().post([this]()
	{
        if (m_state != kWaiting || m_player_count < 1) 
		{
            m_players[0].link->sendPacket(pkt::res_startGameFailed());
            return ;
        }
        static vector2f s_playersInitPos[4] = {
            {2, 2},
            {38, 2},
            {2, 38},
            {38, 38}
        };
        auto game = vnnew Game(this);
		GameObjectManager gameManager;
		//////////////////////////////////////////////////////
		//使用xml文件载入tileMap信息
		TiXmlDocument doc;
		if (doc.LoadFile("map.xml"))
		{
			gameManager.createGameObjectToMap(doc.RootElement(), game);
		}
		

        size_t seat = 0;
        for (auto &i : m_players) {
            if (i.player) {
                i.tank = game->createPlayer(s_playersInitPos[seat], game::kD_PX, 8, seat,4);
				//ceshi
				//create a protectmask
				game->createProtectMask(i.tank->objectId(), i.tank->position(), i.tank->radius()+vector2f(1,1),60.0f);
			}
            ++seat;
        }
        pkt::res_gameStart pkt;
		game->snapshot(pkt.data);
        for (auto &i : m_players) {
            if (i.link) 
			{
				VN_LOG_INFO("send packet id is "<<pkt.pid());
                i.link->sendPacket(pkt);
            }
        }
        m_game.reset(game);
        m_state = kFighting;
    });
}





void Room::async_backGame(PlayerPtr player, PublicLinkPtr link) {
    auto fn = [this](PlayerPtr &player, PublicLinkPtr &link) {
        if (m_state != kFighting) {
            return ;
        }
        size_t seat = 0;
        for (auto &i : m_players) {
            if (i.player == player) {
                i.link = std::move(link);
                i.link->setRoom(this, seat);
                break;
            }
            ++seat;
        }
        if (seat == m_players.size()) {
            return ;
        }
        pkt::res_gameBack pkt;
        pkt.data.players.reserve(m_player_count - 1);
        pkt.data.seat = (u8)seat;
        for (size_t i = 0; i < m_players.size(); ++i) {
            if (i != seat && m_players[i].player) {
                pkt.data.players.push_back({});
                auto &info = pkt.data.players.back();
                info.seat = (u8)i;
                info.playerName = m_players[i].player->playerName();
            }
        }
        m_game->snapshot(pkt.data.game);
        m_players[seat].link->sendPacket(pkt);
    };
    RoomManager::instance().post(std::bind(fn, std::move(player), std::move(link)));
}





void Room::async_moveTank(size_t seat, u32 direction) {
    RoomManager::instance().post([this, seat, direction](){
        if (m_state != kFighting) {
            return ;
        }
        auto &info = m_players[seat];
        if (!info.tank || direction >= game::kD_MAX) {
            return ;
        }
        info.tank->move((game::Direction)direction);
    });
}

void Room::async_stopTank(size_t seat) {
    RoomManager::instance().post([this, seat](){
        if (m_state != kFighting) {
            return ;
        }
        auto &info = m_players[seat];
        if (!info.tank) {
            return ;
        }
        info.tank->stop();
    });
}

void Room::async_fireCommonAttack(size_t seat, u32 direction) {
    RoomManager::instance().post([this, seat, direction](){
        if (m_state != kFighting) {
            return ;
        }
        auto &info = m_players[seat];
        if (!info.tank || direction >= game::kD_MAX) {
            return ;
        }
        info.tank->fireCommonAttack((game::Direction)direction);
    });
}



void Room::async_firePowerAttack(size_t seat, u32 direction) {
	RoomManager::instance().post([this, seat, direction](){
		if (m_state != kFighting) {
			return;
		}
		auto &info = m_players[seat];
		if (!info.tank || direction >= game::kD_MAX) {
			return;
		}
		info.tank->firePowerAttack((game::Direction)direction);
	});
}






void Room::_game_broadcast(const std::vector<PacketPtr> &packets) {
    pkt::res_gameTimeStamp head;
    head.data = m_game->time_stamp();
    for (auto &i : m_players) {
        if (i.link) {
            i.link->sendPackets(head, packets);
        }
    }
}


void Room::_update(f32 deltaTime) {
    if (m_state == kFighting) {
        m_game->update(deltaTime);
    }
}

void Room::_enter(PlayerPtr player, PublicLinkPtr link) {
	if (m_state != kWaiting || m_player_count == m_players.size()) {
		link->sendPacket(pkt::res_enterRoomFailed());
		return ;
	}
	size_t old_count = m_player_count++;
	size_t seat = 1;
	for (; seat < m_players.size(); ++seat) {
		if (!m_players[seat].player) {
			break;
		}
	}
	auto &info = m_players[seat];
	info.player = std::move(player);
	info.player->async_notify_enterRoom({this, true});
	info.link = std::move(link);
	info.link->setRoom(this, seat);
	
    VN_LOG_DEBUG("[Room] player \"" << info.player->playerName() << "\" enter \"" << m_name << "\"");
    
	pkt::res_enterRoomOk pkt0;
	pkt::res_playerEnterRoom pkt1;
	pkt0.data.seat = pkt1.data.seat = (u8)seat;
	pkt0.data.players.reserve(old_count);
	pkt1.data.playerName = info.player->playerName();
	for (size_t i = 0; old_count; ++i) {
		if (i == seat) {
			continue;
		}
        --old_count;
		pkt0.data.players.push_back({});
		auto &t = pkt0.data.players.back();
		t.seat = (u8)i;
		t.playerName = m_players[i].player->playerName();
		m_players[i].link->sendPacket(pkt1);
	}
	info.link->sendPacket(pkt0);
}

void Room::_destroy() {
	m_state = kNotInit;
    for (auto &i : m_players) {
		i.link.reset();
		if (i.player) {
			i.player->async_notify_destroyRoomNoMsg();
		}
    }
}





//游戏结束
void Room::_endDemoGame() 
{
	pkt::res_gameEnd pktGameEnd;
	uint seat = 0;
	//获取玩家的得分信息
	for (auto &i : m_players)
	{
		if (i.player)
		{
			//VN_LOG_INFO(i.player->playerName() << "getscore" << i.tank->getScore());
			pktGameEnd.data.ranks.push_back({ seat, i.tank->getScore() });

		}
		++seat;
	}
	//根据score排序
	std::sort(pktGameEnd.data.ranks.begin(), pktGameEnd.data.ranks.end(), [](const pkt::PlayerRank &lhs, const pkt::PlayerRank &rhs){
			return lhs.scores > rhs.scores;	});

    if (m_state != kFighting) {
        return ;
    }
    m_state = kWaiting;
    
	m_game.reset();
	

    for (auto &info : m_players) {
        info.tank.reset();
        if (info.link) {
            info.link->sendPacket(pktGameEnd);
        }
    }
    if (m_players[0].link) {
        for (size_t seat = 1; seat < m_players.size(); ++seat) {
            auto &info = m_players[seat];
            if (info.player && !info.link) {
                VN_LOG_DEBUG("[Room] \"" << info.player->playerName() << "\" leave \"" << m_name << "\"");
                info.player->async_notify_exitRoom();
                info.player.reset();
                --m_player_count;
                pkt::res_playerExitRoom pkt;
                pkt.data.seat = (u8)seat;
                for (auto &i : m_players) {
                    if (i.link) {
                        i.link->sendPacket(pkt);
                    }
                }
            }
        }
    } else 
{
        auto &info = m_players[0];
        VN_LOG_DEBUG("[Room] \"" << info.player->playerName() << "\" leave \"" << m_name << "\"");
        info.player->async_notify_exitRoom();
        info.player.reset();
        for (auto &i : m_players) {
            if (i.player) {
                VN_LOG_DEBUG("[Room] \"" << i.player->playerName() << "\" leave \"" << m_name << "\"");
                i.player->async_notify_destroyRoom();
            }
        }
        m_state = kNotInit;
        RoomManager::instance().destroy(m_name);
    }
}
