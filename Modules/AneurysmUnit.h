#ifndef ANEURYSMUNIT_H
#define ANEURYSMUNIT_H
#include <string>
#include <vector>
#include <map>

#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkActor2D.h>
#include <vtkActor2DCollection.h>
#include <vtkMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>
#include <vtkLight.h>
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
#include <vtkLegendBoxActor.h>
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkWidgetEvent.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty2D.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkColor.h>
#include <vtkPointData.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkClipPolyData.h>
#include <vtkCameraRepresentation.h>
#include <vtkCameraWidget.h>


#include <vtkBalloonRepresentation.h>
#include <vtkBalloonWidget.h>

#include <vtkPointPicker.h>
#include <vtkSphereSource.h>

#include <vtkImageActor.h>
#include <vtkLookupTable.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageMapToColors.h>
#include <vtkMatrix4x4.h>
#include <vtkMetaImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkImageReslice.h>

#include "fastdef.h"
#include "CenLineUnit.h"
#include "VolSurRendering.h"
#include "Utils/vtkhelper.h"

/**
 * to do list
 * legend  ref : http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/Legend
 * colormap ref: chrome-extension://oemmndcbldboiebfnladdacbdfmadadm/http://www.inf.ed.ac.uk/teaching/courses/vis/lecture_notes/lecture5-2x2.pdf
 *               vtkLookupTable
 *          ref: http://www.vtk.org/Wiki/VTK/Examples/Cxx/Meshes/ColoredElevationMap
 *          ref: chrome-extension://oemmndcbldboiebfnladdacbdfmadadm/http://cs.iupui.edu/~sfang/cs552/cs552-surface.pdf
 *               vtkContourFilter  (extract contour with same attribute value)
 */

class AneurysmUnit
{
public:
    AneurysmUnit(vtkRenderWindow *renWin);
    ~AneurysmUnit();
    vtkRenderer * GetRenderer();
    vtkRenderer * GetLeftRenderer();
    vtkRenderer * GetRightRenderer();
    ctkVTKVolumePropertyWidget * GetVolumePropertyWidget();
    vtkRenderer * GetBLRenderer();
    vtkRenderer * GetBRRenderer();
    vtkRenderer * GetULRenderer();
    vtkRenderer * GetURRenderer();
    vtkRenderer * GettranViewerRenderer();
    vtkRenderer * GetcorViewerRenderer();
    vtkRenderer * GetsagViewerRenderer();
    /// rendering
    void ReadInputSegmentationModel(std::string fileName, int option);
    void ShowSegmentationModel(int option);
    void ShowFrameMode(int option);
    void ShowLineMode(int option);
    void ShowPointMode(int option);
    void HideSegmentationModel(int option);
    void ShowCenterPoints(vtkSmartPointer<vtkActor> LineModel, 
                          std::vector<Point3f> &CenterPoints);
    void GetCenterLine(int option);
    void DrawCenterLine(int option, bool isLeft);
    void HideCenterLine(int option, bool isLeft);
    /// roaming
    void SetRoamingRoute(int id = 1);
    void SetRoamingStep(int step);
    void OnRoam();
    void RoamNext();
    void RoamPrevious();

    bool LoadRawData(std::string fileName);
    bool RawDataExist();
    void DrawSliceFactory(vtkSmartPointer<vtkRenderer> renderer
                              , vtkSmartPointer<vtkImageActor>imgActor
                              , double transformMat[16], double pos[3]);
    void Draw3DSlice(double pos[3]);
    /// comparing --surface and volume
    void DrawVolume_Surface();

    void SetVisibilitySTLCuttingPlane(bool show = true);
    void SetVisibilityVirtualCuttingWidget(bool show = true);
    void DoStlCutting(vtkSmartPointer<vtkSTLReader> stlreader);
    void DoSTLCut();

