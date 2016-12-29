#ifndef IYKUTILS
#define IYKUTILS

# include "itkVersion.h"
# include "itkImageIOFactory.h"
# include "itkPNGImageIOFactory.h"
# include "itkMetaImageIOFactory.h"
# include "itkBMPImageIOFactory.h"
# include "itkJPEGImageIOFactory.h"
# include "itkGDCMImageIOFactory.h"
# include "itkImageSeriesReader.h"
# include "itkGDCMImageIO.h"
# include "itkGDCMSeriesFileNames.h"
# include "itkImageFileWriter.h"
# include "itkMetaImageIO.h"
# include "itkImage.h"
# include "itkImageToVTKImageFilter.h"
# include "itkImageSeriesWriter.h"
# include "gdcmUIDGenerator.h"
# include "itkNumericSeriesFileNames.h"
# include "vtkSmartPointer.h"
# include "vtkImageFlip.h"
# include "vtkImageData.h"
# include <string>
# include <sstream>

class ItkUtils
{
public:
    static void RegisterAllImageIOFactories()
    {
        itk::PNGImageIOFactory::RegisterOneFactory();
        itk::BMPImageIOFactory::RegisterOneFactory();
        itk::MetaImageIOFactory::RegisterOneFactory();
        itk::JPEGImageIOFactory::RegisterOneFactory();
        itk::GDCMImageIOFactory::RegisterOneFactory();
    }

    ///
    /// srcFolder: DICOM folder
    /// destPath : .mhd file path
    ///
    template<typename InputElementType, typename OutputElementType>
    static void DicomSeries2Raw(std::string srcFolder, std::string destPath)
    {
        const size_t Dimension = 3;
        typedef itk::Image<InputElementType, Dimension> InputImageType;
        typedef itk::Image<OutputElementType, Dimension> OutputImageType;

        typedef itk::ImageSeriesReader<InputImageType> ReaderType;
        typedef itk::GDCMImageIO ImageIOType;

        ReaderType::Pointer reader = itk::ImageSeriesReader<InputImageType>::New();
        ImageIOType::Pointer dicomIO = itk::GDCMImageIO::New();

        reader->SetImageIO(dicomIO);

        typedef itk::GDCMSeriesFileNames NamesGeneratorType;

        NamesGeneratorType::Pointer nameGenerator = itk::GDCMSeriesFileNames::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->AddSeriesRestriction("0008|0021");

        nameGenerator->SetDirectory(srcFolder);

        std::cout << "read dicom ...: " << srcFolder << std::endl;
        try {
            typedef std::vector< std::string >    SeriesIdContainer;

            const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
            std::string seriesIdentifier = *seriesUID.begin();
            std::vector< std::string > fileNames = nameGenerator->GetFileNames( seriesIdentifier );
            reader->SetFileNames( fileNames );
            reader->Update();
            std::cout << "successfully load dicom" << std::endl;
        } catch (itk::ExceptionObject &ex) {
            std::cout << ex.GetDescription() << std::endl;
            return;
//            throw;
        }
        std::cout << "read dicom complete" << std::endl;
        std::cout << "write mhd ..." << destPath << std::endl;
        try {
            typedef itk::ImageFileWriter< OutputImageType > WriterType;

            WriterType::Pointer writer = itk::ImageFileWriter<OutputImageType>::New();
            writer->SetFileName(destPath);
            writer->SetInput(reader->GetOutput());
            writer->SetImageIO(itk::MetaImageIO::New());
            writer->Update();
        } catch (itk::ExceptionObject &ex) {
            std::cerr << ex.GetDescription() << std::endl;
//            throw;
            return ;
        }
        std::cout << "write mhd complete" << std::endl;
    }

