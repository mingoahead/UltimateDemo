#ifndef DATAADAPTOR_H
#define DATAADAPTOR_H

#include <vtkImageData.h>
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>

template <class ImagePixelType, size_t Dimension>
class DataAdaptor
{
    typedef itk::Image< ImagePixelType, Dimension > ImageType;
    static vtkImageData *GetVtkImageDataPointer(ImageType *img);
};

#endif // DATAADAPTOR_H
