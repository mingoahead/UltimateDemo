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
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkAssembly.h>
#include <vtkTransform.h>
#include <vtkActorCollection.h>


#include "fastdef.h"
class Test_VisualAlgorithms {
public:

    int RunTest()
    {
//        return testclipper();
//        return testassembly();
        return testActorCollection();
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

    int testassembly()
    {
        try{
            Instantiate(sphere, vtkSphereSource);
            Instantiate(sphereMapper, vtkPolyDataMapper);
            sphereMapper->SetInputConnection(sphere->GetOutputPort());
            Instantiate(sphereActor, vtkActor);
            sphereActor->SetMapper(sphereMapper);
            sphereActor->SetOrigin(2, 1, 3);
            sphereActor->RotateY(6);
            sphereActor->GetProperty()->SetColor(1, 0, 1);
            Instantiate(cube, vtkCubeSource);
            Instantiate(cubeMapper, vtkPolyDataMapper);
            cubeMapper->SetInputConnection(cube->GetOutputPort());
            Instantiate(cubeActor, vtkActor);
            cubeActor->SetMapper(cubeMapper);
            cubeActor->SetPosition(0.0, 0.25, 0);
            cubeActor->GetProperty()->SetColor(0, 0, 1);
            Instantiate(cone, vtkConeSource);
            Instantiate(coneMapper, vtkPolyDataMapper);
            coneMapper->SetInputConnection(cone->GetOutputPort());
            Instantiate(coneActor, vtkActor);
            coneActor->SetMapper(coneMapper);
            coneActor->SetPosition(0, 0, 0.25);
            coneActor->GetProperty()->SetColor(0, 1, 0);
            Instantiate(cylinder, vtkCylinderSource);
            Instantiate(cylinderMapper, vtkPolyDataMapper);
            cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
            cylinderMapper->SetResolveCoincidentTopologyToPolygonOffset();
            Instantiate(cylinderActor, vtkActor);
            cylinderActor->SetMapper(cylinderMapper);
            cylinderActor->GetProperty()->SetColor(1, 0, 0);

            Instantiate(assembly, vtkAssembly);
            assembly->AddPart(cylinderActor);
            assembly->AddPart(sphereActor);
            assembly->AddPart(cubeActor);
            assembly->AddPart(coneActor);
            assembly->SetOrigin(5, 10, 15);
            assembly->AddPosition(5, 0, 0);
            assembly->RotateX(15);
            Instantiate(renderwindow, vtkRenderWindow);
            Instantiate(renderer, vtkRenderer);
            renderer->SetBackground(1.0, 1.0, 1.0);
            renderer->AddActor(assembly);
            renderer->AddActor(coneActor);
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

    int testActorCollection()
    {
        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();

          // Create a cone
          vtkSmartPointer<vtkConeSource> coneSource =
            vtkSmartPointer<vtkConeSource>::New();
          coneSource->SetHeight(3);

          vtkSmartPointer<vtkPolyDataMapper> coneMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          coneMapper->SetInputConnection(coneSource->GetOutputPort());
          vtkSmartPointer<vtkActor> coneActor =
            vtkSmartPointer<vtkActor>::New();
          coneActor->SetMapper(coneMapper);

          // Create a cube
          vtkSmartPointer<vtkCubeSource> cubeSource =
            vtkSmartPointer<vtkCubeSource>::New();

          vtkSmartPointer<vtkPolyDataMapper> cubeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          cubeMapper->SetInputConnection(cubeSource->GetOutputPort());
          vtkSmartPointer<vtkActor> cubeActor =
            vtkSmartPointer<vtkActor>::New();
          cubeActor->SetMapper(cubeMapper);

          // Create a sphere
          vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();

          vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
          vtkSmartPointer<vtkActor> sphereActor =
            vtkSmartPointer<vtkActor>::New();
          sphereActor->SetMapper(sphereMapper);
          renderer->AddActor(sphereActor);
          renderer->AddActor(cubeActor);
          renderer->AddActor(coneActor);

          vtkSmartPointer<vtkActorCollection> actorCollection =
            vtkSmartPointer<vtkActorCollection>::New();
          actorCollection->AddItem(cubeActor);
          actorCollection->AddItem(coneActor);
          actorCollection->InitTraversal();

          vtkSmartPointer<vtkTransform> transform =
            vtkSmartPointer<vtkTransform>::New();
          transform->PostMultiply(); //this is the key line
          transform->Translate(5.0, 0, 0);

          //actorCollection->SetUserTransform(transform);


          for(vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++)
            {
            vtkActor* actor = actorCollection->GetNextActor();
            actor->SetUserTransform(transform);
//            double pos[3];
//            double org[3];
//            actor->GetPosition(pos);
//            actor->GetOrigin(org);
//            std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
//            std::cout << org[0] << ", " << org[1] << ", " << org[2] << std::endl;
//            renderer->AddActor(actor);
            }

          // Create a renderer, render window, and interactor

          vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
          renderWindow->AddRenderer(renderer);
          vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
          renderWindowInteractor->SetRenderWindow(renderWindow);

          // Add the actor to the scene
          renderer->SetBackground(.2, .3, .4);

          // Render and interact
          renderWindow->Render();
          renderWindowInteractor->Start();

          return EXIT_SUCCESS;
    }
};
#endif // TEST_VISUALALGORITHMS