    void RegisterDisplay(int mod)
    {
        switch(mod) {
        case SINGLE_MOD: {
            RemoveAllRenderers();
//            m_renInteractor->RemoveAllObservers();
            m_renderer -> SetViewport(0, 0, 1, 1);
            m_renderer -> SetBackground(.1, .2, .3);
//            m_renderer -> SetBackground(.0,.0,.0);
            m_renderWindow -> AddRenderer(m_renderer);
//            std::cout << "Current Style : "
//                      << m_renInteractor->GetInteractorStyle()->GetClassName() << std::endl;
            int m = m_renInteractor->GetInteractorStyle()->IsTypeOf("Util::CusInteractorStylePickPoint");
            if(0 == m) {
                m_renInteractor->RemoveObserver((vtkCommand*)m_renInteractor->GetInteractorStyle());
                Instantiate(t_pointPickerStyle, Util::CusInteractorStylePickPoint);
                m_renInteractor->SetInteractorStyle(t_pointPickerStyle);
                t_pointPickerStyle->PreparedRenderer(m_renderer);
                t_pointPickerStyle->SetInteractor(m_renInteractor);
            }
            break;
        }
//            m_renderWindow -> RemoveAllObservers();

        case COMP2_MOD: {
            RemoveAllRenderers();
//            m_renInteractor->RemoveAllObservers();
            m_left_renderer -> SetViewport(0, 0, 0.5, 1);
            m_left_renderer -> SetBackground(.3, .4, .0);
            m_right_renderer -> SetViewport(.5, 0, 1, 1);
            m_right_renderer -> SetBackground(.0, .1, .3);
            m_renderWindow -> AddRenderer(m_left_renderer);
            m_renderWindow -> AddRenderer(m_right_renderer);
            break;
        }

        case COMP4_MOD: {
            RemoveAllRenderers();
            m_ul_renderer -> SetViewport(0, 0, 0.5, 0.5);
            m_ul_renderer -> SetBackground(0.1, 0.1, 0.2);
            m_ur_renderer -> SetViewport(.5, 0, 1, 0.5);
            m_ur_renderer -> SetBackground(0.1, 0.2, 0.1);
            m_bl_renderer -> SetViewport(0, 0.5, 0.5, 1);
            m_bl_renderer -> SetBackground(0.2, 0.1, 0.2);
            m_br_renderer -> SetViewport(0.5, 0.5, 1, 1);
            m_br_renderer -> SetBackground(.2, .2, .2);
            m_renderWindow -> AddRenderer(m_ul_renderer);
            m_renderWindow -> AddRenderer(m_ur_renderer);
            m_renderWindow -> AddRenderer(m_bl_renderer);
            m_renderWindow -> AddRenderer(m_br_renderer);
            break;
        }

        case PLANE_MOD: {
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
            m_tranViewerRenderer -> AddViewProp(m_tranAnnotation);
            m_corViewerRenderer = m_corViewer -> GetRenderer();
            m_corViewerRenderer -> SetViewport(0.75, 0.33, 1, 0.67);
            m_corViewerRenderer -> GradientBackgroundOn();
            m_corViewerRenderer -> SetBackground(0.4, 0.4, 0.6);
            m_corViewerRenderer -> AddViewProp(m_corAnnotation);
            m_sagViewerRenderer = m_sagViewer -> GetRenderer();
            m_sagViewerRenderer -> SetViewport(0.75, 0.67, 1, 1);
            m_sagViewerRenderer -> GradientBackgroundOn();
            m_sagViewerRenderer -> SetBackground(0.7, 0.6, 0.7);
            m_sagViewerRenderer -> AddViewProp(m_sagAnnotation);
            m_renderWindow -> AddRenderer(m_tranViewerRenderer);
            m_renderWindow -> AddRenderer(m_corViewerRenderer);
            m_renderWindow -> AddRenderer(m_sagViewerRenderer);
//            std::cout << "Current Style : "
//                      << m_renInteractor->GetInteractorStyle()->GetClassName() << std::endl;
            int m = m_renInteractor->GetInteractorStyle()->IsTypeOf("vtkInteractorStyleImage");
            if(0 == m) {
                m_renInteractor->RemoveObserver((vtkCommand*)m_renInteractor->GetInteractorStyle());
                Instantiate(t_SliceViewStyle, vtkInteractorStyleImage);
                m_renInteractor->SetInteractorStyle(t_SliceViewStyle);
                t_SliceViewStyle->SetInteractor(m_renInteractor);
            }
            break;
        }

        default:
            break;
        }
    }
    void RemoveAllRenderers()
    {
        m_renderWindow -> RemoveRenderer(m_renderer);
        m_renderWindow -> RemoveRenderer(m_left_renderer);
        m_renderWindow -> RemoveRenderer(m_right_renderer);
        m_renderWindow -> RemoveRenderer(m_bl_renderer);
        m_renderWindow -> RemoveRenderer(m_br_renderer);
        m_renderWindow -> RemoveRenderer(m_ul_renderer);
        m_renderWindow -> RemoveRenderer(m_ur_renderer);
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

    void InitSliders();
    bool BindSlider(vtkSmartPointer<vtkActor> actor,
                    vtkSmartPointer<Util::vtkSliderCallBack> sliderCallBack,
                    vtkSmartPointer<vtkSliderWidget> sliderWidget);
    void SetPointPickerEnabled(bool enabled = true);
    std::string GetRawFilename();
private:
    vtkRenderWindow * m_renderWindow;
    vtkSmartPointer<vtkLight> m_light;
    vtkSmartPointer<vtkRenderWindowInteractor> m_renInteractor;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderer> m_ul_renderer;
    vtkSmartPointer<vtkRenderer> m_ur_renderer;
    vtkSmartPointer<vtkRenderer> m_bl_renderer;
    vtkSmartPointer<vtkRenderer> m_br_renderer;
    vtkSmartPointer<vtkRenderer> m_left_renderer;
    vtkSmartPointer<vtkRenderer> m_right_renderer;
    vtkSmartPointer<vtkActor> m_Surface;
    vtkSmartPointer<vtkVolume> m_Volume;
    ctkVTKVolumePropertyWidget *m_VolumePropertyWidget;

    vtkSmartPointer<vtkActor> m_3DReg_segmentationModel;
    vtkSmartPointer<vtkActor> m_LevelSet_segmentationModel;
    vtkSmartPointer<vtkActor> m_RegDetect_segmentationModel;
    vtkSmartPointer<vtkSTLReader> m_3DReg_segmentationReader;
    vtkSmartPointer<vtkSTLReader> m_LevelSet_segmentationReader;
    vtkSmartPointer<vtkSTLReader> m_RegDetect_segmentationReader;

    vtkSmartPointer<vtkActor> m_cuttingPlane;
    vtkSmartPointer<vtkImplicitPlaneWidget2> m_cuttingPlaneWidget;
    vtkSmartPointer<vtkImplicitPlaneRepresentation> m_implicitPlaneRepresentation;
    vtkSmartPointer<vtkPlane> m_cuttingVtkPlane;
    vtkSmartPointer<vtkActor> m_cuttingPlane2;

    // centerline related
    vtkSmartPointer<vtkPointPicker> m_lineInfoPointPicker;
//    vtkSmartPointer<CusInteractorStylePickPoint> m_pointPickerInteractorStyle;
//    vtkSmartPointer<vtkInteractorStyleImage> m_sliceViewStyle;

    vtkSmartPointer<vtkActor> m_leftLineModel;
    vtkSmartPointer<vtkActor> m_rightLineModel;
    std::pair<std::string, std::string> m_filename;
    CenLineUnit *m_centerLine;

    int m_currentRoamingRouteId;
    int m_currentRoamingIndex;
    int m_currentRoamingStep;
    void GetCurrentRoamingPoint(double p[3]);
    void UpdateRoamingCamera();


    vtkSmartPointer<vtkImageData> m_rawData;
    vtkSmartPointer<vtkImageViewer> m_tranViewer;
    vtkSmartPointer<vtkImageViewer> m_sagViewer;
    vtkSmartPointer<vtkImageViewer> m_corViewer;
    vtkSmartPointer<vtkImageActor> m_tranActor;
    vtkSmartPointer<vtkImageActor> m_sagActor;
    vtkSmartPointer<vtkImageActor> m_corActor;

    vtkSmartPointer<vtkCornerAnnotation> m_tranAnnotation;
    vtkSmartPointer<vtkCornerAnnotation> m_sagAnnotation;
    vtkSmartPointer<vtkCornerAnnotation> m_corAnnotation;
    vtkRenderer *m_tranViewerRenderer;
    vtkRenderer *m_sagViewerRenderer;
    vtkRenderer *m_corViewerRenderer;
    enum DisMode { SINGLE_MOD = 1, COMP2_MOD,  PLANE_MOD, COMP4_MOD} ;

    //slider related
    vtkSmartPointer<vtkSliderRepresentation2D> m_slider1Rep;
    vtkSmartPointer<vtkSliderWidget> m_slider1Widget;
    vtkSmartPointer<Util::vtkSliderCallBack> m_slider1CallBack;
    vtkSmartPointer<vtkSliderRepresentation2D> m_slider2Rep;
    vtkSmartPointer<vtkSliderWidget> m_slider2Widget;
    vtkSmartPointer<Util::vtkSliderCallBack> m_slider2CallBack;
    vtkSmartPointer<vtkSliderRepresentation2D> m_slider3Rep;
    vtkSmartPointer<vtkSliderWidget> m_slider3Widget;
    vtkSmartPointer<Util::vtkSliderCallBack> m_slider3CallBack;
    //camera widget related
    vtkSmartPointer<vtkCameraRepresentation> m_cameraRep;
    vtkSmartPointer<vtkCameraWidget>         m_cameraWidget;


private:
    void InitAnnotation();
    void InitCamerasWidgets();



};

#endif // ANEURYSMUNIT_H
