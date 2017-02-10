#ifndef ANEURYSMUNIT_H
#define ANEURYSMUNIT_H
#include <string>
#include <vector>
#include <map>
#include <sstream>

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
#include <vtkInteractorStyleSwitch.h>
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
#include <vtkImagePlaneWidget.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkPointPicker.h>
#include <vtkTransform.h>
#include <vtkAlgorithmOutput.h>

#include <vtkBalloonRepresentation.h>
#include <vtkBalloonWidget.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkImageTracerWidget.h>

#include <vtkPointPicker.h>
#include <vtkSphereSource.h>

#include <vtkOutlineFilter.h>
#include <vtkCellPicker.h>
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
    vtkRenderer * GettranViewerRenderer();
    vtkRenderer * GetcorViewerRenderer();
    vtkRenderer * GetsagViewerRenderer();
    /// rendering
    std::string GetCurInteractorStyle();
    void ReadInputSegmentationModel(std::string fileName, int option);
    void ShowSegmentationModel(int option);
    void ShowFrameMode(int option);
    void ShowLineMode(int option);
    void ShowPointMode(int option);
    void HideSegmentationModel(int option);

    void SetVisibilityCollectiOn();
    void SetVisibilityCollectiOff();
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
    void Update3DSlice(double pos[3]);

    void SetVisibilityVirtualCuttingWidget(bool show = true);
    void InitCuttingDisplay();
    void UpdateCuttingSource();
    void DoSTLCut();

    void SetVisibilityDistanceWidgetOn(bool on);
    void SetVisibilityImagePlaneWidget(bool on);

    void RegisterDisplay(int mod);
    void RemoveAllRenderers();
    void SetPointPickerEnabled(bool enabled = true);
    std::string GetRawFilename();
    std::string GetRawInfo();
private:
    vtkRenderWindow * m_renderWindow;
    vtkSmartPointer<vtkLight> m_light;
    vtkSmartPointer<vtkRenderWindowInteractor> m_renInteractor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_roamingStyle;
    vtkSmartPointer<vtkInteractorStyleImage> m_xyzSliceStyle;
    vtkSmartPointer<Util::CusInteractorPickPointStyle> m_pointpickerStyle;
    vtkSmartPointer<vtkInteractorStyleImage> m_resliceStyle;

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageViewer2> m_resliceViewer;
    vtkSmartPointer<vtkRenderer> m_resliceViewerRenderer;
    vtkSmartPointer<vtkImageViewer2> m_binresliceViewer;
    vtkSmartPointer<vtkRenderer> m_binresliceViewerRenderer;
    vtkSmartPointer<vtkContextView> m_measureResView;
    vtkSmartPointer<vtkRenderer> m_measureResRenderer;
    vtkSmartPointer<vtkImageTracerWidget> m_resliceTracerWidget;
    vtkSmartPointer<Util::vtkImageTracerCallback> m_tracCallback;

    vtkSmartPointer<Util::SegmentActor> m_3DReg_segmentationModel;
    vtkSmartPointer<Util::SegmentActor> m_LevelSet_segmentationModel;
    vtkSmartPointer<Util::SegmentActor> m_RegDetect_segmentationModel;
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

    vtkSmartPointer<vtkActor> m_leftLineModel;
    vtkSmartPointer<vtkActor> m_rightLineModel;
    CenLineUnit *m_centerLine;

    int m_currentRoamingRouteId;
    int m_currentRoamingIndex;
    int m_currentRoamingStep;
    void GetCurrentRoamingPoint(double p[3]);
    void UpdateRoamingCamera();

    vtkSmartPointer<vtkImageData> m_rawData;
    RawDataInfo m_rawinfo;

    vtkSmartPointer<Util::InteractionCallBackHandler> m_interactionHandler;
    vtkSmartPointer<vtkRenderer> m_tranViewerRenderer;
    vtkSmartPointer<vtkRenderer> m_sagViewerRenderer;
    vtkSmartPointer<vtkRenderer> m_corViewerRenderer;
    vtkSmartPointer<vtkImageActor> m_tranActor;
    vtkSmartPointer<vtkImageActor> m_sagActor;
    vtkSmartPointer<vtkImageActor> m_corActor;
    vtkSmartPointer<vtkActor> m_transphere;
    vtkSmartPointer<vtkActor> m_sagsphere;
    vtkSmartPointer<vtkActor> m_corsphere;

    vtkSmartPointer<vtkCornerAnnotation> m_tranAnnotation;
    vtkSmartPointer<vtkCornerAnnotation> m_sagAnnotation;
    vtkSmartPointer<vtkCornerAnnotation> m_corAnnotation;
    enum DisMode { SINGLE_MOD = 1,  COMP_MOD = 2, PLANE_MOD = 3, COMP4_MOD = 4} ;

    vtkSmartPointer<vtkImagePlaneWidget> m_resliceImagePlaneWidget;
    vtkSmartPointer<vtkDistanceWidget> m_distanceWidget;
    vtkSmartPointer<vtkPointPicker> m_diameterMeasuringPointPicker;

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
    void InitSingleMode();
    void InitSegModelFactory(Util::SegmentActor * actor, std::string name,double color[]);
    void InitSegModels();
    bool GetOnlySegModelSource(Util::SegmentActor * actor);
    void InitMeasureMode();
    int GetNumOfCurrentSegmentModels(vtkActorCollection* collection);
    bool BindSlider(vtkActor* actor, Util::vtkSliderCallBack* callback,
                    vtkSliderWidget* widget);
    void InitSliders();
    void InitSliderFactory(vtkSliderRepresentation2D * rep, vtkSliderWidget * widget,
                           const char * text,double pos[]);
    void InitAnnotationFactory(vtkCornerAnnotation* annotation, const char * text, double color[]);
    void InitAnnotation();
    void InitInfoSphereFactory(vtkActor* sphere);
    void InitInfoSphere();
    void DrawSliceFactory(vtkSmartPointer<vtkRenderer> renderer
                              , vtkSmartPointer<vtkImageActor>imgActor
                              , double transformMat[16], double pos[3]);
    void DrawInfoSphereFactory(vtkSmartPointer<vtkRenderer> renderer
                               , vtkSmartPointer<vtkActor> sphereactor
                               ,double pos[3]);
    void InitCamerasWidgets();



};

#endif // ANEURYSMUNIT_H
