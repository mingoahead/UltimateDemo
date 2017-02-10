#include "AneurysmUnit.h"
vtkStandardNewMacro(Util::CusInteractorPickPointStyle);
vtkStandardNewMacro(Util::SegmentActor);

AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{

    vsp(m_renInteractor);
    m_renInteractor->SetRenderWindow(m_renderWindow);
//    m_renderWindow->SetInteractor(m_renInteractor);
    vsp(m_roamingStyle);
    vsp(m_xyzSliceStyle);
    vsp(m_pointpickerStyle);
    vsp(m_resliceStyle);
    vsp(m_renderer);
    m_renderer -> SetBackground(.098, .098, .2);
    vsp(m_light);
    InitSingleMode();
    InitMeasureMode();
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
    m_rawinfo.extent = NULL;
    m_rawinfo.spacing = NULL;
    m_rawinfo.origin = NULL;
    vsp(m_interactionHandler);
    InitCamerasWidgets();
    m_renderer->ResetCamera();
}

AneurysmUnit::~AneurysmUnit()
{
    delete m_centerLine;
}
void AneurysmUnit::InitSegModelFactory(Util::SegmentActor *actor, std::string name, double color[])
{
    actor->GetProperty()->SetColor(color);
    actor->SetSegmentType(name);
}

void AneurysmUnit::InitSegModels()
{
    vsp(m_3DReg_segmentationModel);
    vsp(m_LevelSet_segmentationModel);
    vsp(m_RegDetect_segmentationModel);
    double color[3][3] = {{.4, .4, .5}, {.3, .2, .7}, {1.0, .49, .25}};
    InitSegModelFactory(m_3DReg_segmentationModel, "3D Reg", color[0]);
    InitSegModelFactory(m_LevelSet_segmentationModel, "Level Set", color[1]);
    InitSegModelFactory(m_RegDetect_segmentationModel, "Shape Detect", color[2]);
}

bool AneurysmUnit::GetOnlySegModelSource(Util::SegmentActor *actor)
{
    Instantiate(collection, vtkActorCollection);
    if(GetNumOfCurrentSegmentModels(collection) != 1) {
        actor = nullptr;
        return false;
    }
    collection->InitTraversal();
    actor = Util::SegmentActor::SafeDownCast(collection->GetNextActor());
    std::cout << "Current Segmentation Model : " << actor->GetSegmentType()
              << "  Model path : " << actor->GetSegmentPath() << std::endl;
//    vtkAlgorithm * algo =  actor->GetMapper()->GetInputConnection(0, 0)->GetProducer();
//    reader = vtkSTLReader::SafeDownCast(algo);
//    std::cout << "Input connections : " << reader->GetTotalNumberOfInputConnections() <<std::endl;
    return true;
}
void AneurysmUnit::InitSingleMode()
{
    vsp(m_3DReg_segmentationReader);
    vsp(m_LevelSet_segmentationReader);
    vsp(m_RegDetect_segmentationReader);
    InitSegModels();
    InitCuttingDisplay();
    vsp(m_tranActor);
    vsp(m_corActor);
    vsp(m_sagActor);
    InitAnnotation();
    InitInfoSphere();
    //3 plain views setting part
    vsp(m_tranViewerRenderer);
    m_tranViewerRenderer -> GradientBackgroundOn();
    m_tranViewerRenderer -> SetBackground(0.3, 0.5, 0.5);
    m_tranViewerRenderer -> AddViewProp(m_tranAnnotation);
//    m_tranViewerRenderer -> AddActor(m_transphere);
    vsp(m_corViewerRenderer);
    m_corViewerRenderer -> GradientBackgroundOn();
    m_corViewerRenderer -> SetBackground(0.4, 0.4, 0.6);
    m_corViewerRenderer -> AddViewProp(m_corAnnotation);
//    m_corViewerRenderer -> AddActor(m_corsphere);
    vsp(m_sagViewerRenderer);
    m_sagViewerRenderer -> GradientBackgroundOn();
    m_sagViewerRenderer -> SetBackground(0.7, 0.6, 0.7);
    m_sagViewerRenderer -> AddViewProp(m_sagAnnotation);
    InitSliders();
    vsp(m_lineInfoPointPicker);
    m_renInteractor->SetPicker(m_lineInfoPointPicker);
}

