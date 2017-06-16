#ifndef __CGameHelper__
#define __CGameHelper__
#include <time.h>
#include <ctime>
#include <cstdlib>
class CGameHelper
{
public:
	//generate random int ([0,_maxNum))
	static int  generatorImmediateNumber(int _maxNum)
	{
		if (_maxNum <= 0)
		{
			return 0;
		}
		srand(time(NULL));
		return rand() % _maxNum;
	}

	//generate random float (0 - 0.9)
	static float generatorImmediateFloat()
	{
		return generatorImmediateNumber(9)*0.1;
	}
    
protected:
    
};

#endif /* defined(__demo_svr__GameEntity__) */
