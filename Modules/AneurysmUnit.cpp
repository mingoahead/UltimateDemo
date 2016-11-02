#include "AneurysmUnit.h"


AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{

//    m_renderer = vtkSmartPointer<vtkRenderer>::New();
//    m_segmentationModel = vtkSmartPointer<vtkActor>::New();
//    m_3DReg_segmentationReader = vtkSmartPointer<vtkSTLReader>::New();

    vsp(m_renderer);
    vsp(m_ul_renderer);
    vsp(m_ur_renderer);
    vsp(m_bl_renderer);
    vsp(m_br_renderer);
    vsp(m_3DReg_segmentationModel);
    vsp(m_LevelSet_segmentationModel);
    vsp(m_RegDetect_segmentationModel);
    vsp(m_3DReg_segmentationReader);
    vsp(m_LevelSet_segmentationReader);
    vsp(m_RegDetect_segmentationReader);
//    init three model color
    m_3DReg_segmentationModel -> GetProperty() -> SetColor(1.0, .0, .0);
    m_LevelSet_segmentationModel -> GetProperty() -> SetColor(.0, 1.0, .0);
    m_RegDetect_segmentationModel -> GetProperty() -> SetColor(.0, .0, 1.0);
    vsp(m_tranViewer);
    vsp(m_corViewer);
    vsp(m_sagViewer);
    InitAnnotation();
    InitSliders();
}

AneurysmUnit::~AneurysmUnit()
{

}

vtkRenderer *AneurysmUnit::GetRenderer() {return m_renderer;}

vtkRenderer *AneurysmUnit::GetBLRenderer() {return m_bl_renderer; }

vtkRenderer *AneurysmUnit::GetBRRenderer() {return m_br_renderer;}

vtkRenderer *AneurysmUnit::GetULRenderer() {return m_ul_renderer;}

vtkRenderer *AneurysmUnit::GetURRenderer() {return m_br_renderer;}

vtkRenderer *AneurysmUnit::GettranViewerRenderer() {return m_tranViewerRenderer;}

vtkRenderer *AneurysmUnit::GetcorViewerRenderer() {return m_corViewerRenderer;}

vtkRenderer *AneurysmUnit::GetsagViewerRenderer() {return m_sagViewerRenderer;}

void AneurysmUnit::ReadInputSegmentationModel(std::string fileName, int option)
{

    switch(option) {
    case 1:
        m_3DReg_segmentationReader -> SetFileName(fileName.c_str());
        m_3DReg_segmentationReader -> Update();
        {
        vtkSmartPointer<vtkPolyDataMapper> mapper1
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper1 -> SetInputConnection(m_3DReg_segmentationReader -> GetOutputPort());
        m_3DReg_segmentationModel -> SetMapper(mapper1);
        }

        break;
    case 2:
        m_LevelSet_segmentationReader -> SetFileName(fileName.c_str());
        m_LevelSet_segmentationReader -> Update();
        {vtkSmartPointer<vtkPolyDataMapper> mapper2
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper2 -> SetInputConnection(m_LevelSet_segmentationReader -> GetOutputPort());
        m_LevelSet_segmentationModel -> SetMapper(mapper2);
        }
        break;
    case 3:
        m_RegDetect_segmentationReader -> SetFileName(fileName.c_str());
        m_RegDetect_segmentationReader -> Update();
        {vtkSmartPointer<vtkPolyDataMapper> mapper3
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper3 -> SetInputConnection(m_RegDetect_segmentationReader -> GetOutputPort());
        m_RegDetect_segmentationModel -> SetMapper(mapper3);
        }
        break;
    default:
        break;

    }
    //m_renderer -> ResetCamera();
}


void AneurysmUnit::ShowSegmentationModel(int option)
{
//    switch (option) {
//    case 1:
//        m_renderer -> AddActor(m_3DReg_segmentationModel);
//        break;
//    case 2:
//        m_renderer -> AddActor(m_LevelSet_segmentationModel);
//        break;
//    case 3:
//        m_renderer -> AddActor(m_RegDetect_segmentationModel);
//        break;
//    default:
//        break;
//    }
//    m_renderer -> ResetCamera();
    ShowFrameMode(option);
}