void AneurysmUnit::InitMeasureMode()
{
    vsp(m_resliceViewer);
    m_resliceViewerRenderer = m_resliceViewer->GetRenderer();
    m_resliceViewerRenderer -> SetBackground(0.098, 0.098, 0.2);
    vsp(m_binresliceViewer);
    m_binresliceViewerRenderer = m_binresliceViewer->GetRenderer();
    m_binresliceViewerRenderer -> SetBackground(0.15, 0.15, 0.7);
    vsp(m_measureResView);
    m_measureResRenderer = m_measureResView->GetRenderer();
    vsp(m_resliceTracerWidget);
    m_resliceTracerWidget->GetLineProperty()->SetLineWidth(3);
    m_resliceTracerWidget->SetInteractor(m_renInteractor);
    vsp(m_tracCallback);
    vsp(m_resliceImagePlaneWidget);
    vsp(m_distanceWidget);
    vsp(m_diameterMeasuringPointPicker);
    m_distanceWidget->SetInteractor(m_renderWindow->GetInteractor());
    m_distanceWidget->CreateDefaultRepresentation();
    static_cast<vtkDistanceRepresentation *>(m_distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");
    m_resliceImagePlaneWidget->SetPicker(m_diameterMeasuringPointPicker);
}
vtkRenderer *AneurysmUnit::GetRenderer() {return m_renderer;}

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
    switch(option) {
    case 1:
        m_3DReg_segmentationReader -> SetFileName(fileName.c_str());
        m_3DReg_segmentationReader -> Update();
        {
        vtkSmartPointer<vtkPolyDataMapper> mapper1
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper1 -> SetInputConnection(m_3DReg_segmentationReader -> GetOutputPort());
        m_3DReg_segmentationModel -> SetMapper(mapper1);
        m_3DReg_segmentationModel -> SetSegmentPath(fileName);
        }
        break;
    case 2:
        m_LevelSet_segmentationReader -> SetFileName(fileName.c_str());
        m_LevelSet_segmentationReader -> Update();
        {vtkSmartPointer<vtkPolyDataMapper> mapper2
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper2 -> SetInputConnection(m_LevelSet_segmentationReader -> GetOutputPort());
        m_LevelSet_segmentationModel -> SetMapper(mapper2);
        m_LevelSet_segmentationModel -> SetSegmentPath(fileName);
        }
        break;
    case 3:
        m_RegDetect_segmentationReader -> SetFileName(fileName.c_str());
        m_RegDetect_segmentationReader -> Update();
        {vtkSmartPointer<vtkPolyDataMapper> mapper3
                = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper3 -> SetInputConnection(m_RegDetect_segmentationReader -> GetOutputPort());
        m_RegDetect_segmentationModel -> SetMapper(mapper3);
        m_RegDetect_segmentationModel -> SetSegmentPath(fileName);
        }
        break;
    default:
        break;

    }
    //m_renderer -> ResetCamera();
}

