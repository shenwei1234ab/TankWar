//
//  PublicLink.cpp
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "PublicLink.h"
#include "LinkManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Room.h"
#include "vnLog.h"
#include <vector>
#include <mutex>

void PublicLink::_init() 
{
    LinkManager::instance().add(this);
    m_peer = this->peer();
}





//����
void PublicLink::_destroy() 
{
    LinkManager::instance().remove(this);
    m_playerLock.lock();
    m_roomLock.lock();
    if (m_player) {
        m_player->async_disconnected();
    }
    if (m_room) {
        m_room->async_disconnected(m_room_seat);
    }
    m_login = false;
    m_roomLock.unlock();
    m_playerLock.unlock();
    VN_LOG_INFO("[public] client " << m_peer.to_string() << " disconnected");
    this->drop();
}





void PublicLink::_disconnected() {
    this->close();
}

void PublicLink::_sent(size_t bytes, void *tag) {
    switch ((SentAction)(long)tag) {
        case kSA_None:
            break;
            
        case kSA_Close:
            this->close();
            break;
    }
}

void PublicLink::_received(LinkBufferPtr buffers) {
#ifdef VN_DEBUG
    if (m_console_mode) {
        _console_recv(std::move(buffers));
        return ;
    }
#endif
    
    m_unpacking.append(std::move(buffers));
    
    while (m_unpacking.ready()) {
        try {
            u32 pid;
            m_unpacking >> DataStream::cu(pid);
#ifdef VN_DEBUG
            if (!pid) {
                m_console_mode = true;
                VN_LOG_DEBUG("[public] client " << m_peer.to_string() << " console mode");
                return ;
            }
#endif
            auto pkt = PacketFactory<PublicLink>::instance().createRequest(pid);
            if (pkt) {
                pkt->load(m_unpacking);
                pkt->process(this);
            } else {
                VN_LOG_ERROR("[public] client " << m_peer.to_string() << " unknown packet #" << pid);
                this->close();
                break;
            }
        } catch (DataStream::Exception &) {
            VN_LOG_ERROR("[public] client " << m_peer.to_string() << " invalid packet data");
            this->close();
            break;
        }
    }
}

#ifdef VN_DEBUG
void PublicLink::_console_recv(LinkBufferPtr buffers) {
    m_unpacking.append(std::move(buffers));
    while (m_unpacking.ready()) {
        try {
            u32 pid;
            m_unpacking >> DataStream::cu(pid);
            Variable_object vobj;
            vobj.load(m_unpacking);
            auto pkt = PacketFactory<PublicLink>::instance().createRequest(pid);
            if (!pkt) {
                VN_LOG_DEBUG("[public] client " << m_peer.to_string() << " invalid console pid: " << pid);
            } else {
                pkt->load(vobj);
                pkt->process(this);
            }
        } catch(DataStream::Exception &) {
            VN_LOG_DEBUG("[public] client " << m_peer.to_string() << " invalid console data");
            this->close();
        }
    }
}

void PublicLink::_console_send(const Packet &pkt, SentAction sa) {
    m_packing << DataStream::ccu(pkt.pid());
    Variable_object vobj;
    pkt.save(vobj);
    vobj.save(m_packing);
    this->send(m_packing.pack((void *)(long)sa));
}
#endif

void PublicLink::async(std::function<void()> fn) {
    this->post(std::move(fn));
}

void PublicLink::sendPacket(const Packet &pkt, SentAction sa) {
    std::unique_lock<SpinLock> lck(m_packingLock);
#ifdef VN_DEBUG
    if (m_console_mode) {
	
        _console_send(pkt, sa);
        return ;
    }
#endif
	VN_LOG_INFO("PublicLink send packet id is " << pkt.pid());
    m_packing << DataStream::ccu(pkt.pid());
    pkt.save(m_packing);
    auto buffers = m_packing.pack((void *)(long)sa);
    this->send(std::move(buffers));
}

void PublicLink::sendPackets(const Packet &head, const std::vector<PacketPtr> &packets) {
    std::unique_lock<SpinLock> lck(m_packingLock);
#ifdef VN_DEBUG
    if (m_console_mode) {
        _console_send(head);
        for (auto &i : packets) {
			VN_LOG_INFO("PublicLink send packet id is " << i->pid());
            _console_send(*i);
        }
        return ;
    }
#endif
    m_packing << DataStream::ccu(head.pid());
    head.save(m_packing);
    for (auto &i : packets) {
        m_packing << DataStream::ccu(i->pid());
        i->save(m_packing);
    }
    auto buffers = m_packing.pack();
    this->send(std::move(buffers));
}

