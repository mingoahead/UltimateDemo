#ifndef TEST_ITK_DFB
#define TEST_ITK_DFB
//itk includes
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkDanielssonDistanceMapImageFilter.h>
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>
#include <itkUnaryFunctorImageFilter.h>
#include <itkPowImageFilter.h>
#include <itkSquareImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageSliceConstIteratorWithIndex.h>
#include <itkGradientMagnitudeImageFilter.h>
#include <itkInvertIntensityImageFilter.h>
#include <itkThresholdImageFilter.h>
#include <itkTimeProbe.h>
#include <itkMath.h>
#include <itkNumericTraits.h>
#include <itkPolyLineParametricPath.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkGradientDescentOptimizer.h>
#include <itkRegularStepGradientDescentOptimizer.h>
#include <itkPathIterator.h>
#include <itkNeighborhoodIterator.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkImageToVTKImageFilter.h>

#include <vtkImageViewer2.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkImageMapper3D.h>
#include <vtkImageMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>

class Test_DFB {
public:
    int RunTest()
    {
        return test_distanceMap();
    }
private:
    int test_distanceMap()
    {
        try{
            const unsigned int Dimension = 3;
            typedef unsigned char InputPixelType;
            typedef float InterPixelType;
            typedef unsigned char OutputPixelType;
            typedef itk::Image< InputPixelType, Dimension >                       InputImageType;
            typedef itk::Image< InterPixelType, Dimension >                       InterImageType;
            typedef itk::Image< OutputPixelType, Dimension>                       OutputImageType;
            typedef itk::ImageFileReader< InputImageType >                        ReaderType;

            std::string filename = "D://3dresearch//TestData//stl//122902.mhd";
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetFileName(filename);
            reader->Update();
            typedef itk::SignedMaurerDistanceMapImageFilter<
                    InputImageType,InterImageType> FilterType;
            FilterType::Pointer dft = FilterType::New();
            dft->SetInput(reader->GetOutput());
            dft->SetUseImageSpacing(true);
            dft->SetSquaredDistance(false);
            dft->SetInsideIsPositive(true);
            typedef itk::SmoothingRecursiveGaussianImageFilter<
                    InterImageType,InterImageType> SmoothType;
            SmoothType::Pointer smoother = SmoothType::New();
            smoother->SetInput(dft->GetOutput());
            smoother->SetSigma(1.0);
            typedef itk::ThresholdImageFilter<
                    InterImageType> ThresholdType;
            ThresholdType::Pointer thresholder = ThresholdType::New();
            thresholder->SetInput(smoother->GetOutput());
            thresholder->ThresholdBelow(-2);
            thresholder->ThresholdAbove(25);
            thresholder->SetOutsideValue(-3);
            thresholder->Update();
            typedef itk::RescaleIntensityImageFilter<
                    InterImageType,InterImageType> RescaleType;
            RescaleType::Pointer scaler = RescaleType::New();
            scaler->SetInput(thresholder->GetOutput());
            scaler->SetOutputMinimum(0.0);
            scaler->SetOutputMaximum(1.0);

            typedef itk::SquareImageFilter<
                    InterImageType, InterImageType> SquareType;
            SquareType::Pointer square1 = SquareType::New();
            square1->SetInput(scaler->GetOutput());
            SquareType::Pointer square2 = SquareType::New();
            square2->SetInput(square1->GetOutput());
            SquareType::Pointer square3 = SquareType::New();
            square3->SetInput(square2->GetOutput());
            SquareType::Pointer square4 = SquareType::New();
            square4->SetInput(square3->GetOutput());
            SquareType::Pointer square5 = SquareType::New();
            square5->SetInput(square4->GetOutput());
            square5->Update();

            RescaleType::Pointer scaler1 = RescaleType::New();
            scaler1->SetInput(square5->GetOutput());
            scaler1->SetOutputMinimum(0.0);
            scaler1->SetOutputMaximum(255.0);
            scaler1->Update();
            typedef itk::ImageToVTKImageFilter<InterImageType> ConnectorType;
            ConnectorType::Pointer connector1 = ConnectorType::New();

            connector1->SetInput(scaler1->GetOutput());
            connector1->Update();

//            vtkSmartPointer<vtkLookupTable> lookupTable =
//                vtkSmartPointer<vtkLookupTable>::New();
//            lookupTable->SetNumberOfTableValues(32);
//            lookupTable->SetRange(0.0, 255.0);
//            lookupTable->Build();
//            vtkSmartPointer<vtkImageMapToColors> scalarValuesToColors  =
//                    vtkSmartPointer<vtkImageMapToColors>::New();
//            scalarValuesToColors->SetLookupTable(lookupTable);
//            scalarValuesToColors->PassAlphaToOutputOn();
//            scalarValuesToColors->SetInputData(connector1->GetOutput());

            vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter1 =
                vtkSmartPointer<vtkImageDataGeometryFilter>::New();
            imageDataGeometryFilter1->SetInputData(connector1->GetOutput());
            imageDataGeometryFilter1->Update();
//            scalarValuesToColors->SetInputConnection(connector1->GetOutputPort());

//            vtkSmartPointer<vtkImageActor> imageActor =
//                    vtkSmartPointer<vtkImageActor>::New();
//            imageActor->GetMapper()->SetInputConnection(
//                scalarValuesToColors->GetOutputPort());
//            imageActor->GetProperty()->SetInterpolationTypeToNearest();
            vtkSmartPointer<vtkPolyDataMapper> mapper1 =
                    vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper1->SetInputData(imageDataGeometryFilter1->GetOutput());
            mapper1->Update();

            vtkSmartPointer<vtkActor> actor1 =
                    vtkSmartPointer<vtkActor>::New();
            actor1->SetMapper(mapper1);
//            actor1->GetProperty()->SetInterpolationToPhong();
            std::cout << "complet building color actor" << std::endl;
            vtkSmartPointer<vtkRenderer> renderer =
                    vtkSmartPointer<vtkRenderer>::New();
            renderer->AddActor(actor1);
            vtkSmartPointer<vtkRenderWindow> renderwindow =
                    vtkSmartPointer<vtkRenderWindow>::New();
            renderwindow->AddRenderer(renderer);
            vtkSmartPointer<vtkRenderWindowInteractor> iren =
                    vtkSmartPointer<vtkRenderWindowInteractor>::New();
            iren->SetRenderWindow(renderwindow);
            iren->Initialize();
            renderwindow->Render();
            iren->Start();
        }catch(itk::ExceptionObject& exp){
            std::cerr << exp.what() << std::endl;
            return EXIT_FAILURE;
        }
       return EXIT_SUCCESS;
    }
};

#endif // TEST_ITK_DFB

