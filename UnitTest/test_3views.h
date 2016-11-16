#ifndef TEST_3VIEWS
#define TEST_3VIEWS

#include "vtkDICOMImageReader.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkImageMapToColors.h"
#include "vtkImageActor.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkJPEGReader.h"
#include "vtkMetaImageReader.h"
#include "vtkSmartPointer.h"
#include <vtkExtractVOI.h>
#include <vtkPNGWriter.h>
#include "vtkImageMapper.h"
#include "vtkImageMapper3D.h"
#include "vtkImageProperty.h"
#include "vtkActor2D.h"
#include "vtkProperty2D.h"
#include "vtkImageMapper.h"
#include "vtkVolume16Reader.h"
#include "vtkDebugLeaks.h"

class vtkWidgetWindowLevelCallbackTest : public vtkCommand
{
public:
    static vtkWidgetWindowLevelCallbackTest *New()
    {
        return new vtkWidgetWindowLevelCallbackTest;
    }
    void Execute( vtkObject *caller, unsigned long vtkNotUsed( event ),
                  void *callData )
    {
        vtkImagePlaneWidget* self =
            reinterpret_cast< vtkImagePlaneWidget* >( caller );
        if(!self) return;
        double* wl = static_cast<double*>( callData );
        if ( self == this->WidgetX ) {
            this->WidgetY->SetWindowLevel(wl[0],wl[1]);
            this->WidgetZ->SetWindowLevel(wl[0],wl[1]);
        } else if( self == this->WidgetY ) {
            this->WidgetX->SetWindowLevel(wl[0],wl[1]);
            this->WidgetZ->SetWindowLevel(wl[0],wl[1]);
        } else if (self == this->WidgetZ) {
            this->WidgetX->SetWindowLevel(wl[0],wl[1]);
            this->WidgetY->SetWindowLevel(wl[0],wl[1]);
        }
    }
    vtkWidgetWindowLevelCallbackTest():WidgetX( 0 ), WidgetY( 0 ), WidgetZ ( 0 ) {}
    vtkImagePlaneWidget* WidgetX;
    vtkImagePlaneWidget* WidgetY;
    vtkImagePlaneWidget* WidgetZ;
};

