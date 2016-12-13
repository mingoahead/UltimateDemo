#ifndef TEST_VTKIMAGEPROC
#define TEST_VTKIMAGEPROC
///histogram
#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkBarChartActor.h>
#include <string>
#include <vtkActor.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkJPEGReader.h>
#include <vtkPNGReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXYPlotActor.h>
#include <vtkImageMagnitude.h>
#include <vtkLegendBoxActor.h>
//dicom related
#include <vtkDICOMImageReader.h>
#include <vtkImageCast.h>
#include <vtkBMPReader.h>
#include <vtkBMPWriter.h>
class Test_VtkImageProcess {
public:
    int RunTest()
    {
//        return testHistogram_3();
//        return testHistogram_1();
        return testDicom2bmp();
    }
private:
    int testHistogram_3()
    {
        int ignoreZero = 1;
        // Read a jpeg image
        vtkSmartPointer<vtkJPEGReader> reader =
          vtkSmartPointer<vtkJPEGReader>::New();
        reader->SetFileName( "D://3dresearch//TestData//image//scene.jpg");
        reader->Update();
        int numComponents = reader->GetOutput()->GetNumberOfScalarComponents();
        if( numComponents > 3 )
          {
          std::cout << "Error: cannot process an image with "
                    << numComponents << " components!" << std::endl;
          return EXIT_FAILURE;
          }

        // Create a vtkXYPlotActor
        vtkSmartPointer<vtkXYPlotActor> plot =
          vtkSmartPointer<vtkXYPlotActor>::New();
        plot->ExchangeAxesOff();
        plot->SetLabelFormat( "%g" );
        plot->SetXTitle( "Level" );
        plot->SetYTitle( "Frequency" );
        plot->SetXValuesToValue();
        double xmax = 0.;
          double ymax = 0.;

          double colors[3][3] = {
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 } };

          const char* labels[3] = {
            "Red", "Green", "Blue" };

