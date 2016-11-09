#ifndef TESTUNIT
#define TESTUNIT

#include "testdistancewidget.h"
#include "testcutter.h"
#include "testimageslice.h"
#include "testextractpolylines.h"
#include "testimageplanewidget.h"
#include "test_3views.h"
#include "test_scalarbaractor.h"
#include "test_cameraactor.h"
#include "test_camerawidget.h"
#include "test_ballonwidget.h"
class TestUnit
{
public:
    static void RunTest()
    {
//        Test_distanceWidget::RunTest();
//        Test_Cutter::RunTest();
//        Test_ImageSlice::RunTest();
//        Test_ExtractPolyLine::RunTest();
//        Test_ImagePlaneWidget::RunTest();

//        Test_3Views *test3views = new Test_3Views();
//        test3views -> SetInputDicomSeriesDirectory("D:\\3dresearch\\TestData\\dicom");
//        test3views -> RunTest();
//        delete test3views;
//        Test_ScalarBarActor::RunTest();
//        Test_CameraActor::RunTest();
//        Test_CameraWidget::RunTest();
        Test_BalloonWidget::RunTest();
    }
};

#endif // TESTUNIT

