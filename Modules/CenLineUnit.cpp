#include "CenLineUnit.h"

CenLineUnit::CenLineUnit()
{
    clear();
}

int CenLineUnit::GetCenterLinePointNums(int id)
{
    return (id == 1) ? CenterPoints1.size() : CenterPoints2.size();
}

void CenLineUnit::GetCenterLinePoint(int id, int index, double p[3])
{
    Point3f tmp = {.0, .0, .0};
    if(id == 1) {
        if(0 <= index && index < CenterPoints1.size()) {
            tmp = CenterPoints1[index];
        }
    }else {
        if(0 <= index && index < CenterPoints2.size()) {
            tmp = CenterPoints2[index];
        }
    }
    p[0] = tmp.x;
    p[1] = tmp.y;
    p[2] = tmp.z;
}
/**
 * @brief CenLineUnit::Path_GradientDescent
 * @param filename : binary 3D image name
 * @param ps : Set an given point
 * @param pe : Set an given point
 * @return : true or false
 */

int CenLineUnit::Path_GradientDescent(std::string filename, double ps[], double pe[])
{
    if(filename.empty())    return EXIT_FAILURE;
    CenterPoints1.clear();
    CenterPoints2.clear();//remove last centerline results;
    const unsigned int Dimension = 3;
    typedef unsigned char InputPixelType;
    typedef float InterPixelType;
    typedef unsigned char OutputPixelType;
    typedef itk::Image< InputPixelType, Dimension >                       InputImageType;
    typedef itk::Image< InterPixelType, Dimension >                       InterImageType;
    typedef itk::Image< OutputPixelType, Dimension>                       OutputImageType;
    typedef itk::ImageFileReader< InputImageType >                        ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >                       WriterType;
    typedef itk::PolyLineParametricPath< Dimension >                      PathType;
    typedef itk::SpeedFunctionToPathFilter< InterImageType,PathType >     PathFilterType;
    typedef PathFilterType::CostFunctionType::CoordRepType       CoordRepType;
    typedef itk::PathIterator< OutputImageType, PathType >                PathIteratorType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    reader->Update();
//    // step1: Distance transform
//    typedef itk::SignedMaurerDistanceMapImageFilter<
//            InputImageType,
//            InterImageType> FilterType;
//    FilterType::Pointer dft = FilterType::New();
//    dft->SetInput(reader->GetOutput());
//    // step2: Rescale Distance transform to [0,1]
//    typedef itk::RescaleIntensityImageFilter<
//            InterImageType,InterImageType > RescalerType;
//    RescalerType::Pointer scaler = RescalerType::New();
//    scaler->SetInput(dft->GetOutput());
//    scaler->SetOutputMinimum(.0);
//    scaler->SetOutputMaximum(1.);
//    // step3: reverse intensity value to [1,0]
//    typedef itk::InvertIntensityImageFilter<
//            InterImageType,InterImageType> InverterType;
//    InverterType::Pointer inverter = InverterType::New();
//    inverter->SetInput(scaler->GetOutput());
//    inverter->SetMaximum(1.0);
// //    inverter->Update();
//   // step4: general threshold(only keep pixelvalue >= 0.9)
//    typedef itk::ThresholdImageFilter<
//            InterImageType> ThresholdType;
//    ThresholdType::Pointer thresholder = ThresholdType::New();
//    thresholder->SetInput(inverter->GetOutput());
//    thresholder->ThresholdBelow(0.95);
//    thresholder->SetOutsideValue(0.0);
//    thresholder->Update();
//    // get speed function(DFT --->  Rescale --->  reverse  --->  threshold)
//    InterImageType::Pointer speed = thresholder->GetOutput();

    typedef itk::SignedMaurerDistanceMapImageFilter<
            InputImageType,InterImageType> FilterType;
    FilterType::Pointer dft = FilterType::New();
    dft->SetInput(reader->GetOutput());
    dft->SetUseImageSpacing(true);
    dft->SetSquaredDistance(false);
    dft->SetInsideIsPositive(true);
    typedef itk::SmoothingRecursiveGaussianImageFilter<
            InterImageType,InterImageType> SmoothType;
    SmoothType::Pointer smoother = SmoothType::New();
    smoother->SetInput(dft->GetOutput());
    smoother->SetSigma(1.0);
    typedef itk::ThresholdImageFilter<
            InterImageType> ThresholdType;
    ThresholdType::Pointer thresholder = ThresholdType::New();
    thresholder->SetInput(smoother->GetOutput());
    thresholder->ThresholdBelow(-2);
    thresholder->ThresholdAbove(25);
    thresholder->SetOutsideValue(-3);
    thresholder->Update();
    typedef itk::RescaleIntensityImageFilter<
            InterImageType,InterImageType> RescaleType;
    RescaleType::Pointer scaler = RescaleType::New();
    scaler->SetInput(thresholder->GetOutput());
    scaler->SetOutputMinimum(0.0);
    scaler->SetOutputMaximum(1.0);
//    typedef itk::PowImageFilter<InterImageType,
//            InputImageType,
//            InterImageType> PowImageType;
//    PowImageType::Pointer power = PowImageType::New();
//    power->SetInput1(scaler->GetOutput());
//    power->SetConstant2(10);
//    power->Update();
    typedef itk::SquareImageFilter<
            InterImageType, InterImageType> SquareType;
    SquareType::Pointer square1 = SquareType::New();
    square1->SetInput(scaler->GetOutput());
    SquareType::Pointer square2 = SquareType::New();
    square2->SetInput(square1->GetOutput());
    SquareType::Pointer square3 = SquareType::New();
    square3->SetInput(square2->GetOutput());
    SquareType::Pointer square4 = SquareType::New();
    square4->SetInput(square3->GetOutput());
    SquareType::Pointer square5 = SquareType::New();
    square5->SetInput(square4->GetOutput());
//    square5->Update();
//    SquareType::Pointer square6 = SquareType::New();
//    square6->SetInput(square5->GetOutput());
//    SquareType::Pointer square7 = SquareType::New();
//    square7->SetInput(square6->GetOutput());
    InterImageType::Pointer speed = square5->GetOutput();
    square5->Update();


    // create interpolater
    typedef itk::LinearInterpolateImageFunction<InterImageType, CoordRepType>
            InterpolatorType;
    InterpolatorType::Pointer interp = InterpolatorType::New();

    // create cost function
    PathFilterType::CostFunctionType::Pointer cost =
            PathFilterType::CostFunctionType::New();
    cost->SetInterpolator(interp);

//     create optimizer
    typedef itk::GradientDescentOptimizer OptimizerType;
    OptimizerType::Pointer optimizer = OptimizerType::New();
    optimizer->SetNumberOfIterations( 5000 );
//    typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
//    OptimizerType::Pointer optimizer = OptimizerType::New();
//    optimizer->SetNumberOfIterations(50000);
//    optimizer->SetMaximumStepLength(2);
//    optimizer->SetMinimumStepLength(0.5);
//    optimizer->SetRelaxationFactor(0.5);

    // create path filter
    PathFilterType::Pointer pathFilter = PathFilterType::New();
    pathFilter->SetInput( speed );
    pathFilter->SetCostFunction( cost );
    pathFilter->SetOptimizer( optimizer );
    pathFilter->SetTerminationValue( 2.0 );

    // setup path points;
    PathFilterType::PointType start, end;

    start[0] = ps[0]; start[1] = ps[1]; start[2] = ps[2];
    end[0] = pe[0]; end[1] = pe[1];end[2] = pe[2];

    // add path information
    PathFilterType::PathInfo info;
    info.SetStartPoint( start );
    info.SetEndPoint( end );
    pathFilter->AddPathInfo( info );

    // compute the path
    std::cout << "Computing path..." << std::endl;
    itk::TimeProbe time;
    time.Start();
    pathFilter->Update();
    time.Stop();
    std::cout << std::setprecision(3) << "Path computed in: " << time.GetMeanTime()
              << " seconds" << std::endl;

    // allocate output image
    OutputImageType::Pointer output = OutputImageType::New();
    output->SetRegions( speed->GetLargestPossibleRegion() );
    output->SetSpacing( speed->GetSpacing() );
    output->SetOrigin( speed->GetOrigin() );
    output->Allocate( );
    output->FillBuffer( itk::NumericTraits<OutputPixelType>::Zero );

    // get valid path
    for(unsigned int i = 0;i< pathFilter->GetNumberOfOutputs();i++){
        PathType::Pointer path = pathFilter->GetOutput(i);
        //check if path is valid
        if(path->GetVertexList()->Size() == 0 ){
            std::cout << "WARNING: Path " << (i+1) << " Contains no Points!"
                      << std::endl;
            continue;
        }
        //iterate path and convert to image
        PathIteratorType it(output, path);
        PathType::PointType point;
        Point3f Cenpoint;
        int count = 1;
        for(it.GoToBegin(); !it.IsAtEnd(); ++it){
//            it.Set( itk::NumericTraits<OutputPixelType>::max() )
            std::cout << (it.GetIndex())[0] << ", "
                      << (it.GetIndex())[1] << ", "
                      << (it.GetIndex())[2] << "\t\t" << it.Get() << "*****";
            speed->TransformIndexToPhysicalPoint(it.GetIndex(),point);
            std::cout << point[0] << ", " << point[1] << ", " << point[2] << "\n";
            Cenpoint.x = point[0];Cenpoint.y = point[1];Cenpoint.z = point[2];
//            CenterPoints.push_back(static_cast<Point3f>(point));
            CenterPoints1.push_back(Cenpoint);
            count++;
        }
        std::cout << "Successfully find path: " << (i+1)
                  << " and it contains " << count << " points."<< std::endl;
    }

    // writer output
//    WriterType::Pointer writer = WriterType::New();
//    std::string OutputFilename = "testcenterline.mhd";
//    writer->SetFileName( OutputFilename );
//    writer->SetInput( output );
//    writer->Update();
    return EXIT_SUCCESS;
    
}

