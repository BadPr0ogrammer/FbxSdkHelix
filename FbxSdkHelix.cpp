#include "MainWindow.h"
#include "MainViewModel.h"

using namespace FbxSdkHelix;

//libxml2.lib;alembic.lib;zlib.lib;alembic.lib;libfbxsdk.lib;Advapi32.lib

[System::STAThreadAttribute]
int main(array<System::String^>^ args)
{
	Application^ app = gcnew Application();
	
	MainViewModel^ model = gcnew MainViewModel(args[0]);
	MainWindow^ window = gcnew MainWindow(model);

	app->Run(window);

	return 0;
}
