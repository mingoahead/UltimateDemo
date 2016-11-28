#include "VolSurRendering.h"


int VolSurRendering::PrepareSurface(std::string StlName, vtkActor *Sur)
{
    vtkSmartPointer<vtkSTLReader> reader3 =
                    vtkSmartPointer<vtkSTLReader>::New();
    reader3->SetFileName(StlName.c_str());
    try{
        reader3->Update();
    }catch(...) {
        std::cout << "Uninvalid stl name : " << StlName << std::endl;
        return EXIT_FAILURE;
    }
    reader3->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapper3 =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper3->SetInputData(reader3->GetOutput());
    mapper3->Update();
    Sur->SetMapper(mapper3);
    Sur->GetProperty()->SetColor(0.4, 0.4, 0.5);
    return EXIT_SUCCESS;
}

int VolSurRendering::PrepareVolume(std::string RawName, std::string MaskName,
                                   vtkVolume *Vol, ctkVTKVolumePropertyWidget *widget)
{
    const unsigned int Dimension = 3;
    typedef signed short PixelType;
    typedef itk::Image<PixelType, Dimension > ImageType;
    // Read image
    typedef itk::ImageFileReader< ImageType > ReaderType;
    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(RawName);
    try{
        reader1->Update();
    }catch (itk::ExceptionObject &ex) {
        std::cout << ex << std::endl;
        return EXIT_FAILURE;
    }
    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(MaskName);
    try{
        reader2->Update();
    }catch (itk::ExceptionObject &ex) {
        std::cout << ex << std::endl;
        return EXIT_FAILURE;
    }
    typedef itk::MaskImageFilter<ImageType, ImageType> MaskFilterType;
    MaskFilterType::Pointer maskfilter = MaskFilterType::New();
    maskfilter->SetInput(reader1->GetOutput());
    maskfilter->SetMaskImage(reader2->GetOutput());
//        maskfilter->Update();

    typedef itk::ImageToVTKImageFilter <ImageType> ConnectorType;
    ConnectorType::Pointer connector1 = ConnectorType::New();
    connector1->SetInput(maskfilter->GetOutput());
    connector1->Update();

    vtkSmartPointer<vtkImageShiftScale> scale
            = vtkSmartPointer<vtkImageShiftScale>::New();
    scale -> SetInputData(connector1->GetOutput());
    scale -> SetOutputScalarTypeToUnsignedChar();
    scale -> Update();

    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction
            = vtkSmartPointer<vtkPiecewiseFunction>::New();
//        opacityTransferFunction->AddPoint(0.0, 0.0);
//        opacityTransferFunction->AddPoint(36.0, 0.125);
//        opacityTransferFunction->AddPoint(72.0, 0.25);
//        opacityTransferFunction->AddPoint(108.0, 0.375);
//        opacityTransferFunction->AddPoint(144.0, 0.5);
//        opacityTransferFunction->AddPoint(180.0, 0.625);
//        opacityTransferFunction->AddPoint(216.0, 0.75);
//        opacityTransferFunction->AddPoint(255.0, 0.875);
    opacityTransferFunction->AddPoint(50.0, 0.0);
    opacityTransferFunction->AddPoint(120.0, 0.5);
    opacityTransferFunction->AddPoint(220.0, 0.0);

    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction
            = vtkSmartPointer<vtkColorTransferFunction>::New();
//        colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
//        colorTransferFunction->AddRGBPoint(36.0, 1.0, 0.0, 0.0);
//        colorTransferFunction->AddRGBPoint(72.0, 1.0, 1.0, 0.0);
//        colorTransferFunction->AddRGBPoint(108.0, 0.0, 1.0, 0.0);
//        colorTransferFunction->AddRGBPoint(144.0, 0.0, 1.0, 1.0);
//        colorTransferFunction->AddRGBPoint(180.0, 0.0, 0.0, 1.0);
//        colorTransferFunction->AddRGBPoint(216.0, 1.0, 0.0, 1.0);
//        colorTransferFunction->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(40.0, 0.1, 0.1, 0.7);
    colorTransferFunction->AddRGBPoint(70.0, 0.1, 0.1, 0/7);
    colorTransferFunction->AddRGBPoint(160.0,1.0,0.35,0.0);
    colorTransferFunction->AddRGBPoint(190.0, 0.99, 0.99, 0.99);

    vtkSmartPointer<vtkVolumeProperty> volumeProperty
            = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.25);
    volumeProperty->SetDiffuse(0.9);
    volumeProperty->SetSpecular(0.2);
    volumeProperty->SetSpecularPower(20);
    volumeProperty->SetInterpolationTypeToLinear();
    // add control widget
    widget->setVolumeProperty(volumeProperty);

    vtkSmartPointer<vtkSmartVolumeMapper> smartMapper
            = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    smartMapper->SetMaxMemoryInBytes(1847483648);
    smartMapper->SetRequestedRenderMode(vtkSmartVolumeMapper::GPURenderMode);
    smartMapper->SetInputData(scale->GetOutput());

    Vol->SetMapper(smartMapper);
    Vol->SetProperty(volumeProperty);
    return EXIT_SUCCESS;
}

void VolSurRendering::InstallSurface(vtkRenderer *renderer, vtkActor *Sur)
{
    if(renderer == NULL || Sur == NULL) {
        return ;
    }
    renderer->AddActor(Sur);
}

void VolSurRendering::InstallVolume(vtkRenderer *renderer, vtkVolume *Vol)
{
    if(renderer == NULL || Vol == NULL) {
        return ;
    }
    renderer->AddVolume(Vol);
}
