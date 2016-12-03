#ifndef VOLUMERENDERING_H
#define VOLUMERENDERING_H
#include "itkImageFileReader.h"
#include "itkMaskImageFilter.h"
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
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
const unsigned int Dimension = 3;
typedef signed short PixelType;
typedef itk::Image<PixelType, Dimension > ImageType;
// Read image
typedef itk::ImageFileReader< ImageType > ReaderType;
class VolumeRendering
{
public:
    VolumeRendering(vtkRenderWindow * renWin);
    ~VolumeRendering();
    int LoadRawData(std::string filename);
    int LoadMaskData(std::string maskname);
    void LoadImageData();
    vtkRenderer * GetRenderer();
    ctkVTKVolumePropertyWidget *GetPropertyWidget();

private:

    vtkRenderWindow * m_renderWindow;
    ctkVTKVolumePropertyWidget *m_ctkwidget;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkVolume> m_volumedata;
    vtkSmartPointer<vtkPiecewiseFunction> m_opacityTransferFunction;
    vtkSmartPointer<vtkColorTransferFunction> m_colorTransferFunction;
    vtkSmartPointer<vtkVolumeProperty> m_volumeproperty;
    vtkSmartPointer<vtkSmartVolumeMapper> m_volumemapper;

    ReaderType::Pointer m_rawdataReader;
    ReaderType::Pointer m_maskdataReader;


};

#endif // VOLUMERENDERING_H
