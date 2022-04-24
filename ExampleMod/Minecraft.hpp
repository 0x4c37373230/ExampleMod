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

class BlockLegacy
{
public:
	auto getFullName() const
	{
		return *(std::string*)((VA)this + 112);
	}

	auto getBlockItemId() const
	{
		short v3 = *(short*)((VA)this + 268);

		if (v3 < 0x100)
			return v3;

		return (short)(255 - v3);
	}
};

class BlockPos 
{
public:
	int x;
	int y;
	int z;

	INT32* getPosition() const 
	{
		return reinterpret_cast<INT32*>(reinterpret_cast<VA>(this));
	}
};

class Block 
{
public:
	const BlockLegacy* getLegacyBlock() const 
	{
		return (BlockLegacy*)(**((VA**)this + 2));
	}
};

class BlockActor 
{
public:
	Block* getBlock() 
	{
		return *reinterpret_cast<Block**>(reinterpret_cast<VA>(this) + 16);
	}

	BlockPos* getPosition() 
	{
		return reinterpret_cast<BlockPos*>(reinterpret_cast<VA>(this) + 44);
	}
};

class BlockSource 
{
public:
	Block* getBlock(const BlockPos* blkpos) 
	{
		return SYMCALL(Block *, MD5_b39e5e5d8d859123e0149e23a9a7460b, this, blkpos);
	}
};

struct Vec3 
{
	float x;
	float y;
	float z;
};

class Actor 
{
public:
	BlockSource* getRegion() 
	{
		return *reinterpret_cast<BlockSource**>(reinterpret_cast<VA>(this) + 414*sizeof(void*));
	}

	int getDimension() 
	{
		return *reinterpret_cast<int*>(reinterpret_cast<VA>(this) + 204);
	}

	const std::string* getNameTag() const 
	{
		return SYMCALL(const std::string *, MD5_7044ab83168b0fd345329e6566fd47fd, this);
	}

	const byte isStand() 
	{
		return *reinterpret_cast<byte *>(reinterpret_cast<VA>(this) + 416);
	}

	Vec3* getPos() 
	{
		return SYMCALL(Vec3 *, MD5_cecb628ba58436266fb3ca43d5d88fdc, this);
	}
};

class Mob
	:Actor 
{};

class Player
	:Mob 
{};

class GameMode 
{};

class ContainerManagerModel 
{
public:
	Player* getPlayer() 
	{
		return *reinterpret_cast<Player**>(reinterpret_cast<VA>(this) + 8);
	}
};
class LevelContainerManagerModel
	:ContainerManagerModel 
{};

class ItemStack 
{
public:
	int getId() 
	{
		return SYMCALL(short,
			MD5_20bbe6b0f3ccd51bca5e7f128592acaf,
			this);
	}

	std::string getName() 
	{
		std::string str;

		SYMCALL(VA, MD5_6d581a35d7ad70fd364b60c3ebe93394, this, &str);

		return str;
	}

	int getCount() 
	{
		return *((char*)this + 34);
	}
};

class LevelContainerModel 
{
public:
	Player* getPlayer() 
	{
		return ((Player**)this)[26];
	}
};