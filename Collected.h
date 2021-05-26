#pragma once
#include<stack>
#include<vector>
/*
All gl buffers are basically malloced ... wich sucks ... here comes the class that counts the amount of instances
of a given buffer (or more precise object in carge of buffer/s)  and provides a virtual "clear" function wich becomes
kinda the destructor
*/

// Vector Safe

class Collected
{
public:
	Collected();
	Collected(const Collected& other);
	Collected& operator=(const Collected& other);
	void dtor();

	virtual void clear() = 0;
private:
	unsigned _idx;
	static std::stack<unsigned> _s_free_spaces;
	static std::vector<unsigned> _s_counts;
};