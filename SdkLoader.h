#pragma once

#include <fbxsdk.h>

#include <vector>

namespace FbxSdkHelix
{   
    class SdkLoader
    {
    public:
        fbxsdk::FbxManager* _manager = NULL;
        fbxsdk::FbxScene* _scene = NULL;
        fbxsdk::FbxImporter* _importer = NULL;

        std::vector<FbxObject*> _sceneGeom;

        SdkLoader();
        ~SdkLoader();

        bool importFile(const char* fileName);
        //bool getSceneGeom();
    };
}