class Test_3Views {
private:
    std::string m_dicomDirectory;
public:
    void SetInputDicomSeriesDirectory(std::string dicomDirectory)
    {
        m_dicomDirectory = dicomDirectory;
    }
    int RunTest()
    {
//        vtkDICOMImageReader *DicomReader = vtkDICOMImageReader::New();
//        DicomReader->SetDataByteOrderToLittleEndian();
//        DicomReader->SetDirectoryName(m_dicomDirectory.c_str());
//        DicomReader->Update();
        vtkVolume16Reader* v16 =  vtkVolume16Reader::New();
        v16->SetDataDimensions( 64, 64);
        v16->SetDataByteOrderToLittleEndian();
        v16->SetImageRange( 1, 93);
        v16->SetDataSpacing( 3.2, 3.2, 1.5);
        v16->SetFilePrefix("D://3dresearch//TestData//vtkdata//Data//headsq//quarter");
        v16->SetDataMask( 0x7fff);
        v16->Update();
      vtkOutlineFilter* outline = vtkOutlineFilter::New();
        outline->SetInputConnection(v16->GetOutputPort());
      vtkPolyDataMapper* outlineMapper = vtkPolyDataMapper::New();
        outlineMapper->SetInputConnection(outline->GetOutputPort());
      vtkActor* outlineActor =  vtkActor::New();
        outlineActor->SetMapper( outlineMapper);

          vtkRenderer* ren1 = vtkRenderer::New();
          vtkRenderer* ren2 = vtkRenderer::New();
          vtkRenderer* ren3 = vtkRenderer::New();
          vtkRenderer* ren = vtkRenderer::New();
          vtkRenderWindow* renWin = vtkRenderWindow::New();

            renWin->AddRenderer(ren1);
            renWin->AddRenderer(ren2);
            renWin->AddRenderer(ren3);
            renWin->AddRenderer(ren);
          vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
            iren->SetRenderWindow(renWin);
          vtkCellPicker* picker = vtkCellPicker::New();
            picker->SetTolerance(0.005);
          vtkProperty* ipwProp = vtkProperty::New();

           //assign default props to the ipw's texture plane actor

          vtkImagePlaneWidget* planeWidgetX = vtkImagePlaneWidget::New();
            planeWidgetX->SetInteractor( iren);
            planeWidgetX->SetKeyPressActivationValue('x');
            planeWidgetX->SetPicker(picker);
            planeWidgetX->RestrictPlaneToVolumeOn();
            planeWidgetX->GetPlaneProperty()->SetColor(1,0,0);
            planeWidgetX->SetTexturePlaneProperty(ipwProp);
            planeWidgetX->TextureInterpolateOff();
//            planeWidgetX->SetResliceInterpolateToNearestNeighbour();
            planeWidgetX->SetResliceInterpolateToLinear();
            planeWidgetX->SetInputData(v16->GetOutput());
            planeWidgetX->SetPlaneOrientationToXAxes();
            planeWidgetX->SetSliceIndex(32);
            planeWidgetX->GetTexturePlaneProperty()->SetOpacity(1);
            planeWidgetX->DisplayTextOn();
            planeWidgetX->On();
            planeWidgetX->InteractionOn();

          vtkImagePlaneWidget* planeWidgetY = vtkImagePlaneWidget::New();
            planeWidgetY->SetInteractor( iren);
            planeWidgetY->SetKeyPressActivationValue('y');
            planeWidgetY->SetPicker(picker);
            planeWidgetY->GetPlaneProperty()->SetColor(1,1,0);
            planeWidgetY->SetTexturePlaneProperty(ipwProp);
            planeWidgetY->TextureInterpolateOn();
            planeWidgetY->SetResliceInterpolateToLinear();
            planeWidgetY->SetInputData(v16->GetOutput());
            planeWidgetY->SetPlaneOrientationToYAxes();
            planeWidgetY->SetSlicePosition(122.5);
            planeWidgetY->GetTexturePlaneProperty()->SetOpacity(1);
//            planeWidgetY->SetLookupTable( planeWidgetX->GetLookupTable());
            planeWidgetY->DisplayTextOn();
//            planeWidgetY->UpdatePlacement();
            planeWidgetY->On();
          vtkImagePlaneWidget* planeWidgetZ = vtkImagePlaneWidget::New();
            planeWidgetZ->SetInteractor( iren);
            planeWidgetZ->SetKeyPressActivationValue('z');
            planeWidgetZ->SetPicker(picker);
            planeWidgetZ->GetPlaneProperty()->SetColor(0,0,1);
            planeWidgetZ->SetTexturePlaneProperty(ipwProp);
            planeWidgetZ->TextureInterpolateOn();
//            planeWidgetZ->SetResliceInterpolateToCubic();
            planeWidgetZ->SetResliceInterpolateToLinear();
            planeWidgetZ->SetInputData(v16->GetOutput());
            planeWidgetZ->SetPlaneOrientationToZAxes();
            planeWidgetZ->SetSliceIndex(46);
            planeWidgetZ->GetTexturePlaneProperty()->SetOpacity(1);
//            planeWidgetZ->SetLookupTable( planeWidgetX->GetLookupTable());
            planeWidgetZ->DisplayTextOn();
            planeWidgetZ->On();
           vtkWidgetWindowLevelCallbackTest* cbk = vtkWidgetWindowLevelCallbackTest::New();
           cbk->WidgetX = planeWidgetX;
           cbk->WidgetY = planeWidgetY;
           cbk->WidgetZ = planeWidgetZ;
//           planeWidgetX->AddObserver( vtkCommand::EndWindowLevelEvent, cbk );
//           planeWidgetY->AddObserver( vtkCommand::EndWindowLevelEvent, cbk );
//           planeWidgetZ->AddObserver( vtkCommand::EndWindowLevelEvent, cbk );
           cbk->Delete();
          //double wl[2];
        // planeWidgetZ->GetWindowLevel(wl);

          // Add a 2D image to test the GetReslice method
          //
           vtkImageMapToColors* colorMap1 = vtkImageMapToColors::New();
            colorMap1->PassAlphaToOutputOff();
            colorMap1->SetActiveComponent(0);
            colorMap1->SetOutputFormatToLuminance();
            colorMap1->SetInputData((vtkImageData*)planeWidgetX->GetResliceOutput());
            colorMap1->SetLookupTable((vtkScalarsToColors*)planeWidgetX->GetLookupTable());
           vtkImageActor* imageActor1 = vtkImageActor::New();
            imageActor1->PickableOff();
            imageActor1->SetInputData(colorMap1->GetOutput());

           vtkImageMapToColors* colorMap2 = vtkImageMapToColors::New();
            colorMap2->PassAlphaToOutputOff();
            colorMap2->SetActiveComponent(0);
            colorMap2->SetOutputFormatToLuminance();
            colorMap2->SetInputData((vtkImageData*)planeWidgetY->GetResliceOutput());
            colorMap2->SetLookupTable((vtkScalarsToColors*)planeWidgetX->GetLookupTable());
          vtkImageActor* imageActor2 = vtkImageActor::New();
            imageActor2->PickableOff();
            imageActor2->SetInputData(colorMap2->GetOutput());
          vtkImageMapToColors* colorMap3 = vtkImageMapToColors::New();
            colorMap3->PassAlphaToOutputOff();
            colorMap3->SetActiveComponent(0);
            colorMap3->SetOutputFormatToLuminance();
            colorMap3->SetInputData((vtkImageData*)planeWidgetZ->GetResliceOutput());
            colorMap3->SetLookupTable((vtkScalarsToColors*)planeWidgetX->GetLookupTable());
          vtkImageActor* imageActor3 = vtkImageActor::New();
            imageActor3->PickableOff();
            imageActor3->SetInputData(colorMap3->GetOutput());
           //Add the actors

          ren->AddActor( outlineActor);
          ren1->AddActor( imageActor1);
          ren2->AddActor( imageActor2);
          ren3->AddActor( imageActor3);
          ren->SetBackground( 0.1, 0.1, 0.2);
          ren1->SetBackground( 0.1, 0.2, 0.1);
          ren2->SetBackground( 0.2, 0.1, 0.2);
          ren3->SetBackground( 0.2, 0.2, 0.2);
          renWin->SetSize( 600, 400);
          ren->SetViewport(0,0.5,0.5,1);
          ren1->SetViewport(0.5,0.5,1,1);
          ren2->SetViewport(0,0,0.5,0.5);
          ren3->SetViewport(0.5,0,1,0.5);
          // Set the actors' postions
          //
//          renWin->Render();
          //iren->SetEventPosition(100,200);
          //iren->SetKeyCode('r');
          //iren->InvokeEvent(vtkCommand::CharEvent,NULL);
          //iren->SetEventPosition( 200,200);
          //iren->SetKeyCode('r');
        // iren->InvokeEvent(vtkCommand::CharEvent,NULL);


          ren->GetActiveCamera()->Elevation(110);
          ren->GetActiveCamera()->SetViewUp(0, 0, -1);
          ren->GetActiveCamera()->Azimuth(45);
          ren->GetActiveCamera()->Dolly(1.15);
          ren->ResetCameraClippingRange();

          iren->Initialize();
          iren->Start();
          renWin->Render();
          // Test SetKeyPressActivationValue for one of the widgets
          //
          //iren->SetKeyCode('z');
          //iren->InvokeEvent(vtkCommand::CharEvent,NULL);
          //iren->SetKeyCode('z');
          //iren->InvokeEvent(vtkCommand::CharEvent,NULL);

          // Remove the observers so we can go interactive. Without this the "-I"
          // testing option fails.
//          int retVal = vtkRegressionTestImage( renWin );

//          if ( retVal == vtkRegressionTester::DO_INTERACTOR)
//          {
//            iren->Start();
//          }
           //Clean up
//          cbk->Delete();
//          ipwProp->Delete();
//          planeWidgetX->Delete();
//          planeWidgetY->Delete();
//          planeWidgetZ->Delete();
//          colorMap1->Delete();
//          imageActor1->Delete();
//          colorMap2->Delete();
//          imageActor2->Delete();
//          colorMap3->Delete();
//          imageActor3->Delete();
//          picker->Delete();
//          outlineActor->Delete();
//          outlineMapper->Delete();
//          outline->Delete();
//          iren->Delete();
//          renWin->Delete();
//          ren->Delete();
//          ren1->Delete();
//          ren2->Delete();
//          ren3->Delete();
//          v16->Delete();
          return 0;

//        vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
//        jpegReader->SetFileName("D:\\3dresearch\\TestData\\image\\apple.jpg");
//        jpegReader->Update();

//        vtkSmartPointer<vtkImageMapper> imageMapper = vtkSmartPointer<vtkImageMapper>::New();
//        imageMapper->SetInputConnection(jpegReader->GetOutputPort());

//        vtkSmartPointer<vtkActor2D> imageActor = vtkSmartPointer<vtkActor2D>::New();
//        imageActor->SetMapper(imageMapper);

//        vtkSmartPointer<vtkExtractVOI> extractVOI = vtkSmartPointer<vtkExtractVOI>::New();
//        extractVOI->SetInputConnection(colorMap3->GetOutputPort());
//        extractVOI->SetVOI(0,511,0,511, 0, 0);
//        extractVOI->Update();
//        vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
//        writer->SetFileName("D:\\3dresearch\\TestData\\image\\test.png");
//        writer->SetInputConnection(extractVOI->GetOutputPort());
//        writer->Write();
    }
};
#endif // TEST_3VIEWS

