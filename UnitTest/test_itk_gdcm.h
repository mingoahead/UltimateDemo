#ifndef TEST_ITK_GDCM
#define TEST_ITK_GDCM

#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>

class Test_Itk_gdcm{
public:
    int RunTest()
    {
        return test_gdcm();
    }
private:
    int test_gdcm()
    {
        std::string input_dirs = "D://3dresearch//TestData//160104-hospital9//06//DICOM//PA1//ST1//SE1";
        const unsigned int InputDimension = 3;
        const unsigned int OutputDimension = 2;
        typedef signed short PixelType;

          typedef itk::Image< PixelType, InputDimension >
            InputImageType;
          typedef itk::Image< PixelType, OutputDimension >
            OutputImageType;
          typedef itk::ImageSeriesReader< InputImageType >
            ReaderType;
          typedef itk::GDCMImageIO
            ImageIOType;
          typedef itk::GDCMSeriesFileNames
            InputNamesGeneratorType;
          typedef itk::NumericSeriesFileNames
            OutputNamesGeneratorType;
        typedef itk::ImageSeriesWriter< InputImageType, OutputImageType >
          SeriesWriterType;
        ImageIOType::Pointer gdcmIO = ImageIOType::New();
         InputNamesGeneratorType::Pointer inputNames = InputNamesGeneratorType::New();
         inputNames->SetInputDirectory(input_dirs);
         const ReaderType::FileNamesContainer & filenames =
                                     inputNames->GetInputFileNames();
         ReaderType::Pointer reader = ReaderType::New();
         reader->SetImageIO(gdcmIO);
         reader->SetFileNames(filenames);
         try{
             reader->Update();
         }catch (itk::ExceptionObject &exp){
             std::cerr << exp.what() << std::endl;
             return EXIT_FAILURE;
         }
         char* name = new char[50];
         char* manufacture = new char[50];
         char* patientID = new char[50];
         char* time = new char[50];
         unsigned int dims = 0;
         gdcmIO->GetMetaDataDictionary();
         gdcmIO->GetPatientName(name);
         gdcmIO->GetManufacturer(manufacture);
         gdcmIO->GetPatientID(patientID);
         gdcmIO->GetStudyDate(time);
         gdcmIO->GetDimensions(dims);
         int pixelType =  gdcmIO->GetPixelType();
         int componetType = gdcmIO->GetComponentType();
         int fileType = gdcmIO->GetFileType();
         ImageIOType::SizeType imgSize;
         imgSize = gdcmIO->GetImageSizeInPixels();
         std::cout << "load series successfully : " << name << "****"
                   << patientID << "****"
                   << time << "****"
                   << manufacture << "****"
                   << dims << "****"
                   << pixelType << "****"
                   << componetType << "****"
                   << fileType << std::endl;
         return EXIT_SUCCESS;

    }
};
#endif // TEST_ITK_GDCM

