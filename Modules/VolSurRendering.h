#ifndef VOLSURRENDERING_H
#define VOLSURRENDERING_H

#include "itkImageFileReader.h"
#include "itkMaskImageFilter.h"
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageShiftScale.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkSTLReader.h>

#include "fastdef.h"
#include "initials.h"

#include "ctkTransferFunction.h"
#include "ctkVTKColorTransferFunction.h"
#include "ctkTransferFunctionView.h"
#include "ctkTransferFunctionGradientItem.h"
#include "ctkTransferFunctionControlPointsItem.h"
#include "ctkVTKVolumePropertyWidget.h"
//class AuxiWidget {
//private:
//    static ctkVTKVolumePropertyWidget volumePropertywidget;
//public:
//    static ctkVTKVolumePropertyWidget& GetWidget()
//    {
//        return volumePropertywidget;
//    }
//};

class VolSurRendering
{
public:
    static int PrepareSurface(std::string StlName, vtkActor* Sur);
    static int PrepareVolume(std::string RawName, std::string MaskName,
                             vtkVolume* Vol, ctkVTKVolumePropertyWidget *widget);
    static void InstallSurface(vtkRenderer* renderer, vtkActor* Sur);
    static void InstallVolume(vtkRenderer* renderer, vtkVolume* Vol);

};

#endif // VOLSURRENDERING_H
