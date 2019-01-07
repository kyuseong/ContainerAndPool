// ObjectBank.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#define ASSERT(a, b)

#include "src/ObjectPool.h"
#include "src/FixedContainer.h"
#include "src/PtrList.h"
#include "src/Profiler.h"

#include "Duck.h"

#include <assert.h>

class MemoryLeakCheaker
{
public:
	MemoryLeakCheaker()
	{

		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	}
	~MemoryLeakCheaker()
	{
		_CrtDumpMemoryLeaks();
	}
};

MemoryLeakCheaker t;

int main()
{
	ReusableObjectPool<Duck, SpinLock> pool;

	cPtrList< Duck, 30000 > Table;
	
	for(int i=0;i<3000;++i)
		Table.Add(pool.Allocate());
	
	for (int i= 0; i < 1000; ++i)
	{
		for (auto Obj : Table)
		{
			Table.Add(pool.Allocate());
		}
	}

	return 0;
}
