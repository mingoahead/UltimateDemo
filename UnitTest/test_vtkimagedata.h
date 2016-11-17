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
        std::string filePath = "D://3dresearch//TestData//stl//untitled.mhd";
        vtkSmartPointer<vtkMetaImageReader> reader =
                vtkSmartPointer<vtkMetaImageReader>::New();
        reader->SetFileName(filePath.c_str());
        reader->Update();
        std::cout << "successfully load image: " << filePath << std::endl;
        int extent[6];
        double spacing[3];
        double origin[3];
        reader->GetOutput()->GetExtent(extent);
        reader->GetOutput()->GetSpacing(spacing);
        reader->GetOutput()->GetOrigin(origin);
        double center[3];
        center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
        center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
        center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);
        std::cout << center[0] << ", " << center[1] << ", " << center[2] << std::endl;
        vtkSmartPointer<vtkImageResliceMapper> mapper =
                vtkSmartPointer<vtkImageResliceMapper>::New();
        mapper->SetInputConnection(reader->GetOutputPort());
        mapper->SliceFacesCameraOn();
        mapper->SliceAtFocalPointOn();
        mapper->BorderOff();
        vtkSmartPointer<vtkImageProperty> property =
                vtkSmartPointer<vtkImageProperty>::New();
        property->SetColorWindow(1000);
        property->SetColorLevel(500);
        property->SetAmbient(0.0);
        property->SetDiffuse(1.0);
        property->SetOpacity(1.0);
        property->SetInterpolationTypeToLinear();
        vtkSmartPointer<vtkImageSlice> slice =
                vtkSmartPointer<vtkImageSlice>::New();
        slice->SetMapper(mapper);
        slice->SetProperty(property);

        vtkSmartPointer<vtkRenderer> renderer =
                vtkSmartPointer<vtkRenderer>::New();
        renderer->AddViewProp(slice);
        renderer->SetBackground(.2, .3, .4);
        vtkSmartPointer<vtkRenderWindow> renwin =
                vtkSmartPointer<vtkRenderWindow>::New();
        renwin->AddRenderer(renderer);
        vtkSmartPointer<vtkRenderWindowInteractor> renderwindowIteractor =
                vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderwindowIteractor->SetRenderWindow(renwin);
        vtkSmartPointer<vtkInteractorStyleImage> style =
                vtkSmartPointer<vtkInteractorStyleImage>::New();
//        style->SetInteractionModeToImage3D();
        style->SetInteractionModeToImageSlicing();
        style->SetInteractor(renderwindowIteractor);
        renwin->Render();
        renderwindowIteractor->Start();
        return EXIT_SUCCESS;
    }
};

#endif // TEST_VTKIMAGEDATA