void AneurysmUnit::ShowFrameMode(int option)
{
    switch (option) {
    case 1:
        m_renderer -> AddActor(m_3DReg_segmentationModel);
        m_3DReg_segmentationModel -> GetProperty()->SetRepresentationToSurface();
        m_slider1Widget -> EnabledOn();
        break;
    case 2:
        m_renderer -> AddActor(m_LevelSet_segmentationModel);
        m_LevelSet_segmentationModel -> GetProperty() -> SetRepresentationToSurface();
        m_slider2Widget->EnabledOn();
        break;
    case 3:
        m_renderer -> AddActor(m_RegDetect_segmentationModel);
        m_RegDetect_segmentationModel -> GetProperty() -> SetRepresentationToSurface();
        m_slider3Widget->EnabledOn();
        break;
    default:
        break;
    }
    m_renderer -> ResetCamera();
}

void AneurysmUnit::ShowLineMode(int option)
{
    switch (option) {
    case 1:
        m_renderer -> AddActor(m_3DReg_segmentationModel);
        m_3DReg_segmentationModel -> GetProperty()->SetRepresentationToWireframe();
        m_slider1Widget->EnabledOff();
        break;
    case 2:
        m_renderer -> AddActor(m_LevelSet_segmentationModel);
        m_LevelSet_segmentationModel -> GetProperty() -> SetRepresentationToWireframe();
        m_slider2Widget->EnabledOff();
        break;
    case 3:
        m_renderer -> AddActor(m_RegDetect_segmentationModel);
        m_RegDetect_segmentationModel -> GetProperty() -> SetRepresentationToWireframe();
        m_slider3Widget->EnabledOff();
        break;
    default:
        break;
    }
    m_renderer -> ResetCamera();
}

void AneurysmUnit::ShowPointMode(int option)
{
    switch (option) {
    case 1:
        m_renderer -> AddActor(m_3DReg_segmentationModel);
        m_3DReg_segmentationModel -> GetProperty()->SetRepresentationToPoints();
        m_slider1Widget->EnabledOff();
        break;
    case 2:
        m_renderer -> AddActor(m_LevelSet_segmentationModel);
        m_LevelSet_segmentationModel -> GetProperty() -> SetRepresentationToPoints();
        m_slider2Widget->EnabledOff();
        break;
    case 3:
        m_renderer -> AddActor(m_RegDetect_segmentationModel);
        m_RegDetect_segmentationModel -> GetProperty() -> SetRepresentationToPoints();
        m_slider3Widget->EnabledOff();
        break;
    default:
        break;
    }
    m_renderer -> ResetCamera();
}

void AneurysmUnit::HideSegmentationModel(int option)
{
    switch (option) {
    case 1:
        m_renderer -> RemoveActor(m_3DReg_segmentationModel);
        m_slider1Widget->EnabledOff();
        break;
    case 2:
        m_renderer -> RemoveActor(m_LevelSet_segmentationModel);
        m_slider2Widget->EnabledOff();
        break;
    case 3:
        m_renderer -> RemoveActor(m_RegDetect_segmentationModel);
        m_slider3Widget->EnabledOff();
    default:
        break;
    }
}