void AneurysmUnit::ShowSegmentationModel(int option)
{
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
//    m_renderer->Render();
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
//    m_renderer->Render();
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
//    m_renderer->Render();
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

int AneurysmUnit::GetNumOfCurrentSegmentModels(vtkActorCollection *collection)
{
    collection->RemoveAllItems();
    vtkActorCollection* actorCollection = m_renderer->GetActors();
    actorCollection->InitTraversal();
    int cnt = 0;
    for(vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++) {
        vtkActor* actor = actorCollection->GetNextActor();
        if(actor->IsA("SegmentActor")) {
            collection->AddItem(actor);
            cnt++;
        }
    }
    return cnt;

}
void AneurysmUnit::SetVisibilityCollectiOn()
{
    Instantiate(collection, vtkActorCollection);
    int cnt = GetNumOfCurrentSegmentModels(collection);
    std::cout << "segment model number : " << cnt << std::endl;
    if(cnt < 2) return ;
    else if(cnt == 2) {
        collection->InitTraversal();
        for(vtkIdType i = 0; i < collection->GetNumberOfItems(); i++) {
            Instantiate(transform, vtkTransform);
            transform->PostMultiply();
            transform->Translate(expand2[i]);
            vtkActor *actor = collection->GetNextActor();
            if(actor != nullptr) {
                actor->SetUserTransform(transform);
                std::cout << "actor  " << i << " : translated!" << std::endl;
            }
        }
    }
    else if(cnt == 3) {
        collection->InitTraversal();
        for(vtkIdType i = 0; i < collection->GetNumberOfItems(); i++) {
            Instantiate(transform, vtkTransform);
            transform->PostMultiply();
            transform->Translate(expand3[i]);
            vtkActor *actor = collection->GetNextActor();
            if(actor != nullptr) {
                actor->SetUserTransform(transform);
                std::cout << "actor  " << i << " : translated!" << std::endl;
            }
        }
    }
    m_renderer->Render();
}

void AneurysmUnit::SetVisibilityCollectiOff()
{
    Instantiate(collection, vtkActorCollection);
    int cnt = GetNumOfCurrentSegmentModels(collection);
    std::cout << "segment model number : " << cnt << std::endl;
    if(cnt < 2) return ;
    collection->InitTraversal();
    for(vtkIdType i = 0; i < collection->GetNumberOfItems(); i++) {
        Instantiate(transform, vtkTransform);
        transform->PostMultiply();
        transform->Translate(0, 0, 0);
        vtkActor *actor = collection->GetNextActor();
        if(actor != nullptr) {
            actor->SetUserTransform(transform);
            std::cout << "actor  " << i << " : translated!" << std::endl;
        }
    }

    m_renderer->Render();
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
    reader->SetDataScalarTypeToShort();
    reader->SetDataByteOrderToLittleEndian();
    reader->Update();
//    std::cout << m_rawData->GetActualMemorySize() << std::endl;
    m_rawData = reader->GetOutput();
//    std::cout << m_rawData->GetActualMemorySize() << std::endl;
    int extent[6];
    m_rawData->GetExtent(extent);
    m_rawinfo.extent = extent;
    double origin[3];
    m_rawData->GetOrigin(origin);
    m_rawinfo.origin = origin;
    double spacing[3];
    m_rawData->GetSpacing(spacing);
    m_rawinfo.spacing = spacing;
    std::ostringstream infostr;
    infostr << "origin : (" << m_rawinfo.origin[0] << ", " << m_rawinfo.origin[1] << ", "
            << m_rawinfo.origin[2] << ")" << std::endl;
    infostr << "spacing : (" << m_rawinfo.origin[0] << ", " << m_rawinfo.origin[1] << ", "
            << m_rawinfo.origin[2] << ")" << std::endl;
    infostr << "extent : (" << m_rawinfo.extent[0] << ", " << m_rawinfo.extent[1] << ", "
                            << m_rawinfo.extent[2] << ", " << m_rawinfo.extent[3] << ", "
                            << m_rawinfo.extent[4] << ", " << m_rawinfo.extent[5] << ")" << std::endl;
    m_rawinfo.infoText = infostr.str();
//    std::cout << m_rawinfo.infoText << std::endl;
    Instantiate(rawOutline, vtkOutlineFilter);
    rawOutline->SetInputConnection(reader->GetOutputPort());
    Instantiate(picker, vtkCellPicker);
    picker->SetTolerance(0.005);
    m_resliceImagePlaneWidget->SetInteractor(m_renderWindow->GetInteractor());
    m_resliceImagePlaneWidget->SetKeyPressActivationValue('x');
    m_resliceImagePlaneWidget->SetPicker(picker);
    m_resliceImagePlaneWidget->RestrictPlaneToVolumeOn();
    m_resliceImagePlaneWidget->GetPlaneProperty()->SetColor(0.0, 0.0, 1.0);
    m_resliceImagePlaneWidget->TextureInterpolateOff();
    m_resliceImagePlaneWidget->SetInputData((vtkDataSet*)m_rawData);
    m_resliceImagePlaneWidget->SetWindowLevel(2000, 800);
    m_resliceImagePlaneWidget->SetPlaneOrientationToXAxes();
    m_resliceImagePlaneWidget->SetSliceIndex(255);
    m_resliceImagePlaneWidget->GetTexturePlaneProperty()->SetOpacity(0.7);
    m_resliceImagePlaneWidget->Off();
    m_resliceViewer->SetInputData(m_resliceImagePlaneWidget->GetResliceOutput());
    m_renderer->ResetCamera();
    m_binresliceViewer->SetInputData(m_resliceImagePlaneWidget->GetResliceOutput());
    m_binresliceViewerRenderer->ResetCamera();
    return RawDataExist() ? true : false;
}

bool AneurysmUnit::RawDataExist()
{
    return m_rawData->GetActualMemorySize() != 0 ? true : false;
}

void AneurysmUnit::DrawSliceFactory(vtkSmartPointer<vtkRenderer> renderer,
                                    vtkSmartPointer<vtkImageActor> imgActor,
                                    double transformMat[], double pos[])
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
    imgActor->SetScale(7.0, 7.0, 1.0);
//    double *scale = imgActor->GetScale();
//    std::cout << "scale : " << scale[0] << ", " << scale[1] << ", " << scale[2] << std::endl;
    double *rangex = static_cast<double*>(imgActor->GetXRange());
    std::cout << "xrange : " << rangex[0] << ", " << rangex[1] << std::endl;
    double *rangey = static_cast<double*>(imgActor->GetYRange());
    std::cout << "yrange : " << rangey[0] << ", " << rangey[1] << std::endl;
    double *rangez = static_cast<double*>(imgActor->GetZRange());
    std::cout << "zrange : " << rangez[0] << ", " << rangez[1] << std::endl;

//    double m[16];
//    imgActor->GetMatrix(m);
//    std::cout << "Matrix : " << m[0] << "," << m[1] << "," << m[2] << "," << m[3]
//              << "----" << m[4] << "," << m[5] << "," << m[6] << "," << m[7]
//              << "----" << m[8] << "," << m[9] << "," << m[10] << "," << m[11]
//              << "----" << m[12] << "," << m[13] << "," << m[14] << "," << m[15] << std::endl;
}

