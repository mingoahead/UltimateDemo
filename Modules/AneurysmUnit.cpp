#include "AneurysmUnit.h"


AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{

//    m_renderer = vtkSmartPointer<vtkRenderer>::New();
//    m_segmentationModel = vtkSmartPointer<vtkActor>::New();
//    m_segmentationModelReader = vtkSmartPointer<vtkSTLReader>::New();

    vsp(m_renderer);
    vsp(m_segmentationModel);
    vsp(m_segmentationModelReader);

    m_renderer -> SetBackground(.1, .2, .3);
}

AneurysmUnit::~AneurysmUnit()
{

}

vtkRenderer *AneurysmUnit::GetRenderer()
{
    return m_renderer;
}

void AneurysmUnit::ReadInputSegmentationModel(std::string fileName)
{
    m_segmentationModelReader -> SetFileName(fileName.c_str());
    m_segmentationModelReader -> Update();
    vtkSmartPointer<vtkPolyDataMapper> mapper
            = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper -> SetInputConnection(m_segmentationModelReader -> GetOutputPort());
    m_segmentationModel -> SetMapper(mapper);
    m_renderer -> ResetCamera();
}

void AneurysmUnit::ShowSegmentationModel()
{
    m_renderer -> AddActor(m_segmentationModel);
    m_renderer -> ResetCamera();
}

void AneurysmUnit::HideSegmentationModel()
{
    m_renderer -> RemoveActor(m_segmentationModel);
}
