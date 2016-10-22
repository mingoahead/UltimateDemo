#include "NavigationUnit.h"

NavigationUnit::NavigationUnit(vtkRenderWindow *renWin)
    :m_renWindow(renWin)
{
    vsp(m_renderer);
    vsp(m_annotation);
    m_annotation->SetLinearFontScaleFactor( 2 );
    m_annotation->SetNonlinearFontScaleFactor( 1 );
    m_annotation->SetMaximumFontSize( 15 );
    m_annotation->SetText(1 ,"current position:\n(.0, .0, .0)");
    m_annotation->GetTextProperty()->SetColor( 1, 0, 0 );
    m_renderer->AddViewProp(m_annotation);
    m_renderer->SetBackground(0.1, 0.2, 0.4);
    m_renWindow->AddRenderer(m_renderer);
    m_renWindow->Render();
}

void NavigationUnit::UpdatePosInfo()
{

}

void NavigationUnit::InitMainModel()
{

}

void NavigationUnit::UpdateView()
{

}
