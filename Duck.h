#pragma once

#include <iostream>

class Duck
{
public:
	int id;
	int * a = nullptr;
	Duck(int a) { id = a; }

	Duck()
	{
		std::cout << "Duck::Duck()\n";
		id = 0;
		a = nullptr;
	}

	~Duck()
	{
		std::cout << "Duck::~Duck()\n";
		if( a != nullptr)
			delete a;

	}

	void SetUniqueID(int i)
	{
		id = i;
	}
	int GetID() const { return id; }
};
