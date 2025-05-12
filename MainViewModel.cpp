#include "MainViewModel.h"

using Vector3 = System::Numerics::Vector3;
using BoundingBox = HelixToolkit::Maths::BoundingBox;
using ConvExt = HelixToolkit::Wpf::ConverterExtensions;

#include "SdkLoader.h"
#include <codecvt>
using namespace System::Runtime::InteropServices;

namespace FbxSdkHelix
{
    std::string SysToStdString(System::String^ s, std::wstring& buf);

    MainViewModel::MainViewModel(System::String^ fileName)
    {
        _sdkLoader = new SdkLoader();
        
        std::wstring wfname;
        std::string fname = SysToStdString(fileName, wfname);

        if (!_sdkLoader->importFile(fname.c_str()))
            return;
        
        _modelVisual = gcnew ModelVisual3D();

        _modelGroup = gcnew Model3DGroup();

        _meshBuilder = gcnew MeshBuilder(false, false, false);
        
        _meshBuilder->AddBox(ConvExt::ToVector3(Point3D(0, 0, 1)), 1, 2, 0.5f);
        // Create a mesh from the builder (and freeze it)
        auto mesh = _meshBuilder->ToMesh();
        // Create some materials
        auto greenMaterial = MaterialHelper::CreateMaterial(Colors::Green);
        // Add 3 models to the group (using the same mesh, that's why we had to freeze it)
        auto geom = ConvExt::ToWndMeshGeometry3D(mesh, true);
        _modelGroup->Children->Add(gcnew GeometryModel3D(geom, greenMaterial));
        
        _modelVisual->Content = _modelGroup;

        // Set the property, which will be bound to the Content property of the ModelVisual3D (see MainWindow.xaml)
        Model = _modelGroup;
    }

    MainViewModel::~MainViewModel()
    {
        delete _sdkLoader;
    }

    std::string SysToStdString(System::String^ s, std::wstring& buf)
    {
        const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
        buf = chars;
        Marshal::FreeHGlobal(System::IntPtr((void*)chars));
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        return convert.to_bytes(buf);
    }
}
