#pragma once

#include "fbxsdk.h"
#include <map>

class FbxSdkConverter
{
public:
	fbxsdk::FbxManager* _manager = NULL;
	fbxsdk::FbxScene* _scene = NULL;
	fbxsdk::FbxImporter* _importer = NULL;

	int _asciiFormatIdx = -1;
	int _majorMgr = 0, _minorMgr = 0, _revisionMgr = 0;


	~FbxSdkConverter();

	bool managerCreate(bool read_ascii);
	bool importerCreate();
	bool importerLoad(const char* fileName);

	bool ConvertObjects();
};
