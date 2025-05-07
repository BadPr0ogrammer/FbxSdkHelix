#include "MainWindow.h"
#include "MainViewModel.h"

using namespace FbxSdkHelix;

#include <codecvt>
using namespace System::Runtime::InteropServices;

std::string SysToStdString(System::String^ s, std::wstring& buf)
{
	const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
	buf = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
	return convert.to_bytes(buf);
}

[System::STAThreadAttribute]
int main(array<System::String^>^ args)
{
	Application^ app = gcnew Application();
	MainViewModel^ model = gcnew MainViewModel();
	MainWindow^ window = gcnew MainWindow(model);
	app->Run(window);
	return 0;
}
