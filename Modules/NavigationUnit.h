#ifndef NAVIGATIONUNIT_H
#define NAVIGATIONUNIT_H

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include "../Utils/fastdef.h"

class NavigationUnit
{
public:
    NavigationUnit(vtkRenderWindow *renWin);
    void UpdatePosInfo();
    void InitMainModel();
    void UpdateView();


private:
    vtkRenderWindow * m_renWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkCornerAnnotation> m_annotation;

};

#endif // NAVIGATIONUNIT_H