void AneurysmUnit::DrawInfoSphereFactory(vtkSmartPointer<vtkRenderer> renderer,
                                         vtkSmartPointer<vtkActor> sphereactor,
                                         double pos[])
{
//    Instantiate(source1, vtkSphereSource);
////    source1->SetCenter(pos);
////    source1->SetCenter(0, 0, 0);
//    source1->SetRadius(100.0);
//    source1->Update();
//    Instantiate(mapper1, vtkPolyDataMapper);
//    mapper1->SetInputConnection(source1->GetOutputPort());
//    sphereactor->SetMapper(mapper1);
//    renderer->AddActor(sphereactor);
//    Instantiate(transform, vtkTransform);
//    transform->PostMultiply();
//    transform->Translate(pos);
//    sphereactor->SetUserTransform(transform);
//    sphereactor->Modified();
//    sphereactor->SetPosition(pos);
//    sphereactor->SetPosition(0, 0, 0);
//    double *p = sphereactor->GetPosition();
//    sphereactor->GetMatrix(m);

//    vtkLinearTransform *m = sphereactor->GetUserTransform();
//    std::cout << "Matrix : " << m[0] << "," << m[1] << "," << m[2] << "," << m[3]
//              << "----" << m[4] << "," << m[5] << "," << m[6] << "," << m[7]
//              << "----" << m[8] << "," << m[9] << "," << m[10] << "," << m[11]
//              << "----" << m[12] << "," << m[13] << "," << m[14] << "," << m[15] << std::endl;
//    renderer->DisplayToView();

//    ***********************alternatative way*********************************
    vtkAlgorithm *algo = sphereactor->GetMapper()->GetInputConnection(0, 0)->GetProducer();
    vtkSphereSource *src = vtkSphereSource::SafeDownCast(algo);
    src->SetCenter(pos[0], pos[1], pos[2]);
    src->SetRadius(100);
    src->Update();
    sphereactor->Modified();
    renderer->AddActor(sphereactor);
//    sphereactor->SetPosition(pos);
    renderer->Render();
}

