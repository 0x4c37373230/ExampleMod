#pragma once

#include "pch.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "Minecraft.hpp"
#include "./yaml/Yaml.hpp"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING // I am using C++14 and I'm lazy  // NOLINT(clang-diagnostic-reserved-macro-identifier, bugprone-reserved-identifier)
#include <experimental/filesystem>

class ConfigManager
{
	bool mLogAll{ true };
	bool mLogArm{ true };
	bool mLogUpdates{ true };
	bool mLogMovement{ true };
	bool mDeleteBlocksOnExtension{ false };
	int mPistonsToIgnore{};
	std::vector<BlockPos> mIgnoreList{};

	Yaml::Node mRoot;

	bool shouldLogAll() const { return mLogAll; }
public:
	ConfigManager() 
	{
		if (!std::experimental::filesystem::exists("./plugins/RtP"))
		{
			CreateDirectoryA("./plugins/RtP", nullptr);

			std::ofstream cfgFile("./plugins/RtP/RtP-cfg.txt");

			cfgFile << "logAll: true\nlogArm: true\nlogUpdates : true\nlogMovement: true\ndeleteBlocksOnExtension : false";
			cfgFile.close();
		}
		else
		{
			Parse(mRoot, "./plugins/RtP/RtP-cfg.txt");

			mLogAll = mRoot["logAll"].As<bool>();
			mLogArm = mRoot["logArm"].As<bool>();
			mLogUpdates = mRoot["logUpdates"].As<bool>();
			mLogMovement = mRoot["logMovement"].As<bool>();
			mDeleteBlocksOnExtension = mRoot["deleteBlocksOnExtension"].As<bool>();
			mPistonsToIgnore = mRoot["pistonsToBeIgnored"].As<int>();

			if (mPistonsToIgnore)
			{
				for (int i = 0; i < mPistonsToIgnore; i++)
				{
					mIgnoreList.push_back({ mRoot["ignorePistons"][i]["x"].As<int>(),
											mRoot["ignorePistons"][i]["y"].As<int>(),
											mRoot["ignorePistons"][i]["z"].As<int>() });
				}
			}
		}
	}

	bool shouldLogArm() const { return mLogArm || mLogAll; }
	bool shouldLogUpdates() const { return mLogUpdates || mLogAll; }
	bool shouldLogMovement() const { return mLogMovement || mLogAll; }
	bool shouldDeleteBlocks() const { return mDeleteBlocksOnExtension; }
	bool willIgnore() const { return mPistonsToIgnore;  }
	void pistonsToIgnore() const 
	{
		for (int i = 0; i < mPistonsToIgnore; i++)
		{
			std::cout << "\t" << mIgnoreList[i].x << ", " << mIgnoreList[i].y << ", " << mIgnoreList[i].z << "\n";
		}
	}
	/**
	 * \brief Determines whether certain piston events should be ignored by comparing piston positions
	 * \param pistonPos Position of the piston that did something
	 * \return a boolean: true if the piston event should be ignored, false if it should be logged
	 */
	bool shouldIgnore(const BlockPos* pistonPos) const 
	{ 
		for (int i = 0; i < mPistonsToIgnore; i++) 
		{
			if (pistonPos->x == mIgnoreList[i].x && pistonPos->y == mIgnoreList[i].y && pistonPos->z == mIgnoreList[i].z)
				return true;
		} 

		return false;
	}
};