        // Process the image, extracting and plotting a histogram for each
        // component
         for( int i = 0; i < numComponents; ++i )
           {
           vtkSmartPointer<vtkImageExtractComponents> extract =
             vtkSmartPointer<vtkImageExtractComponents>::New();
           extract->SetInputConnection( reader->GetOutputPort() );
           extract->SetComponents( i );
           extract->Update();

           double range[2];
           extract->GetOutput()->GetScalarRange( range );

           vtkSmartPointer<vtkImageAccumulate> histogram =
             vtkSmartPointer<vtkImageAccumulate>::New();
           histogram->SetInputConnection( extract->GetOutputPort() );
           histogram->SetComponentExtent(
             0,
             static_cast<int>(range[1])-static_cast<int>(range[0])-1,0,0,0,0 );
           histogram->SetComponentOrigin( range[0],0,0 );
           histogram->SetComponentSpacing( 1,0,0 );
           histogram->SetIgnoreZero( ignoreZero );
           histogram->Update();
           if( range[1] > xmax )
                {
                xmax = range[1];
                }
              if( histogram->GetOutput()->GetScalarRange()[1] > ymax )
                {
                ymax = histogram->GetOutput()->GetScalarRange()[1];
                }

          #if VTK_MAJOR_VERSION <= 5
              plot->AddInput( histogram->GetOutput() );
          #else
              plot->AddDataSetInputConnection( histogram->GetOutputPort() );
          #endif
          if( numComponents > 1 )
            {
            plot->SetPlotColor(i,colors[i]);
            plot->SetPlotLabel(i,labels[i]);
            plot->LegendOn();
            }
          }
        plot->SetXRange( 0, xmax );
        plot->SetYRange( 0, ymax );
        vtkSmartPointer<vtkRenderer> renderer =
          vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(plot);
        vtkSmartPointer<vtkRenderWindow> renderWindow =
          vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer( renderer );
        renderWindow->SetSize(640, 480);
        vtkSmartPointer<vtkRenderWindowInteractor> interactor =
          vtkSmartPointer<vtkRenderWindowInteractor>::New();
        interactor->SetRenderWindow( renderWindow );
        interactor->Initialize();
        interactor->Start();
        return EXIT_SUCCESS;

    }

    int testHistogram_1()
    {
        // Read a jpeg image
//          vtkSmartPointer<vtkJPEGReader> reader =
//            vtkSmartPointer<vtkJPEGReader>::New();
          vtkSmartPointer<vtkPNGReader> reader =
                  vtkSmartPointer<vtkPNGReader>::New();
          reader->SetFileName("D://3dresearch//TestData//image//cells2.png");
//          reader->SetFileName("D://3dresearch//TestData//image//scene.jpg");

          reader->Update();

          vtkSmartPointer<vtkImageMagnitude> magnitude =
            vtkSmartPointer<vtkImageMagnitude>::New();
          magnitude->SetInputConnection(reader->GetOutputPort());
          magnitude->Update();

          double red[3] = {1, 0, 0 };

          vtkSmartPointer<vtkIntArray> frequencies =
            vtkSmartPointer<vtkIntArray>::New();

          vtkSmartPointer<vtkImageAccumulate> histogram =
            vtkSmartPointer<vtkImageAccumulate>::New();
          histogram->SetInputConnection(magnitude->GetOutputPort());
          histogram->SetComponentExtent(0,255,0,0,0,0);
          histogram->SetComponentOrigin(0,0,0);
          histogram->SetComponentSpacing(1,0,0);
          histogram->IgnoreZeroOn();
          histogram->Update();

          frequencies->SetNumberOfComponents(1);
          frequencies->SetNumberOfTuples(256);
          int* output = static_cast<int*>(histogram->GetOutput()->GetScalarPointer());

          for(int j = 0; j < 256; ++j)
            {
            frequencies->SetTuple1(j, *output++);
            }

          vtkSmartPointer<vtkDataObject> dataObject =
            vtkSmartPointer<vtkDataObject>::New();

          dataObject->GetFieldData()->AddArray( frequencies );

          // Create a vtkBarChartActor
          vtkSmartPointer<vtkBarChartActor> barChart =
            vtkSmartPointer<vtkBarChartActor>::New();

          barChart->SetInput(dataObject);
          barChart->SetTitle("Histogram");
          barChart->GetPositionCoordinate()->SetValue(0.05,0.05,0.0);
          barChart->GetPosition2Coordinate()->SetValue(0.95,0.85,0.0);
          barChart->GetProperty()->SetColor(1,1,1);

          barChart->GetLegendActor()->SetNumberOfEntries(dataObject->GetFieldData()->GetArray(0)->GetNumberOfTuples());
          barChart->LegendVisibilityOff();
          barChart->LabelVisibilityOff();

          int count = 0;
          for(int i = 0; i < 256; ++i)
            {
            barChart->SetBarColor( count++, red );
            }

          // Visualize the histogram
          vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
          renderer->AddActor(barChart);

          vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
          renderWindow->AddRenderer(renderer);
          renderWindow->SetSize(640, 480);

          vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
          interactor->SetRenderWindow(renderWindow);

          // Initialize the event loop and then start it
          interactor->Initialize();
          interactor->Start();

          return  EXIT_SUCCESS;
    }

    int testDicom2bmp()
    {
        vtkSmartPointer<vtkDICOMImageReader> reader
                = vtkSmartPointer<vtkDICOMImageReader>::New();
        std::string inputname = "D://3dresearch//TestData//img_in.dcm";
        reader->SetFileName(inputname.c_str());
        reader->Update();
        vtkSmartPointer<vtkImageCast> castfilter
                = vtkSmartPointer<vtkImageCast>::New();
//        castfilter->SetOutputScalarTypeToChar();
        castfilter->SetOutputScalarTypeToUnsignedChar();
        castfilter->SetInputConnection(reader->GetOutputPort());
        castfilter->Update();
        vtkSmartPointer<vtkBMPWriter> writer
                = vtkSmartPointer<vtkBMPWriter>::New();
        writer->SetInputConnection(castfilter->GetOutputPort());
        std::string outputname = "D://3dresearch//TestData//img_out.bmp";
        writer->SetFileName(outputname.c_str());
        writer->Write();
        return EXIT_SUCCESS;

    }
};
#endif // TEST_VTKIMAGEPROC