    template<typename InputElementType, typename OutputElementType>
    static void SaveAsStdDicom(std::string srcFolder, std::string destFolder)
    {
        const size_t InputDimension = 3;
        const size_t OutputDimension = 2;
        typedef itk::Image<InputElementType, InputDimension> InputImageType;
        typedef itk::Image<OutputElementType, OutputDimension> OutputImageType;

        typedef itk::ImageSeriesReader<InputImageType> ReaderType;
        typedef itk::GDCMImageIO ImageIOType;

        ReaderType::Pointer reader = itk::ImageSeriesReader<InputImageType>::New();
        ImageIOType::Pointer dicomIO = itk::GDCMImageIO::New();

        reader->SetImageIO(dicomIO);

        typedef itk::GDCMSeriesFileNames NamesGeneratorType;

        NamesGeneratorType::Pointer nameGenerator = itk::GDCMSeriesFileNames::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->AddSeriesRestriction("0008|0021");

        nameGenerator->SetDirectory(srcFolder);

        try {
            typedef std::vector< std::string >    SeriesIdContainer;

            const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
            std::string seriesIdentifier = *seriesUID.begin();
            std::vector< std::string > fileNames = nameGenerator->GetFileNames( seriesIdentifier );
            reader->SetFileNames( fileNames );
            reader->Update();
        } catch (itk::ExceptionObject &ex) {
            std::cout << ex.GetDescription() << std::endl;
            throw;
        }

        const InputImageType::SpacingType& inputSpacing = reader->GetOutput()->GetSpacing();
        const InputImageType::RegionType& inputRegion = reader->GetOutput()->GetLargestPossibleRegion();
        const InputImageType::SizeType& inputSize = inputRegion.GetSize();

        typedef itk::NumericSeriesFileNames OutputNamesGeneratorType;

        OutputNamesGeneratorType::Pointer outputNames = OutputNamesGeneratorType::New();
        std::string seriesFormat(destFolder);
        seriesFormat = seriesFormat + "/" + "IM%05d.dcm";
        outputNames->SetSeriesFormat (seriesFormat);
        outputNames->SetStartIndex (1);
        outputNames->SetEndIndex (inputSize[2]);


        ReaderType::DictionaryArrayType outputArray;

        ReaderType::DictionaryRawPointer inputDict = (*(reader->GetMetaDataDictionaryArray()))[0];

        gdcm::UIDGenerator suid;
        std::string seriesUID = suid.Generate();
        gdcm::UIDGenerator fuid;
        std::string frameOfReferenceUID = fuid.Generate();

        std::string studyUID;
        std::string sopClassUID;
        itk::ExposeMetaData<std::string>(*inputDict, "0020|000d", studyUID);
        itk::ExposeMetaData<std::string>(*inputDict, "0008|0016", sopClassUID);
        dicomIO->KeepOriginalUIDOn();
        for (unsigned int f = 0; f < inputSize[2]; f++) {
            ReaderType::DictionaryRawPointer dict = new ReaderType::DictionaryType;
            CopyDictionary (*inputDict, *dict);
            itk::EncapsulateMetaData<std::string>(*dict,"0020|000d", studyUID);
            itk::EncapsulateMetaData<std::string>(*dict,"0020|000e", seriesUID);
            itk::EncapsulateMetaData<std::string>(*dict,"0020|0052", frameOfReferenceUID);
            gdcm::UIDGenerator sopuid;
            std::string sopInstanceUID = sopuid.Generate();
            itk::EncapsulateMetaData<std::string>(*dict,"0008|0018", sopInstanceUID);
            itk::EncapsulateMetaData<std::string>(*dict,"0002|0003", sopInstanceUID);
            std::ostringstream value;
            value.str("");
            value << f + 1;
            itk::EncapsulateMetaData<std::string>(*dict,"0020|0013", value.str());

            std::string oldSeriesDesc;
            itk::ExposeMetaData<std::string>(*inputDict, "0008|103e", oldSeriesDesc);
            value.str("");
            value << oldSeriesDesc
                  << inputSpacing[0] << ", "
                  << inputSpacing[1] << ", "
                  << inputSpacing[2];
            unsigned lengthDesc = value.str().length();

            std::string seriesDesc( value.str(), 0, lengthDesc > 64 ? 64 : lengthDesc);
            itk::EncapsulateMetaData<std::string>(*dict,"0008|103e", seriesDesc);

            // Series Number
            value.str("");
            value << 1001;
            itk::EncapsulateMetaData<std::string>(*dict,"0020|0011", value.str());

            value.str("");
            value << "surfaceRenderer : " << ITK_SOURCE_VERSION;

            lengthDesc = value.str().length();
            std::string derivationDesc( value.str(), 0, lengthDesc > 1024 ? 1024 : lengthDesc);
            itk::EncapsulateMetaData<std::string>(*dict,"0008|2111", derivationDesc);

            InputImageType::PointType position;
            InputImageType::IndexType index;
            index[0] = 0;
            index[1] = 0;
            index[2] = f;
            reader->GetOutput()->TransformIndexToPhysicalPoint(index, position);

            value.str("");
            value << position[0] << "\\" << position[1] << "\\" << position[2];
            itk::EncapsulateMetaData<std::string>(*dict,"0020|0032", value.str());
            value.str("");
            value << position[2];
            itk::EncapsulateMetaData<std::string>(*dict,"0020|1041", value.str());

            outputArray.push_back(dict);
        }

        typedef itk::ImageSeriesWriter< InputImageType, OutputImageType >  WriterType;
        WriterType::Pointer writer = WriterType::New();
        writer->SetInput( reader->GetOutput() );
        dicomIO->SetByteOrderToLittleEndian();
        writer->SetImageIO( dicomIO );
        writer->SetFileNames( outputNames->GetFileNames() );
        writer->SetMetaDataDictionaryArray( &outputArray );

        try {
            writer->Update();
        } catch( itk::ExceptionObject & excp ) {
            std::cerr << excp << std::endl;
            throw;
        }
    }

