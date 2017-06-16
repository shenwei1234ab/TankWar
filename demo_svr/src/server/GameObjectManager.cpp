#include "GameObjectManager.h"
#include "stdafx.h"
#include "CProtectMask.h"
#include "CWall.h"
#include "GameTank.h"
#include "Game.h"
#include "CRiver.h"
#include "CGrass.h"

GameObject *  GameObjectManager::CreateElement(const  str8  &name, Game * pGame)
{
	if (name == "CWall")
	{
		GameObject *pCWall = vnnew CWall(pGame);
		return pCWall;
	}
	else if (name == "CRiver")
	{
		GameObject *pCRiver = vnnew CRiver(pGame);
		return pCRiver;
	}
	else if (name == "CGrass")
	{
		GameObject *pCGrass = vnnew CGrass(pGame);
		return pCGrass;
	}
	return nullptr;
}

void GameObjectManager::createGameObjectToMap(TiXmlElement* root, Game *pGame)
{
	if (!root)
	{
		VN_LOG_INFO("root is null");
	}
	GameObject* p = CreateElement(root->ValueStr(), pGame);
	if (!p)
	{
		VN_LOG_INFO("Unknown Element");
	}
	if (p)
	{
		p->Init(root);
		pGame->_addObject(p);
	}
	for (TiXmlElement *xml = root->FirstChildElement(); xml; xml = xml->NextSiblingElement())
	{
		createGameObjectToMap(xml, pGame);
	}
}