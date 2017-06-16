#ifndef __GameObjectManager__
#define __GameObjectManager__
#include "GameObject.h"
class TiXmlElement;
class Game;
class GameObjectManager  {
public:
	GameObject *  CreateElement(const str8 &name, Game * pGame);
	 void createGameObjectToMap(TiXmlElement* root, Game *pGame);
};

#endif /* defined(__demo_svr__DBManager__) */
