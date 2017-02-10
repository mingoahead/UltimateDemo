#ifndef TESTUNIT
#define TESTUNIT
#include "testimageslice.h"
#include "test_3views.h"
#include "test_vtkimagedata.h"
#include "test_imageslicing.h"
#include "test_itk_labelstatistics.h"
#include "test_itk_thinning3d.h"
#include "test_itk_levelset.h"
#include "test_itk_maskfilter.h"
#include "test_visualalgorithms.h"
#include "test_vtkwidget.h"
#include "test_vtkimageproc.h"
#include "test_itk_dfb.h"
#include "test_centerline.h"
#include "test_itk_gdcm.h"
class TestUnit
{
public:
    static void RunTest()
    {
//        Test_ImageSlice::RunTest();

//       Test_3Views *test3views = new Test_3Views();
//        test3views -> SetInputDicomSeriesDirectory("D:\\3dresearch\\TestData\\dicom");
//        test3views -> RunTest();
//        delete test3views;
        std::cout << "**************TEST PART BEGIN:*****************" << std::endl;
//        Test_vtkImageData::RunTest();

//        Test_Itk_LabelStatistics *test_itk_labelstat = new Test_Itk_LabelStatistics();
//        test_itk_labelstat -> RunTest();
//        delete test_itk_labelstat;

//       Test_Itk_3DThinning *test_itk_3dthinning = new Test_Itk_3DThinning();
//       test_itk_3dthinning -> RunTest();
//       delete test_itk_3dthinning;

//        Test_Itk_LevelSet *test_itk_levelset = new Test_Itk_LevelSet();
//        test_itk_levelset -> RunTest();
//        delete test_itk_levelset;

//        Test_Itk_MaskFilter *test_itk_maskfilter = new Test_Itk_MaskFilter();
//        test_itk_maskfilter -> RunTest();
//        delete test_itk_maskfilter;

//        Test_BiDimensionalWidget *test_bidimensionalwidget = new Test_BiDimensionalWidget();
//        test_bidimensionalwidget->RunTest();
//        delete test_bidimensionalwidget;

//        Test_VisualAlgorithms *test_visual = new Test_VisualAlgorithms();
//        test_visual->RunTest();
//        delete test_visual;

//        Test_VtkImageProcess *test_proc = new Test_VtkImageProcess();
//        test_proc->RunTest();
//        delete test_proc;

        Test_vtkWidget *test_widget = new Test_vtkWidget();
        test_widget->RunTest();
        delete test_widget;

//        Test_DFB *test_dfb = new Test_DFB();
//        test_dfb->RunTest();
//        delete test_dfb;

//        Test_CenterLine *test_cenline = new Test_CenterLine();
//        test_cenline->RunTest();
//        delete test_cenline;
//        Test_Itk_gdcm *test_gdcm = new Test_Itk_gdcm();
//        test_gdcm->RunTest();
//        delete test_gdcm;
        std::cout << "**************TEST PART END:*****************" << std::endl;

    }
};

#endif // TESTUNIT

