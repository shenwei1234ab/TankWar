//
//  PublicLink.h
//  demo_svr
//
//  Created by Wayne on 7/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__PublicLink__
#define __demo_svr__PublicLink__

#include "AsyncObject.h"

#include "vnLink.h"
#include "vnPackingStream.h"
#include "vnUnpackingStream.h"
#include "vnPacketFactory.h"

using namespace vn;

class Player;
class Room;

class PublicLink : public Link_TCP_Connection, public AsyncObject {
    VN_DECL_PACKET_HANDLER();
public:
    enum SentAction {
        kSA_None = 0,
        kSA_Close
    };
    
    virtual void async(std::function<void()> fn);
    
    void sendPacket(const Packet &pkt, SentAction sa = kSA_None);
    void sendPackets(const Packet &head, const std::vector<PacketPtr> &packets);
    
    void setPlayer(Player *player);
    void setRoom(Room *room, size_t seat);
protected:
    virtual void _init();
    virtual void _destroy();
    virtual void _disconnected();
    virtual void _sent(size_t bytes, void *tag);
    virtual void _received(LinkBufferPtr buffers);

    
    PackingStream m_packing;
    SpinLock m_packingLock;
    UnpackingStream m_unpacking;
    
    //关联上层业务对象
    Player *m_player;
    SpinLock m_playerLock;
    
    Room *m_room;
    size_t m_room_seat = 0;
    SpinLock m_roomLock;
    
    bool m_login = false;
    
private:
#ifdef VN_DEBUG
    bool m_console_mode = false;
    void _console_recv(LinkBufferPtr buffers);
    void _console_send(const Packet &pkt, SentAction sa = kSA_None);
#endif
    
    NetAddress m_peer;
};

typedef RefCountedPtr<PublicLink> PublicLinkPtr;

#endif /* defined(__demo_svr__PublicLink__) */
