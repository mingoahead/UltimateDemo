#ifndef CENLINEUNIT_H
#define CENLINEUNIT_H
//itk includes
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkDanielssonDistanceMapImageFilter.h>
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>
#include <itkUnaryFunctorImageFilter.h>
#include <itkPowImageFilter.h>
#include <itkSquareImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageSliceConstIteratorWithIndex.h>
#include <itkGradientMagnitudeImageFilter.h>
#include <itkInvertIntensityImageFilter.h>
#include <itkThresholdImageFilter.h>
#include <itkTimeProbe.h>
#include <itkMath.h>
#include <itkNumericTraits.h>
#include <itkPolyLineParametricPath.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkGradientDescentOptimizer.h>
#include <itkRegularStepGradientDescentOptimizer.h>
#include <itkPathIterator.h>
#include <itkNeighborhoodIterator.h>
#include <itkNearestNeighborInterpolateImageFunction.h>

//appendix func includes
#include "Modules/CenLine_1/itkArrivalFunctionToPathFilter.h"
#include "Modules/CenLine_1/itkSpeedFunctionToPathFilter.h"

#include <vnl/vnl_math.h>
#include <itkPoint.h>
//vtk includes
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
//general includes
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "initials.h"
#include "fastdef.h"

const unsigned int dimension = 3;
typedef unsigned char                                              InputPixelType;
typedef unsigned int                                               OutputPixelType;
typedef itk::Image< InputPixelType,dimension >                     InputImageType;
typedef itk::Image< OutputPixelType,dimension >                    OutputImageType;

class CenLineUnit
{
public:
    CenLineUnit();
    int GetCenterLinePointNums(int id);
    void GetCenterLinePoint(int id, int index, double p[3]);
    // method 1
    // ***********************************************************************
    int Path_GradientDescent(std::string filename, double ps[3], double pe[3]);
    std::vector<Point3f> CenterPoints1;
    std::vector<Point3f> CenterPoints2;
    // ***********************************************************************
private:
    std::vector<OutputImageType::IndexType> localMinVec;
    std::vector<OutputImageType::PointType> MinPoints;
    void clear()
    {
        localMinVec.clear();
    }
};

#endif // CENLINEUNIT_H