void PublicLink::setPlayer(Player *player) {
    m_playerLock.lock();
    m_player = player;
    if (player && !m_login) {
        player->async_disconnected();
    }
    m_playerLock.unlock();
}

void PublicLink::setRoom(Room *room, size_t seat) {
    m_roomLock.lock();
    m_room = room;
    m_room_seat = seat;
    if (room && !m_login) {
        room->async_disconnected(seat);
    }
    m_roomLock.unlock();
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_login) {
    if (m_login) {
        VN_LOG_ERROR("[public] client " << m_peer.to_string() << " invalid login");
        this->close();
        return ;
    }
    m_login = true;   
    //把回调函数post 到DBManager线程中,通过AsyncObjectPtr obj再post回传给LinkPool线程回调
    DBManager::instance().getPlayerId(std::move(pkt->data.ppId), {this, true}, [this](bool _new, u32 playerId){
    	//以下函数由本线程（thread3:LinkPool）回调,..
        auto player = PlayerManager::instance().get(playerId);
        if (_new) {
            VN_LOG_INFO("[public] client " << m_peer.to_string() << " new player #" << playerId);
            player->async_loginNew({this, true});
            return ;
        }
        VN_LOG_INFO("[public] client " << m_peer.to_string() << " login player #" << player->playerId());
        player->async_login({this, true});
        int i=1;
        i++;
    });
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_getPlayerBaseInfo) {
   //这里为什么要上锁
	std::unique_lock<SpinLock> lck(m_playerLock);
    if (!m_player) {
        this->close();
        return ;
    }
	//post    到PlayerManager线程中
    m_player->async_sendPlayerBaseInfo();
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_setPlayerName) {
    std::unique_lock<SpinLock> lck(m_playerLock);
    if (!m_player) {
        this->close();
        return ;
    }
    m_player->async_setName(std::move(pkt->data.playerName));
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_createDemoRoom) {
    std::unique_lock<SpinLock> lck(m_playerLock);
    if (!m_player) {
        this->close();
        return ;
    }
    m_player->async_createDemoRoom();
}




VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_endDemoGame) {
    std::unique_lock<SpinLock> lck(m_roomLock);
    if (!m_room) {
        return ;
    }
    m_room->async_endDemoGame();
}






VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_exitRoom) {
	std::unique_lock<SpinLock> lck(m_roomLock);
	if (m_room) {
		m_room->async_exit(m_room_seat);
		return ;
	}
	this->sendPacket(pkt::res_exitRoomFailed());
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_enterRoom) {
	m_roomLock.lock();
	if (m_room) {
		m_roomLock.unlock();
		this->sendPacket(pkt::res_enterRoomFailed());
		return ;
	}
    m_roomLock.unlock();
	std::unique_lock<SpinLock> lck(m_playerLock);
	if (!m_player) {
        this->close();
		return ;
	}
	m_player->async_enterRoom(std::move(pkt->data.roomName));
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_startGame) {
    std::unique_lock<SpinLock> lck(m_roomLock);
    if (m_room && m_room_seat == 0) {
		//VN_LOG_INFO("StartGame" );
        m_room->async_startGame();
        return ;
    }
    this->sendPacket(pkt::res_startGameFailed());
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_backGame) {
    m_roomLock.lock();
    if (m_room) {
        m_roomLock.unlock();
        this->sendPacket(pkt::res_backGameFailed());
        return ;
    }
    m_roomLock.unlock();
    std::unique_lock<SpinLock> lck(m_playerLock);
    if (!m_player) {
        this->close();
        return ;
    }
    m_player->async_backGame();
}





VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_tankMove) {
    std::unique_lock<SpinLock> lck(m_roomLock);
    if (!m_room) {
        return ;
    }
	VN_LOG_INFO("req_tankMove");
	//VN_LOG_INFO("req_tankMove data" << pkt->data);
    m_room->async_moveTank(m_room_seat, pkt->data);
}


VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_tankStop) {
    std::unique_lock<SpinLock> lck(m_roomLock);
    if (!m_room) {
        return ;
    }
    m_room->async_stopTank(m_room_seat);
}

VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_tankCommonAttack) {
    std::unique_lock<SpinLock> lck(m_roomLock);
    if (!m_room) {
        return ;
    }
    m_room->async_fireCommonAttack(m_room_seat, pkt->data);
}



VN_IMPL_PACKET_REQUEST2(PublicLink, pkt, req_tankPowerAttack) {
	std::unique_lock<SpinLock> lck(m_roomLock);
	if (!m_room) {
		return;
	}
	m_room->async_firePowerAttack(m_room_seat, pkt->data);
}




