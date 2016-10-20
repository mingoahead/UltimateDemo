#ifndef ANEURYSMUNIT_H
#define ANEURYSMUNIT_H
#include <string>
#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkActor2D.h>
#include <vtkActor2DCollection.h>
#include <vtkMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkSTLReader.h>
#include <vtkCommand.h>
#include <vtkImageViewer2.h>
#include <vtkImageViewer.h>
#include <vtkRenderer.h>

#include "../Utils/fastdef.h"

class AneurysmUnit
{
public:
    AneurysmUnit(vtkRenderWindow *renWin);
    ~AneurysmUnit();
    vtkRenderer * GetRenderer();
    vtkRenderer * GettranViewerRenderer();
    vtkRenderer * GetcorViewerRenderer();
    vtkRenderer * GetsagViewerRenderer();

    void ReadInputSegmentationModel(std::string fileName);
    void ShowSegmentationModel();
    void HideSegmentationModel();
private:
    vtkRenderWindow * m_renderWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkActor> m_segmentationModel;
    vtkSmartPointer<vtkSTLReader> m_segmentationModelReader;

    vtkSmartPointer<vtkImageViewer> m_tranViewer;
    vtkSmartPointer<vtkImageViewer> m_sagViewer;
    vtkSmartPointer<vtkImageViewer> m_corViewer;
    vtkRenderer *m_tranViewerRenderer;
    vtkRenderer *m_sagViewerRenderer;
    vtkRenderer *m_corViewerRenderer;
};

#endif // ANEURYSMUNIT_H
