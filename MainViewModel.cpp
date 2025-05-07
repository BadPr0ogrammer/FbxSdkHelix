#include "MainViewModel.h"

using Vector3 = System::Numerics::Vector3;
using BoundingBox = HelixToolkit::Maths::BoundingBox;
using ConvExt = HelixToolkit::Wpf::ConverterExtensions;

namespace FbxSdkHelix
{
    MainViewModel::MainViewModel()
    {
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
    /*
    libxml2.lib;alembic.lib;zlib.lib;alembic-md.lib;libfbxsdk-md.lib;Advapi32.lib
    bool MainViewModel::InitSdk()
    {
        _fbxConverter = new FbxSdkConverter();
        _fbxLog = new FbxSdkLog();

        if (!_fbxConverter->managerCreate(false))
            return false;
        if (!_fbxConverter->importerCreate())
            return false;
        if (!_fbxConverter->ConvertObjects())
            return false;
        return true;
    }

        //FbxSdkConverter* _fbxConverter = nullptr;
        //FbxSdkLog* _fbxLog = nullptr;
        #include "FbxSdkConverter.h"
#include "FbxSdkLog.h"

    */
}
