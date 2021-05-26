#pragma once
#include <string>
#include "Object.h"


class Named : virtual public Object
{
public:
	Named() {
		register_as(TYPE::NAMED);
	}
	virtual std::string name() const { return ""; }
};

class DynamicallyNamed : public Named
{
public:
	DynamicallyNamed(const char* aname = "") : mname(aname) {}
	virtual std::string name() const override
	{
		return mname;
	}
	virtual void rename(const char* newname)
	{
		mname = newname;
	}
protected:
	DynamicallyNamed& assign(const DynamicallyNamed& other) {
		if (this != &other) mname = other.mname;
		return *this;
	}
private:
	std::string mname;
};
