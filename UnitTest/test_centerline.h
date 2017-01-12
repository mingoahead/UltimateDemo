#ifndef TEST_CENTERLINE
#define TEST_CENTERLINE
///medial curve mtthod
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkGradientImageFilter.h>
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include <itkThresholdImageFilter.h>
using namespace std;
class Test_CenterLine{
public:
    int RunTest()
    {
        string input_name = "D://3dresearch//TestData//stl//09//122802.mhd";
        string output_name = "D://3dresearch//TestData//stl//09//cl_122802.mhd";
        return 0;
    }
private:

};
#endif // TEST_CENTERLINE

