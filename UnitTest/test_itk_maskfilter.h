#ifndef TEST_ITK_MASKFILTER
#define TEST_ITK_MASKFILTER

#include "itkImageFileReader.h"
#include "itkMaskImageFilter.h"
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageShiftScale.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkSTLReader.h>

class Test_Itk_MaskFilter {
public:
    int RunTest()
    {
        std::string InFileName = "D://3dresearch//Wu-yj//Ultimate//Roam-part//data//testdata//09//untitled.mhd";
        std::string MaskFileName = "D://3dresearch//Wu-yj//Ultimate//Roam-part//data//testdata//09//122802.mhd";
        std::string StlFilaName = "D://3dresearch//Wu-yj//Ultimate//Roam-part//data//testdata//09//122802.stl";
        const unsigned int Dimension = 3;
        typedef signed short PixelType;
        typedef itk::Image<PixelType, Dimension > ImageType;
        // Read image
        typedef itk::ImageFileReader< ImageType > ReaderType;
        ReaderType::Pointer reader1 = ReaderType::New();
        reader1->SetFileName(InFileName);
        try{
            reader1->Update();
        }catch (itk::ExceptionObject &ex) {
            std::cout << ex << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Sucessfully read input:  " << InFileName << std::endl;
        ReaderType::Pointer reader2 = ReaderType::New();
        reader2->SetFileName(MaskFileName);
        try{
            reader2->Update();
        }catch (itk::ExceptionObject &ex) {
            std::cout << ex << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Sucessfully read mask : " << MaskFileName << std::endl;
        typedef itk::MaskImageFilter<ImageType, ImageType> MaskFilterType;
        MaskFilterType::Pointer maskfilter = MaskFilterType::New();
        maskfilter->SetInput(reader1->GetOutput());
        maskfilter->SetMaskImage(reader2->GetOutput());
//        maskfilter->Update();

        typedef itk::ImageToVTKImageFilter <ImageType> ConnectorType;
        ConnectorType::Pointer connector1 = ConnectorType::New();
        connector1->SetInput(maskfilter->GetOutput());
        connector1->Update();

        vtkSmartPointer<vtkImageShiftScale> scale
                = vtkSmartPointer<vtkImageShiftScale>::New();
        scale -> SetInputData(connector1->GetOutput());
        scale -> SetOutputScalarTypeToUnsignedChar();
        scale -> Update();

        vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction
                = vtkSmartPointer<vtkPiecewiseFunction>::New();
//        opacityTransferFunction->AddPoint(0.0, 0.0);
//        opacityTransferFunction->AddPoint(36.0, 0.125);
//        opacityTransferFunction->AddPoint(72.0, 0.25);
//        opacityTransferFunction->AddPoint(108.0, 0.375);
//        opacityTransferFunction->AddPoint(144.0, 0.5);
//        opacityTransferFunction->AddPoint(180.0, 0.625);
//        opacityTransferFunction->AddPoint(216.0, 0.75);
//        opacityTransferFunction->AddPoint(255.0, 0.875);
        opacityTransferFunction->AddPoint(50.0, 0.0);
        opacityTransferFunction->AddPoint(120.0, 0.5);
        opacityTransferFunction->AddPoint(220.0, 0.0);

        vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction
                = vtkSmartPointer<vtkColorTransferFunction>::New();
//        colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
//        colorTransferFunction->AddRGBPoint(36.0, 1.0, 0.0, 0.0);
//        colorTransferFunction->AddRGBPoint(72.0, 1.0, 1.0, 0.0);
//        colorTransferFunction->AddRGBPoint(108.0, 0.0, 1.0, 0.0);
//        colorTransferFunction->AddRGBPoint(144.0, 0.0, 1.0, 1.0);
//        colorTransferFunction->AddRGBPoint(180.0, 0.0, 0.0, 1.0);
//        colorTransferFunction->AddRGBPoint(216.0, 1.0, 0.0, 1.0);
//        colorTransferFunction->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
        colorTransferFunction->AddRGBPoint(40.0, 0.1, 0.1, 0.7);
        colorTransferFunction->AddRGBPoint(70.0, 0.1, 0.1, 0/7);
        colorTransferFunction->AddRGBPoint(160.0,1.0,0.35,0.0);
        colorTransferFunction->AddRGBPoint(190.0, 0.99, 0.99, 0.99);

        vtkSmartPointer<vtkVolumeProperty> volumeProperty
                = vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->SetColor(colorTransferFunction);
        volumeProperty->SetScalarOpacity(opacityTransferFunction);
        volumeProperty->ShadeOn();
        volumeProperty->SetAmbient(0.25);
        volumeProperty->SetDiffuse(0.9);
        volumeProperty->SetSpecular(0.2);
        volumeProperty->SetSpecularPower(20);
        volumeProperty->SetInterpolationTypeToLinear();

        vtkSmartPointer<vtkSmartVolumeMapper> smartMapper
                = vtkSmartPointer<vtkSmartVolumeMapper>::New();
        smartMapper->SetMaxMemoryInBytes(1847483648 * 1.5);
        smartMapper->SetRequestedRenderMode(vtkSmartVolumeMapper::GPURenderMode);
        smartMapper->SetInputData(scale->GetOutput());

        vtkSmartPointer<vtkVolume> volume =
                vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(smartMapper);
        volume->SetProperty(volumeProperty);





//        vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter1
//                = vtkSmartPointer<vtkImageDataGeometryFilter>::New();
//        imageDataGeometryFilter1->SetInputData(connector1->GetOutput());
//        imageDataGeometryFilter1->Update();

//        vtkSmartPointer<vtkPolyDataMapper> mapper1
//                = vtkSmartPointer<vtkPolyDataMapper>::New();
//        mapper1->SetInputData(imageDataGeometryFilter1->GetOutput());
//        mapper1->Update();

//        vtkSmartPointer<vtkActor> actor1 =
//                vtkSmartPointer<vtkActor>::New();
//        actor1->SetMapper(mapper1);
        vtkSmartPointer<vtkSTLReader> reader3 =
                vtkSmartPointer<vtkSTLReader>::New();
        reader3->SetFileName(StlFilaName.c_str());
        reader3->Update();
        vtkSmartPointer<vtkPolyDataMapper> mapper3 =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper3->SetInputData(reader3->GetOutput());
        mapper3->Update();
        vtkSmartPointer<vtkActor> segactor =
                vtkSmartPointer<vtkActor>::New();
        segactor->SetMapper(mapper3);
        segactor->GetProperty()->SetColor(0.4, 0.4, 0.5);
        vtkSmartPointer<vtkRenderer> renderer1 =
                vtkSmartPointer<vtkRenderer>::New();
        renderer1->SetBackground(.2, .3, .4);
        renderer1->AddVolume(volume);
        vtkSmartPointer<vtkRenderer> renderer2 =
                vtkSmartPointer<vtkRenderer>::New();
        renderer2->SetBackground(.3, .4, .4);

        renderer2->AddActor(segactor);
        renderer1 -> SetViewport(0, 0, 0.5, 1);
        renderer2 -> SetViewport(.5, 0, 1, 1);
        vtkSmartPointer<vtkRenderWindow> renderWindow =
                vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->SetSize(1000, 500);
        renderWindow->AddRenderer(renderer1);
        renderWindow->AddRenderer(renderer2);
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
                vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);

        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style
                = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        renderWindowInteractor->SetInteractorStyle(style);

        renderWindow->Render();
        renderWindowInteractor->Initialize();
        renderWindowInteractor->Start();

        return EXIT_SUCCESS;







    }
};

#endif // TEST_ITK_MASKFILTER

