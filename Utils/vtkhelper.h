#ifndef VTKHELPER
#define VTKHELPER

#include <vtkObjectFactory.h>
#include <vtkRenderingCoreModule.h>
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

#include <vtkCallbackCommand.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkImageTracerWidget.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkInteractorStyleImage.h>
#include <vtkBiDimensionalWidget.h>
#include <vtkBiDimensionalRepresentation2D.h>
#include <vtkOpenGLActor.h>

#include "QVTKWidget.h"

#include "fastdef.h"
/// index 1: vtkSliderCallBack (type Widget)
/// index 2: vtkIPWCallback    (type Widget)
/// index 3: CusInteractorPickPointStyle (type interactorstyle)
/// index 4: InteractionCallBackHandler  (type interactorstyle)
/// index 5: FreeRoamingInteractorStyle  (type interactorstyle)
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

class CusInteractorPickPointStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static CusInteractorPickPointStyle* New();
    vtkTypeMacro(CusInteractorPickPointStyle,vtkInteractorStyleTrackballCamera);

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
        std::cout << m_pbeg.x << ", " << m_pbeg.y << ", " << m_pbeg.z << std::endl;
        p1[0] = m_pbeg.x;
        p1[1] = m_pbeg.y;
        p1[2] = m_pbeg.z;

        std::cout << m_pend.x << ", " << m_pend.y << ", " << m_pend.z << std::endl;
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
        if(m_enabled){
            m_PickedPointsCnt %= 2;
            if(m_PickedPointsCnt == 0)
                SetVisiblityOff();
            int *pos ;
            pos  = this->Interactor->GetEventPosition();
            this->Interactor->GetPicker()->Pick(pos[0],
                                                pos[1],
                                                0,
                                                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
            std::cout << pos[0] << ", " << pos[1] << std::endl;
            double p[3];
            this->Interactor->GetPicker()->GetPickPosition(p);
            std::cout << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
            ++m_PickedPointsCnt;
            switch(m_PickedPointsCnt) {
            case 1: {
                m_pbeg = {p[0], p[1], p[2]};
                std::cout << "Begin Point : " << m_pbeg << std::endl;
                vtkSmartPointer<vtkSphereSource> spheresource =
                        vtkSmartPointer<vtkSphereSource>::New();
                spheresource->SetCenter(p);
                spheresource->SetRadius(5.0);
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
                std::cout << "End Point : " << m_pend << std::endl;
                vtkSmartPointer<vtkSphereSource> spheresource =
                        vtkSmartPointer<vtkSphereSource>::New();
                spheresource->SetCenter(p);
                spheresource->SetRadius(5.0);
                spheresource->Update();
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(spheresource->GetOutputPort());
                m_endPointActor->SetMapper(mapper);
                m_endPointActor->GetProperty()->SetColor(.0, .5, .5);
                m_renderer->AddActor(m_endPointActor);
                break;
            }
            }
        }
    }
};

class InteractionCallBackHandler : public vtkCommand {
public:
    static InteractionCallBackHandler *New()
    {
        return new InteractionCallBackHandler;
    }

    InteractionCallBackHandler()
    {
        m_lookupTable.clear();
        m_interactor = nullptr;
    }

    void Connect(vtkRenderer * renderer, vtkInteractorStyle * style)
    {
        m_lookupTable[ renderer ] = style;
    }

    void SetInteractor(vtkRenderWindowInteractor * interactor)
    {
        m_interactor = interactor;
    }

    vtkRenderWindowInteractor * GetInteractor()
    {
        return m_interactor;
    }

    vtkInteractorStyle * GetInteractorStyle(vtkRenderer * renderer)
    {
        return m_lookupTable[renderer];
    }
    void RemoveAll()
    {
        m_lookupTable.clear();
    }

    void Disconnect(vtkRenderer * renderer)
    {
        m_lookupTable.erase(renderer);
    }

