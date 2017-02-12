#ifndef NAVIGATIONUNIT_H
#define NAVIGATIONUNIT_H

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include <vtkSTLReader.h>
#include <vtkAlgorithmOutput.h>
#include "fastdef.h"
#include "vtkhelper.h"

class NavigationUnit
{
public:
    NavigationUnit(vtkRenderWindow *renWin);
    vtkRenderWindow * m_renWindow;

    void InitPosInfo();
    void UpdatePosInfo(double *pos);
    void InitMainModel();
    void UpdateMainModel(std::string path);
    void InitPathLine();
    void UpdatePathLine();
    void UpdateTotalView(std::string path, double *pos);

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkCornerAnnotation> m_annotation;
    vtkSmartPointer<vtkActor> m_mainModel;
    vtkSmartPointer<vtkActor> m_movingModel;

};

#endif // NAVIGATIONUNIT_H
