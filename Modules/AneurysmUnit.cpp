#include "AneurysmUnit.h"
vtkStandardNewMacro(Util::CusInteractorPickPointStyle);

AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{

    vsp(m_renInteractor);
    m_renInteractor->SetRenderWindow(m_renderWindow);
    vsp(m_renderer);
    m_renderer -> SetBackground(.1, .2, .3);
    vsp(m_light);
    vsp(m_ul_renderer);
    vsp(m_ur_renderer);
    vsp(m_bl_renderer);
    vsp(m_br_renderer);
    m_ul_renderer -> SetBackground(0.1, 0.1, 0.2);
    m_ur_renderer -> SetBackground(0.1, 0.2, 0.1);
    m_bl_renderer -> SetBackground(0.2, 0.1, 0.2);
    m_br_renderer -> SetBackground(.2, .2, .2);
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

    vsp(m_cuttingPlane);
    vsp(m_cuttingPlaneWidget);
    vsp(m_cuttingPlane2);
    vsp(m_cuttingVtkPlane);
    vsp(m_implicitPlaneRepresentation);

    m_centerLine = new CenLineUnit;
    m_currentRoamingRouteId = 1;
    m_currentRoamingStep = 15;
    Instantiate(tmp_camera, vtkCamera);
    tmp_camera->SetPosition(0, 0, 20);
    tmp_camera->SetFocalPoint(0, 0, 0);
    m_light->SetColor(1.0, 1.0, 1.0);
    m_light->SetPosition(tmp_camera->GetPosition());
    m_light->SetFocalPoint(tmp_camera->GetFocalPoint());
    m_light->SetIntensity(.5);
    m_renderer->SetActiveCamera(tmp_camera);

    vsp(m_leftLineModel);
    vsp(m_rightLineModel);
    vsp(m_rawData);

    vsp(m_tranActor);
    vsp(m_corActor);
    vsp(m_sagActor);
    InitAnnotation();
    //3 plain views setting part
    vsp(m_tranViewerRenderer);
    m_tranViewerRenderer -> GradientBackgroundOn();
    m_tranViewerRenderer -> SetBackground(0.3, 0.5, 0.5);
    m_tranViewerRenderer -> AddViewProp(m_tranAnnotation);
    vsp(m_corViewerRenderer);
    m_corViewerRenderer -> GradientBackgroundOn();
    m_corViewerRenderer -> SetBackground(0.4, 0.4, 0.6);
    m_corViewerRenderer -> AddViewProp(m_corAnnotation);
    vsp(m_sagViewerRenderer);
    m_sagViewerRenderer -> GradientBackgroundOn();
    m_sagViewerRenderer -> SetBackground(0.7, 0.6, 0.7);
    m_sagViewerRenderer -> AddViewProp(m_sagAnnotation);
    vsp(m_lineInfoPointPicker);
    Instantiate(t_pointPickerStyle, Util::CusInteractorPickPointStyle);
    m_renInteractor->SetInteractorStyle(t_pointPickerStyle);
    t_pointPickerStyle->PreparedRenderer(m_renderer);

//    vsp(m_pointPickerInteractorStyle);
//    m_renInteractor->SetInteractorStyle(m_pointPickerInteractorStyle);
//    m_pointPickerInteractorStyle->PreparedRenderer(m_renderer);

//    vsp(m_sliceViewStyle);

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

std::string AneurysmUnit::GetCurInteractorStyle()
{
    return m_renderWindow->GetInteractor()->GetInteractorStyle()->GetClassName();
}


void AneurysmUnit::ReadInputSegmentationModel(std::string fileName, int option)
{
    int size = fileName.size();
    if(size < 3) return ;
    m_filename.first = fileName;
    m_filename.second = (m_filename.first).substr(0, size - 3) + "mhd";
    std::cout << "the Segment Binary mhd filename is " << m_filename.second << std::endl;

    Instantiate(plane, vtkPlane);
    Instantiate(clipper, vtkClipPolyData);
    clipper->SetClipFunction(plane);
    switch(option) {
    case 1:
        m_3DReg_segmentationReader -> SetFileName(fileName.c_str());
        m_3DReg_segmentationReader -> Update();
        {
        vtkSmartPointer<vtkPolyDataMapper> mapper1
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper1 -> SetInputConnection(m_3DReg_segmentationReader -> GetOutputPort());
        m_3DReg_segmentationModel -> SetMapper(mapper1);
        clipper->SetInputConnection(m_3DReg_segmentationReader->GetOutputPort());
        }

        break;
    case 2:
        m_LevelSet_segmentationReader -> SetFileName(fileName.c_str());
        m_LevelSet_segmentationReader -> Update();
        {vtkSmartPointer<vtkPolyDataMapper> mapper2
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper2 -> SetInputConnection(m_LevelSet_segmentationReader -> GetOutputPort());
        m_LevelSet_segmentationModel -> SetMapper(mapper2);
        clipper->SetInputConnection(m_LevelSet_segmentationReader->GetOutputPort());
        }
        break;
    case 3:
        m_RegDetect_segmentationReader -> SetFileName(fileName.c_str());
        m_RegDetect_segmentationReader -> Update();
        {vtkSmartPointer<vtkPolyDataMapper> mapper3
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper3 -> SetInputConnection(m_RegDetect_segmentationReader -> GetOutputPort());
        m_RegDetect_segmentationModel -> SetMapper(mapper3);
        clipper->SetInputConnection(m_RegDetect_segmentationReader->GetOutputPort());
        }
        break;
    default:
        break;

    }
    Instantiate(clipmapper, vtkPolyDataMapper);
    clipmapper->SetInputConnection(clipper->GetOutputPort());
    m_cuttingPlane->SetMapper(clipmapper);
    Instantiate(backFaces, vtkProperty);
    backFaces->SetDiffuseColor(.8, .8, .4);
    m_cuttingPlane->SetBackfaceProperty(backFaces);
    m_implicitPlaneRepresentation->SetPlaceFactor(1.0);
    m_implicitPlaneRepresentation->PlaceWidget(m_cuttingPlane->GetBounds());
    m_implicitPlaneRepresentation->SetNormal(plane->GetNormal());
    m_implicitPlaneRepresentation->SetOrigin(30, -100, 1100);
    m_implicitPlaneRepresentation->SetEdgeColor(.0, .7, .7);

    Instantiate(callback, Util::vtkIPWCallback);
    callback->Plane = plane;
    callback->Actor = m_cuttingPlane;
    m_cuttingPlaneWidget->SetInteractor(m_renderWindow->GetInteractor());
    m_cuttingPlaneWidget->EnabledOff();
    m_cuttingPlaneWidget->SetRepresentation(m_implicitPlaneRepresentation);
    m_cuttingPlaneWidget->AddObserver(vtkCommand::InteractionEvent, callback);
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
    std::string stmp = m_renInteractor->GetInteractorStyle()->GetClassName();


//    std::cout << "already have picker enabled ? " << m << std::endl;
    std::cout << stmp << std::endl;
    if(stmp != "CusInteractorPickPointStyle") {
        std::cout << m_renInteractor->GetInteractorStyle()->GetClassName() << std::endl;
        Instantiate(t_pickPointStyle, Util::CusInteractorPickPointStyle);
        m_renInteractor->SetInteractorStyle(t_pickPointStyle);
        t_pickPointStyle->PreparedRenderer(m_renderer);
    }
    std::cout << m_renInteractor->GetInteractorStyle()->GetClassName() << std::endl;
    Util::CusInteractorPickPointStyle* cur_pointPickerStyle
            = (Util::CusInteractorPickPointStyle*)m_renInteractor->GetInteractorStyle();

    cur_pointPickerStyle->PreparedRenderer(m_renderer);
    int m1 = m_renInteractor->GetInteractorStyle()->IsTypeOf("CusInteractorPickPointStyle");
//    if(!(cur_pointPickerStyle->GetEnabled())) {
//        return ;
//    }
    cur_pointPickerStyle->GetMarkedPoints(s, e);
    std::cout << "start point : " << s[0] << ", " << s[1] << ", " << s[2] << std::endl;
    std::cout << "end point : " << e[0] << ", " << e[1] << ", " << e[2] << std::endl;

    int tmp = option % 3;  // chose centerline method
    if(tmp == 0) {
        m_centerLine->Path_GradientDescent(GetRawFilename(), s, e);
    }
}

void AneurysmUnit::DrawCenterLine(int option, bool isLeft)
{
//    GetCenterLine(option);
    if(isLeft){
        ShowCenterPoints(m_leftLineModel, m_centerLine->CenterPoints1);
    }else{
        ShowCenterPoints(m_rightLineModel, m_centerLine->CenterPoints2);
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

void AneurysmUnit::SetRoamingRoute(int id)
{
    if((id == 1 || id == 2) && (m_currentRoamingRouteId != id)) {
        m_currentRoamingRouteId = id;
    }
}

void AneurysmUnit::SetRoamingStep(int step)
{
    m_currentRoamingStep = step;
}

void AneurysmUnit::OnRoam()
{
    m_currentRoamingIndex = 0;
    UpdateRoamingCamera();
}

void AneurysmUnit::RoamNext()
{
    m_currentRoamingIndex += m_currentRoamingStep;
    int limit = m_centerLine->GetCenterLinePointNums(m_currentRoamingRouteId);
    if(m_currentRoamingIndex >= limit) {
        m_currentRoamingIndex = std::max(0, limit - 1);
    }
    UpdateRoamingCamera();
}

void AneurysmUnit::RoamPrevious()
{
    m_currentRoamingIndex -= m_currentRoamingStep;
    if(m_currentRoamingIndex < 0) {
        m_currentRoamingIndex = 0;
    }
    UpdateRoamingCamera();
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
//    int extent[6];
//    m_rawData->GetExtent(extent);
//    if(extent[1] != 0 && extent[3] != 0 && extent[5] != 0)
//        return true;
//    else
//        return false;
    return m_rawData->GetActualMemorySize() != 0 ? true : false;
}

void AneurysmUnit::DrawSliceFactory(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkImageActor> imgActor, double transformMat[], double pos[])
{
    if(!RawDataExist())
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
//    vtkSmartPointer<vtkInteractorStyleImage> style =
//            vtkSmartPointer<vtkInteractorStyleImage>::New();
//    m_renInteractor->SetInteractorStyle(style);


//    viewer->GetRenderer()->AddActor(imgActor);

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


void AneurysmUnit::SetVisibilitySTLCuttingPlane(bool show)
{
    show ? m_renderer->AddActor(m_cuttingPlane2) : m_renderer->RemoveActor(m_cuttingPlane2);
}

void AneurysmUnit::SetVisibilityVirtualCuttingWidget(bool show)
{
    show ? m_cuttingPlaneWidget->SetEnabled(1) : m_cuttingPlaneWidget->SetEnabled(0);
}

void AneurysmUnit::DoStlCutting(vtkSmartPointer<vtkSTLReader> stlreader)
{
    m_cuttingVtkPlane->SetOrigin(m_implicitPlaneRepresentation->GetOrigin());
    m_cuttingVtkPlane->SetNormal(m_implicitPlaneRepresentation->GetNormal());
    Instantiate(cutter, vtkCutter);
    cutter->SetCutFunction(m_cuttingVtkPlane);
    cutter->SetInputConnection(stlreader->GetOutputPort());
    cutter->Update();

    Instantiate(cutterMapper, vtkPolyDataMapper);
    cutterMapper->SetInputConnection(cutter->GetOutputPort());

    m_cuttingPlane2->GetProperty()->SetColor(1.0, 1.0, 0);
    m_cuttingPlane2->GetProperty()->SetLineWidth(2);
    m_cuttingPlane2->SetMapper(cutterMapper);
}

void AneurysmUnit::DoSTLCut()
{

    DoStlCutting(m_3DReg_segmentationReader);
//    DoStlCutting(m_LevelSet_segmentationReader);
    //    DoStlCutting(m_RegDetect_segmentationReader);
}
void AneurysmUnit::RemoveAllRenderers()
{
    // clean previous renderers and then add the current renderer

    m_renderWindow->RemoveRenderer(m_renderer);
    m_renderWindow->RemoveRenderer(m_ul_renderer);
    m_renderWindow->RemoveRenderer(m_ur_renderer);
    m_renderWindow->RemoveRenderer(m_bl_renderer);
    m_renderWindow->RemoveRenderer(m_br_renderer);
    m_renderWindow->RemoveRenderer(m_corViewerRenderer);
    m_renderWindow->RemoveRenderer(m_tranViewerRenderer);
    m_renderWindow->RemoveRenderer(m_sagViewerRenderer);

//    auto collection = m_renderWindow->GetRenderers();
//    std::cout << collection->GetNumberOfItems() << std::endl;
//    auto item = collection->GetNextItem();
//    while (item != NULL)
//    {
//        m_renderWindow->RemoveRenderer(item);
//        item = collection->GetNextItem();
//    }
//    std::cout << collection->GetNumberOfItems() << std::endl;

}
void AneurysmUnit::RegisterDisplay(int mod)
{
    RemoveAllRenderers();
    switch(mod) {
    case SINGLE_MOD: {
        m_renderer -> SetViewport(0, 0, 1, 1);
        m_renderWindow -> AddRenderer(m_renderer);
        int m = m_renInteractor->GetInteractorStyle()->IsTypeOf("Util::CusInteractorPickPointStyle");
        if(0 == m) {
            m_renInteractor->RemoveObserver((vtkCommand*)m_renInteractor->GetInteractorStyle());
            Instantiate(t_pointPickerStyle, Util::CusInteractorPickPointStyle);
            m_renInteractor->SetInteractorStyle(t_pointPickerStyle);
            t_pointPickerStyle->PreparedRenderer(m_renderer);
            t_pointPickerStyle->SetInteractor(m_renInteractor);
        }
        break;
    }

    case COMP_MOD: {
        break;
    }
    case COMP4_MOD: {
        m_ul_renderer -> SetViewport(0, 0, 0.5, 0.5);
        m_ur_renderer -> SetViewport(.5, 0, 1, 0.5);;
        m_bl_renderer -> SetViewport(0, 0.5, 0.5, 1);
        m_br_renderer -> SetViewport(0.5, 0.5, 1, 1);
        m_renderWindow -> AddRenderer(m_ul_renderer);
        m_renderWindow -> AddRenderer(m_ur_renderer);
        m_renderWindow -> AddRenderer(m_bl_renderer);
        m_renderWindow -> AddRenderer(m_br_renderer);
        break;
    }

    case PLANE_MOD: {
        m_renderer -> SetViewport(0, 0, 0.75, 1);
//            m_renderer -> SetBackground(.1, .2, .3);
        //3 plain views setting part
        m_tranViewerRenderer -> SetViewport(0.75, 0, 1, 0.33);
        m_corViewerRenderer -> SetViewport(0.75, 0.33, 1, 0.67);
        m_sagViewerRenderer -> SetViewport(0.75, 0.67, 1, 1);
        m_renderWindow -> AddRenderer(m_renderer);
        m_renderWindow -> AddRenderer(m_tranViewerRenderer);
        m_renderWindow -> AddRenderer(m_corViewerRenderer);
        m_renderWindow -> AddRenderer(m_sagViewerRenderer);

        int m = m_renInteractor->GetInteractorStyle()->IsTypeOf("vtkInteractorStyleImage");
        if(0 == m) {
            m_renInteractor->RemoveObserver((vtkCommand*)m_renInteractor->GetInteractorStyle());
            Instantiate(t_SliceViewStyle, vtkInteractorStyleImage);
            m_renInteractor->SetInteractorStyle(t_SliceViewStyle);
            t_SliceViewStyle->SetInteractor(m_renInteractor);
        }
        break;
    }

    default: {
        break;
    }

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
                              vtkSmartPointer<Util::vtkSliderCallBack> sliderCallBack,
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
    Util::CusInteractorPickPointStyle* cur_pointPickerStyle
            = (Util::CusInteractorPickPointStyle*)m_renInteractor->GetInteractorStyle();
    cur_pointPickerStyle->SetPickerEnabled(enabled);
//    m_pointPickerInteractorStyle->SetPickerEnabled(enabled);
}

std::string AneurysmUnit::GetRawFilename() {      return m_filename.second;     }

void AneurysmUnit::GetCurrentRoamingPoint(double p[3])
{
    m_centerLine->GetCenterLinePoint(m_currentRoamingRouteId, m_currentRoamingIndex, p);
}

void AneurysmUnit::UpdateRoamingCamera()
{
    double p[3];
    GetCurrentRoamingPoint(p);
    double fp[3];
    m_centerLine->GetCenterLinePoint(m_currentRoamingRouteId,
                                     m_currentRoamingIndex-m_currentRoamingStep / 2, fp);
    if(m_currentRoamingIndex - m_currentRoamingStep / 2 < 0) {
        fp[0] = p[0], fp[1] = p[1], fp[2] = p[2];
    }
    double h[3];
    m_centerLine->GetCenterLinePoint(m_currentRoamingRouteId,
                                     m_currentRoamingIndex + m_currentRoamingStep / 2, h);
    int limit = m_centerLine->GetCenterLinePointNums(m_currentRoamingRouteId);
    if(m_currentRoamingIndex + m_currentRoamingStep / 2 >= limit) {
        h[0] = p[0], h[1] = p[1], h[2] = p[2];
    }
    fp[0] = p[0] + h[0] - fp[0];
    fp[1] = p[1] + h[1] - fp[1];
    fp[2] = p[2] + h[2] - fp[2];
    m_renderer->GetActiveCamera()->SetPosition(p);
    m_renderer->GetActiveCamera()->SetFocalPoint(fp);

    m_light->SetPosition(m_renderer->GetActiveCamera()->GetPosition());
    m_light->SetFocalPoint(m_renderer->GetActiveCamera()->GetFocalPoint());

    m_renderer->Render();
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
