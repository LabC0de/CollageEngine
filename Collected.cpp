#include "Collected.h"

Collected::Collected()
{
	_idx = _s_free_spaces.top();
	// printf("\t[!] Created idx: %u\n", _idx);
	if (_s_free_spaces.size() > 1)
	{
		_s_free_spaces.pop();
		_s_counts.at(_idx) = 1;
	}
	else
	{
		++_s_free_spaces.top();
		_s_counts.push_back(1);
	}
}

Collected::Collected(const Collected& other)
{
	if (this != &other)
	{
		_idx = other._idx;
		++_s_counts.at(_idx);
		//printf("\t[!] Count idx: %u -> %u\n", _idx, _s_counts.at(_idx));
	}
}

Collected& Collected::operator=(const Collected& other)
{
	if (this != &other)
	{
		dtor();
		_idx = other._idx;
		++_s_counts.at(_idx);
		//printf("\t[!] Count idx: %u -> %u\n", _idx, _s_counts.at(_idx));
	}
	return *this;
}

void Collected::dtor()
{
	if (!--_s_counts.at(_idx))
	{
		_s_free_spaces.push(_idx);
		this->clear();
		//printf("\t[!] Collected idx: %u\n", _idx);
	}
}

std::stack<unsigned> Collected::_s_free_spaces = std::stack<unsigned>({ 0 });
std::vector<unsigned> Collected::_s_counts = std::vector<unsigned>();
