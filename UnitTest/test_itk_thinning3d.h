#ifndef TEST_ITK_THINNING3D
#define TEST_ITK_THINNING3D

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageToVTKImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImageSliceIteratorWithIndex.h"

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

#include "../Modules/CenLine_2/itkBinaryThinningImageFilter3D.h"

#include <string>
#include <cstdlib>
#include <QFileDialog>
#include <QString>
class Test_Itk_3DThinning {
private:
     std::string m_inputFile;
     void SetInputName(std::string filename)
     {
         m_inputFile = filename;
     }
public:
     int RunTest()
     {
         std::string fileName = "D://3dresearch//TestData//stl//122902.mhd";
//         std::string fileName = "D://3dresearch//TestData//img_in.dcm";
         SetInputName(fileName);
         const unsigned int Dimension = 3;
         typedef signed short PixelType;
         typedef itk::Image<PixelType, Dimension > ImageType;

         // Read image
         typedef itk::ImageFileReader< ImageType > ReaderType;
         ReaderType::Pointer reader = ReaderType::New();
         reader->SetFileName( m_inputFile.c_str() );
         try
           {
             reader->Update();
           }
           catch (itk::ExceptionObject &ex)
           {
             std::cout << ex << std::endl;
             return EXIT_FAILURE;
           }
         std::cout << " sucessfully read " << m_inputFile <<  std::endl;
         ImageType::Pointer image = reader->GetOutput();
         ImageType::SizeType regionSize;
         regionSize[0] = 500;
         regionSize[1] = 500;
         regionSize[2] = 10;
         ImageType::IndexType regionIndex;
         regionIndex[0] = 0;
         regionIndex[1] = 0;
         regionIndex[2] = 100;
         ImageType::RegionType region;
         region.SetSize(regionSize);
         region.SetIndex(regionIndex);
         typedef itk::RegionOfInterestImageFilter<ImageType, ImageType> RegionCutType;
         RegionCutType::Pointer subregion = RegionCutType::New();
         subregion->SetInput(reader->GetOutput());
         subregion->SetRegionOfInterest(region);

         std::cout << " sucessfully cut subRegion " << m_inputFile <<  std::endl;
         typedef itk::ImageSliceIteratorWithIndex<ImageType> SliceIteratorType;
         ImageType::Pointer InputImg = subregion->GetOutput();
         subregion->Update();

         SliceIteratorType inputIt(InputImg, InputImg->GetRequestedRegion());
         inputIt.SetFirstDirection(0);
         inputIt.SetSecondDirection(1);
         inputIt.GoToBegin();
         while (!inputIt.IsAtEnd()) {
             int cnt = 0;
             while (!inputIt.IsAtEndOfSlice()) {
                 while (!inputIt.IsAtEndOfLine()) {
                     if(inputIt.Get() != 0) {
//                         std::cout << inputIt.Get() << "  ";
                         cnt++;
                     }
                     ++inputIt;

                 }
//                 std::cout << std::endl;
                 inputIt.NextLine();
             }
             std::cout << "Total num of '1's : " << cnt << std::endl;
             inputIt.NextSlice();
         }
         std::cout << "---------------------------------" << std::endl;
         typedef itk::ImageToVTKImageFilter <ImageType> ConnectorType;
         ConnectorType::Pointer connector1 = ConnectorType::New();

         connector1->SetInput(subregion->GetOutput());
         connector1->Update();

         vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter1 =
             vtkSmartPointer<vtkImageDataGeometryFilter>::New();
         imageDataGeometryFilter1->SetInputData(connector1->GetOutput());
         imageDataGeometryFilter1->Update();

         vtkSmartPointer<vtkPolyDataMapper> mapper1 =
                 vtkSmartPointer<vtkPolyDataMapper>::New();
         mapper1->SetInputData(imageDataGeometryFilter1->GetOutput());
         mapper1->Update();

         vtkSmartPointer<vtkActor> actor1 =
                 vtkSmartPointer<vtkActor>::New();
         actor1->SetMapper(mapper1);


         // Define the thinning filter
         //***********************************************************************
         typedef itk::BinaryThinningImageFilter3D< ImageType, ImageType > ThinningFilterType;
         ThinningFilterType::Pointer thinningFilter = ThinningFilterType::New();
         thinningFilter->SetInput( subregion->GetOutput() );
         thinningFilter->Update();

         ImageType::Pointer OutputImg = thinningFilter->GetOutput();
         SliceIteratorType outputIt(OutputImg, OutputImg->GetRequestedRegion());
         outputIt.SetFirstDirection(0);
         outputIt.SetSecondDirection(1);
         outputIt.GoToBegin();
         while (!outputIt.IsAtEnd()) {
             int cnt = 0;
             while (!outputIt.IsAtEndOfSlice()) {
                 while (!outputIt.IsAtEndOfLine()) {
                     if(outputIt.Get() != 0) {
//                         std::cout << inputIt.Get() << "  ";
                         cnt++;
                     }
                     ++outputIt;

                 }
//                 std::cout << std::endl;
                 outputIt.NextLine();
             }
             std::cout << "Total num of '1's : " << cnt << std::endl;
             outputIt.NextSlice();
         }

         ConnectorType::Pointer connector2 = ConnectorType::New();
         connector2->SetInput(thinningFilter->GetOutput());
         connector2->Update();

        //***********************************************************************

//         std::cout << "build vtk imagedata for : " << m_inputFile << std::endl;

         vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter2 =
             vtkSmartPointer<vtkImageDataGeometryFilter>::New();
         imageDataGeometryFilter2->SetInputData(connector2->GetOutput());
         imageDataGeometryFilter2->Update();
//         std::cout << "build vtk polydata for : " << m_inputFile << std::endl;

         vtkSmartPointer<vtkPolyDataMapper> mapper2 =
                 vtkSmartPointer<vtkPolyDataMapper>::New();
         mapper2->SetInputData(imageDataGeometryFilter2->GetOutput());
         mapper2->Update();
//         vtkSmartPointer<vtkImageActor> actor =
//                 vtkSmartPointer<vtkImageActor>::New();
//         actor->GetMapper()->SetInputData(connector->GetOutput());

         vtkSmartPointer<vtkActor> actor2 =
                 vtkSmartPointer<vtkActor>::New();
         actor2->SetMapper(mapper2);

//         actor->GetProperty()->SetColor(.0, 1.0, .0);
         vtkSmartPointer<vtkRenderer> renderer1 =
                 vtkSmartPointer<vtkRenderer>::New();
         vtkSmartPointer<vtkRenderer> renderer2 =
                 vtkSmartPointer<vtkRenderer>::New();
         renderer1->SetBackground(.2, .3, .4);
         renderer1->SetViewport(0, 0, 0.5, 1);
         renderer2->SetBackground(.4, .5, .4);
         renderer2->SetViewport(0.5, 0., 1, 1);
         renderer1->AddActor(actor1);
         renderer2->AddActor(actor2);
         vtkSmartPointer<vtkRenderWindow> renderWindow =
                 vtkSmartPointer<vtkRenderWindow>::New();
         renderWindow->SetSize(1000, 500);
         renderWindow->AddRenderer(renderer1);
         renderWindow->AddRenderer(renderer2);
         vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
                 vtkSmartPointer<vtkRenderWindowInteractor>::New();

         renderWindowInteractor->SetRenderWindow(renderWindow);
         renderWindow->Render();
         renderWindowInteractor->Initialize();
         renderWindowInteractor->Start();

         return EXIT_SUCCESS;
     }

};
#endif // TEST_ITK_THINNING3D