    template<typename ItkImageType>
    static vtkImageData* ToVtkImageData(ItkImageType* itkImageData)
    {
        typedef itk::ImageToVTKImageFilter<ItkImageType> Itk2VtkFilterType;

        Itk2VtkFilterType::Pointer filter = Itk2VtkFilterType::New();
        filter->SetInput(itkImageData);
        filter->Update();

        vtkSmartPointer<vtkImageFlip> flipX = vtkSmartPointer<vtkImageFlip>::New();
        vtkSmartPointer<vtkImageFlip> flipY = vtkSmartPointer<vtkImageFlip>::New();

        flipX->SetInputData(filter->GetOutput());

        flipX->SetFilteredAxis(1);

        flipX->Update();

        flipY->SetInputData(flipX->GetOutput());

        flipY->SetFilteredAxis(2);

        flipY->Update();

        vtkImageData* vtkImage = flipY->GetOutput();

        return vtkImage;
    }
private:
    static void CopyDictionary (itk::MetaDataDictionary &fromDict, itk::MetaDataDictionary &toDict)
    {
        typedef itk::MetaDataDictionary DictionaryType;

        DictionaryType::ConstIterator itr = fromDict.Begin();
        DictionaryType::ConstIterator end = fromDict.End();
        typedef itk::MetaDataObject< std::string > MetaDataStringType;

        while( itr != end ) {
            itk::MetaDataObjectBase::Pointer  entry = itr->second;

            MetaDataStringType::Pointer entryvalue =
                    dynamic_cast<MetaDataStringType *>( entry.GetPointer() ) ;
            if( entryvalue ) {
                std::string tagkey   = itr->first;
                std::string tagvalue = entryvalue->GetMetaDataObjectValue();
                itk::EncapsulateMetaData<std::string>(toDict, tagkey, tagvalue);
            }
            ++itr;
        }
    }


};
#endif // IYKUTILS

