#include "AneurysmUnit.h"


AneurysmUnit::AneurysmUnit(vtkRenderWindow *renWin) : m_renderWindow(renWin)
{

//    m_renderer = vtkSmartPointer<vtkRenderer>::New();
//    m_segmentationModel = vtkSmartPointer<vtkActor>::New();
//    m_segmentationModelReader = vtkSmartPointer<vtkSTLReader>::New();

    vsp(m_renderer);
    vsp(m_ul_renderer);
    vsp(m_ur_renderer);
    vsp(m_bl_renderer);
    vsp(m_br_renderer);
    vsp(m_3DReg_segmentationModel);
    vsp(m_LevelSet_segmentationModel);
    vsp(m_RegDetect_segmentationModel);
    vsp(m_segmentationModelReader);
//    init three model color
    m_3DReg_segmentationModel -> GetProperty() -> SetColor(1.0, .0, .0);
    m_LevelSet_segmentationModel -> GetProperty() -> SetColor(.0, 1.0, .0);
    m_RegDetect_segmentationModel -> GetProperty() -> SetColor(.0, .0, 1.0);
    vsp(m_tranViewer);
    vsp(m_corViewer);
    vsp(m_sagViewer);

    vsp(m_tranAnnotation);
    m_tranAnnotation->SetLinearFontScaleFactor(2);
    m_tranAnnotation->SetNonlinearFontScaleFactor(1);
    m_tranAnnotation->SetMaximumFontSize(18);
    m_tranAnnotation->SetText(3, "Tran View");
    m_tranAnnotation->GetTextProperty()->SetColor(0, 0, 1);
    vsp(m_corAnnotation);
    m_corAnnotation->SetLinearFontScaleFactor(2);
    m_corAnnotation->SetNonlinearFontScaleFactor(1);
    m_corAnnotation->SetMaximumFontSize(18);
    m_corAnnotation->SetText(3, "Cor View");
    m_corAnnotation->GetTextProperty()->SetColor(0, 1, 0);
    vsp(m_sagAnnotation);
    m_sagAnnotation->SetLinearFontScaleFactor(2);
    m_sagAnnotation->SetNonlinearFontScaleFactor(1);
    m_sagAnnotation->SetMaximumFontSize(18);
    m_sagAnnotation->SetText(3, "Sag View");
    m_sagAnnotation->GetTextProperty()->SetColor(1, 0, 0);
    RegisterDisplay(1);

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
    m_segmentationModelReader -> SetFileName(fileName.c_str());
    m_segmentationModelReader -> Update();
    vtkSmartPointer<vtkPolyDataMapper> mapper
            = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper -> SetInputConnection(m_segmentationModelReader -> GetOutputPort());
    switch(option) {
    case 1:
        m_3DReg_segmentationModel -> SetMapper(mapper);

        break;
    case 2:
        m_LevelSet_segmentationModel -> SetMapper(mapper);
        break;
    case 3:
        m_RegDetect_segmentationModel -> SetMapper(mapper);
        break;
    default:
        break;

    }
    m_renderer -> ResetCamera();
}


void AneurysmUnit::ShowSegmentationModel(int option)
{
    switch (option) {
    case 1:
        m_renderer -> AddActor(m_3DReg_segmentationModel);
        break;
    case 2:
        m_renderer -> AddActor(m_LevelSet_segmentationModel);
        break;
    case 3:
        m_renderer -> AddActor(m_RegDetect_segmentationModel);
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
        break;
    case 2:
        m_renderer -> RemoveActor(m_LevelSet_segmentationModel);
        break;
    case 3:
        m_renderer -> RemoveActor(m_RegDetect_segmentationModel);
    default:
        break;
    }
}
