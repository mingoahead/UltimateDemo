#ifndef SURFACERENDERING_H
#define SURFACERENDERING_H

# include <vtkSmartPointer.h>
# include <vtkMetaImageReader.h>
# include <vtkMarchingCubes.h>
# include <vtkSTLWriter.h>
# include <vtkSmoothPolyDataFilter.h>
# include <vtkPolyDataMapper.h>
# include <vtkRenderWindow.h>
# include <vtkRenderer.h>
# include <vtkActor.h>
# include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
# include <string>

#include "fastdef.h"
#include "dirhelper.h"

class SurfaceRendering
{
public:
    SurfaceRendering(vtkRenderWindow * renWin);

    void LoadImageData(std::string filename);

    void SetIterations(unsigned int iterNums);
    void SetRelaxationFactor(double value);
    void SetValue(double value);

    void Update();

    void WriteStl(std::string filename);

    vtkRenderer * GetRenderer();

    void SetFreeRoamingMode(bool on);

private:
    vtkRenderWindow * m_renderWindow;

    vtkSmartPointer<vtkMetaImageReader> m_metaImage;
    vtkSmartPointer<vtkMarchingCubes> m_MCFilter;
    vtkSmartPointer<vtkPolyDataMapper> m_stlMapper;
    vtkSmartPointer<vtkSTLWriter> m_stlWriter;
    vtkSmartPointer<vtkSmoothPolyDataFilter> m_smoothFilter;
    vtkSmartPointer<vtkActor> m_stlActor;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkSTLReader> m_inputStlData;

};

#endif // SURFACERENDERING_H
