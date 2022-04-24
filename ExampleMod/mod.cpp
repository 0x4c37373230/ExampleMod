#include "pch.h"
#include "mod.h"
#include "Minecraft.hpp"
#include "SymHook.hpp"
#include "./termcolor/termcolor.hpp"

void mod_init() 
{
	std::cout << "Example Mod has been successfully injected\n";
}

void mod_exit() 
{
}

// This function is a replica of 'PistonBlockActor::getCorrectArmBlock'. Keep in mind the offset (this + 200) may need to be updated along with the RVAs
std::string getCorrectArmBlock(void* pistonThis)
{
	if (*(static_cast<BYTE*>(pistonThis) + 200))
		return "Sticky";
	return "Regular";
}

void pistonLogger(void* _pistonThis, char state, BlockPos* pistonCoords)
{
	std::cout
		<< "Piston Action\n" << termcolor::bright_green
		<< "\tPiston Type: " << termcolor::bright_magenta << getCorrectArmBlock(_pistonThis) << termcolor::bright_green << std::endl;

	switch (state)
	{
	case 'e':
		std::cout << "\tPiston Action: " << termcolor::bright_magenta << "Extending" << std::endl;
		break;
	case 'r':
		std::cout << "\tPiston Action: " << termcolor::bright_magenta << "Retracting" << std::endl;
		break;
	case 'm':
		std::cout << "\tPiston Action: " << termcolor::bright_magenta << "Moved" << std::endl;
		break;
	default:
		std::cout << "\tPiston Action: " << termcolor::bright_magenta << "Updated" << std::endl;
	}

	std::cout << termcolor::bright_green
		// Calls PistonBlockActor::_hasBlocksAttached
		<< "\tBlocks Attached?: " << termcolor::bright_magenta << SYMCALL(bool, MD5_8ea3c476e8f3596d47f45dcc40e7e5ce, _pistonThis, pistonCoords) << termcolor::bright_green
		<< "\tAt: " << termcolor::bright_magenta << pistonCoords->x << ", " << pistonCoords->y << ", " << pistonCoords->z << termcolor::reset << std::endl;
}

// Hooks into PistonBlock::canSurvive
THook(bool, MD5_52ca93bd7ea30062b1258c8d2c9d842f, void* _this, BlockSource* a2, BlockPos* a3)
{
	// Converts the PistonBlock* into a PistonBlockActor* by calling BlockSource::getBlockEntity
	auto pBlockActor{ SYMCALL(BlockActor*, MD5_638cb3beca85bc61a794f129df3ea53f, a2, a3) };

	// Calls PistonBlockActor::isExtending
	if (SYMCALL(bool, MD5_1a3dadff7530cb78717ef6ade0566ab1, pBlockActor))
		pistonLogger(pBlockActor, 'e', a3);
	// Calls PistonBlockActor::isRetracting
	else if (SYMCALL(bool, MD5_a98435288228c1d7b6dad513eaecdb78, pBlockActor))
		pistonLogger(pBlockActor, 'r', a3);
	// Calls PistonBlockActor::isMoving
	else if (SYMCALL(bool, MD5_8ea3c476e8f3596d47f45dcc40e7e5ce, pBlockActor))
		pistonLogger(pBlockActor, 'm', a3);
	else
		pistonLogger(pBlockActor, 'u', a3);

	return original(_this, a2, a3);
}