void AneurysmUnit::Update3DSlice(double pos[])
{
    DrawSliceFactory(m_tranViewerRenderer, m_tranActor, tranElements, pos);
    DrawSliceFactory(m_corViewerRenderer, m_corActor, coronalElements, pos);
    DrawSliceFactory(m_sagViewerRenderer, m_sagActor, sagittalElements, pos);
    DrawInfoSphereFactory(m_tranViewerRenderer, m_transphere, pos);
    DrawInfoSphereFactory(m_corViewerRenderer, m_corsphere, pos);
    DrawInfoSphereFactory(m_sagViewerRenderer, m_sagsphere, pos);
}

void AneurysmUnit::SetVisibilityVirtualCuttingWidget(bool show)
{
    UpdateCuttingSource();
    show ? m_cuttingPlaneWidget->SetEnabled(1) : m_cuttingPlaneWidget->SetEnabled(0);
}

void AneurysmUnit::InitCuttingDisplay()
{
    vsp(m_cuttingPlane);
    vsp(m_cuttingPlaneWidget);
    vsp(m_cuttingPlane2);
    vsp(m_cuttingVtkPlane);
    vsp(m_implicitPlaneRepresentation);
//    Instantiate(plane, vtkPlane);
//    Instantiate(clipper, vtkClipPolyData);
//    Instantiate(reader, vtkSTLReader);
//    reader->SetFileName("invalid input");
//    clipper->SetClipFunction(plane);
//    clipper->SetInputConnection(reader->GetOutputPort());
//    reader->Update();
//    Instantiate(clipmapper, vtkPolyDataMapper);
//    clipmapper->SetInputConnection(clipper->GetOutputPort());
//    m_cuttingPlane->SetMapper(clipmapper);
//    Instantiate(backFaces, vtkProperty);
//    backFaces->SetDiffuseColor(.8, .8, .4);
//    m_cuttingPlane->SetBackfaceProperty(backFaces);
//    m_implicitPlaneRepresentation->SetPlaceFactor(1.0);
//    m_implicitPlaneRepresentation->PlaceWidget(m_cuttingPlane->GetBounds());
//    m_implicitPlaneRepresentation->SetNormal(plane->GetNormal());
//    m_implicitPlaneRepresentation->SetOrigin(30, -100, 1100);
//    m_implicitPlaneRepresentation->SetEdgeColor(.0, .7, .7);
//    Instantiate(callback, Util::vtkIPWCallback);
//    callback->Plane = plane;
//    callback->Actor = m_cuttingPlane;
//    m_cuttingPlaneWidget->SetInteractor(m_renderWindow->GetInteractor());
//    m_cuttingPlaneWidget->EnabledOff();
//    m_cuttingPlaneWidget->SetRepresentation(m_implicitPlaneRepresentation);
//    m_cuttingPlaneWidget->AddObserver(vtkCommand::InteractionEvent, callback);
}

void AneurysmUnit::UpdateCuttingSource()
{
//    Instantiate(actor, Util::SegmentActor)
//    if(!GetOnlySegModelSource(actor))  return ;
////    std::map<std::string, vtkSTLReader*> pipes;
////    pipes.insert(std::make_pair("3D Reg", m_3DReg_segmentationReader));
////    pipes.insert(std::make_pair("Level Set", m_LevelSet_segmentationReader));
////    pipes.insert(std::make_pair("Shape Detect", m_RegDetect_segmentationReader));
////    vtkSTLReader* reader = pipes[actor->GetSegmentType()];
//    m_cuttingVtkPlane->SetOrigin(m_implicitPlaneRepresentation->GetOrigin());
//    m_cuttingVtkPlane->SetNormal(m_implicitPlaneRepresentation->GetNormal());
//    Instantiate(cutter, vtkCutter);
//    cutter->SetCutFunction(m_cuttingVtkPlane);
//    Instantiate(reader, vtkSTLReader);
//    reader->SetFileName(actor->GetSegmentPath().c_str());
//    cutter->SetInputConnection(reader->GetOutputPort());
//    cutter->Update();
}

