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
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkVolume16Reader.h>
///cutter
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
///camera actor
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
///scalar bar actor
#include <vtkVersion.h>
#include <vtkActor.h>
#include <vtkFloatArray.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
///extractor polyline
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkStripper.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkSphereSource.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
///multiple lights
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkVersion.h>

#if VTK_MAJOR_VERSION <= 5
#include <vtkRenderer.h>
#else
#include <vtkOpenGLRenderer.h>
#endif

#include <vtkLightActor.h>
#include <vtkFrameBufferObject.h>
#include <vtkImageSinusoidSource.h>
#include <vtkImageData.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkLookupTable.h>
#include <vtkCamera.h>
#include <vtkCameraPass.h>
#include <vtkLightsPass.h>
#include <vtkSequencePass.h>
#include <vtkOpaquePass.h>
#include <vtkDepthPeelingPass.h>
#include <vtkTranslucentPass.h>
#include <vtkVolumetricPass.h>
#include <vtkOverlayPass.h>
#include <vtkRenderPassCollection.h>
#include <vtkShadowMapBakerPass.h>
#include <vtkShadowMapPass.h>
#include <vtkConeSource.h>
#include <vtkPlaneSource.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkInformation.h>
#include <vtkProperty.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <assert.h>
#include <vtkMath.h>
#include <vtkFrustumSource.h>
#include <vtkPlanes.h>
#include <vtkActorCollection.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointData.h>

#include "fastdef.h"
class Test_VisualAlgorithms {
public:

