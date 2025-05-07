#include "FbxSdkConverter.h"

#include <iostream>
#include <vector>

bool FbxSdkConverter::managerCreate(bool read_ascii)
{
	_manager = fbxsdk::FbxManager::Create();
	if (_manager == nullptr)
	{
		std::cout << "FBX SDK Manager is null!\n";
		return false;
	}
	// Get the file version number generate by the FBX SDK.
	int major = 0, minor = 0, revision = 0;
	_manager->GetFileFormatVersion(major, minor, revision);
	_majorMgr = major, _minorMgr = minor, _revisionMgr = revision;
	std::cout << "FBX SDK Manager:" << major << minor << revision << "\n";
	if (read_ascii)
	{
		int n = _manager->GetIOPluginRegistry()->GetReaderFormatCount();
		for (int i = 0; i < n; i++)
		{
			if (_manager->GetIOPluginRegistry()->ReaderIsFBX(i))
			{
				const char* desc = _manager->GetIOPluginRegistry()->GetReaderFormatDescription(i);
				if (strstr(desc, "ascii"))
				{
					_asciiFormatIdx = i;
					break;
				}
			}
		}
		if (read_ascii && _asciiFormatIdx == -1)
		{
			std::cout << "FBX SDK Manager: ASCII Reader not found!\n";
			return false;
		}
	}
	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(_manager, IOSROOT);
	if (ios == nullptr)
	{
		std::cout << "FBX SDK Manager: IO Settings is null!\n";
		return false;
	}
	_manager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	//FbxString lPath = FbxGetApplicationDirectory();
	//_manager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	_scene = FbxScene::Create(_manager, "MyScene");
	if (!_scene)
	{
		std::cout << "FBX SDK Manager: Create scene failed!\n";
		return false;
	}
	return true;
}

bool FbxSdkConverter::importerCreate()
{
	_importer = FbxImporter::Create(_manager, "");
	if (_importer == nullptr)
	{
		std::cout << "FBX SDK Manager: Importer is null!\n";
		return false;
	}
 
	// Set the import states. By default, the import states are always set to 
	// true. The code below shows how to change these states.
	/*
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, true);
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
	_manager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	*/
	return true;
}

bool FbxSdkConverter::importerLoad(const char* fileName)
{
	// Initialize the importer by providing a filename.
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

FbxSdkConverter::~FbxSdkConverter()
{
	if (_scene)
		_scene->Destroy();
	if (_importer)
		_importer->Destroy();
	if (_manager)
		_manager->Destroy();
}

std::vector<FbxObject*> FindObjectsByName(FbxScene* pScene, const std::string& name) 
{
	std::vector<FbxObject*> objects;
	int count = pScene->GetSrcObjectCount<FbxObject>();
	for (int i = 0; i < count; ++i) {
		FbxObject* obj = pScene->GetSrcObject<FbxObject>(i);
		if (obj && obj->GetName() == name) {
			objects.push_back(obj);
		}
	}
	return objects;
}

bool FbxSdkConverter::ConvertObjects()
{
	std::vector<FbxObject*> objs = FindObjectsByName(_scene, "Objects");
	for (auto obj : objs)
	{

	}
	return true;
}
/*
std::map<uint64_t, FbxObject*> FindObjectsByName(FbxScene* pScene, const std::string& name)
{
	std::map<uint64_t, FbxObject*> ret;
		int count = pScene->GetSrcObjectCount<FbxObject>();

		for (int i = 0; i < count; ++i) {
			FbxObject* obj = pScene->GetSrcObject<FbxObject>(i);
			if (obj && obj->GetName() == name) {
				map.push_back(obj);
			}
		}

		return objects;
	}
}
*/