void AneurysmUnit::DoSTLCut()
{
    Instantiate(actor, Util::SegmentActor);
    if(!GetOnlySegModelSource(actor))  return ;
    m_cuttingVtkPlane->SetOrigin(m_implicitPlaneRepresentation->GetOrigin());
    m_cuttingVtkPlane->SetNormal(m_implicitPlaneRepresentation->GetNormal());
    Instantiate(cutter, vtkCutter);
    cutter->SetCutFunction(m_cuttingVtkPlane);
    Instantiate(reader, vtkSTLReader);
    reader->SetFileName(actor->GetSegmentPath().c_str());
    cutter->SetInputConnection(reader->GetOutputPort());
    cutter->Update();
    Instantiate(cutterMapper, vtkPolyDataMapper);
    cutterMapper->SetInputConnection(cutter->GetOutputPort());
    m_cuttingPlane2->GetProperty()->SetColor(1.0, 1.0, 0);
    m_cuttingPlane2->GetProperty()->SetLineWidth(2);
    m_cuttingPlane2->SetMapper(cutterMapper);
    m_renderer->AddActor(m_cuttingPlane2);
}

void AneurysmUnit::SetVisibilityDistanceWidgetOn(bool on)
{
    on ? m_distanceWidget->SetEnabled(1) : m_distanceWidget->SetEnabled(0);
}

