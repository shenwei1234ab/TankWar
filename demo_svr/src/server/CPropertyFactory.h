#ifndef _CPropertyFactory_H_
#define _CPropertyFactory_H_
#include <functional>
class Game;
class CProperty;
class CPropertyFactory 
{
public:
	static CPropertyFactory & getInstance()
	{
		static CPropertyFactory s_propertyFactory;
		return s_propertyFactory;
	}

	CProperty * produceProperty(Game *pGame);

	void registFun(std::function<CProperty*(Game *) > _fun);

	CPropertyFactory()
	{
		m_functions.reserve(10);
	}
protected:
	std::vector<std::function<CProperty*(Game * ) >> m_functions;
};
#endif