#ifndef TESTIMAGESLICE
#define TESTIMAGESLICE

#include <vtkVersion.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageMapper.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageSlice.h>

class Test_ImageSlice {
public:
    static int RunTest ()
    {
        vtkSmartPointer<vtkImageData> colorImage = vtkSmartPointer<vtkImageData>::New();
        CreateColorImage(colorImage);

        vtkSmartPointer<vtkImageResliceMapper> imageResliceMapper = vtkSmartPointer<vtkImageResliceMapper>::New();
#if VTK_MAJOR_VERSION <= 5
        imageResliceMapper->SetInputConnection(colorImage->GetProducerPort());
#else
        imageResliceMapper->SetInputData(colorImage);
#endif

        vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
        imageSlice->SetMapper(imageResliceMapper);

        // Setup renderers
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddViewProp(imageSlice);
        renderer->ResetCamera();

        // Setup render window
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->SetSize(300, 300);
        renderWindow->AddRenderer(renderer);

        // Setup render window interactor
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();

        vtkSmartPointer<vtkInteractorStyleImage> style =
            vtkSmartPointer<vtkInteractorStyleImage>::New();

        renderWindowInteractor->SetInteractorStyle(style);

        // Render and start interaction
        renderWindowInteractor->SetRenderWindow(renderWindow);
        renderWindowInteractor->Initialize();

        renderWindowInteractor->Start();

        return EXIT_SUCCESS;
    }

private:
    static void CreateColorImage(vtkImageData* image)
    {
        image->SetDimensions(10, 10, 1);
#if VTK_MAJOR_VERSION <= 5
        image->SetScalarTypeToUnsignedChar();
        image->SetNumberOfScalarComponents(3);
        image->AllocateScalars();
#else
        image->AllocateScalars(VTK_UNSIGNED_CHAR,3);
#endif
        for(unsigned int x = 0; x < 10; x++) {
            for(unsigned int y = 0; y < 10; y++) {
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(x,y,0));
                pixel[0] = 255;
                pixel[1] = 0;
                pixel[2] = 255;
            }
        }
    }
};

#endif // TESTIMAGESLICE

