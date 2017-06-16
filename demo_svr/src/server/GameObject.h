//
//  GameObject.h
//  demo_svr
//
//  Created by Wayne on 7/21/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__GameObject__
#define __demo_svr__GameObject__

#include "vnRefCounted.h"

#include "GameCommon.h"

using namespace vn;

class Game;
class TiXmlElement;
class GameObject : public RefCounted {
public:
	GameObject(){};
    GameObject(Game *_game);
   
	GameObject(const GameObject &);
    
    virtual void update(f32 deltaTime) {}
    
    virtual void snapshot(pkt::Game &game) {}
    
    u32 objectId() const { return m_objectId; }
    
    virtual void init() 
	{

	}
    
	virtual void fini() {}
	
	//ÃÌº”xml∂¡»Î
	virtual void Init(TiXmlElement* e);
protected:
    Game *m_game;
    u32 m_objectId;
};

typedef RefCountedPtr<GameObject> GameObjectPtr;

#endif /* defined(__demo_svr__GameObject__) */
