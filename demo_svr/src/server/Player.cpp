//
//  Player.cpp
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "Player.h"
#include "PlayerManager.h"
#include "DBManager.h"
#include "RoomManager.h"

#include "vnPacket.h"
#include "vnLog.h"

#include <vector>
#include <array>



void Player::async(std::function<void()>  fn) {
    PlayerManager::instance().post(std::move(fn));
}

void Player::async_loginNew(RefCountedPtr<PublicLink> link) {
    PlayerManager::instance().post(std::bind([this](RefCountedPtr<PublicLink> link){
        
        m_link = std::move(link);
        m_link->setPlayer(this);
        m_link->sendPacket(pkt::res_loginOk());
        
    }, std::move(link)));
}

void Player::async_login(RefCountedPtr<PublicLink> link) {
	//post 到playermanager线程中
    PlayerManager::instance().post(std::bind([this](RefCountedPtr<PublicLink> link){
    	//playermanager线程中执行，通过this获取post
    	if (m_link)
		{
            m_link->setPlayer(nullptr);
            m_link->setRoom(nullptr, 0);
            m_link->sendPacket(pkt::res_loginAnother(), PublicLink::kSA_Close);
        }
        m_link = std::move(link);
        if (m_initialized) 
		{
            if (m_room) 
			{
                m_isInRoom = false;
				VN_LOG_INFO("relogin ");
                m_room->async_relogin({this, true}, m_link);
            } 
			else 
			{
                m_link->setPlayer(this);
                m_link->sendPacket(pkt::res_loginOk());
            }
        } else {
            DBManager::instance().loadPlayerInfo(m_playerId, {this, true}, [this](bool result, DBManager::PlayerInfo &info){
                if (!result) {
                    VN_LOG_ERROR("player #" << m_playerId << " load info FAILED!");
                    m_link->sendPacket(pkt::res_loginFailed(), PublicLink::kSA_Close);
                    m_link.reset();
                    PlayerManager::instance().remove(m_playerId);
                    return ;
                }
				
                //m_playerName = std::move(info.playerName);
                m_playerName.assign(info.playerName.c_str(), info.playerName.length());
                m_initialized = true;
                m_link->setPlayer(this);
                m_link->sendPacket(pkt::res_loginOk());
            });
        }
    }, std::move(link)));

}

void Player::async_disconnected() {
    PlayerManager::instance().post([this](){
        m_link->setPlayer(nullptr);
        m_link.reset();
        m_isInRoom = false;
        if (!m_room) 
		{
            PlayerManager::instance().remove(m_playerId);
        }
    });
}

void Player::async_setName(str8 name) {
    PlayerManager::instance().post(std::bind([this](str8 &name){
        if (!m_playerName.empty()) {
            m_link->close();
            return ;
        }
        DBManager::instance().setPlayerName(m_playerId, std::move(name), {this, true}, [this](bool result, str8 &name){
            if (result) {
                m_playerName = std::move(name);
                if (m_link) {
                    m_link->sendPacket(pkt::res_setPlayerNameOk());
                }
            } else if (m_link) {
                m_link->sendPacket(pkt::res_setPlayerNameFailed());
            }
        });
    }, std::move(name)));
}

void Player::async_sendPlayerBaseInfo() {
	//post    到PlayerManager线程中
    PlayerManager::instance().post([this](){
    	//以下函数由playermanager线程回调
        pkt::res_playerBaseInfo pkt;
        pkt.data.playerName = m_playerName;
        pkt.data.inRoom = (bool)m_room;
        m_link->sendPacket(pkt);
    });
}

void Player::async_createDemoRoom() {
    PlayerManager::instance().post([this](){
        if (m_playerName.empty() || m_room) {
            m_link->sendPacket(pkt::res_createDemoRoomFailed());
            return ;
        }
        m_room = RoomManager::instance().create(m_playerName);
        if (!m_room) {
            VN_LOG_ERROR("[RoomManager] create room \"" << m_playerName << "\" failed!");
            m_link->sendPacket(pkt::res_createDemoRoomFailed());
            return ;
        }
        m_isInRoom = true;
        m_room->async_initDemo({this, true}, m_link);
        m_link->setRoom(m_room.get(), 0);
        m_link->sendPacket(pkt::res_createDemoRoomOk());
    });
}

void Player::async_enterRoom(str8 roomName) {
	PlayerManager::instance().post(std::bind([this](str8 &roomName){
		if (m_room || m_playerName.empty()) {
			m_link->sendPacket(pkt::res_enterRoomFailed());
			return ;
		}
		auto room = RoomManager::instance().find(roomName);
		if (!room) {
			m_link->sendPacket(pkt::res_enterRoomFailed());
			return ;
		}
        RoomManager::instance().enter(std::move(room), {this, true}, m_link);
	}, std::move(roomName)));
}

void Player::async_backGame() {
    PlayerManager::instance().post([this](){
        if (!m_room || m_isInRoom) {
            m_link->sendPacket(pkt::res_backGameFailed());
            return ;
        }
        m_isInRoom = true;
        m_room->async_backGame({this, true}, m_link);
    });
}

void Player::async_notify_reloginRoom(bool ok) {
    PlayerManager::instance().post([this, ok](){
        if (ok) {
            if (m_link) {
                m_link->setPlayer(this);
                m_link->sendPacket(pkt::res_loginOk());
            }
        } else {
            auto room = m_room.release();
            if (m_link) {
                m_link->setPlayer(this);
                m_link->sendPacket(pkt::res_loginOk());
            } else {
                PlayerManager::instance().remove(m_playerId);
            }
            room->drop();
        }
    });
}

void Player::async_notify_enterRoom(RefCountedPtr<Room> room) {
	PlayerManager::instance().post(std::bind([this](RoomPtr &room){
		m_room = std::move(room);
        m_isInRoom = true;
	}, std::move(room)));
}

void Player::async_notify_exitRoom() {
	PlayerManager::instance().post([this](){
		auto room = m_room.release();
		if (m_link) {
			m_link->setRoom(nullptr, 0);
            if (m_isInRoom) {
                m_link->sendPacket(pkt::res_exitRoomOk());
            }
            m_isInRoom = false;
		} else {
			PlayerManager::instance().remove(m_playerId);
		}
		room->drop();
	});
}

void Player::async_notify_destroyRoom() {
	PlayerManager::instance().post([this](){
		auto room = m_room.release();
		if (m_link) {
            m_link->setRoom(nullptr, 0);
            if (m_isInRoom) {
                m_isInRoom = false;
                pkt::res_playerExitRoom pkt;
                pkt.data.seat = 0;
                m_link->sendPacket(pkt);
            }
		} else {
			PlayerManager::instance().remove(m_playerId);
		}
		room->drop();
	});
}

void Player::async_notify_destroyRoomNoMsg() {
	PlayerManager::instance().post([this](){
        m_isInRoom = false;
		auto room = m_room.release();
		if (m_link) {
			m_link->setRoom(nullptr, 0);
		} else {
			PlayerManager::instance().remove(m_playerId);
		}
		room->drop();
	});
}
