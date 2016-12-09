#ifndef TEST_VISUALALGORITHMS
#define TEST_VISUALALGORITHMS
#include <vtkBYUReader.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlane.h>
#include <vtkPolyDataNormals.h>
#include <vtkClipPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkColor.h>
#include <vtkExecutive.h>

#include "fastdef.h"
class Test_VisualAlgorithms {
public:

    int RunTest()
    {
        return testclipper();
    }
private:
    int testclipper()
    {

        try{
            Instantiate(cow, vtkSTLReader);
            cow->SetFileName("D://3dresearch//TestData//stl//122902.stl");
//            Instantiate(cow, vtkBYUReader);
//            cow->SetInputConnection("D://3dresearch//TestData//stl//122902.stl");
            Instantiate(cowNormals, vtkPolyDataNormals);
            cowNormals->SetInputConnection(cow->GetOutputPort());
            Instantiate(plane, vtkPlane);
            plane->SetOrigin(0.45, 0, 0);
            plane->SetNormal(-1, 0, 0);
            Instantiate(clipper, vtkClipPolyData);
            clipper->SetInputConnection(cowNormals->GetOutputPort());
            clipper->SetClipFunction(plane);
            clipper->GenerateClipScalarsOn();
            clipper->GenerateClippedOutputOn();
            clipper->SetValue(0.5);
            Instantiate(clipMapper, vtkPolyDataMapper);
            clipMapper->SetInputConnection(clipper->GetOutputPort());
            clipMapper->ScalarVisibilityOff();
            Instantiate(backProp, vtkProperty);
            backProp->SetDiffuseColor(0.4, 0.4, 0.5);
            Instantiate(clipActor, vtkActor);
            clipActor->SetMapper(clipMapper);
            clipActor->GetProperty()->SetColor(0.3, 0.49, 0.25);
            clipActor->SetBackfaceProperty(backProp);


            Instantiate(restMapper, vtkPolyDataMapper);

            restMapper->SetInputConnection((vtkAlgorithmOutput*)clipper->GetClippedOutput());
            restMapper->ScalarVisibilityOff();
            Instantiate(restActor, vtkActor);
            restActor->SetMapper(restMapper);
            restActor->GetProperty()->SetRepresentationToWireframe();
            Instantiate(renderwindow, vtkRenderWindow);
            Instantiate(renderer, vtkRenderer);
            renderer->SetBackground(1.0, 1.0, 1.0);
            renderer->AddActor(clipActor);
            renderer->AddActor(restActor);
            renderwindow->AddRenderer(renderer);
            renderwindow->Render();
            Instantiate(interactor, vtkRenderWindowInteractor);
            interactor->SetRenderWindow(renderwindow);
            interactor->Initialize();
            interactor->Start();

        }catch(...) {
            std::cerr << "Caught Error!" << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
};
#endif // TEST_VISUALALGORITHMS

