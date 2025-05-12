#pragma once

#include <fbxsdk.h>

namespace FbxSdkHelix
{
    class SdkLoader
    {
    public:
        fbxsdk::FbxManager* _manager = NULL;
        fbxsdk::FbxScene* _scene = NULL;
        fbxsdk::FbxImporter* _importer = NULL;
        
        SdkLoader();
        ~SdkLoader();

        bool importFile(const char* fileName);
    };
}