void AneurysmUnit::SetVisibilityImagePlaneWidget(bool on)
{
    if(on) {
        m_resliceImagePlaneWidget->On();
        m_resliceViewer->SetInputData(m_resliceImagePlaneWidget->GetResliceOutput());
        m_resliceViewerRenderer->ResetCamera();
        m_binresliceViewer->SetInputData(m_resliceImagePlaneWidget->GetResliceOutput());
        m_binresliceViewerRenderer->ResetCamera();
        m_resliceTracerWidget->SetViewProp(m_binresliceViewer->GetImageActor());
        m_resliceTracerWidget->AddObserver(vtkCommand::EndInteractionEvent, m_tracCallback);

        m_resliceTracerWidget->On();
    }else {
        m_resliceImagePlaneWidget->Off();
    }

}
void AneurysmUnit::RemoveAllRenderers()
{
    // clean previous renderers and then add the current renderer

    m_renderWindow->RemoveRenderer(m_renderer);
    m_renderWindow->RemoveRenderer(m_resliceViewerRenderer);
    m_renderWindow->RemoveRenderer(m_binresliceViewerRenderer);
    m_renderWindow->RemoveRenderer(m_measureResRenderer);
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
    m_interactionHandler->RemoveAll();
    m_renInteractor->RemoveAllObservers();
    switch(mod) {
    case SINGLE_MOD: {
        m_renderer -> SetViewport(0, 0, 1, 1);
        m_renderWindow -> AddRenderer(m_renderer);
//        int m = m_renInteractor->GetInteractorStyle()->IsTypeOf("Util::CusInteractorPickPointStyle");
//        if(0 == m) {
//            m_renInteractor->RemoveObserver((vtkCommand*)m_renInteractor->GetInteractorStyle());
//            Instantiate(t_pointPickerStyle, Util::CusInteractorPickPointStyle);
//            m_renInteractor->SetInteractorStyle(t_pointPickerStyle);
//            t_pointPickerStyle->PreparedRenderer(m_renderer);
//            t_pointPickerStyle->SetInteractor(m_renInteractor);
//        }
        m_interactionHandler->Connect(m_renderer, m_pointpickerStyle);
        m_pointpickerStyle->PreparedRenderer(m_renderer);
        m_interactionHandler->SetInteractor(m_renInteractor);
        m_renInteractor->AddObserver(vtkCommand::MouseMoveEvent, m_interactionHandler);
        break;
    }

    case COMP_MOD: {
        m_renInteractor->SetInteractorStyle(m_roamingStyle);
        break;
    }
    case COMP4_MOD: {
        m_renderer -> SetViewport(0, 0, 0.5, 1);
        m_resliceViewerRenderer -> SetViewport(0.5, 0, 0.75, 0.5);
        m_resliceViewerRenderer -> ResetCamera();
        m_binresliceViewerRenderer -> SetViewport(0.75, 0, 1, 0.5 );
        m_binresliceViewerRenderer -> ResetCamera();
        m_measureResRenderer -> SetViewport(0.5, 0.5, 1, 1);
        m_renderWindow -> AddRenderer(m_renderer);
        m_renderWindow -> AddRenderer(m_resliceViewerRenderer);
        m_renderWindow -> AddRenderer(m_binresliceViewerRenderer);
        m_renderWindow -> AddRenderer(m_measureResRenderer);
        m_interactionHandler->Connect(m_renderer, m_roamingStyle);
        m_interactionHandler->Connect(m_resliceViewerRenderer, m_resliceStyle);
        m_interactionHandler->Connect(m_binresliceViewerRenderer, m_resliceStyle);
        m_interactionHandler->SetInteractor(m_renInteractor);
        m_renInteractor->AddObserver(vtkCommand::MouseMoveEvent, m_interactionHandler);
        break;
    }

    case PLANE_MOD: {
        m_renderer -> SetViewport(0, 0, 0.75, 1);
//            m_renderer -> SetBackground(.1, .2, .3);
        //3 plain views setting part
        m_tranViewerRenderer -> SetViewport(0.75, 0, 1, 0.33);
        m_corViewerRenderer -> SetViewport(0.75, 0.33, 1, 0.67);
        m_sagViewerRenderer -> SetViewport(0.75, 0.67, 1, 1);
        m_tranViewerRenderer -> ResetCamera();
        m_corViewerRenderer -> ResetCamera();
        m_sagViewerRenderer -> ResetCamera();
        m_renderWindow -> AddRenderer(m_renderer);
        m_renderWindow -> AddRenderer(m_tranViewerRenderer);
        m_renderWindow -> AddRenderer(m_corViewerRenderer);
        m_renderWindow -> AddRenderer(m_sagViewerRenderer);

        m_interactionHandler->SetInteractor(m_renInteractor);
        m_interactionHandler->Connect(m_renderer, m_roamingStyle);
        m_interactionHandler->Connect(m_tranViewerRenderer, m_xyzSliceStyle);
        m_interactionHandler->Connect(m_sagViewerRenderer, m_xyzSliceStyle);
        m_interactionHandler->Connect(m_corViewerRenderer, m_xyzSliceStyle);
        m_renInteractor->AddObserver(vtkCommand::MouseMoveEvent, m_interactionHandler);
        break;
    }

    default: {
        break;
    }

    }
}
void AneurysmUnit::InitSliderFactory(vtkSliderRepresentation2D *rep, vtkSliderWidget *widget,
                               const char * text, double pos[])
{
    rep = vtkSliderRepresentation2D::SafeDownCast(rep);
    rep->SetMinimumValue(0.0);
    rep->SetMaximumValue(1.0);
    rep->SetValue(1.0);
    rep->SetTitleText(text);
    rep->GetSliderProperty()->SetColor(1, 0, 0);
    rep->GetTitleProperty()->SetColor(1, 0, 0);
    rep->GetLabelProperty()->SetColor(1, 1, 0);
    rep->GetSliderProperty()->SetColor(0, 1, 0);
    rep->GetTubeProperty()->SetColor(1, 1, 0);
    rep->GetCapProperty()->SetColor(1, 1, 0);
    rep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    rep->GetPoint1Coordinate()->SetValue(pos[0], pos[1]);
    rep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    rep->GetPoint2Coordinate()->SetValue(pos[2], pos[3]);
    widget = vtkSliderWidget::SafeDownCast(widget);
    widget->SetRepresentation(rep);
    widget->EnabledOff();
}

