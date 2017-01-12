#ifndef TEST_VTKWIDGET
#define TEST_VTKWIDGET
#include <vtkActor.h>
#include <vtkBalloonRepresentation.h>
#include <vtkBalloonWidget.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkSphereSource.h>
#include <vtkBiDimensionalWidget.h>
#include <vtkBiDimensionalRepresentation2D.h>
#include <vtkCommand.h>
///distance widget
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
///imageplane widget
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
///camera widget
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCameraRepresentation.h>
#include <vtkCameraWidget.h>


class vtkBiDimensionalCallback : public vtkCommand
{
  public:
    static vtkBiDimensionalCallback *New()
    {
      return new vtkBiDimensionalCallback;
    }

    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {

      vtkBiDimensionalWidget *biDimensionalWidget =
          reinterpret_cast<vtkBiDimensionalWidget*>(caller);

      vtkBiDimensionalRepresentation2D* representation =
          static_cast<vtkBiDimensionalRepresentation2D*>(biDimensionalWidget->GetRepresentation());

      double p1[3];
      representation->GetPoint1DisplayPosition(p1);
      double p2[3];
      representation->GetPoint1DisplayPosition(p2);
      double p3[3];
      representation->GetPoint1DisplayPosition(p3);
      double p4[3];
      representation->GetPoint1DisplayPosition(p4);

      // Display one of the points, just so we know it's working
      std::cout << "P1: " << p1[0] << " " << p1[1] << " " << p1[2] << std::endl;

    }
    vtkBiDimensionalCallback(){}

};
class Test_vtkWidget {
public:
    int RunTest()
    {
//        return testBallonWidget();
//        return testBidimenWidget();
//        return testDistanceWidget();
//        return testimageplaneWidget();
        return testCameraWidget();
    }
private:
    int testBallonWidget()
    {
        // Sphere
          vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
          sphereSource->SetCenter(-4.0, 0.0, 0.0);
          sphereSource->SetRadius(4.0);
          sphereSource->Update();

          vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

          vtkSmartPointer<vtkActor> sphereActor =
            vtkSmartPointer<vtkActor>::New();
          sphereActor->SetMapper(sphereMapper);

          // Regular Polygon
          vtkSmartPointer<vtkRegularPolygonSource> regularPolygonSource =
            vtkSmartPointer<vtkRegularPolygonSource>::New();
          regularPolygonSource->SetCenter(4.0, 0.0, 0.0);
          regularPolygonSource->SetRadius(4.0);
          regularPolygonSource->Update();

          vtkSmartPointer<vtkPolyDataMapper> regularPolygonMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          regularPolygonMapper->SetInputConnection(regularPolygonSource->GetOutputPort());

          vtkSmartPointer<vtkActor> regularPolygonActor =
            vtkSmartPointer<vtkActor>::New();
          regularPolygonActor->SetMapper(regularPolygonMapper);

          // A renderer and render window
          vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
          vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
          renderWindow->AddRenderer(renderer);

          // An interactor
          vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
          renderWindowInteractor->SetRenderWindow(renderWindow);

          // Create the widget
          vtkSmartPointer<vtkBalloonRepresentation> balloonRep =
            vtkSmartPointer<vtkBalloonRepresentation>::New();
          balloonRep->SetBalloonLayoutToImageRight();

          vtkSmartPointer<vtkBalloonWidget> balloonWidget =
            vtkSmartPointer<vtkBalloonWidget>::New();
          balloonWidget->SetInteractor(renderWindowInteractor);
          balloonWidget->SetRepresentation(balloonRep);
          balloonWidget->AddBalloon(sphereActor,
                                    "This is a sphere",NULL);
          balloonWidget->AddBalloon(regularPolygonActor,
                                    "This is a regular polygon",NULL);

          // Add the actors to the scene
          renderer->AddActor(sphereActor);
          renderer->AddActor(regularPolygonActor);

          // Render
          renderWindow->Render();
          balloonWidget->EnabledOn();

          // Begin mouse interaction
          renderWindowInteractor->Start();

          return EXIT_SUCCESS;
    }

    int testDistanceWidget()
    {
        // A renderer and render window
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);

        // An interactor
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);

        vtkSmartPointer<vtkDistanceWidget> distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
        distanceWidget->SetInteractor(renderWindowInteractor);
        distanceWidget->CreateDefaultRepresentation();
        static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");

        // Render an image (lights and cameras are created automatically)
        renderWindow->Render();

        renderWindowInteractor->Initialize();
        renderWindow->Render();
        distanceWidget->On();

        // Begin mouse interaction
        renderWindowInteractor->Start();

        return EXIT_SUCCESS;
    }

    int testBidimenWidget()
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

          vtkSmartPointer<vtkBiDimensionalWidget> biDimensionalWidget =
            vtkSmartPointer<vtkBiDimensionalWidget>::New();
          biDimensionalWidget->SetInteractor(renderWindowInteractor);
          biDimensionalWidget->CreateDefaultRepresentation();

          vtkSmartPointer<vtkBiDimensionalCallback> biDimensionalCallback =
              vtkSmartPointer<vtkBiDimensionalCallback>::New();

          biDimensionalWidget->AddObserver(vtkCommand::InteractionEvent,biDimensionalCallback);

          // Render an image (lights and cameras are created automatically)
          renderWindow->Render();

          renderWindowInteractor->Initialize();
          renderWindow->Render();
          biDimensionalWidget->On();

          // Begin mouse interaction
          renderWindowInteractor->Start();

          return EXIT_SUCCESS;
    }

    int testimageplaneWidget()
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

        return EXIT_SUCCESS;
    }

    int testCameraWidget()
    {
        // A sphere
          vtkSmartPointer<vtkSphereSource> sphereSource =
              vtkSmartPointer<vtkSphereSource>::New();
          sphereSource->Update();

          vtkSmartPointer<vtkPolyDataMapper> mapper =
              vtkSmartPointer<vtkPolyDataMapper>::New();
//          mapper->SetInput(sphereSource->GetOutput());
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

          // An interactor
          vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
              vtkSmartPointer<vtkRenderWindowInteractor>::New();
          renderWindowInteractor->SetRenderWindow(renderWindow);

          // Add the actors to the scene
          renderer->AddActor(actor);

          // Render an image (lights and cameras are created automatically)
          renderWindow->Render();

          vtkSmartPointer<vtkCameraRepresentation> cameraRep =
              vtkSmartPointer<vtkCameraRepresentation>::New();
          cameraRep->SetNumberOfFrames(2400);

          vtkSmartPointer<vtkCameraWidget> cameraWidget =
              vtkSmartPointer<vtkCameraWidget>::New();
          cameraWidget->SetInteractor(renderWindowInteractor);
          cameraWidget->SetRepresentation(cameraRep);

          //cameraWidget->EnabledOn();
          cameraWidget->On();

          renderWindow->Render();

          renderWindowInteractor->Start();

          return EXIT_SUCCESS;
    }
};
#endif // TEST_VTKWIDGET

