#ifndef TESTIMAGEPLANEWIDGET
#define TESTIMAGEPLANEWIDGET
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkImagePlaneWidget.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>

class Test_ImagePlaneWidget
{
public:
    static int RunTest()
    {
        vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();

        // Create a mapper and actor
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphereSource->GetOutputPort());
        vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        // A renderer and render window
        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        renderer->AddActor(actor);

        // An interactor
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);


        //vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
        //vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

        renderWindowInteractor->SetInteractorStyle( style );

        vtkSmartPointer<vtkImagePlaneWidget> planeWidget =
            vtkSmartPointer<vtkImagePlaneWidget>::New();
        planeWidget->SetInteractor(renderWindowInteractor);

        planeWidget->SetKeyPressActivation('x');

        double origin[3] = {0, 1,0};
        planeWidget->SetOrigin(origin);
        planeWidget->UpdatePlacement();

        // Render
        renderWindow->Render();

        renderWindowInteractor->Initialize();
        renderWindow->Render();
        planeWidget->On();

        // Begin mouse interaction
        renderWindowInteractor->Start();

        return 0;
    }
};

#endif // TESTIMAGEPLANEWIDGET

