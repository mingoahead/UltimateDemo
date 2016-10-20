#include "AneurysmUnit.h"


AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{

//    m_renderer = vtkSmartPointer<vtkRenderer>::New();
//    m_segmentationModel = vtkSmartPointer<vtkActor>::New();
//    m_segmentationModelReader = vtkSmartPointer<vtkSTLReader>::New();

    vsp(m_renderer);
    vsp(m_segmentationModel);
    vsp(m_segmentationModelReader);

    m_renderer -> SetViewport(0, 0, 0.75, 1);
    m_renderer -> SetBackground(.1, .2, .3);
    vsp(m_tranViewer);
    vsp(m_corViewer);
    vsp(m_sagViewer);
    m_tranViewerRenderer = m_tranViewer -> GetRenderer();
    m_tranViewerRenderer -> SetViewport(0.75, 0, 1, 0.33);
    m_tranViewerRenderer -> GradientBackgroundOn();
    m_tranViewerRenderer -> SetBackground(0.3, 0.4, 0.5);
    m_corViewerRenderer = m_corViewer -> GetRenderer();
    m_corViewerRenderer -> SetViewport(0.75, 0.33, 1, 0.67);
    m_corViewerRenderer -> GradientBackgroundOn();
    m_corViewerRenderer -> SetBackground(0.4, 0.5, 0.6);
    m_sagViewerRenderer = m_sagViewer -> GetRenderer();
    m_sagViewerRenderer -> SetViewport(0.75, 0.67, 1, 1);
    m_sagViewerRenderer -> GradientBackgroundOn();
    m_sagViewerRenderer -> SetBackground(0.5, 0.6, 0.7);

    m_renderWindow -> AddRenderer(m_tranViewerRenderer);
    m_renderWindow -> AddRenderer(m_corViewerRenderer);
    m_renderWindow -> AddRenderer(m_sagViewerRenderer);

}

AneurysmUnit::~AneurysmUnit()
{

}

vtkRenderer *AneurysmUnit::GetRenderer()
{
    return m_renderer;
}

vtkRenderer *AneurysmUnit::GettranViewerRenderer()
{
    return m_tranViewerRenderer;
}

vtkRenderer *AneurysmUnit::GetcorViewerRenderer()
{
    return m_corViewerRenderer;
}

vtkRenderer *AneurysmUnit::GetsagViewerRenderer()
{
    return m_sagViewerRenderer;
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
