#include "SurfaceRendering.h"
#include "Utils/vtkhelper.h"
SurfaceRendering::SurfaceRendering(vtkRenderWindow *renWin)
    :m_renderWindow(renWin)
{
    vsp(m_renderer);
    m_renderer->SetBackground(.4, .4, .5);
    m_renderer->SetViewport(0.5,0, 1, 1);
    vsp(m_metaImage);
    vsp(m_MCFilter);
    vsp(m_smoothFilter);
    vsp(m_stlWriter);
    vsp(m_stlMapper);
    vsp(m_stlActor);
    vsp(m_inputStlData);
    m_MCFilter->SetInputConnection(m_metaImage->GetOutputPort());
    m_MCFilter->ComputeNormalsOn();
    m_MCFilter->SetValue(0, 1);

    m_smoothFilter->SetRelaxationFactor(0.1);
    m_smoothFilter->FeatureEdgeSmoothingOff();
    m_smoothFilter->BoundarySmoothingOn();

    m_stlMapper->SetInputConnection(m_smoothFilter->GetOutputPort());
    m_stlMapper->ScalarVisibilityOff();
    m_stlActor->SetMapper(m_stlMapper);
    m_stlWriter->SetInputConnection(m_smoothFilter->GetOutputPort());
}

void SurfaceRendering::LoadImageData(std::string filename)
{
    if (Util::Dirs::StringEndsWith(filename, ".stl")) {
        try {
            m_inputStlData->SetFileName(filename.c_str());
            m_inputStlData->Update();

            m_smoothFilter->SetInputConnection(m_inputStlData->GetOutputPort());
        } catch (...) {
            std::cerr << "stl read failed." << std::endl;
        }
    } else if(Util::Dirs::StringEndsWith(filename, ".mhd") || Util::Dirs::StringEndsWith(filename, ".mha")) {
        m_metaImage->SetFileName(filename.c_str());
        m_metaImage->UpdateWholeExtent();
        m_smoothFilter->SetInputConnection(m_MCFilter->GetOutputPort());
    }
    Update();

    m_renderer->AddActor(m_stlActor);
    m_renderer->ResetCamera();
}

void SurfaceRendering::SetIterations(unsigned int iterNums)
{
    m_smoothFilter->SetNumberOfIterations(iterNums);
    m_smoothFilter->Update();
}

void SurfaceRendering::SetRelaxationFactor(double value)
{
    m_smoothFilter->SetRelaxationFactor(value);
    m_smoothFilter->Update();
}

void SurfaceRendering::SetValue(double value)
{
    m_MCFilter->SetValue(0, value);
    m_MCFilter->Update();
}

void SurfaceRendering::Update()
{
    m_smoothFilter->UpdateWholeExtent();
    m_smoothFilter->Update();

    m_stlMapper->UpdateWholeExtent();
}

void SurfaceRendering::WriteStl(std::string stlFilename)
{
    m_stlWriter->SetFileTypeToBinary();
    m_stlWriter->SetFileName(stlFilename.c_str());
    m_stlWriter->Write();
}

vtkRenderer * SurfaceRendering::GetRenderer()
{
    return m_renderer;
}

void SurfaceRendering::SetFreeRoamingMode(bool on)
{
    if (on) {
        vtkSmartPointer<Util::FreeRoamingInteractorStyle> style = vtkSmartPointer<Util::FreeRoamingInteractorStyle>::New();
        style->SetRoamingCamera(GetRenderer()->GetActiveCamera());
        style->SetMovingSpeed(10);
        style->SetTurningSpeed(5.0);

        m_renderWindow->GetInteractor()->SetInteractorStyle(style);
        style->SetCurrentRenderer(m_renderer);
    } else {
        //
    }
}