void AneurysmUnit::InitSliders()
{
    vsp(m_slider1Rep);
    m_slider1Rep->SetMinimumValue(0.0);
    m_slider1Rep->SetMaximumValue(1.0);
    m_slider1Rep->SetValue(1.0);
//    m_slider1Rep->SetTitleText("Opacity 1");
    m_slider1Rep->GetSliderProperty()->SetColor(1, 0, 0); //red
    m_slider1Rep->GetTitleProperty()->SetColor(1, 0, 0);
    m_slider1Rep->GetLabelProperty()->SetColor(1, 1, 0);
    m_slider1Rep->GetSelectedProperty()->SetColor(0, 1, 0); //green
    m_slider1Rep->GetTubeProperty()->SetColor(1, 1, 0); //yellow
    m_slider1Rep->GetCapProperty()->SetColor(1,1,0);
    m_slider1Rep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    m_slider1Rep->GetPoint1Coordinate()->SetValue(1030, 40);
    m_slider1Rep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    m_slider1Rep->GetPoint2Coordinate()->SetValue(1130, 40);
    vsp(m_slider1Widget);
    m_slider1Widget->SetRepresentation(m_slider1Rep);
    m_slider1Widget->EnabledOff();
    vsp(m_slider1CallBack);
    vsp(m_slider2Rep);
    m_slider2Rep->SetMinimumValue(0.0);
    m_slider2Rep->SetMaximumValue(1.0);
    m_slider2Rep->SetValue(1.0);
//    m_slider2Rep->SetTitleText("Opacity 2");
    m_slider2Rep->GetSliderProperty()->SetColor(0, 1, 0); //green
    m_slider2Rep->GetTitleProperty()->SetColor(0, 1, 0);
    m_slider2Rep->GetLabelProperty()->SetColor(1, 0, 0);
    m_slider2Rep->GetSelectedProperty()->SetColor(1, 1, 0); //yellow
    m_slider2Rep->GetTubeProperty()->SetColor(1, 0, 0); //red
    m_slider2Rep->GetCapProperty()->SetColor(1,0,0);
    m_slider2Rep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    m_slider2Rep->GetPoint1Coordinate()->SetValue(1030, 140);
    m_slider2Rep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    m_slider2Rep->GetPoint2Coordinate()->SetValue(1130, 140);
    vsp(m_slider2Widget);
    m_slider2Widget->SetRepresentation(m_slider2Rep);
    m_slider2Widget->EnabledOff();
    vsp(m_slider2CallBack);
    vsp(m_slider3Rep);
    m_slider3Rep->SetMinimumValue(0.0);
    m_slider3Rep->SetMaximumValue(1.0);
    m_slider3Rep->SetValue(1.0);
//    m_slider3Rep->SetTitleText("Opacity 3");
    m_slider3Rep->GetSliderProperty()->SetColor(1, 1, 0); //yellow
    m_slider3Rep->GetTitleProperty()->SetColor(1, 1, 0);
    m_slider3Rep->GetLabelProperty()->SetColor(0, 1, 0);
    m_slider3Rep->GetSelectedProperty()->SetColor(1, 0, 0); //red
    m_slider3Rep->GetTubeProperty()->SetColor(0, 1, 0); //green
    m_slider3Rep->GetCapProperty()->SetColor(0, 1, 0);
    m_slider3Rep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    m_slider3Rep->GetPoint1Coordinate()->SetValue(1030, 240);
    m_slider3Rep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    m_slider3Rep->GetPoint2Coordinate()->SetValue(1130, 240);
    vsp(m_slider3Widget);
    m_slider3Widget->SetRepresentation(m_slider3Rep);
    m_slider3Widget->EnabledOff();
    vsp(m_slider3CallBack);
    BindSlider(m_3DReg_segmentationModel, m_slider1CallBack, m_slider1Widget);
    BindSlider(m_LevelSet_segmentationModel, m_slider2CallBack, m_slider2Widget);
    BindSlider(m_RegDetect_segmentationModel, m_slider3CallBack, m_slider3Widget);
}

bool AneurysmUnit::BindSlider(vtkSmartPointer<vtkActor> actor,
                              vtkSmartPointer<vtkSliderCallBack> sliderCallBack,
                              vtkSmartPointer<vtkSliderWidget> sliderWidget)
{
    if(actor.GetPointer() == NULL || sliderCallBack.GetPointer() == NULL
            || sliderWidget.GetPointer() == NULL) {
        return false;
    }
    if(sliderCallBack->SetBoundActor(vtkActor::SafeDownCast(actor))) {
        sliderWidget->AddObserver(vtkCommand::InteractionEvent, sliderCallBack);
        sliderWidget->SetInteractor(m_renderWindow->GetInteractor());
        sliderWidget->SetAnimationModeToAnimate();
        sliderWidget->EnabledOff();

    }
}


void AneurysmUnit::InitAnnotation()
{
    vsp(m_tranAnnotation);
    m_tranAnnotation->SetLinearFontScaleFactor(2);
    m_tranAnnotation->SetNonlinearFontScaleFactor(1);
    m_tranAnnotation->SetMaximumFontSize(18);
    m_tranAnnotation->SetText(3, "Transverse");
    m_tranAnnotation->GetTextProperty()->SetColor(0, 0, 1);
    vsp(m_corAnnotation);
    m_corAnnotation->SetLinearFontScaleFactor(2);
    m_corAnnotation->SetNonlinearFontScaleFactor(1);
    m_corAnnotation->SetMaximumFontSize(18);
    m_corAnnotation->SetText(3, "Coronal");
    m_corAnnotation->GetTextProperty()->SetColor(0, 1, 0);
    vsp(m_sagAnnotation);
    m_sagAnnotation->SetLinearFontScaleFactor(2);
    m_sagAnnotation->SetNonlinearFontScaleFactor(1);
    m_sagAnnotation->SetMaximumFontSize(18);
    m_sagAnnotation->SetText(3, "Sagittal");
    m_sagAnnotation->GetTextProperty()->SetColor(1, 0, 0);
    RegisterDisplay(1);
}
