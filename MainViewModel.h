#pragma once

using namespace HelixToolkit;
using namespace HelixToolkit::Geometry;
using namespace HelixToolkit::Wpf;
using namespace System::Windows::Media;
using namespace System::Windows::Media::Media3D;

#include "SdkLoader.h"

namespace FbxSdkHelix
{
    public ref class MainViewModel
    {
    private:
        Model3DGroup^  _modelGroup = nullptr;
        Model3D^       _model = nullptr;
        SdkLoader*     _sdkLoader = nullptr;
    public:
        ModelVisual3D^ _modelVisual = nullptr;

        MainViewModel(System::String^ fname);
        ~MainViewModel();
       
        void GetMeshes(FbxNode* node);

    public: 
        property Model3D^ Model {

            Model3D^ get() {
                return _model;
            }
            void set(Model3D^ value) {
                _model = value;
            }
        }
    };
}