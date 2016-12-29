#include "VolumeRendering.h"
#include "itkExceptionObject.h"
VolumeRendering::VolumeRendering(vtkRenderWindow *renWin)
    :m_renderWindow(renWin)
{
    m_rawdataReader = ReaderType::New();
    m_maskdataReader = ReaderType::New();
    vsp(m_renderer);
    m_renderer->SetBackground(.1, .2, .3);
    m_renderer->SetViewport(0, 0, 0.5, 1);
    vsp(m_volumedata);
    vsp(m_volumeproperty);
    vsp(m_opacityTransferFunction);
    vsp(m_gradientTransferFunction);
    vsp(m_colorTransferFunction);
    vsp(m_compositeRaycastFunction);
    vsp(m_volumemapper);
    m_ctkwidget = new ctkVTKVolumePropertyWidget;

    m_ctkwidget->setVolumeProperty(m_volumeproperty);
//    m_renderer->AddVolume(m_volumedata);

}
VolumeRendering::~VolumeRendering()
{
    delete m_ctkwidget;
}
int VolumeRendering::LoadRawData(std::string filename)
{
    m_rawdataReader->SetFileName(filename);
    try{
        m_rawdataReader->Update();
    }catch(itk::ExceptionObject &ex){
        ex.Print(std::cerr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int VolumeRendering::LoadMaskData(std::string maskname)
{
    m_maskdataReader->SetFileName(maskname);
    try{
        m_maskdataReader->Update();
    }catch(itk::ExceptionObject &ex){
        ex.Print(std::cerr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void VolumeRendering::LoadImageData()
{
    if(m_maskdataReader->GetFileName().empty())     return ;
    if(m_rawdataReader->GetFileName().empty())  return ;

//    ImageType::Pointer maskImage = m_maskdataReader->GetOutput();
//    ImageType::Pointer rawImage = m_rawdataReader->GetOutput();
//    std::cout << maskImage->GetSpacing() << std::endl;
//    std::cout << rawImage->GetSpacing() << std::endl;
    try{
        typedef itk::MaskImageFilter<ImageType, ImageType> MaskFilterType;
        MaskFilterType::Pointer maskfilter = MaskFilterType::New();
        maskfilter->SetInput(m_rawdataReader->GetOutput());
        maskfilter->SetMaskImage(m_maskdataReader->GetOutput());
        maskfilter->Update();
        typedef itk::ImageToVTKImageFilter <ImageType> ConnectorType;
        ConnectorType::Pointer connector1 = ConnectorType::New();
        connector1->SetInput(maskfilter->GetOutput());
        connector1->Update();
        vtkSmartPointer<vtkImageShiftScale> scale
                = vtkSmartPointer<vtkImageShiftScale>::New();
        scale -> SetInputData(connector1->GetOutput());
        scale -> SetOutputScalarTypeToUnsignedChar();
        scale -> Update();
        m_opacityTransferFunction->AddPoint(50.0, 0.0);
        m_opacityTransferFunction->AddPoint(120.0, 0.5);
        m_opacityTransferFunction->AddSegment(150.0, 0.6, 180.0, .7);
        m_opacityTransferFunction->AddPoint(220.0, 0.0);
        m_gradientTransferFunction->AddPoint(50.0, 2.0);
        m_gradientTransferFunction->AddPoint(120.0, 0.5);
        m_gradientTransferFunction->AddSegment(150.0,0.73, 180.0, 0.9);
        m_gradientTransferFunction->AddPoint(220.0, 2.0);
        m_colorTransferFunction->AddRGBPoint(40.0, 0.1, 0.1, 0.7);
        m_colorTransferFunction->AddRGBPoint(70.0, 0.1, 0.1, 0/7);
        m_colorTransferFunction->AddRGBPoint(160.0,1.0,0.35,0.0);
        m_colorTransferFunction->AddRGBPoint(190.0, 0.99, 0.99, 0.99);

        m_volumeproperty->SetColor(m_colorTransferFunction);
        m_volumeproperty->SetScalarOpacity(m_opacityTransferFunction);
        m_volumeproperty->SetGradientOpacity(m_gradientTransferFunction);
        m_volumeproperty->ShadeOn();
        m_volumeproperty->SetAmbient(0.25);
        m_volumeproperty->SetDiffuse(0.9);
        m_volumeproperty->SetSpecular(0.2);
        m_volumeproperty->SetSpecularPower(20);
        m_volumeproperty->SetInterpolationTypeToLinear();

        m_volumemapper->SetRequestedRenderMode(vtkSmartVolumeMapper::GPURenderMode);
        m_volumemapper->SetMaxMemoryInBytes(1847483648);
        m_volumemapper->SetInputConnection(scale->GetOutputPort());
//        m_volumemapper->SetVolumeRayCastFunction(m_compositeRaycastFunction);
        m_volumedata->SetMapper(m_volumemapper);
        m_volumedata->SetProperty(m_volumeproperty);
        m_volumedata->Update();
        m_renderer->AddVolume(m_volumedata);

        m_renderer->ResetCamera();
    }catch(itk::ExceptionObject &err) {
        std::cerr << err.what() << std::endl;
        return ;
    }

//    m_renderWindow->Render();

}

vtkRenderer *VolumeRendering::GetRenderer()
{
    return m_renderer;
}

ctkVTKVolumePropertyWidget *VolumeRendering::GetPropertyWidget()
{
    return m_ctkwidget;
}
