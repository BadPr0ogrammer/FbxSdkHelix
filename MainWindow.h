#pragma once

using namespace System::Windows;
using namespace System::Windows::Controls;
using namespace System::Windows::Data;
using namespace System::Windows::Documents;
using namespace System::Windows::Input;
using namespace System::Windows::Media;
using namespace System::Windows::Media::Imaging;
using namespace System::Windows::Navigation;
using namespace System::Windows::Shapes;
using namespace System::Windows::Media::Media3D;

using namespace HelixToolkit;
using namespace HelixToolkit::Geometry;
using namespace HelixToolkit::Wpf;

namespace FbxSdkHelix
{
	ref class MainViewModel;

	public ref class MainWindow : Window
	{
		HelixViewport3D^ _viewport;
	public:
		MainWindow(MainViewModel^ model);
	};
}