#include "NavigationUnit.h"

NavigationUnit::NavigationUnit(vtkRenderWindow *renWin)
    :m_renWindow(renWin)
{
    vsp(m_renderer);
//    m_renderer->SetBackground(0.1, 0.2, 0.4);
    Instantiate(tmp_style, vtkInteractorStyleTrackballCamera);
    m_renWindow->GetInteractor()->SetInteractorStyle(tmp_style);
    Instantiate(tmp_camera, vtkCamera);
    tmp_camera->SetPosition(0, -50, 0);
    tmp_camera->SetViewUp(0, 0, 1);
    tmp_camera->SetFocalPoint(0, -48, 0);
    m_renderer->SetActiveCamera(tmp_camera);
    vsp(m_mainModel);
    InitMainModel();
    vsp(m_movingModel);
    InitPosInfo();
//    m_renderer->AddActor(m_mainModel);
    vsp(m_annotation);
    m_annotation->SetLinearFontScaleFactor( 2 );
    m_annotation->SetNonlinearFontScaleFactor( 1 );
    m_annotation->SetMaximumFontSize( 15 );
    m_annotation->SetText(1 ,"position:\n(.0, .0, .0)");
    m_annotation->GetTextProperty()->SetColor( 1, 0, 0 );
    m_renderer->AddViewProp(m_annotation);
    m_renderer->ResetCamera();
    m_renWindow->AddRenderer(m_renderer);
    m_renWindow->Render();
}

void NavigationUnit::InitMainModel()
{

    m_mainModel->GetProperty()->SetColor(1., 1., 1.);
    m_mainModel->GetProperty()->SetOpacity(0.3);

//    Instantiate(reader, vtkSTLReader);
//    reader->SetFileName("D:/~111data/pa09seg1.stl");
//    reader->Update();
//    Instantiate(mapper, vtkPolyDataMapper);
//    mapper->SetInputConnection(reader->GetOutputPort());
//    m_mainModel->SetMapper(mapper);
//    m_renderer->AddActor(m_mainModel);
//    m_renderer->ResetCamera();
//    m_renWindow->Render();
}

void NavigationUnit::UpdateMainModel(std::string path)
{
    std::cout << "input : " << path << std::endl;
    Instantiate(reader, vtkSTLReader);
//    reader->SetFileName("D:/~111data/pa09seg1.stl");
    reader->SetFileName(path.c_str());
    reader->Update();
    Instantiate(mapper, vtkPolyDataMapper);
    mapper->SetInputConnection(reader->GetOutputPort());
    m_mainModel->SetMapper(mapper);
    m_renderer->AddActor(m_mainModel);
    m_renderer->ResetCamera();
    m_renWindow->Render();

//    vtkAlgorithm *algo = actor->GetMapper()->GetInputConnection(0, 0)->GetProducer();
//    Instantiate(mapper, vtkPolyDataMapper);
//    mapper->SetInputConnection(algo->GetOutputPort());
//    m_mainModel->SetMapper(mapper);
    //    m_renderer->AddActor(m_mainModel);
}

void NavigationUnit::InitPathLine()
{

}

void NavigationUnit::UpdatePathLine()
{

}

void NavigationUnit::UpdateTotalView(std::string path, double *pos)
{
    UpdateMainModel(path);
    UpdatePosInfo(pos);
    m_renderer->ResetCamera();
    m_renWindow->Render();
}
void NavigationUnit::InitPosInfo()
{
    Instantiate(src, vtkSphereSource);
    src->SetRadius(1);
    src->SetCenter(0, 0, 0);
    Instantiate(mapper, vtkPolyDataMapper);
    mapper->SetInputConnection(src->GetOutputPort());
    m_movingModel->SetMapper(mapper);
    m_movingModel->GetProperty()->SetColor(.5, .5, .0);
    m_movingModel->GetProperty()->SetOpacity(0.5);
}

void NavigationUnit::UpdatePosInfo(double *pos)
{
    vtkAlgorithm *algo = m_movingModel->GetMapper()->GetInputConnection(0, 0)->GetProducer();
    vtkSphereSource *src = vtkSphereSource::SafeDownCast(algo);
    src->SetCenter(pos[0], pos[1], pos[2]);
    src->SetRadius(1);
    src->Update();
    m_movingModel->Modified();
    m_renderer->AddActor(m_movingModel);
//    m_renderer->ResetCamera();
//    m_renWindow->Render();
}



