#include "stdafx.h"
#include "CPropertyFactory.h"
#include "Game.h"
#include "CGameHelper.h"
#include "CSpeedUpProperty.h"
#include "CMoney.h"

CProperty * CPropertyFactory::produceProperty(Game *pGame)
{
	if (!pGame || m_functions.size() <= 0)
	{
		return nullptr;
	}
	int randNum = CGameHelper::generatorImmediateNumber(m_functions.size());
	CProperty *pProperty = nullptr;
	pProperty = m_functions[randNum](pGame);
	if (!pProperty)
	{
		return pProperty;
	}
	//遍历地图查找可以生成道具的位置
	std::vector<vector2i> poslist;
	poslist.reserve(pGame->m_size.x * pGame->m_size.y);
	for (int i = 0; i < pGame->m_size.x; i++)
	{
		for (int j = 0; j < pGame->m_size.y; j++)
		{
			vector2i pos(i, j);
			if (pGame->any_check(pos, pProperty->getRadius()))
			{
				poslist.push_back(pos);
			}
		}
	}
	if (poslist.size() <= 0)
	{
		return nullptr;
	}
	//随即生成位置
	int randInt = CGameHelper::generatorImmediateNumber(poslist.size());
	if (randInt < 0)
	{
		randInt = 0;
	}
	if (randInt >= (int)poslist.size())
	{
		randInt =(int) poslist.size()-1;
	}
	vector2i randIntVec = poslist[randInt];
	vector2f randFloatVec(randIntVec.x + 0.1, randIntVec.y + 0.1);
	pProperty->setPosition(randFloatVec);
	if (pGame->_addObject(pProperty))
	{
		return pProperty;
	}
	return nullptr;
}



void CPropertyFactory::registFun(std::function<CProperty*(Game *) > _fun)
{
	VN_LOG_DEBUG("push_back");
	m_functions.push_back(std::move(_fun));
}