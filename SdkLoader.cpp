#include "SdkLoader.h"

#include <iostream>

namespace FbxSdkHelix
{
	SdkLoader::SdkLoader()
	{
		_manager = fbxsdk::FbxManager::Create();
		if (!_manager)
		{
			std::cout << "FBX SDK Manager is null!\n";
			return;
		}
		FbxIOSettings* ios = FbxIOSettings::Create(_manager, IOSROOT);
		if (!ios)
		{
			std::cout << "FBX SDK Manager: IO Settings is null!\n";
			return;
		}
		_manager->SetIOSettings(ios);
		_scene = FbxScene::Create(_manager, "MyScene");
		if (!_scene)
		{
			std::cout << "FBX SDK Manager: Create scene failed!\n";
			return;
		}
		_importer = FbxImporter::Create(_manager, "");
		if (!_importer)
		{
			std::cout << "FBX SDK Manager: Importer is null!\n";
			return;
		}
	}

	SdkLoader::~SdkLoader()
	{
		if (_manager)
			_manager->Destroy();
		if (_scene)
			_scene->Destroy();
		if (_importer)
			_importer->Destroy();
	}

	bool SdkLoader::importFile(const char* fileName)
	{
		if (!_importer->Initialize(fileName, -1, _manager->GetIOSettings()))
		{
			std::cout << "FBX SDK Importer: Initialize failed!\n";
			return false;
		}
		if (!_importer->IsFBX())
		{
			std::cout << "FBX SDK Importer: Is DBX failed!\n";
			return false;
		}
		if (!_importer->Import(_scene))
		{
			std::cout << "FBX SDK Importer: Import Failed!\n";
			return false;
		}
		return true;
	}
}
