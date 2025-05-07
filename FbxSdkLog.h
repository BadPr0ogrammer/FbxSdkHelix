#pragma once

#include "fbxsdk.h"

class FbxSdkLog
{
public:
	std::string MetaDataConnections(FbxObject* pObject);
	std::string ControlsPoints(FbxMesh* pMesh);
	std::string Polygons(FbxMesh* pMesh);
};
