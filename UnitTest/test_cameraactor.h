#ifndef TEST_CAMERAACTOR
#define TEST_CAMERAACTOR
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>
#include <vtkPlanes.h>
#include <vtkMapper.h>
#include <vtkCameraActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

class Test_CameraActor {
public:
    static int RunTest()
    {
        // Sphere
         vtkSmartPointer<vtkSphereSource> sphereSource =
           vtkSmartPointer<vtkSphereSource>::New();
         sphereSource->SetRadius(400);
         sphereSource->Update();

         vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
           vtkSmartPointer<vtkPolyDataMapper>::New();
         sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
         vtkSmartPointer<vtkActor> sphereActor =
           vtkSmartPointer<vtkActor>::New();
         sphereActor->SetMapper(sphereMapper);

         // Camera
         vtkSmartPointer<vtkCamera> camera =
           vtkSmartPointer<vtkCamera>::New();

         vtkSmartPointer<vtkCameraActor> cameraActor =
           vtkSmartPointer<vtkCameraActor>::New();
         cameraActor->SetCamera(camera);

         // (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax).
         double* bounds = new double[6];
         bounds = cameraActor->GetBounds();
         std::cout << "bounds: " << bounds[0] << " " << bounds[1] << " " << bounds[2] << " " <<
             bounds[3] << " " << bounds[4] << " " << bounds[5] << std::endl;

         // Visualize
         vtkSmartPointer<vtkRenderer> renderer =
           vtkSmartPointer<vtkRenderer>::New();
         vtkSmartPointer<vtkRenderWindow> renderWindow =
           vtkSmartPointer<vtkRenderWindow>::New();
         renderWindow->AddRenderer(renderer);

         vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
           vtkSmartPointer<vtkRenderWindowInteractor>::New();
         renderWindowInteractor->SetRenderWindow(renderWindow);

         renderer->AddActor(cameraActor);
         renderer->AddActor(sphereActor);

         renderer->SetBackground(1,1,1); // Background color white
         renderWindow->Render();
         renderWindowInteractor->Start();

        return EXIT_SUCCESS;
    }

};
#endif // TEST_CAMERAACTOR

