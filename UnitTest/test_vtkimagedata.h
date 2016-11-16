#ifndef TEST_VTKIMAGEDATA
#define TEST_VTKIMAGEDATA
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCast.h>
#include <vtkMetaImageReader.h>
#include <vtkImageActor.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageReslice.h>
#include <vtkImageProperty.h>
#include <vtkMatrix4x4.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageViewer2.h>
#include <vtkImageViewer.h>
#include <vtkImageShiftScale.h>

class Test_vtkImageData {
public:
    static int RunTest()
    {
//        vtkSmartPointer<vtkImageData> imageData =
//            vtkSmartPointer<vtkImageData>::New();

//          // Specify the size of the image data
//          imageData->SetDimensions(2,3,1);
//        #if VTK_MAJOR_VERSION <= 5
//          imageData->SetNumberOfScalarComponents(1);
//          imageData->SetScalarTypeToDouble();
//        #else
//          imageData->AllocateScalars(VTK_DOUBLE,1);
//        #endif

//          int* dims = imageData->GetDimensions();
//          // int dims[3]; // can't do this

//          std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;

//          std::cout << "Number of points: " << imageData->GetNumberOfPoints() << std::endl;
//          std::cout << "Number of cells: " << imageData->GetNumberOfCells() << std::endl;

//          // Fill every entry of the image data with "2.0"
//          for (int z = 0; z < dims[2]; z++)
//            {
//            for (int y = 0; y < dims[1]; y++)
//              {
//              for (int x = 0; x < dims[0]; x++)
//                {
//                double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
//                pixel[0] = 2.0;
//                }
//              }
//            }

//          // Retrieve the entries from the image data and print them to the screen
//          for (int z = 0; z < dims[2]; z++)
//            {
//            for (int y = 0; y < dims[1]; y++)
//              {
//              for (int x = 0; x < dims[0]; x++)
//                {
//                double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
//                // do something with v
//                std::cout << pixel[0] << " ";
//                }
//              std::cout << std::endl;
//              }
//            std::cout << std::endl;
//            }
        vtkSmartPointer<vtkMetaImageReader> reader =
               vtkSmartPointer<vtkMetaImageReader>::New();

         reader->SetFileName ( "D://3dresearch//TestData//stl//untitled.mhd" );
         reader->Update();
         int extent[6];
         reader->GetOutput()->GetExtent(extent);

         vtkSmartPointer<vtkImageShiftScale> shifter =
                 vtkSmartPointer<vtkImageShiftScale>::New();
 //         shifter->SetInputConnection(reader->GetOutputPort());
         shifter->SetInputData(reader->GetOutput());
         shifter->SetOutputScalarTypeToUnsignedChar();
         shifter->Update();
         vtkSmartPointer<vtkRenderWindowInteractor> iren =
              vtkSmartPointer<vtkRenderWindowInteractor>::New();
         vtkSmartPointer<vtkImageViewer2> viewer =
                 vtkSmartPointer<vtkImageViewer2>::New();
         viewer->GetRenderer()->SetBackground(0.4, 0.5, 0.6);
         viewer->SetSize(viewer->GetRenderer()->GetSize());
//         viewer->SetSize(400, 600);
         viewer->SetInputData(shifter->GetOutput());
//         viewer->SetInputConnection(shifter->GetOutputPort());
         viewer->SetColorLevel(127);
         viewer->SetColorWindow(255);
         viewer->SetupInteractor(iren);
         viewer->SetSliceOrientationToXY();
         viewer->SetSlice(40);
         viewer->Render();
         iren->Start();

//         int extent[6];
//         double spacing[3];
//         double origin[3];
//         reader->GetOutput()->GetExtent(extent);
//         reader->GetOutput()->GetSpacing(spacing);
//         reader->GetOutput()->GetOrigin(origin);
//         std::cout << extent[0] << ", " << extent[1] << ", "
//                          << extent[2] << ", " << extent[3] << ", "
//                          << extent[4] << ", " << extent[5] << std::endl;
//         std::cout << spacing[0] << ", " << spacing[1] << ", " << spacing[2] << std::endl;
//         std::cout << origin[0] << ", " << origin[1] << ", " << origin[2] << std::endl;
//         double center[3];
//         center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
//         center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
//         center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);
//         vtkImageData *imagedata = reader->GetOutput();
//         for (int z = 70; z < 75; z++)
//           {
//           for (int y = 200; y < 205; y++)
//             {
//             for (int x = 200; x < 205; x++)
//               {
//               double* pixel = static_cast<double*>(imagedata->GetScalarPointer(x,y,z));
//               // do something with v
//               std::cout << pixel[0] << " ";
//               }
//             std::cout << std::endl;
//             }
//           std::cout << std::endl;
//           }
//         static double axialElements[16] = {
//                 1, 0, 0, 0,
//                 0, 1, 0, 0,
//                 0, 0, 1, 0,
//                 0, 0, 0, 1 };

//         vtkSmartPointer<vtkMatrix4x4> resliceAxes =
//             vtkSmartPointer<vtkMatrix4x4>::New();
//         resliceAxes->DeepCopy(axialElements);
//         resliceAxes->SetElement(0, 3, center[0]);
//         resliceAxes->SetElement(1, 3, center[1]);
//         resliceAxes->SetElement(2, 3, center[2]);
//         vtkSmartPointer<vtkImageReslice> reslice =
//             vtkSmartPointer<vtkImageReslice>::New();
//         reslice->SetInputConnection(reader->GetOutputPort());
//         reslice->SetOutputDimensionality(2);
//         reslice->SetResliceAxes(resliceAxes);
//         reslice->SetInterpolationModeToLinear();
//         vtkSmartPointer<vtkLookupTable> colorTable =
//             vtkSmartPointer<vtkLookupTable>::New();
//         colorTable->SetRange(0, 1000);
//         colorTable->SetValueRange(0.0, 1.0);
//         colorTable->SetSaturationRange(0.0, 1.0);
//         colorTable->SetRampToLinear();
//         colorTable->Build();
//         vtkSmartPointer<vtkImageMapToColors> colorMap =
//             vtkSmartPointer<vtkImageMapToColors>::New();
//         colorMap->SetLookupTable(colorTable);
//         colorMap->SetInputConnection(reslice->GetOutputPort());
//         vtkSmartPointer<vtkImageActor> imgActor =
//              vtkSmartPointer<vtkImageActor>::New();
// //         imgActor->SetInput(colorMap->GetOutput());
// //         imgActor->SetInputData(colorMap->GetOutput());
//         imgActor->SetInputData((vtkImageData*)colorMap->GetOutput());
//         vtkSmartPointer<vtkRenderer> renderer =
//              vtkSmartPointer<vtkRenderer>::New();
//         renderer->AddActor(imgActor);
//         renderer->SetBackground(.4, .5, .6);
//         vtkSmartPointer<vtkRenderWindow> renderWindow =
//              vtkSmartPointer<vtkRenderWindow>::New();
//         renderWindow->SetSize(500, 500);
//         renderWindow->AddRenderer(renderer);
//         vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//              vtkSmartPointer<vtkRenderWindowInteractor>::New();
//         vtkSmartPointer<vtkInteractorStyleImage> imagestyle =
//              vtkSmartPointer<vtkInteractorStyleImage>::New();
//         renderWindowInteractor->SetInteractorStyle(imagestyle);
//         renderWindowInteractor->SetRenderWindow(renderWindow);
//         renderWindowInteractor->Initialize();
//         renderWindowInteractor->Start();
        return EXIT_SUCCESS;
    }
};

#endif // TEST_VTKIMAGEDATA