    int RunTest()
    {
//        return testclipper();
//        return testassembly();
//        return testActorCollection();
//        return testCamera();
//        return testCutter();
//        return testCameraActor();
//        return testScalarBarActor();
//        return testextractPolyline();
        return testlight();
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

    int testCamera() {
        vtkSmartPointer<vtkRenderer> aRenderer =
           vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renWin =
           vtkSmartPointer<vtkRenderWindow>::New();
        renWin->AddRenderer(aRenderer);

        vtkSmartPointer<vtkRenderWindowInteractor> iren =
             vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iren->SetRenderWindow(renWin);

        // The following reader is used to read a series of 2D slices (images)
        // that compose the volume. The slice dimensions are set, and the
       // pixel spacing. The data Endianness must also be specified. The reader
        // uses the FilePrefix in combination with the slice number to construct
        // filenames using the format FilePrefix.%d. (In this case the FilePrefix
         // is the root name of the file: quarter.)
         vtkSmartPointer<vtkVolume16Reader> v16 =
            vtkSmartPointer<vtkVolume16Reader>::New();
          v16->SetDataDimensions (64,64);
          v16->SetImageRange (1,93);
          v16->SetDataByteOrderToLittleEndian();
          v16->SetFilePrefix ("D://3dresearch//TestData//vtkdata//Data//headsq//quarter");
          v16->SetDataSpacing (3.2, 3.2, 1.5);

          // An isosurface, or contour value of 500 is known to correspond to the
          // skin of the patient. Once generated, a vtkPolyDataNormals filter is
           // is used to create normals for smooth surface shading during rendering.
           vtkSmartPointer<vtkContourFilter> skinExtractor =
            vtkSmartPointer<vtkContourFilter>::New();
           skinExtractor->SetInputConnection(v16->GetOutputPort());
          skinExtractor->SetValue(0, 500);

        vtkSmartPointer<vtkPolyDataNormals> skinNormals =
         vtkSmartPointer<vtkPolyDataNormals>::New();
        skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
        skinNormals->SetFeatureAngle(60.0);

        vtkSmartPointer<vtkPolyDataMapper> skinMapper =
           vtkSmartPointer<vtkPolyDataMapper>::New();
        skinMapper->SetInputConnection(skinNormals->GetOutputPort());
        skinMapper->ScalarVisibilityOff();

        vtkSmartPointer<vtkActor> skin =
            vtkSmartPointer<vtkActor>::New();
          skin->SetMapper(skinMapper);

          skin->GetProperty()->SetDiffuseColor(1, .49, .25);
          skin->GetProperty()->SetSpecular(.3);
          skin->GetProperty()->SetSpecularPower(20);
          skin->GetProperty()->SetOpacity(.5);
          // An outline provides context around the data.
          //
          vtkSmartPointer<vtkContourFilter> boneExtractor =
             vtkSmartPointer<vtkContourFilter>::New();
             boneExtractor->SetInputConnection(v16->GetOutputPort());
             boneExtractor->SetValue(0, 1150);

            vtkSmartPointer<vtkPolyDataNormals> boneNormals =
               vtkSmartPointer<vtkPolyDataNormals>::New();
             boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
            boneNormals->SetFeatureAngle(60.0);

             vtkSmartPointer<vtkStripper> boneStripper =
               vtkSmartPointer<vtkStripper>::New();
             boneStripper->SetInputConnection(boneNormals->GetOutputPort());

            vtkSmartPointer<vtkPolyDataMapper> boneMapper =
               vtkSmartPointer<vtkPolyDataMapper>::New();
             boneMapper->SetInputConnection(boneStripper->GetOutputPort());
             boneMapper->ScalarVisibilityOff();

            vtkSmartPointer<vtkActor> bone =
                vtkSmartPointer<vtkActor>::New();
            bone->SetMapper(boneMapper);
            bone->GetProperty()->SetDiffuseColor(1, 1, .9412);

          vtkSmartPointer<vtkOutlineFilter> outlineData =
            vtkSmartPointer<vtkOutlineFilter>::New();
          outlineData->SetInputConnection(v16->GetOutputPort());

          vtkSmartPointer<vtkPolyDataMapper> mapOutline =
            vtkSmartPointer<vtkPolyDataMapper>::New();
           mapOutline->SetInputConnection(outlineData->GetOutputPort());

       vtkSmartPointer<vtkActor> outline =
           vtkSmartPointer<vtkActor>::New();
         outline->SetMapper(mapOutline);
         outline->GetProperty()->SetColor(0,0,0);

         // It is convenient to create an initial view of the data. The FocalPoint
         // and Position form a vector direction. Later on (ResetCamera() method)
         // this vector is used to position the camera to look at the data in
         // this direction.
         vtkSmartPointer<vtkCamera> aCamera =
           vtkSmartPointer<vtkCamera>::New();
         aCamera->SetViewUp (0, 0, -1);
         aCamera->SetPosition (0, 1, 0);
         aCamera->SetFocalPoint (0, 0, 0);
         aCamera->ComputeViewPlaneNormal();
         aCamera->Azimuth(30.0);
         aCamera->Elevation(30.0);

         // Actors are added to the renderer. An initial camera view is created.
         // The Dolly() method moves the camera towards the FocalPoint,
          // thereby enlarging the image.
         aRenderer->AddActor(outline);
         aRenderer->AddActor(skin);
         aRenderer->AddActor(bone);
         aRenderer->SetActiveCamera(aCamera);
         aRenderer->ResetCamera ();
         aCamera->Dolly(1.5);

         // Set a background color for the renderer and set the size of the
         // render window (expressed in pixels).
         aRenderer->SetBackground(.2, .3, .4);
          renWin->SetSize(640, 480);

         // Note that when camera movement occurs (as it does in the Dolly()
          // method), the clipping planes often need adjusting. Clipping planes
         // consist of two planes: near and far along the view direction. The
          // near plane clips out objects in front of the plane; the far plane
          // clips out objects behind the plane. This way only what is drawn
         // between the planes is actually rendered.
         aRenderer->ResetCameraClippingRange ();

         // Initialize the event loop and then start it.
         iren->Initialize();
         iren->Start();

         return EXIT_SUCCESS;
    }

    int testCutter()
    {
        vtkSmartPointer<vtkCubeSource> cube =
            vtkSmartPointer<vtkCubeSource>::New();
        cube->SetXLength(40);
        cube->SetYLength(30);
        cube->SetZLength(20);
        vtkSmartPointer<vtkPolyDataMapper> cubeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        cubeMapper->SetInputConnection(cube->GetOutputPort());

        // Create a plane to cut,here it cuts in the XZ direction (xz normal=(1,0,0);XY =(0,0,1),YZ =(0,1,0)
        vtkSmartPointer<vtkPlane> plane =
            vtkSmartPointer<vtkPlane>::New();
        plane->SetOrigin(10,0,0);
        plane->SetNormal(1,0,0);

        // Create cutter
        vtkSmartPointer<vtkCutter> cutter =
            vtkSmartPointer<vtkCutter>::New();
        cutter->SetCutFunction(plane);
        cutter->SetInputConnection(cube->GetOutputPort());
        cutter->Update();

        vtkSmartPointer<vtkPolyDataMapper> cutterMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        cutterMapper->SetInputConnection( cutter->GetOutputPort());

        // Create plane actor
        vtkSmartPointer<vtkActor> planeActor =
            vtkSmartPointer<vtkActor>::New();
        planeActor->GetProperty()->SetColor(1.0,1,0);
        planeActor->GetProperty()->SetLineWidth(2);
        planeActor->SetMapper(cutterMapper);

        // Create cube actor
        vtkSmartPointer<vtkActor> cubeActor =
            vtkSmartPointer<vtkActor>::New();
        cubeActor->GetProperty()->SetColor(0.5,1,0.5);
        cubeActor->GetProperty()->SetOpacity(0.5);
        cubeActor->SetMapper(cubeMapper);

        // Create renderers and add actors of plane and cube
        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(planeActor); //display the rectangle resulting from the cut
        renderer->AddActor(cubeActor); //display the cube

        // Add renderer to renderwindow and render
        vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        renderWindow->SetSize(600, 600);

        vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
        interactor->SetRenderWindow(renderWindow);
        renderer->SetBackground(0,0,0);
        renderWindow->Render();

        interactor->Start();

        return EXIT_SUCCESS;
    }

    int testCameraActor()
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

    int testScalarBarActor()
    {
        vtkSmartPointer<vtkSphereSource> sphere =
            vtkSmartPointer<vtkSphereSource>::New();
          sphere->SetCenter(0,0,0);
          sphere->SetRadius(1);
          sphere->Update();

          // Create scalar data to associate with the vertices of the sphere
          int numPts = sphere->GetOutput()->GetPoints()->GetNumberOfPoints();
          vtkSmartPointer<vtkFloatArray> scalars =
            vtkSmartPointer<vtkFloatArray>::New();
          scalars->SetNumberOfValues( numPts );
          for( int i = 0; i < numPts; ++i )
            {
            scalars->SetValue(i,static_cast<float>(i)/numPts);
            }
          vtkSmartPointer<vtkPolyData> poly =
            vtkSmartPointer<vtkPolyData>::New();
          poly->DeepCopy(sphere->GetOutput());
          poly->GetPointData()->SetScalars(scalars);

          vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        #if VTK_MAJOR_VERSION <= 5
          mapper->SetInput(poly);
        #else
          mapper->SetInputData(poly);
        #endif
          mapper->ScalarVisibilityOn();
          mapper->SetScalarModeToUsePointData();
          mapper->SetColorModeToMapScalars();

          vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
          actor->SetMapper(mapper);

          vtkSmartPointer<vtkScalarBarActor> scalarBar =
            vtkSmartPointer<vtkScalarBarActor>::New();
          scalarBar->SetLookupTable(mapper->GetLookupTable());
          scalarBar->SetTitle("Title");
          scalarBar->SetNumberOfLabels(4);

          // Create a lookup table to share between the mapper and the scalarbar
          vtkSmartPointer<vtkLookupTable> hueLut =
            vtkSmartPointer<vtkLookupTable>::New();
          hueLut->SetTableRange (0, 1);
          hueLut->SetHueRange (0, 1);
          hueLut->SetSaturationRange (1, 1);
          hueLut->SetValueRange (1, 1);
          hueLut->Build();

          mapper->SetLookupTable( hueLut );
          scalarBar->SetLookupTable( hueLut );

          // Create a renderer and render window
          vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();

          renderer->GradientBackgroundOn();
          renderer->SetBackground(1,1,1);
          renderer->SetBackground2(0,0,0);

          vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
          renderWindow->AddRenderer(renderer);

          // Create an interactor
          vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
          renderWindowInteractor->SetRenderWindow(renderWindow);

          // Add the actors to the scene
          renderer->AddActor(actor);
          renderer->AddActor2D(scalarBar);

          // Render the scene (lights and cameras are created automatically)
          renderWindow->Render();
          renderWindowInteractor->Start();

          return EXIT_SUCCESS;
    }

    int testextractPolyline()
    {
        vtkSmartPointer<vtkSphereSource> modelSource =
            vtkSmartPointer<vtkSphereSource>::New();

        vtkSmartPointer<vtkPlane> plane =
            vtkSmartPointer<vtkPlane>::New();

        vtkSmartPointer<vtkCutter> cutter =
            vtkSmartPointer<vtkCutter>::New();
        cutter->SetInputConnection(modelSource->GetOutputPort());
        cutter->SetCutFunction(plane);
        cutter->GenerateValues(10, -.5, .5);

        vtkSmartPointer<vtkPolyDataMapper> modelMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        modelMapper->SetInputConnection(modelSource->GetOutputPort());

        vtkSmartPointer<vtkActor> model =
            vtkSmartPointer<vtkActor>::New();
        model->SetMapper(modelMapper);

        vtkSmartPointer<vtkStripper> stripper =
            vtkSmartPointer<vtkStripper>::New();
        stripper->SetInputConnection(cutter->GetOutputPort());

        vtkSmartPointer<vtkPolyDataMapper> linesMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        linesMapper->SetInputConnection(stripper->GetOutputPort());

        vtkSmartPointer<vtkActor> lines =
            vtkSmartPointer<vtkActor>::New();
        lines->SetMapper(linesMapper);
        lines->GetProperty()->SetDiffuseColor(.2, .2, .2);

        vtkSmartPointer<vtkRenderer> ren =
            vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renWin =
            vtkSmartPointer<vtkRenderWindow>::New();

        renWin->AddRenderer(ren);

        vtkSmartPointer<vtkRenderWindowInteractor> iren =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iren->SetRenderWindow(renWin);

        // Add the actors to the renderer
        ren->AddActor(model);
        ren->AddActor(lines);
        ren->SetBackground(0.1, 0.2, 0.4);

        // This starts the event loop and as a side effect causes an initial
        // render.
        renWin->Render();
        iren->Start();


        // Extract the lines from the polydata
        vtkIdType numberOfLines = cutter->GetOutput()->GetNumberOfLines();


        std::cout << "-----------Lines without using vtkStripper" << std::endl;
        std::cout << "There are "
                  << numberOfLines
                  << " lines in the polydata" << std::endl;

        numberOfLines = stripper->GetOutput()->GetNumberOfLines();
        vtkPoints *points = stripper->GetOutput()->GetPoints();
        vtkCellArray *cells = stripper->GetOutput()->GetLines();

        std::cout << "-----------Lines using vtkStripper" << std::endl;
        std::cout << "There are "
                  << numberOfLines
                  << " lines in the polydata" << std::endl;

        vtkIdType *indices;
        vtkIdType numberOfPoints;
        unsigned int lineCount = 0;
        for (cells->InitTraversal();
                cells->GetNextCell(numberOfPoints, indices);
                lineCount++) {
            std::cout << "Line " << lineCount << ": " << std::endl;
            for (vtkIdType i = 0; i < numberOfPoints; i++) {
                double point[3];
                points->GetPoint(indices[i], point);
                std::cout << "\t("
                          << point[0] << ", "
                          << point[1] << ", "
                          << point[2] << ")" << std::endl;
            }
        }
        return EXIT_SUCCESS;
    }

    void AddLightActors(vtkRenderer *r)
    {
        assert("pre: r_exists" && r!=0);

        vtkLightCollection *lights=r->GetLights();

        lights->InitTraversal();
        vtkLight *l=lights->GetNextItem();
        while(l!=0)
          {
          double angle=l->GetConeAngle();
          if(l->LightTypeIsSceneLight() && l->GetPositional()
             && angle<180.0) // spotlight
            {
            vtkLightActor *la=vtkLightActor::New();
            la->SetLight(l);
            r->AddViewProp(la);
            la->Delete();
            }
          l=lights->GetNextItem();
          }
    }
    int testlight()
    {
        vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
          vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
          renderWindow->SetSize(400,400);
          renderWindow->SetMultiSamples(0);

          renderWindow->SetAlphaBitPlanes(1);
          interactor->SetRenderWindow(renderWindow);

        #if VTK_MAJOR_VERSION <= 5
          vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        #else
          vtkSmartPointer<vtkOpenGLRenderer> renderer =
            vtkSmartPointer<vtkOpenGLRenderer>::New();
        #endif
          renderWindow->AddRenderer(renderer);

          bool supported = vtkFrameBufferObject::IsSupported(renderWindow); // adapted from line 182 of vtkShadowMapPass.cxx

          if(!supported)
            {
            std::cerr << "Shadow rendering is not supported by the current video"
                      << " driver!" << std::endl;
            return EXIT_FAILURE;
            }

          vtkSmartPointer<vtkCameraPass> cameraP =
            vtkSmartPointer<vtkCameraPass>::New();

          vtkSmartPointer<vtkOpaquePass> opaque =
            vtkSmartPointer<vtkOpaquePass>::New();

          vtkSmartPointer<vtkDepthPeelingPass> peeling =
            vtkSmartPointer<vtkDepthPeelingPass>::New();
          peeling->SetMaximumNumberOfPeels(200);
          peeling->SetOcclusionRatio(0.1);

          vtkSmartPointer<vtkTranslucentPass> translucent =
            vtkSmartPointer<vtkTranslucentPass>::New();
          peeling->SetTranslucentPass(translucent);

          vtkSmartPointer<vtkVolumetricPass> volume =
            vtkSmartPointer<vtkVolumetricPass>::New();
          vtkSmartPointer<vtkOverlayPass> overlay =
            vtkSmartPointer<vtkOverlayPass>::New();

          vtkSmartPointer<vtkLightsPass> lights =
            vtkSmartPointer<vtkLightsPass>::New();

          vtkSmartPointer<vtkSequencePass> opaqueSequence =
            vtkSmartPointer<vtkSequencePass>::New();

          vtkSmartPointer<vtkRenderPassCollection> passes2 =
            vtkSmartPointer<vtkRenderPassCollection>::New();
          passes2->AddItem(lights);
          passes2->AddItem(opaque);
          opaqueSequence->SetPasses(passes2);

          vtkSmartPointer<vtkCameraPass> opaqueCameraPass =
            vtkSmartPointer<vtkCameraPass>::New();
          opaqueCameraPass->SetDelegatePass(opaqueSequence);

          vtkSmartPointer<vtkShadowMapBakerPass> shadowsBaker =
            vtkSmartPointer<vtkShadowMapBakerPass>::New();
          shadowsBaker->SetOpaquePass(opaqueCameraPass);
          shadowsBaker->SetResolution(1024);
          // To cancel self-shadowing.
          shadowsBaker->SetPolygonOffsetFactor(3.1f);
          shadowsBaker->SetPolygonOffsetUnits(10.0f);

          vtkSmartPointer<vtkShadowMapPass> shadows =
            vtkSmartPointer<vtkShadowMapPass>::New();
          shadows->SetShadowMapBakerPass(shadowsBaker);
          shadows->SetOpaquePass(opaqueSequence);

          vtkSmartPointer<vtkSequencePass> seq =
            vtkSmartPointer<vtkSequencePass>::New();
          vtkSmartPointer<vtkRenderPassCollection> passes =
            vtkSmartPointer<vtkRenderPassCollection>::New();
          passes->AddItem(shadowsBaker);
          passes->AddItem(shadows);
          passes->AddItem(lights);
          passes->AddItem(peeling);
          passes->AddItem(volume);
          passes->AddItem(overlay);
          seq->SetPasses(passes);
          cameraP->SetDelegatePass(seq);

          renderer->SetPass(cameraP);

          vtkSmartPointer<vtkPlaneSource> rectangleSource =
            vtkSmartPointer<vtkPlaneSource>::New();
          rectangleSource->SetOrigin(-5.0,0.0,5.0);
          rectangleSource->SetPoint1(5.0,0.0,5.0);
          rectangleSource->SetPoint2(-5.0,0.0,-5.0);
          rectangleSource->SetResolution(100,100);

          vtkSmartPointer<vtkPolyDataMapper> rectangleMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          rectangleMapper->SetInputConnection(rectangleSource->GetOutputPort());

          rectangleMapper->SetScalarVisibility(0);

          vtkSmartPointer<vtkActor> rectangleActor =
            vtkSmartPointer<vtkActor>::New();
          vtkSmartPointer<vtkInformation> rectangleKeyProperties =
            vtkSmartPointer<vtkInformation>::New();
          rectangleKeyProperties->Set(vtkShadowMapBakerPass::OCCLUDER(),0); // dummy val.
          rectangleKeyProperties->Set(vtkShadowMapBakerPass::RECEIVER(),0); // dummy val.
          rectangleActor->SetPropertyKeys(rectangleKeyProperties);

          rectangleActor->SetMapper(rectangleMapper);
          rectangleActor->SetVisibility(1);
          rectangleActor->GetProperty()->SetColor(1.0,1.0,1.0);

          vtkSmartPointer<vtkCubeSource> boxSource =
            vtkSmartPointer<vtkCubeSource>::New();
          boxSource->SetXLength(2.0);
          vtkSmartPointer<vtkPolyDataNormals> boxNormals =
            vtkSmartPointer<vtkPolyDataNormals>::New();
          boxNormals->SetInputConnection(boxSource->GetOutputPort());
          boxNormals->SetComputePointNormals(0);
          boxNormals->SetComputeCellNormals(1);
          boxNormals->Update();
          boxNormals->GetOutput()->GetPointData()->SetNormals(0);

          vtkSmartPointer<vtkPolyDataMapper> boxMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          boxMapper->SetInputConnection(boxNormals->GetOutputPort());
          boxMapper->SetScalarVisibility(0);

          vtkSmartPointer<vtkActor> boxActor =
            vtkSmartPointer<vtkActor>::New();
          vtkSmartPointer<vtkInformation> boxKeyProperties =
            vtkSmartPointer<vtkInformation>::New();
          boxKeyProperties->Set(vtkShadowMapBakerPass::OCCLUDER(),0); // dummy val.
          boxKeyProperties->Set(vtkShadowMapBakerPass::RECEIVER(),0); // dummy val.
          boxActor->SetPropertyKeys(boxKeyProperties);

          boxActor->SetMapper(boxMapper);
          boxActor->SetVisibility(1);
          boxActor->SetPosition(-2.0,2.0,0.0);
          boxActor->GetProperty()->SetColor(1.0,0.0,0.0);

          vtkSmartPointer<vtkConeSource> coneSource =
            vtkSmartPointer<vtkConeSource>::New();
          coneSource->SetResolution(24);
          coneSource->SetDirection(1.0,1.0,1.0);
          vtkSmartPointer<vtkPolyDataMapper> coneMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          coneMapper->SetInputConnection(coneSource->GetOutputPort());
          coneMapper->SetScalarVisibility(0);

          vtkSmartPointer<vtkActor> coneActor =
            vtkSmartPointer<vtkActor>::New();
          vtkSmartPointer<vtkInformation> coneKeyProperties=
            vtkSmartPointer<vtkInformation>::New();
          coneKeyProperties->Set(vtkShadowMapBakerPass::OCCLUDER(),0); // dummy val.
          coneKeyProperties->Set(vtkShadowMapBakerPass::RECEIVER(),0); // dummy val.
          coneActor->SetPropertyKeys(coneKeyProperties);

          coneActor->SetMapper(coneMapper);
          coneActor->SetVisibility(1);
          coneActor->SetPosition(0.0,1.0,1.0);
          coneActor->GetProperty()->SetColor(0.0,0.0,1.0);
        //  coneActor->GetProperty()->SetLighting(false);

          vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
          sphereSource->SetThetaResolution(32);
          sphereSource->SetPhiResolution(32);
          vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
          sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
          sphereMapper->SetScalarVisibility(0);

          vtkSmartPointer<vtkActor> sphereActor =
            vtkSmartPointer<vtkActor>::New();
          vtkSmartPointer<vtkInformation> sphereKeyProperties =
            vtkSmartPointer<vtkInformation>::New();
          sphereKeyProperties->Set(vtkShadowMapBakerPass::OCCLUDER(),0); // dummy val.
          sphereKeyProperties->Set(vtkShadowMapBakerPass::RECEIVER(),0); // dummy val.
          sphereActor->SetPropertyKeys(sphereKeyProperties);
          sphereActor->SetMapper(sphereMapper);

          sphereActor->SetVisibility(1);
          sphereActor->SetPosition(2.0,2.0,-1.0);
          sphereActor->GetProperty()->SetColor(1.0,1.0,0.0);

          renderer->AddViewProp(rectangleActor);
          renderer->AddViewProp(boxActor);
          renderer->AddViewProp(coneActor);
          renderer->AddViewProp(sphereActor);

          // Spotlights.

          // lighting the box.
          vtkSmartPointer<vtkLight> l1 =
            vtkSmartPointer<vtkLight>::New();
          l1->SetPosition(-4.0,4.0,-1.0);
          l1->SetFocalPoint(boxActor->GetPosition());
          l1->SetColor(1.0,1.0,1.0);
          l1->SetPositional(1);
          renderer->AddLight(l1);
          l1->SetSwitch(1);

          // lighting the sphere
          vtkSmartPointer<vtkLight> l2 =
            vtkSmartPointer<vtkLight>::New();
          l2->SetPosition(4.0,5.0,1.0);
          l2->SetFocalPoint(sphereActor->GetPosition());
          l2->SetColor(1.0,0.0,1.0);
        //  l2->SetColor(1.0,1.0,1.0);
          l2->SetPositional(1);
          renderer->AddLight(l2);
          l2->SetSwitch(1);

          AddLightActors(renderer);

          renderer->SetBackground(0.66,0.66,0.66);
          renderer->SetBackground2(157.0/255.0*0.66,186/255.0*0.66,192.0/255.0*0.66);
          renderer->SetGradientBackground(true);

          renderWindow->Render();
          if(peeling->GetLastRenderingUsedDepthPeeling())
            {
            std::cout << "depth peeling was used" << std::endl;
            }
          else
            {
            std::cout << "depth peeling was not used (alpha blending instead)" << std::endl;
            }

          renderer->ResetCamera();
          vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
          camera->Azimuth(40.0);
          camera->Elevation(10.0);

          renderWindow->Render();

          interactor->Start();

          return EXIT_SUCCESS;
    }

};
#endif // TEST_VISUALALGORITHMS

