#include "pch.h"
#include "mod.h"
#include "Minecraft.hpp"
#include "SymHook.hpp"
#include "ConfigManager.hpp"
#include "EventsManager.hpp"
#include "./termcolor/termcolor"

// ReSharper disable once CppInconsistentNaming
#define that void* THAT  // NOLINT(bugprone-macro-parentheses)

const ConfigManager CFG_MAN{};

void modInit() 
{
	std::cout << "================================================\n";
	std::cout << "Runtime Pistorder has been successfully injected\n";
	std::cout << "\tLog Arm Updates: " << CFG_MAN.shouldLogArm() << std::endl;
	std::cout << "\tLog Updates: " << CFG_MAN.shouldLogUpdates() << std::endl;
	std::cout << "\tLog Movement: " << CFG_MAN.shouldLogMovement() << std::endl;
	std::cout << "\tBlock Deletion on Extension: " << CFG_MAN.shouldDeleteBlocks() << std::endl;
	std::cout << "\tShould Ignore Some Pistons?: " << CFG_MAN.willIgnore() << std::endl;
	std::cout << "\tPistons to Ignore: " << std::endl;
	CFG_MAN.pistonsToIgnore();
	std::cout << "================================================\n";
}

void modExit() 
{
}

// This function is a replica of 'PistonBlockActor::getCorrectArmBlock'. Keep in mind the offset (this + 200) may need to be updated along with the RVAs
/**
 * \brief Determines the piston type
 * \param THAT PistonBlockActor this pointer 
 * \return Piston type, can be either "Sticky" or "Regular"
 */
std::string getCorrectArmBlock(that)
{
	if (*(static_cast<BYTE*>(THAT) + 200))
		return "Sticky";
	return "Regular";
}

/**
 * \brief Outputs useful piston information
 * \param THAT PistonBlockActor this pointer
 * \param state Whether the piston is extending, or moving
 * \param pistonCoords Piston position in the world
 */
void pistonLogger(that, const char state, BlockPos* pistonCoords)
{
	if (CFG_MAN.shouldIgnore(pistonCoords))
		return;

	std::cout
		<< "Piston Action\n" << termcolor::bright_green
		<< "\tPiston Type: " << termcolor::bright_magenta << getCorrectArmBlock(THAT) << termcolor::bright_green << std::endl;

	std::cout << "\tPiston Action: " << termcolor::bright_magenta;

	switch (state)
	{
	case 'e':
		std::cout << "Extending";
		break;
	case 'r':
		std::cout << "Retracting";
		break;
	case 'm':
		std::cout << "Moved";
		break;
	default:
		std::cout << "Updated";
	}

	std::cout << std::endl << termcolor::bright_green
		// Calls PistonBlockActor::_hasBlocksAttached
		<< "\tBlocks Attached?: " << termcolor::bright_magenta << SYMCALL(bool, MD5_8ea3c476e8f3596d47f45dcc40e7e5ce, THAT, pistonCoords) << std::endl << termcolor::bright_green
		<< "\tAt: " << termcolor::bright_magenta << pistonCoords->x << ", " << pistonCoords->y << ", " << pistonCoords->z << std::endl << termcolor::bright_green
		<< "\tTime Since Last Event: " << termcolor::bright_magenta << getTimeSinceLastEvent() << "gt" << termcolor::reset << std::endl;
}

// Hooks into PistonBlockActor::_checkAttachedBlocks
THook(char, MD5_d7a00ac2d9adec414a3662b847fe2d7c, that, BlockSource* a2)
{
	if (CFG_MAN.shouldDeleteBlocks())
		return '1';

	return original(THAT, a2);
}

// Hooks into PistonBlock::canSurvive
THook(bool, MD5_52ca93bd7ea30062b1258c8d2c9d842f, that, BlockSource* a2, BlockPos* a3)
{
	// Converts the PistonBlock* into a PistonBlockActor* by calling BlockSource::getBlockEntity
	const auto pBlockActor{ SYMCALL(BlockActor*, MD5_0adb196a370e919a5ed2f18aad3074e3, a2, a3) };

	// Calls PistonBlockActor::isExtending
	if (SYMCALL(bool, MD5_1a3dadff7530cb78717ef6ade0566ab1, pBlockActor) && CFG_MAN.shouldLogArm())
		pistonLogger(pBlockActor, 'e', a3);
	// Calls PistonBlockActor::isRetracting
	else if (SYMCALL(bool, MD5_a98435288228c1d7b6dad513eaecdb78, pBlockActor) && CFG_MAN.shouldLogArm())
		pistonLogger(pBlockActor, 'r', a3);
	// Calls PistonBlockActor::isMoving
	else if (SYMCALL(bool, MD5_8ea3c476e8f3596d47f45dcc40e7e5ce, pBlockActor) && CFG_MAN.shouldLogMovement())
		pistonLogger(pBlockActor, 'm', a3);
	else if (CFG_MAN.shouldLogUpdates())
		pistonLogger(pBlockActor, 'u', a3);

	return original(THAT, a2, a3);
}