void AneurysmUnit::InitSliders()
{
    double pos[3][4] = {{1030, 60, 1130, 60},
                       {1030, 200, 1130, 200},
                       {1030, 340, 1130, 340}};
    vsp(m_slider1Rep);
    vsp(m_slider1Widget);
    vsp(m_slider1CallBack);
    InitSliderFactory(m_slider1Rep,m_slider1Widget,"S1", pos[0]);
    vsp(m_slider2Rep);
    vsp(m_slider2Widget);
    vsp(m_slider2CallBack);
    InitSliderFactory(m_slider2Rep, m_slider2Widget, "S2", pos[1]);
    vsp(m_slider3Rep);
    vsp(m_slider3Widget);
    vsp(m_slider3CallBack);
    InitSliderFactory(m_slider3Rep, m_slider3Widget, "S3", pos[2]);
    BindSlider(m_3DReg_segmentationModel, m_slider1CallBack, m_slider1Widget);
    BindSlider(m_LevelSet_segmentationModel, m_slider2CallBack, m_slider2Widget);
    BindSlider(m_RegDetect_segmentationModel, m_slider3CallBack, m_slider3Widget);
}

bool AneurysmUnit::BindSlider(vtkActor* actor,
                              Util::vtkSliderCallBack* callback,
                              vtkSliderWidget* widget)
{
    if(actor == nullptr || callback == nullptr || widget == nullptr)
        return false;
    if(callback->SetBoundActor(vtkActor::SafeDownCast(actor))) {
        widget->AddObserver(vtkCommand::InteractionEvent, callback);
        widget->SetInteractor(m_renderWindow->GetInteractor());
        widget->SetAnimationModeToAnimate();
        widget->EnabledOff();
    }
}

void AneurysmUnit::SetPointPickerEnabled(bool enabled)
{
//    Util::CusInteractorPickPointStyle* cur_pointPickerStyle
//            = (Util::CusInteractorPickPointStyle*)m_renInteractor->GetInteractorStyle();
//       cur_pointPickerStyle->SetPickerEnabled(enabled);
//    std::cout << "point picker style is enabled ? "
//              << cur_pointPickerStyle->GetPickerEnabled() << std::endl;
    vtkInteractorStyle* style = m_interactionHandler->GetInteractorStyle(m_renderer);
    std::cout << style->GetClassName() << std::endl;
    Util::CusInteractorPickPointStyle* style2 = Util::CusInteractorPickPointStyle::SafeDownCast(style);
    style2->PreparedRenderer(m_renderer);
    style2->SetPickerEnabled(enabled);
    std::cout << "Point picker style is enabled ? "
              << style2->GetPickerEnabled() << std::endl;

}

std::string AneurysmUnit::GetRawFilename() {return m_rawinfo.filename ;}

std::string AneurysmUnit::GetRawInfo() {return m_rawinfo.infoText;}

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
void AneurysmUnit::InitAnnotationFactory(vtkCornerAnnotation* annotation, const char * text, double color[])
{
    annotation->SetLinearFontScaleFactor(2);
    annotation->SetNonlinearFontScaleFactor(1);
    annotation->SetMaximumFontSize(18);
    annotation->SetText(3, text);
    annotation->GetTextProperty()->SetColor(color);
}
void AneurysmUnit::InitAnnotation()
{
    vsp(m_tranAnnotation);
    double color1[3] = {0, 0, 1};
    InitAnnotationFactory(m_tranAnnotation, "Transverse", color1);
    vsp(m_corAnnotation);
    double color2[3] = {0, 1, 0};
    InitAnnotationFactory(m_corAnnotation, "Coronal", color2);
    vsp(m_sagAnnotation);
    double color3[3] = {1, 0, 0};
    InitAnnotationFactory(m_sagAnnotation, "Sagittal", color3);
}

void AneurysmUnit::InitInfoSphereFactory(vtkActor *sphere)
{
    Instantiate(src, vtkSphereSource);
    src->SetRadius(100);
    Instantiate(mapper, vtkPolyDataMapper);
    mapper->SetInputConnection(src->GetOutputPort());
    sphere->SetMapper(mapper);
    sphere->GetProperty()->SetColor(.8, .1, .0);
}
void AneurysmUnit::InitInfoSphere()
{
    vsp(m_transphere);
    vsp(m_sagsphere);
    vsp(m_corsphere);
    InitInfoSphereFactory(m_transphere);
    InitInfoSphereFactory(m_sagsphere);
    InitInfoSphereFactory(m_corsphere);
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