    virtual void Execute(vtkObject *, unsigned long event, void *)
    {
        vtkRenderWindowInteractor *interactor = this->GetInteractor();

        if (interactor == nullptr) return ;

        int curPos[2];
        interactor->GetEventPosition(curPos);

        if (event == vtkCommand::MouseMoveEvent || event == QVTKWidget::DragMoveEvent) {
            vtkRenderer *renderer = interactor->FindPokedRenderer(curPos[0], curPos[1]);

            vtkInteractorStyle *style = m_lookupTable[renderer];

            if (style) {
                interactor->SetInteractorStyle(style);
            }
            vtkInteractorStyle *style2 = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
            if (style2) {
                style2->OnMouseMove();
            }
        }
    }

private:
    vtkRenderWindowInteractor * m_interactor;
    std::map< vtkRenderer*, vtkInteractorStyle* > m_lookupTable;
};

class FreeRoamingInteractorStyle :  public vtkInteractorStyleTrackballCamera
{
public:
    static FreeRoamingInteractorStyle * New()
    {
        return new FreeRoamingInteractorStyle;
    }

    vtkTypeMacro(FreeRoamingInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnKeyPress()
    {
        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        // Output the key that was pressed
        std::cout << "Pressed " << key << std::endl;

        double pos[3];
        double foc[3];
        double dir[3];
        m_camera->GetPosition(pos);
        m_camera->GetFocalPoint(foc);

        double d2 = 0;
        for (int i = 0; i < 3; ++i) {
            dir[i] = foc[i] - pos[i];
            d2 += dir[i] * dir[i];
        }
        d2 = sqrt(d2);
        for (int i = 0; i < 3; ++i) {
            dir[i] /= d2;
        }

        // view up
        if(key == "Up") {
            m_camera->Yaw(m_turningSpeed);
        }

        // view down
        if (key == "Down") {
            m_camera->Yaw(-m_turningSpeed);
        }

        // view left
        if (key == "Left") {
            m_camera->Elevation(m_turningSpeed);
        }

        // view right
        if (key == "Right") {
            m_camera->Elevation(-m_turningSpeed);
        }

        // move foward
        if(key == "h") {
            for (int i = 0; i < 3; ++i) {
                pos[i] += m_movingSpeed * dir[i];
                foc[i] += m_movingSpeed * dir[i];
            }
        }

        // move backward
        if (key == "j") {
            for (int i = 0; i < 3; ++i) {
                pos[i] -= m_movingSpeed * dir[i];
                foc[i] -= m_movingSpeed * dir[i];
            }
        }

        // move left
        if (key == "g") {
            //
        }

        // move right
        if (key == "k") {
            //
        }

        m_camera->SetPosition(pos);
        m_camera->SetFocalPoint(foc);

        // Forward events
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    void SetRoamingCamera(vtkCamera * cam) { m_camera = cam; }

    void SetMovingSpeed(double movingSpeed) { m_movingSpeed = movingSpeed; }

    void SetTurningSpeed(double turningSpeed) { m_turningSpeed = turningSpeed; }

private:
    vtkCamera * m_camera;

    double m_movingSpeed; // distance
    double m_turningSpeed; // angle

};

class SegmentActor : public vtkOpenGLActor {
public:
    static SegmentActor* New();
    vtkTypeMacro(SegmentActor,vtkOpenGLActor);
    void SetSegmentType(std::string name)  {   this->type = name;  }
    std::string GetSegmentType()  {  return this->type;  }
    void SetSegmentPath(std::string path) { this->path = path;  }
    std::string GetSegmentPath()  {  return this->path;  }
private:
    std::string type;
    std::string path;
};

class vtkImageTracerCallback : public vtkCommand
{
  public:
    static vtkImageTracerCallback *New()
    {
      return new vtkImageTracerCallback;
    }

    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkImageTracerWidget* tracerWidget =
            static_cast<vtkImageTracerWidget*>(caller);

          vtkSmartPointer<vtkPolyData> path =
            vtkSmartPointer<vtkPolyData>::New();

          tracerWidget->GetPath(path);
          std::cout << "There are " << path->GetNumberOfPoints()
                    << " points in the path." << std::endl;

    }
    vtkImageTracerCallback(){}

};
}
#endif // VTKHELPER

