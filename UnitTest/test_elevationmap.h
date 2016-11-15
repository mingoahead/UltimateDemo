#ifndef TEST_ELEVATIONMAP
#define TEST_ELEVATIONMAP
#include <itkBinaryImageToLevelSetImageAdaptor.h>
#include <itkImageFileReader.h>
#include <itkLevelSetContainer.h>
#include <itkLevelSetEquationChanAndVeseInternalTerm.h>
#include <itkLevelSetEquationChanAndVeseExternalTerm.h>
#include <itkLevelSetEquationContainer.h>
#include <itkLevelSetEquationTermContainer.h>
#include <itkLevelSetEvolutionNumberOfIterationsStoppingCriterion.h>
#include <itkLevelSetDenseImage.h>
#include <itkSinRegularizedHeavisideStepFunction.h>

#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <vtkRenderWindowInteractor.h>

#endif // TEST_ELEVATIONMAP

