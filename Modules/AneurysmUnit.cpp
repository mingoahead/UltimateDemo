#include "AneurysmUnit.h"
vtkStandardNewMacro(CusInteractorStylePickPoint);

AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{
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
    m_centerLine = new CenLineUnit;
    vsp(m_leftLineModel);
    vsp(m_rightLineModel);
    vsp(m_rawData);
    vsp(m_tranViewer);
    vsp(m_corViewer);
    vsp(m_sagViewer);
    vsp(m_tranActor);
    vsp(m_corActor);
    vsp(m_sagActor);
    InitAnnotation();
    vsp(m_lineInfoPointPicker);
    vsp(m_pointPickerInteractor);
    m_renderWindow->GetInteractor()->SetInteractorStyle(m_pointPickerInteractor);
    m_pointPickerInteractor->PreparedRenderer(m_renderer);

    InitSliders();
    InitCamerasWidgets();
    m_renderer->ResetCamera();
}

AneurysmUnit::~AneurysmUnit()
{
    delete m_centerLine;
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
    int size = fileName.size();\
    if(size < 3) return ;
    m_filename.first = fileName;
    m_filename.second = (m_filename.first).substr(0, size - 3) + "mhd";
    std::cout << "the mhd filename is " << m_filename.second << std::endl;

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

void AneurysmUnit::ShowCenterPoints(vtkSmartPointer<vtkActor> LineModel,
                                    std::vector<Point3f> &CenterPoints)
{
    int num = CenterPoints.size();
    Instantiate(points, vtkPoints);
    Instantiate(vectices, vtkCellArray);
    Instantiate(lines, vtkCellArray);
    Instantiate(line, vtkLine);
    Instantiate(colors, vtkUnsignedCharArray);
    colors->SetNumberOfComponents(3);
    unsigned char pupple[3] = {255, 0, 255};
    std::vector<Point3f>::const_iterator it = CenterPoints.begin();
    for(unsigned int i = 0; i < num && it != CenterPoints.end(); i++, it++) {
        vtkIdType pid[1];
        pid[0] = points->InsertNextPoint((*it).x, (*it).y, (*it).z);
        colors->InsertNextTupleValue(pupple);
        vectices->InsertNextCell(1, pid);
        if(i < num - 1) {
            line->GetPointIds()->SetId(0, i);
            line->GetPointIds()->SetId(1, i+1);
            lines->InsertNextCell(line);
        }
    }
    Instantiate(polydata, vtkPolyData);
    polydata->SetPoints(points);
    polydata->SetVerts(vectices);
    polydata->SetLines(lines);
    polydata->GetPointData()->SetScalars(colors);
    Instantiate(cenmapper, vtkPolyDataMapper);
    cenmapper->SetInputData(polydata);
    LineModel->SetMapper(cenmapper);
    m_renderer->AddActor(LineModel);
    m_renderWindow->Render();
    
}

void AneurysmUnit::GetCenterLine(int option)
{
    double s[3], e[3];
    m_pointPickerInteractor->GetMarkedPoints(s, e);
    int tmp = option % 3;
    if(tmp == 0) {
        m_centerLine->Path_GradientDescent(GetRawFilename(), s, e);
    }
}

void AneurysmUnit::DrawCenterLine(int option, bool isLeft)
{
//    GetCenterLine(option);
    if(isLeft){
        ShowCenterPoints(m_leftLineModel, m_centerLine->CenterPoints);
    }else{
        ShowCenterPoints(m_rightLineModel, m_centerLine->CenterPoints);
    }
}

void AneurysmUnit::HideCenterLine(int option, bool isLeft)
{
    if(isLeft) {
        m_renderer->RemoveActor(m_leftLineModel);
    }else {
        m_renderer->RemoveActor(m_rightLineModel);
    }
    m_renderer->Render();
}

bool AneurysmUnit::LoadRawData(std::string fileName)
{
    if(fileName.empty()) return false;
    vtkSmartPointer<vtkMetaImageReader> reader =
                vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();
    std::cout << m_rawData->GetActualMemorySize() << std::endl;
    m_rawData = reader->GetOutput();
    std::cout << m_rawData->GetActualMemorySize() << std::endl;
    int extent[6];
    m_rawData->GetExtent(extent);
    std::cout << extent[0] << ", " << extent[1] << ", "
                     << extent[2] << ", " << extent[3] << ", "
                     << extent[4] << ", " << extent[5] << std::endl;
    if(RawDataExist())
        return true;
    else
        return false;
}

bool AneurysmUnit::RawDataExist()
{
    if(m_rawData == NULL) return false;
    int extent[6];
    m_rawData->GetExtent(extent);
    if(extent[1] != 0 && extent[3] != 0 && extent[5] != 0)
        return true;
    return false;
}

void AneurysmUnit::DrawSliceFactory(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkImageActor> imgActor, double transformMat[], double pos[])
{

    if(m_rawData == NULL || !RawDataExist())
        return ;
    vtkSmartPointer<vtkMatrix4x4> resliceAxes =
            vtkSmartPointer<vtkMatrix4x4>::New();
    resliceAxes->DeepCopy(transformMat);
    resliceAxes->SetElement(0, 3, pos[0]);
    resliceAxes->SetElement(1, 3, pos[1]);
    resliceAxes->SetElement(2, 3, pos[2]);
    vtkSmartPointer<vtkImageShiftScale> shifter =
            vtkSmartPointer<vtkImageShiftScale>::New();
    shifter->SetInputData(m_rawData);
    shifter->SetOutputScalarTypeToUnsignedChar();
    shifter->Update();
    vtkSmartPointer<vtkImageReslice> reslice =
            vtkSmartPointer<vtkImageReslice>::New();
    reslice->SetInputConnection(shifter->GetOutputPort());
    reslice->SetOutputDimensionality(2);
    reslice->SetResliceAxes(resliceAxes);
    reslice->SetInterpolationModeToLinear();
    vtkSmartPointer<vtkLookupTable> colorTable =
            vtkSmartPointer<vtkLookupTable>::New();
    colorTable->SetRange(0, 255);
    colorTable->SetValueRange(0.0, 1.0);
    colorTable->SetSaturationRange(0.0, 0.0);
    colorTable->SetRampToLinear();
    colorTable->Build();
    vtkSmartPointer<vtkImageMapToColors> colorMap =
            vtkSmartPointer<vtkImageMapToColors>::New();
    colorMap->SetLookupTable(colorTable);
    colorMap->SetInputConnection(reslice->GetOutputPort());
    imgActor->SetInputData((vtkImageData*)colorMap->GetOutput());
    colorMap->Update();
    renderer->AddActor(imgActor);

//    viewer->GetRenderer()->AddActor(imgActor);
//    vtkSmartPointer<vtkInteractorStyleImage> style =
//            vtkSmartPointer<vtkInteractorStyleImage>::New();
//    viewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
//    viewer->Render();
//    viewer->GetRenderWindow()->GetInteractor()->Start();
}

void AneurysmUnit::Draw3DSlice(double pos[])
{
    DrawSliceFactory(m_tranViewerRenderer, m_tranActor, tranElements, pos);
    DrawSliceFactory(m_corViewerRenderer, m_corActor, coronalElements, pos);
    DrawSliceFactory(m_sagViewerRenderer, m_sagActor, sagittalElements, pos);
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

void AneurysmUnit::SetPointPickerEnabled(bool enabled)
{
    m_pointPickerInteractor->SetPickerEnabled(enabled);
}

std::string AneurysmUnit::GetRawFilename() {      return m_filename.second;     }

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

void AneurysmUnit::InitCamerasWidgets()
{
    vsp(m_cameraRep);
    m_cameraRep->SetNumberOfFrames(20);
    vsp(m_cameraWidget);
    m_cameraWidget->SetInteractor(m_renderWindow->GetInteractor());
    m_cameraWidget->SetRepresentation(m_cameraRep);
    m_cameraWidget->On();
}
