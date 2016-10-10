#ifndef TESTUNIT
#define TESTUNIT

#include "testdistancewidget.h"
#include "testcutter.h"
#include "testimageslice.h"
class TestUnit
{
public:
    static void RunTest()
    {
//        Test_distanceWidget::RunTest();
//        Test_Cutter::RunTest();
        Test_ImageSlice::RunTest();
    }
};

#endif // TESTUNIT

