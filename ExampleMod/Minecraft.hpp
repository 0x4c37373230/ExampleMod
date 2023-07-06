#pragma once

#include "SymHook.hpp"

template<typename T>
struct SharedPtr 
{
	auto get() 
	{
		return **reinterpret_cast<T***>(this);
	}
	auto operator->() 
	{
		return get();
	}
};

struct BlockPos 
{
	int x, y, z;
};

class BlockActor 
{
public:
	BlockPos* getPosition() { return reinterpret_cast<BlockPos*>(reinterpret_cast<VA>(this) + 44); }
};

class BlockSource 
{
};

struct Vec3 
{
	float x, y, z;
};

class Actor 
{
public:
	int getDimension() const { return *reinterpret_cast<int*>(reinterpret_cast<VA>(this) + 204); }
	const std::string* getNameTag() const { return SYMCALL(const std::string *, MD5_7044ab83168b0fd345329e6566fd47fd, this); }
};