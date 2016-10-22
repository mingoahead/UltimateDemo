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
#include <vtkCollection.h>
#include <vtkRendererCollection.h>

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

    void ReadInputSegmentationModel(std::string fileName, int option);
    void ShowSegmentationModel(int option);
    void HideSegmentationModel(int option);
    void SetDisplay(int mod)
    {
        switch(mod) {
        case SINGLE_MOD:
//            m_renderWindow -> RemoveAllObservers();
            RemoveAllRenderers();
            m_renderer -> SetViewport(0, 0, 1, 1);
//            m_renderer -> SetBackground(.1, .2, .3);
            m_renderer -> SetBackground(.0,.0,.0);
            m_renderWindow -> AddRenderer(m_renderer);
            break;
        case COMP2_MOD:
            break;
        case PLANE_MOD:
//            m_renderWindow -> RemoveAllObservers();
            RemoveAllRenderers();
            m_renderer -> SetViewport(0, 0, 0.75, 1);
//            m_renderer -> SetBackground(.1, .2, .3);
            m_renderer -> SetBackground(.0, .0, .0);
            m_renderWindow -> AddRenderer(m_renderer);
            //3 plain views setting part
            m_tranViewerRenderer = m_tranViewer -> GetRenderer();
            m_tranViewerRenderer -> SetViewport(0.75, 0, 1, 0.33);
            m_tranViewerRenderer -> GradientBackgroundOn();
            m_tranViewerRenderer -> SetBackground(0.3, 0.5, 0.5);
            m_corViewerRenderer = m_corViewer -> GetRenderer();
            m_corViewerRenderer -> SetViewport(0.75, 0.33, 1, 0.67);
            m_corViewerRenderer -> GradientBackgroundOn();
            m_corViewerRenderer -> SetBackground(0.4, 0.4, 0.6);
            m_sagViewerRenderer = m_sagViewer -> GetRenderer();
            m_sagViewerRenderer -> SetViewport(0.75, 0.67, 1, 1);
            m_sagViewerRenderer -> GradientBackgroundOn();
            m_sagViewerRenderer -> SetBackground(0.7, 0.6, 0.7);
            m_renderWindow -> AddRenderer(m_tranViewerRenderer);
            m_renderWindow -> AddRenderer(m_corViewerRenderer);
            m_renderWindow -> AddRenderer(m_sagViewerRenderer);
            break;
        default:
            break;
        }
    }
    void RemoveAllRenderers()

    {
        m_renderWindow -> RemoveRenderer(m_renderer);
        m_renderWindow -> RemoveRenderer(m_tranViewer -> GetRenderer());
        m_renderWindow -> RemoveRenderer(m_corViewer -> GetRenderer());
        m_renderWindow -> RemoveRenderer(m_sagViewer -> GetRenderer());
//       vtkRendererCollection *curRenderers = m_renderWindow -> GetRenderers();
//       int cnt = curRenderers -> GetNumberOfItems();
//       while(cnt-- > 0) {
//           vtkRenderer *tmp = curRenderers -> GetNextItem();
//           m_renderWindow -> RemoveRenderer(tmp);
//       }

    }
private:
    vtkRenderWindow * m_renderWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkActor> m_3DReg_segmentationModel;
    vtkSmartPointer<vtkActor> m_LevelSet_segmentationModel;
    vtkSmartPointer<vtkActor> m_RegDetect_segmentationModel;
    vtkSmartPointer<vtkSTLReader> m_segmentationModelReader;

    vtkSmartPointer<vtkImageViewer> m_tranViewer;
    vtkSmartPointer<vtkImageViewer> m_sagViewer;
    vtkSmartPointer<vtkImageViewer> m_corViewer;
    vtkRenderer *m_tranViewerRenderer;
    vtkRenderer *m_sagViewerRenderer;
    vtkRenderer *m_corViewerRenderer;
    enum DisMode { SINGLE_MOD = 1, COMP2_MOD, PLANE_MOD} ;
private:

};

#endif // ANEURYSMUNIT_H
