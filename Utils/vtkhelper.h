#ifndef VTKHELPER
#define VTKHELPER

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
namespace Util {

class vtkSliderCallBack : public vtkCommand {
public:
    static vtkSliderCallBack *New() { return new vtkSliderCallBack; }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkSliderWidget *sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);
        if(this->actor) {
            this->actor->GetProperty()->SetOpacity(
                        static_cast<vtkSliderRepresentation*>(
                            sliderWidget->GetRepresentation()) -> GetValue());

        }
    }
    vtkSliderCallBack() : actor(0) {};
    bool SetBoundActor(vtkActor *giveactor)
    {
        actor = giveactor;
        if(!actor) {
            return false;
        }
        return true;
    }
    vtkActor *actor;

};

// Callback for the interaction
// This does the actual work: updates the vtkPlane implicit function.
// This in turn causes the pipeline to update and clip the object.
class vtkIPWCallback : public vtkCommand
{
public:
    static vtkIPWCallback *New()
    {
        return new vtkIPWCallback;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkImplicitPlaneWidget2 *planeWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
        vtkImplicitPlaneRepresentation *rep = reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
        rep->GetPlane(this->Plane);
    }
    vtkIPWCallback():Plane(0),Actor(0) {}
    vtkPlane *Plane;
    vtkActor *Actor;
};

class CusInteractorStylePickPoint : public vtkInteractorStyleTrackballCamera
{
public:
    static CusInteractorStylePickPoint* New();
    vtkTypeMacro(CusInteractorStylePickPoint,vtkInteractorStyleTrackballCamera);

    void PreparedRenderer( vtkRenderer *renderer)
    {
        m_renderer = renderer;
        m_beginPointActor = vtkSmartPointer<vtkActor>::New();
        m_endPointActor = vtkSmartPointer<vtkActor>::New();

        SetVisiblityOn();
    }

    void SetPickerEnabled(bool enabled = true)
    {
        m_enabled = enabled;
        m_PickedPointsCnt = 0;
    }

    bool GetPickerEnabled()
    {
        return m_enabled;
    }
    void GetMarkedPoints(double p1[3],double p2[3])
    {
        p1[0] = m_pbeg.x;
        p1[1] = m_pbeg.y;
        p1[2] = m_pbeg.z;

        p2[0] = m_pend.x;
        p2[1] = m_pend.y;
        p2[2] = m_pend.z;
    }
    virtual void OnMiddleButtonDown()
    {
        ProcessPick();
        vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }


private:
    bool m_enabled;
    int m_PickedPointsCnt;
    Point3f m_pbeg;
    Point3f m_pend;

    vtkRenderer * m_renderer;
    vtkSmartPointer<vtkActor> m_beginPointActor;
    vtkSmartPointer<vtkActor> m_endPointActor;
    void SetVisiblityOff()
    {
        m_renderer->RemoveActor(m_beginPointActor);
        m_renderer->RemoveActor(m_endPointActor);
    }
    void SetVisiblityOn()
    {
        m_renderer->AddActor(m_beginPointActor);
        m_renderer->AddActor(m_endPointActor);
    }
    void ProcessPick()
    {
        int   state = m_enabled == true ? 1 : 0;
        std::cout << "begin picking point and m_enabled is now " << state<< std::endl;
        if(m_enabled){
            m_PickedPointsCnt %= 2;
            if(m_PickedPointsCnt == 0)
                SetVisiblityOff();
            this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                                                this->Interactor->GetEventPosition()[1],
                                                0,
                                                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
            double p[3];
            this->Interactor->GetPicker()->GetPickPosition(p);
            ++m_PickedPointsCnt;
            switch(m_PickedPointsCnt) {
            case 1: {
                m_pbeg = {p[0], p[1], p[2]};
                vtkSmartPointer<vtkSphereSource> spheresource =
                        vtkSmartPointer<vtkSphereSource>::New();
                spheresource->SetCenter(p);
                spheresource->SetRadius(1.0);
                spheresource->Update();
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(spheresource->GetOutputPort());
                m_beginPointActor->SetMapper(mapper);
                m_beginPointActor->GetProperty()->SetColor(.0, .5, .5);
                m_renderer->AddActor(m_beginPointActor);
                break;

            }
            case 2: {
                m_pend = {p[0], p[1], p[2]};
                vtkSmartPointer<vtkSphereSource> spheresource =
                        vtkSmartPointer<vtkSphereSource>::New();
                spheresource->SetCenter(p);
                spheresource->SetRadius(1.0);
                spheresource->Update();
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(spheresource->GetOutputPort());
                m_endPointActor->SetMapper(mapper);
                m_endPointActor->GetProperty()->SetColor(.0, .5, .5);
                m_renderer->AddActor(m_endPointActor);

            }
            }
        }
    }
};
}
#endif // VTKHELPER

