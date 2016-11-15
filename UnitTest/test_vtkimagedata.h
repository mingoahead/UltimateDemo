#ifndef TEST_VTKIMAGEDATA
#define TEST_VTKIMAGEDATA
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
class Test_vtkImageData {
public:
    static int RunTest()
    {
        vtkSmartPointer<vtkImageData> imageData =
            vtkSmartPointer<vtkImageData>::New();

          // Specify the size of the image data
          imageData->SetDimensions(2,3,1);
        #if VTK_MAJOR_VERSION <= 5
          imageData->SetNumberOfScalarComponents(1);
          imageData->SetScalarTypeToDouble();
        #else
          imageData->AllocateScalars(VTK_DOUBLE,1);
        #endif

          int* dims = imageData->GetDimensions();
          // int dims[3]; // can't do this

          std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;

          std::cout << "Number of points: " << imageData->GetNumberOfPoints() << std::endl;
          std::cout << "Number of cells: " << imageData->GetNumberOfCells() << std::endl;

          // Fill every entry of the image data with "2.0"
          for (int z = 0; z < dims[2]; z++)
            {
            for (int y = 0; y < dims[1]; y++)
              {
              for (int x = 0; x < dims[0]; x++)
                {
                double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
                pixel[0] = 2.0;
                }
              }
            }

          // Retrieve the entries from the image data and print them to the screen
          for (int z = 0; z < dims[2]; z++)
            {
            for (int y = 0; y < dims[1]; y++)
              {
              for (int x = 0; x < dims[0]; x++)
                {
                double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
                // do something with v
                std::cout << pixel[0] << " ";
                }
              std::cout << std::endl;
              }
            std::cout << std::endl;
            }
          return EXIT_SUCCESS;
    }
};

#endif // TEST_VTKIMAGEDATA

