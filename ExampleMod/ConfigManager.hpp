#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "pch.h"
#include "Minecraft.hpp"
#include "./yaml/Yaml.hpp"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING // I am using C++14 and I'm lazy
#include <experimental/filesystem>

class ConfigManager
{
private:
	bool mLogAll{ true };
	bool mLogArm{ true };
	bool mLogUpdates{ true };
	bool mLogMovement{ true };
	bool mDeleteBlocksOnExtension{ false };
	int mPistonsToIgnore{};
	std::vector<BlockPos> mIgnoreList{};

	Yaml::Node root;

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
			Yaml::Parse(root, "./plugins/RtP/RtP-cfg.txt");

			mLogAll = root["logAll"].As<bool>();
			mLogArm = root["logArm"].As<bool>();
			mLogUpdates = root["logUpdates"].As<bool>();
			mLogMovement = root["logMovement"].As<bool>();
			mDeleteBlocksOnExtension = root["deleteBlocksOnExtension"].As<bool>();
			mPistonsToIgnore = root["pistonsToBeIgnored"].As<int>();

			if (mPistonsToIgnore)
			{
				for (int i = 0; i < mPistonsToIgnore; i++)
				{
					mIgnoreList.push_back({ root["ignorePistons"][i]["x"].As<int>(),
											root["ignorePistons"][i]["y"].As<int>(),
											root["ignorePistons"][i]["z"].As<int>() });
				}
			}
		}
	};

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
	bool shouldIgnore(BlockPos* pistonPos) const 
	{ 
		for (int i = 0; i < mPistonsToIgnore; i++) 
		{
			if (pistonPos->x == mIgnoreList[i].x && pistonPos->y == mIgnoreList[i].y && pistonPos->z == mIgnoreList[i].z)
				return true;
		} 

		return false;
	}
};