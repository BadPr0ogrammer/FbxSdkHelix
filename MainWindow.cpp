#include "MainWindow.h"
#include "MainViewModel.h"

namespace FbxSdkHelix
{
	MainWindow::MainWindow(MainViewModel^model)
	{
        _viewport = gcnew HelixViewport3D();
        _viewport->ShowFrameRate = true;
        _viewport->ZoomExtentsWhenLoaded = true;
        _viewport->ZoomAroundMouseDownPoint = true;
        _viewport->RotateAroundMouseDownPoint = true;
        _viewport->IsTopBottomViewOrientedToFrontBack = true;
        _viewport->IsViewCubeEdgeClicksEnabled = true;

        auto light = gcnew SunLight();
        _viewport->Children->Add(light);

        _viewport->Children->Add(model->_modelVisual);

		DataContext = model;
        Content = _viewport;
	}
}
