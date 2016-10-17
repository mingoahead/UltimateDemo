#-------------------------------------------------
#
# Project created by QtCreator 2016-10-10T09:34:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UltimateDemo
TEMPLATE = app

## macros
ItkRoot = $$PWD/../../3rd/InsightToolkit-4.8.0-minSizeRel
VtkRoot = $$PWD/../../3rd/VTK-6.3.0-minSizeRel

## itk-4.8.0
win32: LIBS += -L$${ItkRoot}/lib/
INCLUDEPATH += $${ItkRoot}/include/ITK-4.8/
DEPENDPATH += $${ItkRoot}/include/ITK-4.8/

## vtk-6.3.0
win32: LIBS += -L$${VtkRoot}/lib/
INCLUDEPATH += $${VtkRoot}/include/vtk-6.3/
DEPENDPATH += $${VtkRoot}/include/vtk-6.3/



SOURCES += main.cpp\
        mainwindow.cpp \
    CTK-LIBS/LIBS/Core/ctkLogger.cpp \
    CTK-LIBS/LIBS/Core/ctkUtils.cpp \
    CTK-LIBS/LIBS/Core/ctkValueProxy.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVTKConnection.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVTKObjectEventsObserver.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKChartView.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKColorTransferFunction.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKScalarsToColorsView.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKScalarsToColorsWidget.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKThresholdWidget.cpp \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKVolumePropertyWidget.cpp \
    CTK-LIBS/LIBS/Widgets/ctkBasePopupWidget.cpp \
    CTK-LIBS/LIBS/Widgets/ctkCollapsibleGroupBox.cpp \
    CTK-LIBS/LIBS/Widgets/ctkColorDialog.cpp \
    CTK-LIBS/LIBS/Widgets/ctkColorPickerButton.cpp \
    CTK-LIBS/LIBS/Widgets/ctkDoubleRangeSlider.cpp \
    CTK-LIBS/LIBS/Widgets/ctkDoubleSlider.cpp \
    CTK-LIBS/LIBS/Widgets/ctkDoubleSpinBox.cpp \
    CTK-LIBS/LIBS/Widgets/ctkExpandButton.cpp \
    CTK-LIBS/LIBS/Widgets/ctkHistogram.cpp \
    CTK-LIBS/LIBS/Widgets/ctkMaterialPropertyPreviewLabel.cpp \
    CTK-LIBS/LIBS/Widgets/ctkMaterialPropertyWidget.cpp \
    CTK-LIBS/LIBS/Widgets/ctkPopupWidget.cpp \
    CTK-LIBS/LIBS/Widgets/ctkProxyStyle.cpp \
    CTK-LIBS/LIBS/Widgets/ctkRangeSlider.cpp \
    CTK-LIBS/LIBS/Widgets/ctkRangeWidget.cpp \
    CTK-LIBS/LIBS/Widgets/ctkSliderWidget.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunction.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionBarsItem.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionControlPointsItem.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionGradientItem.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionItem.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionRepresentation.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionScene.cpp \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionView.cpp \
    Modules/dataadaptor.cpp

HEADERS  += mainwindow.h \
    Utils/itklibs.h \
    Utils/vtklibs.h \
    Utils/itkutils.h \
    CTK-LIBS/LIBS/Core/ctkCoreExport.h \
    CTK-LIBS/LIBS/Core/ctkLogger.h \
    CTK-LIBS/LIBS/Core/ctkPimpl.h \
    CTK-LIBS/LIBS/Core/ctkSingleton.h \
    CTK-LIBS/LIBS/Core/ctkUtils.h \
    CTK-LIBS/LIBS/Core/ctkValueProxy.h \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVisualizationVTKCoreExport.h \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVTKConnection.h \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVTKConnection_p.h \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVTKObject.h \
    CTK-LIBS/LIBS/Visualization/VTK/Core/ctkVTKObjectEventsObserver.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVisualizationVTKWidgetsExport.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKChartView.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKColorTransferFunction.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKScalarsToColorsView.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKScalarsToColorsWidget.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKThresholdWidget.h \
    CTK-LIBS/LIBS/Visualization/VTK/Widgets/ctkVTKVolumePropertyWidget.h \
    CTK-LIBS/LIBS/Widgets/ctkBasePopupWidget.h \
    CTK-LIBS/LIBS/Widgets/ctkBasePopupWidget_p.h \
    CTK-LIBS/LIBS/Widgets/ctkCollapsibleGroupBox.h \
    CTK-LIBS/LIBS/Widgets/ctkColorDialog.h \
    CTK-LIBS/LIBS/Widgets/ctkColorPickerButton.h \
    CTK-LIBS/LIBS/Widgets/ctkDoubleRangeSlider.h \
    CTK-LIBS/LIBS/Widgets/ctkDoubleSlider.h \
    CTK-LIBS/LIBS/Widgets/ctkDoubleSpinBox.h \
    CTK-LIBS/LIBS/Widgets/ctkDoubleSpinBox_p.h \
    CTK-LIBS/LIBS/Widgets/ctkExpandButton.h \
    CTK-LIBS/LIBS/Widgets/ctkHistogram.h \
    CTK-LIBS/LIBS/Widgets/ctkMaterialPropertyPreviewLabel.h \
    CTK-LIBS/LIBS/Widgets/ctkMaterialPropertyWidget.h \
    CTK-LIBS/LIBS/Widgets/ctkPopupWidget.h \
    CTK-LIBS/LIBS/Widgets/ctkPopupWidget_p.h \
    CTK-LIBS/LIBS/Widgets/ctkProxyStyle.h \
    CTK-LIBS/LIBS/Widgets/ctkRangeSlider.h \
    CTK-LIBS/LIBS/Widgets/ctkRangeWidget.h \
    CTK-LIBS/LIBS/Widgets/ctkSliderWidget.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunction.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionBarsItem.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionControlPointsItem.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionGradientItem.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionItem.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionRepresentation.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionScene.h \
    CTK-LIBS/LIBS/Widgets/ctkTransferFunctionView.h \
    CTK-LIBS/LIBS/Widgets/ctkWidgetsExport.h \
    Utils/itkvtkpipe.h \
    Utils/fastdef.h \
    Utils/initials.h \
    UnitTest/testunit.h \
    UnitTest/testdistancewidget.h \
    UnitTest/testcutter.h \
    UnitTest/testimageslice.h \
    UnitTest/testextractpolylines.h \
    UnitTest/testimageplanewidget.h \
    Modules/dataadaptor.h \
    UnitTest/test_3views.h

FORMS    += mainwindow.ui

INCLUDEPATH += CTK-LIBS/LIBS/Widgets/ \
               CTK-LIBS/LIBS/Core/ \
               CTK-LIBS/LIBS/Visualization/VTK/Widgets/ \
               CTK-LIBS/LIBS/Visualization/VTK/Core/ \

DISTFILES += \
    SubSys/WTK.exp \
    SubSys/WTK.lib \
    SubSys/WTK.exe
