#include "MainViewModel.h"

using Vector3 = System::Numerics::Vector3;
using BoundingBox = HelixToolkit::Maths::BoundingBox;
using ConvExt = HelixToolkit::Wpf::ConverterExtensions;
using uchar = unsigned char;

#include "SdkLoader.h"
#include <codecvt>

using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

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
#if 0
		_meshBuilder->AddBox(ConvExt::ToVector3(Point3D(0, 0, 1)), 1, 2, 0.5f);
		// Create a mesh from the builder (and freeze it)
		auto mesh = _meshBuilder->ToMesh();
		// Create some materials
		auto greenMaterial = MaterialHelper::CreateMaterial(Colors::Green);
		// Add 3 models to the group (using the same mesh, that's why we had to freeze it)
		auto geom = ConvExt::ToWndMeshGeometry3D(mesh, true);
		_modelGroup->Children->Add(gcnew GeometryModel3D(geom, greenMaterial));
#endif        
		GetMeshes((FbxNode*)(_sdkLoader->_scene));

		Vector3D ra = Vector3D(1, 0, 0);
		AxisAngleRotation3D^ aar = gcnew AxisAngleRotation3D(ra, 90);
		RotateTransform3D^ rt = gcnew RotateTransform3D(aar);
		_modelGroup->Transform = rt;

		_modelVisual->Content = _modelGroup;
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

	void MainViewModel::GetMeshes(FbxNode* node)
	{
		if (!node)
			return;
		int geometryCount = node->GetSrcObjectCount<FbxGeometry>();
		auto greenMt = MaterialHelper::CreateMaterial(Colors::Green);
		for (int i = 0; i < geometryCount; i++)
		{
			FbxGeometry* geometry = node->GetSrcObject<FbxGeometry>(i);
			if (geometry && geometry->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = (FbxMesh*) geometry;
				auto meshBuilder = gcnew MeshBuilder(false, false, false);
				/*
				FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(0); // Get the first material
				if (material) { 
					FbxProperty diffusePr = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
					if (diffusePr.IsValid()) {
						FbxDouble3 color = diffusePr.Get<FbxDouble3>();
						greenMt = MaterialHelper::CreateMaterial(
							Color::FromRgb((uchar)(255 * color[0]), (uchar)(255 * color[1]), (uchar)(255 * color[2])));
					}
				}
				*/
				int polygonCount = mesh->GetPolygonCount();
				FbxVector4* controlPoints = mesh->GetControlPoints();
				for (int j = 0; j < polygonCount; j++)
				{
					int vertexCount = mesh->GetPolygonSize(j);
					auto points = gcnew List<Vector3>();
					for (int k = 0; k < vertexCount; k++)
					{
						int vertexIndex = mesh->GetPolygonVertex(j, k);
						FbxVector4 vertex = controlPoints[vertexIndex];
						points->Add(Vector3((float)vertex[0], (float)vertex[1], (float)vertex[2]));
					}
					meshBuilder->AddPolygon(points);
				}
				_modelGroup->Children->Add(
					gcnew GeometryModel3D(
						ConvExt::ToWndMeshGeometry3D(meshBuilder->ToMesh(), true),
						greenMt));
			}
		}
		for (int i = 0; i < node->GetChildCount(); i++)
			GetMeshes(node->GetChild(i));
	}
}
