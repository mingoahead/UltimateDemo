#include "dataadaptor.h"
#include <itkImageToVTKImageFilter.h>

template <class ImagePixelType, size_t Dimension>
vtkImageData*
DataAdaptor<ImagePixelType, Dimension>
::GetVtkImageDataPointer(ImageType *img)
{
    typedef itk::ImageToVTKImageFilter< ImageType > AdaptorFilterType;
    typedef typename AdaptorFilterType::Pointer AdaptorFilterPointer;

    try {
        AdaptorFilterPointer adaptorFilter = AdaptorFilterType::New();
        adaptorFilter->GetImporter()->SetDataScalarTypeToUnsignedChar();
        adaptorFilter->SetInput(img);
        adaptorFilter->Update();
        return adaptorFilter->GetOutput();
    } catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
