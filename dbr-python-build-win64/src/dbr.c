#include <Python.h>
#include "DynamsoftBarcodeReader.h"
#include <ndarraytypes.h>
#include <structmember.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#if PY_MAJOR_VERSION >= 3
#ifndef IS_PY3K
#define IS_PY3K 1
#endif
#endif

struct module_state
{
    PyObject *error;
};

#if defined(IS_PY3K)
#define GETSTATE(m) ((struct module_state *)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject * ErrorOut(PyObject *m)
{
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
	Py_RETURN_NONE;
}

#define DBR_NO_MEMORY 0
#define DBR_SUCCESS 1

// #define LOG_OFF

#ifdef LOG_OFF

#define printf(MESSAGE, __VA_ARGS__)

#endif

#define DEFAULT_MEMORY_SIZE 4096

typedef struct
{
    PyObject_HEAD
    // Barcode reader handler
    void *hBarcode;
    // Callback function for video mode
    PyObject *py_callback;
} DynamsoftBarcodeReader;

void ToHexString(unsigned char* pSrc, int iLen, char* pDest)
{
	const char HEXCHARS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	int i;
	char* ptr = pDest;

	for(i = 0; i < iLen; ++i)
	{
		snprintf(ptr, 4, "%c%c ", HEXCHARS[ ( pSrc[i] & 0xF0 ) >> 4 ], HEXCHARS[ ( pSrc[i] & 0x0F ) >> 0 ]);
		ptr += 3;
	}
}

static PyObject * CreatePyRuntimeSettings(PublicRuntimeSettings pSettings)
{
    PyObject * pySettings = PyDict_New();
    if(pySettings == NULL)
    {
        return NULL;     
    }

    PyObject * terminatePhase               = Py_BuildValue("i", pSettings.terminatePhase);
    PyObject * terminatePhaseKey            = Py_BuildValue("s", "TerminatePhase");
    PyDict_SetItem(pySettings, terminatePhaseKey,           terminatePhase);
    Py_DECREF(terminatePhase);
    Py_DECREF(terminatePhaseKey);

    PyObject * timeout                      = Py_BuildValue("i", pSettings.timeout);
    PyObject * timeoutKey                   = Py_BuildValue("s", "Timeout");
    PyDict_SetItem(pySettings, timeoutKey, timeout);
    Py_DECREF(timeout);
    Py_DECREF(timeoutKey);

    PyObject * maxAlgorithmThreadCount      = Py_BuildValue("i", pSettings.maxAlgorithmThreadCount );
    PyObject * maxAlgorithmThreadCountKey   = Py_BuildValue("s", "MaxAlgorithmThreadCount");
    PyDict_SetItem(pySettings, maxAlgorithmThreadCountKey,  maxAlgorithmThreadCount);
    Py_DECREF(maxAlgorithmThreadCount);
    Py_DECREF(maxAlgorithmThreadCountKey);

    PyObject * expectedBarcodesCount        = Py_BuildValue("i", pSettings.expectedBarcodesCount);
    PyObject * expectedBarcodesCountKey     = Py_BuildValue("s", "ExpectedBarcodesCount");
    PyDict_SetItem(pySettings, expectedBarcodesCountKey,    expectedBarcodesCount);
    Py_DECREF(expectedBarcodesCount);
    Py_DECREF(expectedBarcodesCountKey);

    PyObject * barcodeFormatIds             = Py_BuildValue("i", pSettings.barcodeFormatIds);
    PyObject * BarcodeFormatIdsKey          = Py_BuildValue("s", "BarcodeFormatIds");
    PyDict_SetItem(pySettings, BarcodeFormatIdsKey,         barcodeFormatIds);
    Py_DECREF(barcodeFormatIds);
    Py_DECREF(BarcodeFormatIdsKey);

    PyObject * barcodeFormatIds_2           = Py_BuildValue("i", pSettings.barcodeFormatIds_2);
    PyObject * barcodeFormatIds_2Key        = Py_BuildValue("s", "BarcodeFormatIds_2");
    PyDict_SetItem(pySettings, barcodeFormatIds_2Key,       barcodeFormatIds_2);
    Py_DECREF(barcodeFormatIds_2);
    Py_DECREF(barcodeFormatIds_2Key);

    PyObject * pdfRasterDPI                 = Py_BuildValue("i", pSettings.pdfRasterDPI);
    PyObject * pdfRasterDPIKey              = Py_BuildValue("s", "PDFRasterDPI");
    PyDict_SetItem(pySettings, pdfRasterDPIKey,             pdfRasterDPI);
    Py_DECREF(pdfRasterDPI);
    Py_DECREF(pdfRasterDPIKey);

    PyObject * scaleDownThreshold           = Py_BuildValue("i", pSettings.scaleDownThreshold);
    PyObject * scaleDownThresholdKey        = Py_BuildValue("s", "ScaleDownThreshold");
    PyDict_SetItem(pySettings, scaleDownThresholdKey,       scaleDownThreshold);
    Py_DECREF(scaleDownThreshold);
    Py_DECREF(scaleDownThresholdKey);

    PyObject * binarizationModes            = PyList_New(8);
    PyObject * localizationModes            = PyList_New(8);
    PyObject * colourClusteringModes        = PyList_New(8);
    PyObject * colourConversionModes        = PyList_New(8);
    PyObject * grayscaleTransformationModes = PyList_New(8);
    PyObject * regionPredetectionModes      = PyList_New(8);
    PyObject * imagePreprocessingModes      = PyList_New(8);
    PyObject * textureDetectionModes        = PyList_New(8);
    PyObject * textFilterModes              = PyList_New(8);
    PyObject * dpmCodeReadingModes          = PyList_New(8);
    PyObject * deformationResistingModes    = PyList_New(8);
    PyObject * barcodeComplementModes       = PyList_New(8);
    PyObject * barcodeColourModes           = PyList_New(8);
    PyObject * textResultOrderModes         = PyList_New(8);
	PyObject * accompanyingTextRecognitionModes = PyList_New(8);
	PyObject * scaleUpModes = PyList_New(8);

    for(int i = 0; i < 8; ++i)
    {
        PyObject * tempBM                   = Py_BuildValue("i", pSettings.binarizationModes[i]);
        PyList_SetItem(binarizationModes,        i, tempBM);
        PyObject * tempLM                   = Py_BuildValue("i", pSettings.localizationModes[i]);
        PyList_SetItem(localizationModes,        i, tempLM);
        PyObject * tempCCM                  = Py_BuildValue("i", pSettings.furtherModes.colourClusteringModes[i]);
        PyList_SetItem(colourClusteringModes,    i, tempCCM);
        PyObject * tempCICM                 = Py_BuildValue("i", pSettings.furtherModes.colourConversionModes[i]);
        PyList_SetItem(colourConversionModes,    i, tempCICM);
        PyObject * tempGTM                  = Py_BuildValue("i", pSettings.furtherModes.grayscaleTransformationModes[i]);
        PyList_SetItem(grayscaleTransformationModes, i, tempGTM);
        PyObject * tempRPM                  = Py_BuildValue("i", pSettings.furtherModes.regionPredetectionModes[i]);
        PyList_SetItem(regionPredetectionModes,  i, tempRPM);
        PyObject * tempIPM                  = Py_BuildValue("i", pSettings.furtherModes.imagePreprocessingModes[i]);
        PyList_SetItem(imagePreprocessingModes,  i, tempIPM);
        PyObject * tempTDM                  = Py_BuildValue("i", pSettings.furtherModes.textureDetectionModes[i]);
        PyList_SetItem(textureDetectionModes,    i, tempTDM);
        PyObject * tempTFM                  = Py_BuildValue("i", pSettings.furtherModes.textFilterModes[i]);
        PyList_SetItem(textFilterModes,          i, tempTFM);
        PyObject * tempDPMCRM               = Py_BuildValue("i", pSettings.furtherModes.dpmCodeReadingModes[i]);
        PyList_SetItem(dpmCodeReadingModes,      i, tempDPMCRM);
        PyObject * tempDRM                  = Py_BuildValue("i", pSettings.furtherModes.deformationResistingModes[i]);
        PyList_SetItem(deformationResistingModes, i, tempDRM);
        PyObject * tempBCM                  = Py_BuildValue("i", pSettings.furtherModes.barcodeComplementModes[i]);
        PyList_SetItem(barcodeComplementModes,   i, tempBCM);
        PyObject * tempBICM                 = Py_BuildValue("i", pSettings.furtherModes.barcodeColourModes[i]);
        PyList_SetItem(barcodeColourModes,       i, tempBICM);
        PyObject * tempTROM                 = Py_BuildValue("i", pSettings.textResultOrderModes[i]);
        PyList_SetItem(textResultOrderModes,     i, tempTROM);
		PyObject * tempATRM                 = Py_BuildValue("i", pSettings.furtherModes.accompanyingTextRecognitionModes[i]);
		PyList_SetItem(accompanyingTextRecognitionModes, i, tempATRM);
		PyObject * tempSUM = Py_BuildValue("i", pSettings.scaleUpModes[i]);
		PyList_SetItem(scaleUpModes, i, tempSUM);
    }

    PyObject * binarizationModesKey         = Py_BuildValue("s", "BinarizationModes");
    PyDict_SetItem(pySettings, binarizationModesKey,        binarizationModes);
    Py_DECREF(binarizationModes);
    Py_DECREF(binarizationModesKey);

    PyObject * localizationModesKey         = Py_BuildValue("s", "LocalizationModes");
    PyDict_SetItem(pySettings, localizationModesKey,        localizationModes);
    Py_DECREF(localizationModes);
    Py_DECREF(localizationModesKey);

    PyObject * colourClusteringModesKey     = Py_BuildValue("s", "ColourClusteringModes");
    PyDict_SetItem(pySettings, colourClusteringModesKey,    colourClusteringModes);
    Py_DECREF(colourClusteringModes);
    Py_DECREF(colourClusteringModesKey);

    PyObject * colourConversionModesKey     = Py_BuildValue("s", "ColourConversionModes");
    PyDict_SetItem(pySettings, colourConversionModesKey,    colourConversionModes);
    Py_DECREF(colourConversionModes);
    Py_DECREF(colourConversionModesKey);

    PyObject * grayscaleTransformationModesKey = Py_BuildValue("s", "GrayscaleTransformationModes");
    PyDict_SetItem(pySettings, grayscaleTransformationModesKey, grayscaleTransformationModes);
    Py_DECREF(grayscaleTransformationModes);
    Py_DECREF(grayscaleTransformationModesKey);
    
    PyObject * regionPredetectionModesKey   = Py_BuildValue("s", "RegionPredetectionModes");
    PyDict_SetItem(pySettings, regionPredetectionModesKey,  regionPredetectionModes);
    Py_DECREF(regionPredetectionModes);
    Py_DECREF(regionPredetectionModesKey);

    PyObject * imagePreprocessingModesKey   = Py_BuildValue("s", "ImagePreprocessingModes");
    PyDict_SetItem(pySettings, imagePreprocessingModesKey,  imagePreprocessingModes);
    Py_DECREF(imagePreprocessingModes);
    Py_DECREF(imagePreprocessingModesKey);

    PyObject * textureDetectionModesKey     = Py_BuildValue("s", "TextureDetectionModes");
    PyDict_SetItem(pySettings, textureDetectionModesKey,    textureDetectionModes);
    Py_DECREF(textureDetectionModes);
    Py_DECREF(textureDetectionModesKey);

    PyObject * textFilterModesKey           = Py_BuildValue("s", "TextFilterModes");
    PyDict_SetItem(pySettings, textFilterModesKey,          textFilterModes);
    Py_DECREF(textFilterModes);
    Py_DECREF(textFilterModesKey);

    PyObject * dpmCodeReadingModesKey       = Py_BuildValue("s", "DPMCodeReadingModes");
    PyDict_SetItem(pySettings, dpmCodeReadingModesKey,      dpmCodeReadingModes);
    Py_DECREF(dpmCodeReadingModes);
    Py_DECREF(dpmCodeReadingModesKey);

    PyObject * deformationResistingModesKey = Py_BuildValue("s", "DeformationResistingModes");
    PyDict_SetItem(pySettings, deformationResistingModesKey, deformationResistingModes);
    Py_DECREF(deformationResistingModes);
    Py_DECREF(deformationResistingModesKey);

    PyObject * barcodeComplementModesKey    = Py_BuildValue("s", "BarcodeComplementModes");
    PyDict_SetItem(pySettings, barcodeComplementModesKey,   barcodeComplementModes);
    Py_DECREF(barcodeComplementModes);
    Py_DECREF(barcodeComplementModesKey);

    PyObject * barcodeColourModesKey        = Py_BuildValue("s", "BarcodeColourModes");
    PyDict_SetItem(pySettings, barcodeColourModesKey,       barcodeColourModes);
    Py_DECREF(barcodeColourModes);
    Py_DECREF(barcodeColourModesKey);

    PyObject * textResultOrderModesKey      = Py_BuildValue("s", "TextResultOrderModes");
    PyDict_SetItem(pySettings, textResultOrderModesKey,     textResultOrderModes);
    Py_DECREF(textResultOrderModes);
    Py_DECREF(textResultOrderModesKey);

	PyObject * accompanyingTextRecognitionModesKey = Py_BuildValue("s", "AccompanyingTextRecognitionModes");
	PyDict_SetItem(pySettings, accompanyingTextRecognitionModesKey, accompanyingTextRecognitionModes);
	Py_DECREF(accompanyingTextRecognitionModes);
	Py_DECREF(accompanyingTextRecognitionModesKey);

	PyObject * scaleUpModesKey = Py_BuildValue("s", "ScaleUpModes");
	PyDict_SetItem(pySettings, scaleUpModesKey, scaleUpModes);
	Py_DECREF(scaleUpModes);
	Py_DECREF(scaleUpModesKey);

    PyObject * textAssistedCorrectionMode   = Py_BuildValue("i", pSettings.furtherModes.textAssistedCorrectionMode);
    PyObject * textAssistedCorrectionModeKey =Py_BuildValue("s", "TextAssistedCorrectionMode");
    PyDict_SetItem(pySettings, textAssistedCorrectionModeKey, textAssistedCorrectionMode);
    Py_DECREF(textAssistedCorrectionMode);
    Py_DECREF(textAssistedCorrectionModeKey);

    PyObject * deblurLevel                  = Py_BuildValue("i", pSettings.deblurLevel);
    PyObject * deblurLevelKey               = Py_BuildValue("s", "DeblurLevel");
    PyDict_SetItem(pySettings, deblurLevelKey,              deblurLevel);
    Py_DECREF(deblurLevel);
    Py_DECREF(deblurLevelKey);

    PyObject * intermediateResultTypes      = Py_BuildValue("i", pSettings.intermediateResultTypes);
    PyObject * intermediateResultTypesKey   = Py_BuildValue("s", "IntermediateResultTypes");
    PyDict_SetItem(pySettings, intermediateResultTypesKey,  intermediateResultTypes);
    Py_DECREF(intermediateResultTypes);
    Py_DECREF(intermediateResultTypesKey);

    PyObject * intermediateResultSavingMode = Py_BuildValue("i", pSettings.intermediateResultSavingMode);
    PyObject * intermediateResultSavingModeKey = Py_BuildValue("s", "IntermediateResultSavingMode");
    PyDict_SetItem(pySettings, intermediateResultSavingModeKey, intermediateResultSavingMode);
    Py_DECREF(intermediateResultSavingMode);
    Py_DECREF(intermediateResultSavingModeKey);

    PyObject * resultCoordinateType         = Py_BuildValue("i", pSettings.resultCoordinateType);
    PyObject * resultCoordinateTypeKey      = Py_BuildValue("s", "ResultCoordinateType");
    PyDict_SetItem(pySettings, resultCoordinateTypeKey,     resultCoordinateType);
    Py_DECREF(resultCoordinateType);
    Py_DECREF(resultCoordinateTypeKey);

    PyObject * returnBarcodeZoneClarity     = Py_BuildValue("i", pSettings.returnBarcodeZoneClarity);
    PyObject * returnBarcodeZoneClarityKey  = Py_BuildValue("s", "ReturnBarcodeZoneClarity");
    PyDict_SetItem(pySettings, returnBarcodeZoneClarityKey, returnBarcodeZoneClarity);
    Py_DECREF(returnBarcodeZoneClarity);
    Py_DECREF(returnBarcodeZoneClarityKey);

    PyObject * regionTop                    = Py_BuildValue("i", pSettings.region.regionTop);
    PyObject * regionTopKey                 = Py_BuildValue("s", "RegionTop");
    PyDict_SetItem(pySettings, regionTopKey,                regionTop);
    Py_DECREF(regionTop);
    Py_DECREF(regionTopKey);

    PyObject * regionBottom                 = Py_BuildValue("i", pSettings.region.regionBottom);
    PyObject * regionBottomKey              = Py_BuildValue("s", "RegionBottom");
    PyDict_SetItem(pySettings, regionBottomKey,             regionBottom);
    Py_DECREF(regionBottom);
    Py_DECREF(regionBottomKey);

    PyObject * regionLeft                   = Py_BuildValue("i", pSettings.region.regionLeft);
    PyObject * regionLeftKey                = Py_BuildValue("s", "RegionLeft");
    PyDict_SetItem(pySettings, regionLeftKey,               regionLeft);
    Py_DECREF(regionLeft);
    Py_DECREF(regionLeftKey);

    PyObject * regionRight                  = Py_BuildValue("i", pSettings.region.regionRight);
    PyObject * regionRightKey               = Py_BuildValue("s", "RegionRight");
    PyDict_SetItem(pySettings, regionRightKey,              regionRight);
    Py_DECREF(regionRight);
    Py_DECREF(regionRightKey);

    PyObject * regionMeasuredByPercentage   = Py_BuildValue("i", pSettings.region.regionMeasuredByPercentage);
    PyObject * regionMeasuredByPercentageKey =Py_BuildValue("s", "RegionMeasuredByPercentage");
    PyDict_SetItem(pySettings, regionMeasuredByPercentageKey, regionMeasuredByPercentage);
    Py_DECREF(regionMeasuredByPercentage);
    Py_DECREF(regionMeasuredByPercentageKey);

    PyObject * minBarcodeTextLength         = Py_BuildValue("i", pSettings.minBarcodeTextLength);
    PyObject * minBarcodeTextLengthKey      = Py_BuildValue("s", "MinBarcodeTextLength");
    PyDict_SetItem(pySettings, minBarcodeTextLengthKey,     minBarcodeTextLength);
    Py_DECREF(minBarcodeTextLength);
    Py_DECREF(minBarcodeTextLengthKey);

    PyObject * minResultConfidence          = Py_BuildValue("i", pSettings.minResultConfidence);
    PyObject * minResultConfidenceKey       = Py_BuildValue("s", "MinResultConfidence");
    PyDict_SetItem(pySettings, minResultConfidenceKey,      minResultConfidence);
    Py_DECREF(minResultConfidence);
    Py_DECREF(minResultConfidenceKey);

    return pySettings;
}

PublicRuntimeSettings CreateCRuntimeSettings(PyObject *o)
{
    PublicRuntimeSettings pSettings;
    pSettings.terminatePhase            = (TerminatePhase)(PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "TerminatePhase"))));
    pSettings.timeout                   = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "Timeout")));
    pSettings.maxAlgorithmThreadCount   = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "MaxAlgorithmThreadCount")));
    pSettings.expectedBarcodesCount     = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "ExpectedBarcodesCount")));
    pSettings.barcodeFormatIds          = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "BarcodeFormatIds")));
    pSettings.barcodeFormatIds_2        = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "BarcodeFormatIds_2")));
    pSettings.pdfRasterDPI              = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "PDFRasterDPI")));
    pSettings.scaleDownThreshold        = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "ScaleDownThreshold")));

    PyObject * binarizationModes            = PyDict_GetItem(o, Py_BuildValue("s", "BinarizationModes"));
    PyObject * localizationModes            = PyDict_GetItem(o, Py_BuildValue("s", "LocalizationModes"));
    PyObject * colourClusteringModes        = PyDict_GetItem(o, Py_BuildValue("s", "ColourClusteringModes"));
    PyObject * colourConversionModes        = PyDict_GetItem(o, Py_BuildValue("s", "ColourConversionModes"));
    PyObject * grayscaleTransformationModes = PyDict_GetItem(o, Py_BuildValue("s", "GrayscaleTransformationModes"));
    PyObject * regionPredetectionModes      = PyDict_GetItem(o, Py_BuildValue("s", "RegionPredetectionModes"));
    PyObject * imagePreprocessingModes      = PyDict_GetItem(o, Py_BuildValue("s", "ImagePreprocessingModes"));
    PyObject * textureDetectionModes        = PyDict_GetItem(o, Py_BuildValue("s", "TextureDetectionModes"));
    PyObject * textFilterModes              = PyDict_GetItem(o, Py_BuildValue("s", "TextFilterModes"));
    PyObject * dpmCodeReadingModes          = PyDict_GetItem(o, Py_BuildValue("s", "DPMCodeReadingModes"));
    PyObject * deformationResistingModes    = PyDict_GetItem(o, Py_BuildValue("s", "DeformationResistingModes"));
    PyObject * barcodeComplementModes       = PyDict_GetItem(o, Py_BuildValue("s", "BarcodeComplementModes"));
    PyObject * barcodeColourModes           = PyDict_GetItem(o, Py_BuildValue("s", "BarcodeColourModes"));
    PyObject * textResultOrderModes         = PyDict_GetItem(o, Py_BuildValue("s", "TextResultOrderModes"));
	PyObject * accompanyingTextRecognitionModes = PyDict_GetItem(o, Py_BuildValue("s", "AccompanyingTextRecognitionModes"));
	PyObject * scaleUpModes					= PyDict_GetItem(o, Py_BuildValue("s", "ScaleUpModes"));

    for(int i = 0; i < 8; ++i)
    {
        pSettings.binarizationModes[i]                          = (BinarizationMode)(PyLong_AsLong(PyList_GetItem(binarizationModes, i)));
        pSettings.localizationModes[i]                          = (LocalizationMode)(PyLong_AsLong(PyList_GetItem(localizationModes, i)));
        pSettings.furtherModes.colourClusteringModes[i]         = (ColourClusteringMode)(PyLong_AsLong(PyList_GetItem(colourClusteringModes, i)));
        pSettings.furtherModes.colourConversionModes[i]         = (ColourConversionMode)(PyLong_AsLong(PyList_GetItem(colourConversionModes, i)));
        pSettings.furtherModes.grayscaleTransformationModes[i]  = (GrayscaleTransformationMode)(PyLong_AsLong(PyList_GetItem(grayscaleTransformationModes, i)));
        pSettings.furtherModes.regionPredetectionModes[i]       = (RegionPredetectionMode)(PyLong_AsLong(PyList_GetItem(regionPredetectionModes, i)));
        pSettings.furtherModes.imagePreprocessingModes[i]       = (ImagePreprocessingMode)(PyLong_AsLong(PyList_GetItem(imagePreprocessingModes, i)));
        pSettings.furtherModes.textureDetectionModes[i]         = (TextureDetectionMode)(PyLong_AsLong(PyList_GetItem(textureDetectionModes, i)));
        pSettings.furtherModes.textFilterModes[i]               = (TextFilterMode)(PyLong_AsLong(PyList_GetItem(textFilterModes, i)));
        pSettings.furtherModes.dpmCodeReadingModes[i]           = (DPMCodeReadingMode)(PyLong_AsLong(PyList_GetItem(dpmCodeReadingModes, i)));
        pSettings.furtherModes.deformationResistingModes[i]     = (DeformationResistingMode)(PyLong_AsLong(PyList_GetItem(deformationResistingModes, i)));
        pSettings.furtherModes.barcodeComplementModes[i]        = (BarcodeComplementMode)(PyLong_AsLong(PyList_GetItem(barcodeComplementModes, i)));
        pSettings.furtherModes.barcodeColourModes[i]            = (BarcodeColourMode)(PyLong_AsLong(PyList_GetItem(barcodeColourModes, i)));
        pSettings.textResultOrderModes[i]                       = (TextResultOrderMode)(PyLong_AsLong(PyList_GetItem(textResultOrderModes, i)));
		pSettings.furtherModes.accompanyingTextRecognitionModes[i] = (AccompanyingTextRecognitionMode)(PyLong_AsLong(PyList_GetItem(accompanyingTextRecognitionModes, i)));
		pSettings.scaleUpModes[i]								= (ScaleUpMode)(PyLong_AsLong(PyList_GetItem(scaleUpModes, i)));
	}

    pSettings.furtherModes.textAssistedCorrectionMode               = (TextAssistedCorrectionMode)(PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "TextAssistedCorrectionMode"))));
    pSettings.deblurLevel                                           = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "DeblurLevel")));
    pSettings.intermediateResultTypes                               = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "IntermediateResultTypes")));
    pSettings.intermediateResultSavingMode                          = (IntermediateResultSavingMode)(PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "IntermediateResultSavingMode"))));
    pSettings.resultCoordinateType                                  = (ResultCoordinateType)(PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "ResultCoordinateType"))));
    pSettings.returnBarcodeZoneClarity                              = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "ReturnBarcodeZoneClarity")));
    pSettings.region.regionTop                                      = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "RegionTop")));
    pSettings.region.regionBottom                                   = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "RegionBottom")));
    pSettings.region.regionLeft                                     = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "RegionLeft")));
    pSettings.region.regionRight                                    = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "RegionRight")));
    pSettings.region.regionMeasuredByPercentage                     = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "RegionMeasuredByPercentage")));
    pSettings.minBarcodeTextLength                                  = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "MinBarcodeTextLength")));
    pSettings.minResultConfidence                                   = PyLong_AsLong(PyDict_GetItem(o, Py_BuildValue("s", "MinResultConfidence")));

    return pSettings;
}

static PyObject * CreatePySamplingImageData(SamplingImageData samplingImage)
{
    //create SamplingImage
    PyObject * pySamplingImage = PyDict_New();
    if(pySamplingImage == NULL)
    { 
        return NULL;     
    }

    if(samplingImage.bytes != NULL)
    {
        PyObject * pySamplingImageBytes     = PyByteArray_FromStringAndSize(samplingImage.bytes, samplingImage.width * samplingImage.height);
        PyObject * pySamplingImageBytesKey  = Py_BuildValue("s", "Bytes");
        PyDict_SetItem(pySamplingImage, pySamplingImageBytesKey, pySamplingImageBytes);
        Py_DECREF(pySamplingImageBytes);
        Py_DECREF(pySamplingImageBytesKey);
    }
    else
    {
        //PyObject * pySamplingImageBytes     = Py_BuildValue("s", "NULL");
        PyObject * pySamplingImageBytesKey  = Py_BuildValue("s", "Bytes");
        PyDict_SetItem(pySamplingImage, pySamplingImageBytesKey, Py_None);
        //Py_DECREF(pySamplingImageBytes);
        Py_DECREF(pySamplingImageBytesKey);
    }

    PyObject * pySamplingImageWidth     = Py_BuildValue("i", samplingImage.width);
    PyObject * pySamplingImageWidthKey  = Py_BuildValue("s", "Width");
    PyDict_SetItem(pySamplingImage, pySamplingImageWidthKey, pySamplingImageWidth);
    Py_DECREF(pySamplingImageWidth);
    Py_DECREF(pySamplingImageWidthKey);

    PyObject * pySamplingImageHeight    = Py_BuildValue("i", samplingImage.height);
    PyObject * pySamplingImageHeightKey = Py_BuildValue("s", "Height");
    PyDict_SetItem(pySamplingImage, pySamplingImageHeightKey, pySamplingImageHeight);
    Py_DECREF(pySamplingImageHeight);
    Py_DECREF(pySamplingImageHeightKey);

    return pySamplingImage;
}

static PyObject * CreatePyDetailedResult(void * pResult, int format)
{
    PyObject * pyResult = PyDict_New();
    if(pyResult == NULL)
    {
        return NULL;     
    }

    if((format & BF_ONED) != 0)
    {
        PyObject * pyModuleSize         = Py_BuildValue("i", ((OneDCodeDetails *)pResult)->moduleSize);
        PyObject * pyModuleSizeKey      = Py_BuildValue("s", "ModuleSize");
        PyDict_SetItem(pyResult, pyModuleSizeKey, pyModuleSize);
        Py_DECREF(pyModuleSize);
        Py_DECREF(pyModuleSizeKey);

        if(((OneDCodeDetails *)pResult)->startCharsBytes != NULL)
        {
            PyObject * pyStartCharsBytes    = PyByteArray_FromStringAndSize(((OneDCodeDetails *)pResult)->startCharsBytes, ((OneDCodeDetails *)pResult)->startCharsBytesLength);
            PyObject * pyStartCharsBytesKey = Py_BuildValue("s", "StartCharsBytes");
            PyDict_SetItem(pyResult, pyStartCharsBytesKey, pyStartCharsBytes);
            Py_DECREF(pyStartCharsBytes);
            Py_DECREF(pyStartCharsBytesKey);

        }
        else
        {
            //PyObject * pyStartCharsBytes    = Py_BuildValue("o", NULL);
            PyObject * pyStartCharsBytesKey = Py_BuildValue("s", "StartCharsBytes");
            PyDict_SetItem(pyResult, pyStartCharsBytesKey, Py_None);
            //Py_DECREF(pyStartCharsBytes);
            Py_DECREF(pyStartCharsBytesKey);
        }

        if(((OneDCodeDetails *)pResult)->stopCharsBytes != NULL)
        {
            PyObject * pyStopCharsBytes     = PyByteArray_FromStringAndSize(((OneDCodeDetails *)pResult)->stopCharsBytes, ((OneDCodeDetails *)pResult)->stopCharsBytesLength);
            PyObject * pyStopCharsBytesKey  = Py_BuildValue("s", "StopCharsBytes");
            PyDict_SetItem(pyResult, pyStopCharsBytesKey, pyStopCharsBytes);
            Py_DECREF(pyStopCharsBytes);
            Py_DECREF(pyStopCharsBytesKey);
        }
        else
        {
            //PyObject * pyStopCharsBytes     = Py_BuildValue("o", NULL);
            PyObject * pyStopCharsBytesKey  = Py_BuildValue("s", "StopCharsBytes");
            PyDict_SetItem(pyResult, pyStopCharsBytesKey, Py_None);
            //Py_DECREF(pyStopCharsBytes);
            Py_DECREF(pyStopCharsBytesKey);
        }
        

        if(((OneDCodeDetails *)pResult)->checkDigitBytes != NULL)
        {
            PyObject * pyCheckDigitBytes    = PyByteArray_FromStringAndSize(((OneDCodeDetails *)pResult)->checkDigitBytes, ((OneDCodeDetails *)pResult)->checkDigitBytesLength);
            PyObject * pyCheckDigitBytesKey = Py_BuildValue("s", "CheckDigitBytes");
            PyDict_SetItem(pyResult, pyCheckDigitBytesKey, pyCheckDigitBytes);
            Py_DECREF(pyCheckDigitBytes);
            Py_DECREF(pyCheckDigitBytesKey);
        }
        else
        {
            //PyObject * pyCheckDigitBytes    = Py_BuildValue("o", NULL);
            PyObject * pyCheckDigitBytesKey = Py_BuildValue("s", "CheckDigitBytes");
            PyDict_SetItem(pyResult, pyCheckDigitBytesKey, Py_None);
            //Py_DECREF(pyCheckDigitBytes);
            Py_DECREF(pyCheckDigitBytesKey);
        }
    }
    else if(format == BF_QR_CODE)
    {
        PyObject * pyModuleSize         = Py_BuildValue("i", ((QRCodeDetails *)pResult)->moduleSize);
        PyObject * pyModuleSizeKey      = Py_BuildValue("s", "ModuleSize");
        PyDict_SetItem(pyResult, pyModuleSizeKey, pyModuleSize);
        Py_DECREF(pyModuleSize);
        Py_DECREF(pyModuleSizeKey);

        PyObject * pyRows               = Py_BuildValue("i", ((QRCodeDetails *)pResult)->rows);
        PyObject * pyRowsKey            = Py_BuildValue("s", "Rows");
        PyDict_SetItem(pyResult, pyRowsKey, pyRows);
        Py_DECREF(pyRows);
        Py_DECREF(pyRowsKey);

        PyObject * pyColumns            = Py_BuildValue("i", ((QRCodeDetails *)pResult)->columns);
        PyObject * pyColumnsKey         = Py_BuildValue("s", "Columns");
        PyDict_SetItem(pyResult, pyColumnsKey, pyColumns);
        Py_DECREF(pyColumns);
        Py_DECREF(pyColumnsKey);
        
        PyObject * pyErrorCorrectionLevel    = Py_BuildValue("i", ((QRCodeDetails *)pResult)->errorCorrectionLevel);
        PyObject * pyErrorCorrectionLevelKey = Py_BuildValue("s", "ErrorCorrectionLevel");
        PyDict_SetItem(pyResult, pyErrorCorrectionLevelKey, pyErrorCorrectionLevel);
        Py_DECREF(pyErrorCorrectionLevel);
        Py_DECREF(pyErrorCorrectionLevelKey);
        
        PyObject * pyVersion            = Py_BuildValue("i", ((QRCodeDetails *)pResult)->version);
        PyObject * pyVersionKey         = Py_BuildValue("s", "Version");
        PyDict_SetItem(pyResult, pyVersionKey, pyVersion);
        Py_DECREF(pyVersion);
        Py_DECREF(pyVersionKey);

        PyObject * pyModel              = Py_BuildValue("i", ((QRCodeDetails *)pResult)->model);
        PyObject * pyModelKey           = Py_BuildValue("s", "Model");
        PyDict_SetItem(pyResult, pyModelKey, pyModel);
        Py_DECREF(pyModel);
        Py_DECREF(pyModelKey);
    }
    else if(format == BF_DATAMATRIX)
    {
        PyObject * pyModuleSize         = Py_BuildValue("i", ((DataMatrixDetails *)pResult)->moduleSize);
        PyObject * pyModuleSizeKey      = Py_BuildValue("s", "ModuleSize");
        PyDict_SetItem(pyResult, pyModuleSizeKey, pyModuleSize);
        Py_DECREF(pyModuleSize);
        Py_DECREF(pyModuleSizeKey);

        PyObject * pyRows               = Py_BuildValue("i", ((DataMatrixDetails *)pResult)->rows);
        PyObject * pyRowsKey            = Py_BuildValue("s", "Rows");
        PyDict_SetItem(pyResult, pyRowsKey, pyRows);
        Py_DECREF(pyRows);
        Py_DECREF(pyRowsKey);

        PyObject * pyColumns            = Py_BuildValue("i", ((DataMatrixDetails *)pResult)->columns);
        PyObject * pyColumnsKey         = Py_BuildValue("s", "Columns");
        PyDict_SetItem(pyResult, pyColumnsKey, pyColumns); 
        Py_DECREF(pyColumns);
        Py_DECREF(pyColumnsKey);

        PyObject * pyDataRegionRows     = Py_BuildValue("i", ((DataMatrixDetails *)pResult)->dataRegionRows);
        PyObject * pyDataRegionRowsKey  = Py_BuildValue("s", "DataRegionRows");
        PyDict_SetItem(pyResult, pyDataRegionRowsKey, pyDataRegionRows);
        Py_DECREF(pyDataRegionRows);
        Py_DECREF(pyDataRegionRowsKey);

        PyObject * pyDataRegionColumns  = Py_BuildValue("i", ((DataMatrixDetails *)pResult)->dataRegionColumns);
        PyObject * pyDataRegionColumnsKey = Py_BuildValue("s", "DataRegionColumns");
        PyDict_SetItem(pyResult, pyDataRegionColumnsKey, pyDataRegionColumns);
        Py_DECREF(pyDataRegionColumns);
        Py_DECREF(pyDataRegionColumnsKey);

        PyObject * pyDataRegionNumber   = Py_BuildValue("i", ((DataMatrixDetails *)pResult)->dataRegionNumber);
        PyObject * pyDataRegionNumberKey= Py_BuildValue("s", "DataRegionNumber");
        PyDict_SetItem(pyResult, pyDataRegionNumberKey, pyDataRegionNumber); 
        Py_DECREF(pyDataRegionNumber);
        Py_DECREF(pyDataRegionNumberKey);
    }
    else if(format == BF_PDF417)
    {
        PyObject * pyModuleSize         = Py_BuildValue("i", ((PDF417Details *)pResult)->moduleSize);
        PyObject * pyModuleSizeKey      = Py_BuildValue("s", "ModuleSize");
        PyDict_SetItem(pyResult, pyModuleSizeKey, pyModuleSize);
        Py_DECREF(pyModuleSize);
        Py_DECREF(pyModuleSizeKey);

        PyObject * pyRows               = Py_BuildValue("i", ((PDF417Details *)pResult)->rows);
        PyObject * pyRowsKey            = Py_BuildValue("s", "Rows");
        PyDict_SetItem(pyResult, pyRowsKey, pyRows);
        Py_DECREF(pyRows);
        Py_DECREF(pyRowsKey);

        PyObject * pyColumns            = Py_BuildValue("i", ((PDF417Details *)pResult)->columns);
        PyObject * pyColumnsKey         = Py_BuildValue("s", "Columns");
        PyDict_SetItem(pyResult, pyColumnsKey, pyColumns);
        Py_DECREF(pyColumns);
        Py_DECREF(pyColumnsKey);
        
        PyObject * pyErrorCorrectionLevel    = Py_BuildValue("i", ((PDF417Details *)pResult)->errorCorrectionLevel);
        PyObject * pyErrorCorrectionLevelKey = Py_BuildValue("s", "ErrorCorrectionLevel");
        PyDict_SetItem(pyResult, pyErrorCorrectionLevelKey, pyErrorCorrectionLevel);
        Py_DECREF(pyErrorCorrectionLevel);
        Py_DECREF(pyErrorCorrectionLevelKey);
    }
    else if(format == BF_AZTEC)
    {
        PyObject * pyModuleSize         = Py_BuildValue("i", ((AztecDetails *)pResult)->moduleSize);
        PyObject * pyModuleSizeKey      = Py_BuildValue("s", "ModuleSize");
        PyDict_SetItem(pyResult, pyModuleSizeKey, pyModuleSize);
        Py_DECREF(pyModuleSize);
        Py_DECREF(pyModuleSizeKey);

        PyObject * pyRows               = Py_BuildValue("i", ((AztecDetails *)pResult)->rows);
        PyObject * pyRowsKey            = Py_BuildValue("s", "Rows");
        PyDict_SetItem(pyResult, pyRowsKey, pyRows);
        Py_DECREF(pyRows);
        Py_DECREF(pyRowsKey);

        PyObject * pyColumns            = Py_BuildValue("i", ((AztecDetails *)pResult)->columns);
        PyObject * pyColumnsKey         = Py_BuildValue("s", "Columns");
        PyDict_SetItem(pyResult, pyColumnsKey, pyColumns); 
        Py_DECREF(pyColumns);
        Py_DECREF(pyColumnsKey);

        PyObject * pyLayerNumber        = Py_BuildValue("i", ((AztecDetails *)pResult)->layerNumber);
        PyObject * pyLayerNumberKey     = Py_BuildValue("s", "LayerNumber");
        PyDict_SetItem(pyResult, pyLayerNumberKey, pyLayerNumber);
        Py_DECREF(pyLayerNumber);
        Py_DECREF(pyLayerNumberKey);
    }
    return pyResult;
}

static PyObject * CreatePyLocalizationResult(LocalizationResult * pResult)
{
    PyObject * pyResult = PyDict_New();
    if(pyResult == NULL)
    {  
        return NULL;     
    }

    PyObject * pyTerminatePhase         = Py_BuildValue("i", pResult->terminatePhase);
    PyObject * pyTerminatePhaseKey      = Py_BuildValue("s", "TerminatePhase");
    PyDict_SetItem(pyResult, pyTerminatePhaseKey, pyTerminatePhase);
    Py_DECREF(pyTerminatePhase);
    Py_DECREF(pyTerminatePhaseKey);

    PyObject * pyBarcodeFormat          = Py_BuildValue("i", pResult->barcodeFormat);
    PyObject * pyBarcodeFormatKey       = Py_BuildValue("s", "BarcodeFormat");
    PyDict_SetItem(pyResult, pyBarcodeFormatKey, pyBarcodeFormat);
    Py_DECREF(pyBarcodeFormat);
    Py_DECREF(pyBarcodeFormatKey);

    if(pResult->barcodeFormatString != NULL)
    {
        PyObject *pyBarcodeFormatString     = Py_BuildValue("s", pResult->barcodeFormatString);
        PyObject * pyBarcodeFormatStringKey = Py_BuildValue("s", "BarcodeFormatString");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey, pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatStringKey);
    }
    else
    {
        //PyObject *pyBarcodeFormatString     = Py_BuildValue("s", "NULL");
        PyObject * pyBarcodeFormatStringKey = Py_BuildValue("s", "BarcodeFormatString");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey, Py_None);
        //Py_DECREF(pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatStringKey);
    }
    

    PyObject * pyBarcodeFormat_2        = Py_BuildValue("i", pResult->barcodeFormat_2);
    PyObject * pyBarcodeFormatKey_2     = Py_BuildValue("s", "BarcodeFormat_2");
    PyDict_SetItem(pyResult, pyBarcodeFormatKey_2, pyBarcodeFormat_2);
    Py_DECREF(pyBarcodeFormat_2);
    Py_DECREF(pyBarcodeFormatKey_2);

    if(pResult->barcodeFormatString_2 != NULL)
    {
        PyObject * pyBarcodeFormatString_2    = Py_BuildValue("s", pResult->barcodeFormatString_2);
        PyObject * pyBarcodeFormatStringKey_2 = Py_BuildValue("s", "BarcodeFormatString_2");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey_2, pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatStringKey_2);
    }
    else
    {
        //PyObject * pyBarcodeFormatString_2    = Py_BuildValue("s", "NULL");
        PyObject * pyBarcodeFormatStringKey_2 = Py_BuildValue("s", "BarcodeFormatString_2");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey_2, Py_None);
        //Py_DECREF(pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatStringKey_2);
    }
    

    PyObject * pyX1                     = Py_BuildValue("i", pResult->x1);
    PyObject * pyX1Key                  = Py_BuildValue("s", "X1");
    PyDict_SetItem(pyResult, pyX1Key, pyX1);
    Py_DECREF(pyX1);
    Py_DECREF(pyX1Key);

    PyObject * pyY1                     = Py_BuildValue("i", pResult->y1);
    PyObject * pyY1Key                  = Py_BuildValue("s", "Y1");
    PyDict_SetItem(pyResult, pyY1Key, pyY1);
    Py_DECREF(pyY1);
    Py_DECREF(pyY1Key);

    PyObject * pyX2                     = Py_BuildValue("i", pResult->x2);
    PyObject * pyX2Key                  = Py_BuildValue("s", "X2");
    PyDict_SetItem(pyResult, pyX2Key, pyX2);
    Py_DECREF(pyX2);
    Py_DECREF(pyX2Key);

    PyObject * pyY2                     = Py_BuildValue("i", pResult->y2);
    PyObject * pyY2Key                  = Py_BuildValue("s", "Y2");
    PyDict_SetItem(pyResult, pyY2Key, pyY2);
    Py_DECREF(pyY2);
    Py_DECREF(pyY2Key);

    PyObject * pyX3                     = Py_BuildValue("i", pResult->x3);
    PyObject * pyX3Key                  = Py_BuildValue("s", "X3");
    PyDict_SetItem(pyResult, pyX3Key, pyX3);
    Py_DECREF(pyX3);
    Py_DECREF(pyX3Key);

    PyObject * pyY3                     = Py_BuildValue("i", pResult->y3);
    PyObject * pyY3Key                  = Py_BuildValue("s", "Y3");
    PyDict_SetItem(pyResult, pyY3Key, pyY3);
    Py_DECREF(pyY3);
    Py_DECREF(pyY3Key);

    PyObject * pyX4                     = Py_BuildValue("i", pResult->x4);
    PyObject * pyX4Key                  = Py_BuildValue("s", "X4");
    PyDict_SetItem(pyResult, pyX4Key, pyX4);
    Py_DECREF(pyX4);
    Py_DECREF(pyX4Key);

    PyObject * pyY4                     = Py_BuildValue("i", pResult->y4);
    PyObject * pyY4Key                  = Py_BuildValue("s", "Y4");
    PyDict_SetItem(pyResult, pyY4Key, pyY4);
    Py_DECREF(pyY4);
    Py_DECREF(pyY4Key);

    PyObject * pyAngle                  = Py_BuildValue("i", pResult->angle);
    PyObject * pyAngleKey               = Py_BuildValue("s", "Angle");
    PyDict_SetItem(pyResult, pyAngleKey, pyAngle);
    Py_DECREF(pyAngle);
    Py_DECREF(pyAngleKey);

    PyObject * pyModuleSize             = Py_BuildValue("i", pResult->moduleSize);
    PyObject * pyModuleSizeKey          = Py_BuildValue("s", "ModuleSize");
    PyDict_SetItem(pyResult, pyModuleSizeKey, pyModuleSize);
    Py_DECREF(pyModuleSize);
    Py_DECREF(pyModuleSizeKey);

    PyObject * pyPageNumber             = Py_BuildValue("i", pResult->pageNumber);
    PyObject * pyPageNumberKey          = Py_BuildValue("s", "PageNumber");
    PyDict_SetItem(pyResult, pyPageNumberKey, pyPageNumber);
    Py_DECREF(pyPageNumber);
    Py_DECREF(pyPageNumberKey);

    if(pResult->regionName != NULL)
    {
        PyObject * pyRegionName             = Py_BuildValue("s", pResult->regionName);
        PyObject * pyRegionNameKey          = Py_BuildValue("s", "RegionName");
        PyDict_SetItem(pyResult, pyRegionNameKey, pyRegionName);
        Py_DECREF(pyRegionName);
        Py_DECREF(pyRegionNameKey);
    }
    else
    {
        PyObject * pyRegionName             = Py_BuildValue("s", "NULL");
        PyObject * pyRegionNameKey          = Py_BuildValue("s", "RegionName");
        PyDict_SetItem(pyResult, pyRegionNameKey, pyRegionName);
        Py_DECREF(pyRegionName);
        Py_DECREF(pyRegionNameKey);
    }
    

    if(pResult->documentName != NULL)
    {
        PyObject * pyDocumentName           = Py_BuildValue("s", pResult->documentName);
        PyObject * pyDocumentNameKey        = Py_BuildValue("s", "DocumentName");
        PyDict_SetItem(pyResult, pyDocumentNameKey, pyDocumentName);
        Py_DECREF(pyDocumentName);
        Py_DECREF(pyDocumentNameKey);
    }
    else
    {
        PyObject * pyDocumentName           = Py_BuildValue("s", "NULL");
        PyObject * pyDocumentNameKey        = Py_BuildValue("s", "DocumentName");
        PyDict_SetItem(pyResult, pyDocumentNameKey, pyDocumentName);
        Py_DECREF(pyDocumentName);
        Py_DECREF(pyDocumentNameKey);
    }
    

    PyObject * pyResultCoordinateType   = Py_BuildValue("i", pResult->resultCoordinateType);
    PyObject * pyResultCoordinateTypeKey= Py_BuildValue("s", "ResultCoordinateType");
    PyDict_SetItem(pyResult, pyResultCoordinateTypeKey, pyResultCoordinateType);
    Py_DECREF(pyResultCoordinateType);
    Py_DECREF(pyResultCoordinateTypeKey);

    if(!(pResult->accompanyingTextBytes == NULL || pResult->accompanyingTextBytesLength == 0))
    {
        PyObject * pyAccompanyingTextBytes    = PyByteArray_FromStringAndSize(pResult->accompanyingTextBytes, pResult->accompanyingTextBytesLength);
        PyObject * pyAccompanyingTextBytesKey = Py_BuildValue("s", "AccompanyingTextBytes");
        PyDict_SetItem(pyResult, pyAccompanyingTextBytesKey, pyAccompanyingTextBytes);
        Py_DECREF(pyAccompanyingTextBytes);
        Py_DECREF(pyAccompanyingTextBytesKey);
    }
    else
    {
        //PyObject * pyAccompanyingTextBytes    = Py_BuildValue("o", NULL);
        PyObject * pyAccompanyingTextBytesKey = Py_BuildValue("s", "AccompanyingTextBytes");
        PyDict_SetItem(pyResult, pyAccompanyingTextBytesKey, Py_None);
        //Py_DECREF(pyAccompanyingTextBytes);
        Py_DECREF(pyAccompanyingTextBytesKey);
    }
    

    PyObject * pyConfidence             = Py_BuildValue("i", pResult->confidence);
    PyObject * pyConfidenceKey          = Py_BuildValue("s", "Confidence");
    PyDict_SetItem(pyResult, pyConfidenceKey, pyConfidence);
    Py_DECREF(pyConfidence);
    Py_DECREF(pyConfidenceKey);

    return pyResult;
}

static PyObject * CreatePyExtendedResult(ExtendedResult * pResult)
{
    PyObject * pyResult = PyDict_New();
    if(pyResult == NULL)
    { 
        return NULL;     
    }

    PyObject * pyResultType             = Py_BuildValue("i", pResult->resultType);
    PyObject * pyResultTypeKey          = Py_BuildValue("s", "ResultType");
    PyDict_SetItem(pyResult, pyResultTypeKey, pyResultType);
    Py_DECREF(pyResultType);
    Py_DECREF(pyResultTypeKey);

    PyObject * pyBarcodeFormat          = Py_BuildValue("i", pResult->barcodeFormat);
    PyObject * pyBarcodeFormatKey       = Py_BuildValue("s", "BarcodeFormat");
    PyDict_SetItem(pyResult, pyBarcodeFormatKey, pyBarcodeFormat);
    Py_DECREF(pyBarcodeFormat);
    Py_DECREF(pyBarcodeFormatKey);

    if(pResult->barcodeFormatString != NULL)
    {
        PyObject * pyBarcodeFormatString    = Py_BuildValue("s", pResult->barcodeFormatString);
        PyObject * pyBarcodeFormatStringKey = Py_BuildValue("s", "BarcodeFormatString");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey, pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatStringKey);
    }
    else
    {
        PyObject * pyBarcodeFormatString    = Py_BuildValue("s", "NULL");
        PyObject * pyBarcodeFormatStringKey = Py_BuildValue("s", "BarcodeFormatString");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey, pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatString);
        Py_DECREF(pyBarcodeFormatStringKey);
    }
    

    PyObject * pyBarcodeFormat_2        = Py_BuildValue("i", pResult->barcodeFormat_2);
    PyObject * pyBarcodeFormatKey_2     = Py_BuildValue("s", "BarcodeFormat_2");
    PyDict_SetItem(pyResult, pyBarcodeFormatKey_2, pyBarcodeFormat_2);
    Py_DECREF(pyBarcodeFormat_2);
    Py_DECREF(pyBarcodeFormatKey_2);

    if(pResult->barcodeFormatString_2 != NULL)
    {
        PyObject * pyBarcodeFormatString_2    = Py_BuildValue("s", pResult->barcodeFormatString_2);
        PyObject * pyBarcodeFormatStringKey_2 = Py_BuildValue("s", "BarcodeFormatString_2");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey_2, pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatStringKey_2);
    }
    else
    {
        PyObject * pyBarcodeFormatString_2    = Py_BuildValue("s", "NULL");
        PyObject * pyBarcodeFormatStringKey_2 = Py_BuildValue("s", "BarcodeFormatString_2");
        PyDict_SetItem(pyResult, pyBarcodeFormatStringKey_2, pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatString_2);
        Py_DECREF(pyBarcodeFormatStringKey_2);
    }

    PyObject * pyConfidence             = Py_BuildValue("i",pResult->confidence);
    PyObject * pyConfidenceKey          = Py_BuildValue("s", "Confidence");
    PyDict_SetItem(pyResult, pyConfidenceKey, pyConfidence);
    Py_DECREF(pyConfidence);
    Py_DECREF(pyConfidenceKey);

    if(pResult->bytes != NULL)
    {
        PyObject * pyBytes                  = PyByteArray_FromStringAndSize(pResult->bytes, pResult->bytesLength);
        PyObject * pyBytesKey               = Py_BuildValue("s", "Bytes");
        PyDict_SetItem(pyResult, pyBytesKey, pyBytes);
        Py_DECREF(pyBytes);
        Py_DECREF(pyBytesKey);
    }
    else
    {
        //PyObject * pyBytes                  = Py_BuildValue("o", NULL);
        PyObject * pyBytesKey               = Py_BuildValue("s", "Bytes");
        PyDict_SetItem(pyResult, pyBytesKey, Py_None);
        //Py_DECREF(pyBytes);
        Py_DECREF(pyBytesKey);
    }

    if(pResult->accompanyingTextBytes != NULL)
    {
        PyObject * pyAccompanyingTextBytes    = PyByteArray_FromStringAndSize(pResult->accompanyingTextBytes, pResult->accompanyingTextBytesLength);
        PyObject * pyAccompanyingTextBytesKey = Py_BuildValue("s", "AccompanyingTextBytes");
        PyDict_SetItem(pyResult, pyAccompanyingTextBytesKey, pyAccompanyingTextBytes);
		Py_DECREF(pyAccompanyingTextBytes);
		Py_DECREF(pyAccompanyingTextBytesKey);
    }
    else
    {
        //PyObject * pyAccompanyingTextBytes    = Py_BuildValue("o", NULL);
        PyObject * pyAccompanyingTextBytesKey = Py_BuildValue("s", "AccompanyingTextBytes");
        PyDict_SetItem(pyResult, pyAccompanyingTextBytesKey, Py_None);
        //Py_DECREF(pyAccompanyingTextBytes);
        Py_DECREF(pyAccompanyingTextBytesKey);
    }
    
    PyObject * pyDeformation            = Py_BuildValue("i", pResult->deformation);
    PyObject * pyDeformationKey         = Py_BuildValue("s", "Deformation");
    PyDict_SetItem(pyResult, pyDeformationKey, pyDeformation);
    Py_DECREF(pyDeformation);
    Py_DECREF(pyDeformationKey);

    if(pResult->detailedResult != NULL)
    {
        PyObject * pyDetailedResult         = CreatePyDetailedResult(pResult->detailedResult, pResult->barcodeFormat);
        PyObject * pyDetailedResultKey      = Py_BuildValue("s", "DetailedResult");
        PyDict_SetItem(pyResult, pyDetailedResultKey, pyDetailedResult);
        Py_DECREF(pyDetailedResult);
        Py_DECREF(pyDetailedResultKey);
    }
    else
    {
        //PyObject * pyDetailedResult         = Py_BuildValue("o", NULL);
        PyObject * pyDetailedResultKey      = Py_BuildValue("s", "DetailedResult");
        PyDict_SetItem(pyResult, pyDetailedResultKey, Py_None);
        //Py_DECREF(pyDetailedResult);
        Py_DECREF(pyDetailedResultKey);
    }
    
    
    PyObject * pySamplingImage          = CreatePySamplingImageData(pResult->samplingImage);
	if (pySamplingImage != NULL)
	{
		PyObject * pySamplingImageKey = Py_BuildValue("s", "SamplingImage");
		PyDict_SetItem(pyResult, pySamplingImageKey, pySamplingImage);
		Py_DECREF(pySamplingImage);
		Py_DECREF(pySamplingImageKey);
	}
	else
	{
		PyObject * pySamplingImageKey = Py_BuildValue("s", "SamplingImage");
		PyDict_SetItem(pyResult, pySamplingImageKey, Py_None);
		Py_DECREF(pySamplingImage);
		Py_DECREF(pySamplingImageKey);
	}

    PyObject * pyClarity                = Py_BuildValue("i",pResult->clarity);
    PyObject * pyClarityKey             = Py_BuildValue("s", "Clarity");
    PyDict_SetItem(pyResult, pyClarityKey, pyClarity);
    Py_DECREF(pyClarity);
    Py_DECREF(pyClarityKey);

    return pyResult;
}

static PyObject * CreatePyTextResults(TextResultArray *pResults)
{
    // Get barcode results
    int count = pResults->resultsCount;

    // Create a Python object to store results
    PyObject *pyTextResults = PyList_New(count);
	if (pyTextResults == NULL)
	{
		return NULL;
	}

    for (int i = 0; i < count; i++)
    {
        PyObject * pyTextResult = PyDict_New();
        if(pyTextResult == NULL)
        {
            return NULL;     
        }

        PyObject * pyBarcodeFormat      = Py_BuildValue("i", pResults->results[i]->barcodeFormat);
        PyObject * pyBarcodeFormatKey   = Py_BuildValue("s", "BarcodeFormat");
        PyDict_SetItem(pyTextResult, pyBarcodeFormatKey, pyBarcodeFormat);
        Py_DECREF(pyBarcodeFormat);
        Py_DECREF(pyBarcodeFormatKey);

        if(pResults->results[i]->barcodeFormatString != NULL)
        {
            PyObject * pyBarcodeFormatString    = Py_BuildValue("s", pResults->results[i]->barcodeFormatString);
            PyObject * pyBarcodeFormatStringKey = Py_BuildValue("s", "BarcodeFormatString");
            PyDict_SetItem(pyTextResult, pyBarcodeFormatStringKey, pyBarcodeFormatString);
            Py_DECREF(pyBarcodeFormatString);
            Py_DECREF(pyBarcodeFormatStringKey);
        }
        else
        {
            PyObject * pyBarcodeFormatString    = Py_BuildValue("s", "NULL");
            PyObject * pyBarcodeFormatStringKey = Py_BuildValue("s", "BarcodeFormatString");
            PyDict_SetItem(pyTextResult, pyBarcodeFormatStringKey, pyBarcodeFormatString);
            Py_DECREF(pyBarcodeFormatString);
            Py_DECREF(pyBarcodeFormatStringKey);
        }    

        PyObject * pyBarcodeFormat_2    = Py_BuildValue("i", pResults->results[i]->barcodeFormat_2);
        PyObject * pyBarcodeFormatKey_2 = Py_BuildValue("s", "BarcodeFormat_2");
        PyDict_SetItem(pyTextResult, pyBarcodeFormatKey_2, pyBarcodeFormat_2);
        Py_DECREF(pyBarcodeFormat_2);
        Py_DECREF(pyBarcodeFormatKey_2);

        if(pResults->results[i]->barcodeFormatString_2 != NULL)
        {
            PyObject * pyBarcodeFormatString_2    = Py_BuildValue("s", pResults->results[i]->barcodeFormatString_2);
            PyObject * pyBarcodeFormatStringKey_2 = Py_BuildValue("s", "BarcodeFormatString_2");
            PyDict_SetItem(pyTextResult, pyBarcodeFormatStringKey_2, pyBarcodeFormatString_2);
            Py_DECREF(pyBarcodeFormatString_2);
            Py_DECREF(pyBarcodeFormatStringKey_2);
        }
        else
        {
            PyObject * pyBarcodeFormatString_2    = Py_BuildValue("s", "NULL");
            PyObject * pyBarcodeFormatStringKey_2 = Py_BuildValue("s", "BarcodeFormatString_2");
            PyDict_SetItem(pyTextResult, pyBarcodeFormatStringKey_2, pyBarcodeFormatString_2);
            Py_DECREF(pyBarcodeFormatString_2);
            Py_DECREF(pyBarcodeFormatStringKey_2);
        }

        if(pResults->results[i]->barcodeText != NULL)
        {
            PyObject * pyBarcodeText        = Py_BuildValue("s", pResults->results[i]->barcodeText);
            if(pyBarcodeText == NULL)
            {
                PyErr_SetString(PyExc_TypeError, "Incorrect character set! Failed to decode barcode results!");
                return NULL;
            }
            PyObject * pyBarcodeTextKey     = Py_BuildValue("s", "BarcodeText");
            PyDict_SetItem(pyTextResult, pyBarcodeTextKey, pyBarcodeText);
            Py_DECREF(pyBarcodeText);
            Py_DECREF(pyBarcodeTextKey);
        }
        else
        {
            PyObject * pyBarcodeText        = Py_BuildValue("s", "NULL");
            PyObject * pyBarcodeTextKey     = Py_BuildValue("s", "BarcodeText");
            PyDict_SetItem(pyTextResult, pyBarcodeTextKey, pyBarcodeText);
            Py_DECREF(pyBarcodeText);
            Py_DECREF(pyBarcodeTextKey);
        }

        if(!(pResults->results[i]->barcodeBytes == NULL || pResults->results[i]->barcodeBytesLength == 0))
        {
            PyObject * pyBarcodeBytes       = PyByteArray_FromStringAndSize(pResults->results[i]->barcodeBytes, pResults->results[i]->barcodeBytesLength);
            PyObject * pyBarcodeBytesKey    = Py_BuildValue("s", "BarcodeBytes");
            PyDict_SetItem(pyTextResult, pyBarcodeBytesKey, pyBarcodeBytes);
            Py_DECREF(pyBarcodeBytes);
            Py_DECREF(pyBarcodeBytesKey);
        }
        else
        {
            //PyObject * pyBarcodeBytes       = Py_BuildValue("o", NULL);
            PyObject * pyBarcodeBytesKey    = Py_BuildValue("s", "BarcodeBytes");
            PyDict_SetItem(pyTextResult, pyBarcodeBytesKey, Py_None);
            //Py_DECREF(pyBarcodeBytes);
            Py_DECREF(pyBarcodeBytesKey);
        }
        
        if(pResults->results[i]->localizationResult != NULL)
        {
            PyObject * pyLocalizationResult     = CreatePyLocalizationResult(pResults->results[i]->localizationResult);
            PyObject * pyLocalizationResultKey  = Py_BuildValue("s", "LocalizationResult");
            PyDict_SetItem(pyTextResult, pyLocalizationResultKey, pyLocalizationResult);
            Py_DECREF(pyLocalizationResult);
            Py_DECREF(pyLocalizationResultKey);
        }
        else
        {
            //PyObject * pyLocalizationResult     = Py_BuildValue("o", NULL);
            PyObject * pyLocalizationResultKey  = Py_BuildValue("s", "LocalizationResult");
            PyDict_SetItem(pyTextResult, pyLocalizationResultKey, Py_None);
            //Py_DECREF(pyLocalizationResult);
            Py_DECREF(pyLocalizationResultKey);
        }

        if(pResults->results[i]->detailedResult != NULL)
        {
            PyObject * pyDetailedResult     = CreatePyDetailedResult(pResults->results[i]->detailedResult, pResults->results[i]->barcodeFormat);
            PyObject * pyDetailedResultKey  = Py_BuildValue("s", "DetailedResult");
            PyDict_SetItem(pyTextResult, pyDetailedResultKey, pyDetailedResult);
            Py_DECREF(pyDetailedResult);
            Py_DECREF(pyDetailedResultKey);
        }
        else
        {
            //PyObject * pyDetailedResult     = Py_BuildValue("o", NULL);
            PyObject * pyDetailedResultKey  = Py_BuildValue("s", "DetailedResult");
            PyDict_SetItem(pyTextResult, pyDetailedResultKey, Py_None);
            //Py_DECREF(pyDetailedResult);
            Py_DECREF(pyDetailedResultKey);
        }

        if(pResults->results[i]->resultsCount != 0 && pResults->results[i]->results != NULL)
        {
            PyObject * pyExtendedResults    = PyList_New(pResults->results[i]->resultsCount);
            for(int j = 0; j < pResults->results[i]->resultsCount; ++j)
            {
                PyObject * pyExtendedResult = CreatePyExtendedResult(pResults->results[i]->results[j]);
                PyList_SetItem(pyExtendedResults, j, pyExtendedResult);
            }
            PyObject * pyExtendedResultsKey = Py_BuildValue("s", "ExtendedResults");
            PyDict_SetItem(pyTextResult, pyExtendedResultsKey, pyExtendedResults);
            Py_DECREF(pyExtendedResults);
            Py_DECREF(pyExtendedResultsKey);
        }
        else
        {
            //PyObject * pyExtendedResults = Py_BuildValue("o", NULL);
            PyObject * pyExtendedResultsKey = Py_BuildValue("s", "ExtendedResults");
            PyDict_SetItem(pyTextResult, pyExtendedResultsKey, Py_None);
            //Py_DECREF(pyExtendedResults);
            Py_DECREF(pyExtendedResultsKey);
        }

        PyList_SetItem(pyTextResults, i, pyTextResult);
    }

    // Release memory
    // DBR_FreeTextResults(&pResults);

    return pyTextResults;
}

static PyObject * CreateIntermediateResultDatas(const void** ppResults, int count, IMResultDataType dataType)
{
    PyObject * pyResults = PyList_New(count);
	if (pyResults == NULL)
	{
		return NULL;
	}

    for(int i = 0; i < count; ++i)
    {
        if(dataType == IMRDT_IMAGE)
        {
            PyObject * pyImageData = PyDict_New();

            if(((ImageData *)(ppResults[i]))->bytes == NULL)
            {
                PyObject * pyBytes       = PyByteArray_FromStringAndSize(((ImageData *)(ppResults[i]))->bytes, ((ImageData *)(ppResults[i]))->bytesLength);
                PyObject * pyBytesKey    = Py_BuildValue("s", "Bytes");
                PyDict_SetItem(pyImageData, pyBytesKey, pyBytes);
                Py_DECREF(pyBytes);
                Py_DECREF(pyBytesKey);
            }
            else
            {
                //PyObject * pyBytes       = Py_BuildValue("o", NULL);
                PyObject * pyBytesKey    = Py_BuildValue("s", "Bytes");
                PyDict_SetItem(pyImageData, pyBytesKey, Py_None);
                //Py_DECREF(pyBytes);
                Py_DECREF(pyBytesKey);
            }
            

            PyObject * pyWidth      = Py_BuildValue("i", ((ImageData *)(ppResults[i]))->width);
            PyObject * pyWidthKey   = Py_BuildValue("s", "Width");
            PyDict_SetItem(pyImageData, pyWidthKey, pyWidth);
            Py_DECREF(pyWidth);
            Py_DECREF(pyWidthKey);

            PyObject * pyHeight      = Py_BuildValue("i", ((ImageData *)(ppResults[i]))->height);
            PyObject * pyHeightKey   = Py_BuildValue("s", "Height");
            PyDict_SetItem(pyImageData, pyHeightKey, pyHeight);
            Py_DECREF(pyHeight);
            Py_DECREF(pyHeightKey);

            PyObject * pyStride      = Py_BuildValue("i", ((ImageData *)(ppResults[i]))->stride);
            PyObject * pyStrideKey   = Py_BuildValue("s", "Stride");
            PyDict_SetItem(pyImageData, pyStrideKey, pyStride);
            Py_DECREF(pyStride);
            Py_DECREF(pyStrideKey);

            PyObject * pyImagePixelFormat      = Py_BuildValue("i", ((ImageData *)(ppResults[i]))->format);
            PyObject * pyImagePixelFormatKey   = Py_BuildValue("s", "ImagePixelFormat");
            PyDict_SetItem(pyImageData, pyImagePixelFormatKey, pyImagePixelFormat);
            Py_DECREF(pyImagePixelFormat);
            Py_DECREF(pyImagePixelFormatKey);

            PyList_SetItem(pyResults, i, pyImageData);
        }
        else if(dataType == IMRDT_CONTOUR)
        {
            PyObject * pyContour = PyDict_New();
            int pointCount = ((Contour *)(ppResults[i]))->pointsCount;
            if(pointCount != 0)
            {
                PyObject * pyPoints = PyList_New(pointCount);
                for(int j = 0; j < pointCount; ++j)
                {
                    PyObject * pyPoint = PyDict_New();

                    PyObject * pyPointX = Py_BuildValue("i",((Contour *)(ppResults[i]))->points[j].x);
                    PyObject * pyPointXKey   = Py_BuildValue("s", "X");
                    PyDict_SetItem(pyPoint, pyPointXKey, pyPointX);
                    Py_DECREF(pyPointX);
                    Py_DECREF(pyPointXKey);

                    PyObject * pyPointY = Py_BuildValue("i",((Contour *)(ppResults[i]))->points[j].y);
                    PyObject * pyPointYKey   = Py_BuildValue("s", "Y");
                    PyDict_SetItem(pyPoint, pyPointYKey, pyPointY);
                    Py_DECREF(pyPointY);
                    Py_DECREF(pyPointYKey);

                    PyList_SetItem(pyPoints, j, pyPoint);
                }
                PyObject * pyPointsKey   = Py_BuildValue("s", "Points");
                PyDict_SetItem(pyContour, pyPointsKey, pyPoints);
                Py_DECREF(pyPoints);
                Py_DECREF(pyPointsKey);
                PyList_SetItem(pyResults, i, pyContour);
            }
            else
            {
                //PyObject * pyPoints   = Py_BuildValue("o", NULL);
                PyObject * pyPointsKey   = Py_BuildValue("s", "Points");
                PyDict_SetItem(pyContour, pyPointsKey, Py_None);
                //Py_DECREF(pyPoints);
                Py_DECREF(pyPointsKey);
                PyList_SetItem(pyResults, i, pyContour);
            }
            
        }
        else if(dataType == IMRDT_LINESEGMENT)
        {
            PyObject * pyLineSegment = PyDict_New();

            PyObject * pyStartPoint = PyDict_New();
            PyObject * pyStartPointX = Py_BuildValue("i",((LineSegment *)(ppResults[i]))->startPoint.x);
            PyObject * pyStartPointXKey   = Py_BuildValue("s", "X");
            PyDict_SetItem(pyStartPoint, pyStartPointXKey, pyStartPointX);
            Py_DECREF(pyStartPointX);
            Py_DECREF(pyStartPointXKey);

            PyObject * pyStartPointY = Py_BuildValue("i",((LineSegment *)(ppResults[i]))->startPoint.y);
            PyObject * pyStartPointYKey   = Py_BuildValue("s", "Y");
            PyDict_SetItem(pyStartPoint, pyStartPointYKey, pyStartPointY);
            Py_DECREF(pyStartPointY);
            Py_DECREF(pyStartPointYKey);
            PyObject * pyStartPointKey   = Py_BuildValue("s", "StartPoint");
            PyDict_SetItem(pyLineSegment, pyStartPointKey, pyStartPoint);
            Py_DECREF(pyStartPoint);
            Py_DECREF(pyStartPointKey);

            PyObject * pyEndPoint = PyDict_New();
            PyObject * pyEndPointX = Py_BuildValue("i",((LineSegment *)(ppResults[i]))->endPoint.x);
            PyObject * pyEndPointXKey   = Py_BuildValue("s", "X");
            PyDict_SetItem(pyEndPoint, pyEndPointXKey, pyEndPointX);
            Py_DECREF(pyEndPointX);
            Py_DECREF(pyEndPointXKey);

            PyObject * pyEndPointY = Py_BuildValue("i",((LineSegment *)(ppResults[i]))->endPoint.y);
            PyObject * pyEndPointYKey   = Py_BuildValue("s", "Y");
            PyDict_SetItem(pyEndPoint, pyEndPointYKey, pyEndPointY);
            Py_DECREF(pyEndPointY);
            Py_DECREF(pyEndPointYKey);
            PyObject * pyEndPointKey   = Py_BuildValue("s", "EndPoint");
            PyDict_SetItem(pyLineSegment, pyEndPointKey, pyEndPoint);
            Py_DECREF(pyEndPoint);
            Py_DECREF(pyEndPointKey);

            if(((LineSegment *)(ppResults[i]))->linesConfidenceCoefficients != NULL)
            {
                PyObject * pyLinesConfidenceCoefficients = PyList_New(4);
                for(int j = 0; j < 4; ++j)
                {
                    PyObject * pyLinesConfidenceCoefficient = Py_BuildValue("i",((LineSegment *)(ppResults[i]))->linesConfidenceCoefficients[j]);
                    PyList_SetItem(pyLinesConfidenceCoefficients, j, pyLinesConfidenceCoefficient);
                }
                PyObject * pyLinesConfidenceCoefficientsKey   = Py_BuildValue("s", "LinesConfidenceCoefficients");
                PyDict_SetItem(pyLineSegment, pyLinesConfidenceCoefficientsKey, pyLinesConfidenceCoefficients);
                Py_DECREF(pyLinesConfidenceCoefficients);
                Py_DECREF(pyLinesConfidenceCoefficientsKey);
            }
            else
            {
                //PyObject * pyLinesConfidenceCoefficients   = Py_BuildValue("o", NULL);
                PyObject * pyLinesConfidenceCoefficientsKey   = Py_BuildValue("s", "LinesConfidenceCoefficients");
                PyDict_SetItem(pyLineSegment, pyLinesConfidenceCoefficientsKey, Py_None);
                //Py_DECREF(pyLinesConfidenceCoefficients);
                Py_DECREF(pyLinesConfidenceCoefficientsKey);
            }
            

            PyList_SetItem(pyResults, i, pyLineSegment);
        }
        else if(dataType == IMRDT_LOCALIZATIONRESULT)
        {
            PyObject * pyLR = PyDict_New();
            PyObject * pyLocalizationResult = CreatePyLocalizationResult((LocalizationResult *)(ppResults[i]));
            if(pyLocalizationResult != NULL)
            {
                PyObject * pyLRKey   = Py_BuildValue("s", "LocalizationRsult");
                PyDict_SetItem(pyLR, pyLRKey, pyLocalizationResult);
                Py_DECREF(pyLocalizationResult);
                Py_DECREF(pyLRKey);
                PyList_SetItem(pyResults, i, pyLR);
            }
            else
            {
                //PyObject * pyLocalizationResult   = Py_BuildValue("o", NULL);
                PyObject * pyLRKey   = Py_BuildValue("s", "LocalizationRsult");
                PyDict_SetItem(pyLR, pyLRKey, Py_None);
                //Py_DECREF(pyLocalizationResult);
                Py_DECREF(pyLRKey);
                PyList_SetItem(pyResults, i, pyLR);
            }
            
        }
        else if(dataType == IMRDT_REGIONOFINTEREST)
        {
            PyObject * pyRegionOfInterest = PyDict_New();

            PyObject * pyROIId      = Py_BuildValue("i", ((RegionOfInterest *)(ppResults[i]))->roiId);
            PyObject * pyROIIdKey   = Py_BuildValue("s", "ROIId");
            PyDict_SetItem(pyRegionOfInterest, pyROIIdKey, pyROIId);
            Py_DECREF(pyROIId);
            Py_DECREF(pyROIIdKey);

            PyObject * pyPoint = PyDict_New();

            PyObject * pyPointX = Py_BuildValue("i", ((RegionOfInterest *)(ppResults[i]))->point.x);
            PyObject * pyPointXKey   = Py_BuildValue("s", "X");
            PyDict_SetItem(pyPoint, pyPointXKey, pyPointX);
            Py_DECREF(pyPointX);
            Py_DECREF(pyPointXKey);

            PyObject * pyPointY = Py_BuildValue("i", ((RegionOfInterest *)(ppResults[i]))->point.y);
            PyObject * pyPointYKey   = Py_BuildValue("s", "Y");
            PyDict_SetItem(pyPoint, pyPointYKey, pyPointY);
            Py_DECREF(pyPointY);
            Py_DECREF(pyPointYKey);
            PyObject * pyPointKey   = Py_BuildValue("s", "Point");
            PyDict_SetItem(pyRegionOfInterest, pyPointKey, pyPoint);
            Py_DECREF(pyPoint);
            Py_DECREF(pyPointKey);

            PyObject * pyWidth      = Py_BuildValue("i", ((RegionOfInterest *)(ppResults[i]))->width);
            PyObject * pyWidthKey   = Py_BuildValue("s", "Width");
            PyDict_SetItem(pyRegionOfInterest, pyWidthKey, pyWidth);
            Py_DECREF(pyWidth);
            Py_DECREF(pyWidthKey);

            PyObject * pyHeight      = Py_BuildValue("i", ((RegionOfInterest *)(ppResults[i]))->height);
            PyObject * pyHeightKey   = Py_BuildValue("s", "Height");
            PyDict_SetItem(pyRegionOfInterest, pyHeightKey, pyHeight);
            Py_DECREF(pyHeight);
            Py_DECREF(pyHeightKey);

            PyList_SetItem(pyResults, i, pyRegionOfInterest);
        }
    }

    return pyResults;
}

static PyObject * CreatePyIntermediateResults(IntermediateResultArray * pResults)
{
    // Get barcode results
    int count = pResults->resultsCount;

    // Create a Python object to store results
    PyObject * pyIntermediateResults = PyList_New(count);

    for(int i = 0; i < count; ++i)
    {
        PyObject * pyIntermediateResult = PyDict_New();
        if(pyIntermediateResult == NULL)
        {
            return NULL;     
        }

        PyObject * pyDataType      = Py_BuildValue("i", pResults->results[i]->dataType);
        PyObject * pyDataTypeKey   = Py_BuildValue("s", "DataType");
        PyDict_SetItem(pyIntermediateResult, pyDataTypeKey, pyDataType);
        Py_DECREF(pyDataType);
        Py_DECREF(pyDataTypeKey);

        if(pResults->results[i]->results != NULL)
        {
            PyObject * pyResults      = CreateIntermediateResultDatas(pResults->results[i]->results, pResults->results[i]->resultsCount, pResults->results[i]->dataType);
            PyObject * pyResultsKey   = Py_BuildValue("s", "IMResults");
            PyDict_SetItem(pyIntermediateResult, pyResultsKey, pyResults);
            Py_DECREF(pyResults);
            Py_DECREF(pyResultsKey);
        }
        else
        {
            //PyObject * pyResults   = Py_BuildValue("o", NULL);
            PyObject * pyResultsKey   = Py_BuildValue("s", "IMResults");
            PyDict_SetItem(pyIntermediateResult, pyResultsKey, Py_None);
            //Py_DECREF(pyResults);
            Py_DECREF(pyResultsKey);
        }
        

        PyObject * pyResultType      = Py_BuildValue("i", pResults->results[i]->resultType);
        PyObject * pyResultTypeKey   = Py_BuildValue("s", "ResultType");
        PyDict_SetItem(pyIntermediateResult, pyResultTypeKey, pyResultType);
        Py_DECREF(pyResultType);
        Py_DECREF(pyResultTypeKey);

        PyObject * pyBarcodeComplementMode      = Py_BuildValue("i", pResults->results[i]->barcodeComplementMode);
        PyObject * pyBarcodeComplementModeKey   = Py_BuildValue("s", "BarcodeComplementMode");
        PyDict_SetItem(pyIntermediateResult, pyBarcodeComplementModeKey, pyBarcodeComplementMode);
        Py_DECREF(pyBarcodeComplementMode);
        Py_DECREF(pyBarcodeComplementModeKey);

        PyObject * pyBCMIndex      = Py_BuildValue("i", pResults->results[i]->bcmIndex);
        PyObject * pyBCMIndexKey   = Py_BuildValue("s", "BCMIndex");
        PyDict_SetItem(pyIntermediateResult, pyBCMIndexKey, pyBCMIndex);
        Py_DECREF(pyBCMIndex);
        Py_DECREF(pyBCMIndexKey);

        PyObject * pyDPMCodeReadingMode      = Py_BuildValue("i", pResults->results[i]->dpmCodeReadingMode);
        PyObject * pyDPMCodeReadingModeKey   = Py_BuildValue("s", "DPMCodeReadingMode");
        PyDict_SetItem(pyIntermediateResult, pyDPMCodeReadingModeKey, pyDPMCodeReadingMode);
        Py_DECREF(pyDPMCodeReadingMode);
        Py_DECREF(pyDPMCodeReadingModeKey);

        PyObject * pyDPMCRMIndex      = Py_BuildValue("i", pResults->results[i]->dpmcrmIndex);
        PyObject * pyDPMCRMIndexKey   = Py_BuildValue("s", "DPMCRMIndex");
        PyDict_SetItem(pyIntermediateResult, pyDPMCRMIndexKey, pyDPMCRMIndex);
        Py_DECREF(pyDPMCRMIndex);
        Py_DECREF(pyDPMCRMIndexKey);

        PyObject * pyRotationMatrix = PyList_New(9);
        for(int j = 0; j < 9; ++j)
        {
            PyObject * temp = Py_BuildValue("d",pResults->results[i]->rotationMatrix[j]);
            PyList_SetItem(pyRotationMatrix, j, temp);
        }
        PyObject * pyRotationMatrixKey   = Py_BuildValue("s", "RotationMatrix");
        PyDict_SetItem(pyIntermediateResult, pyRotationMatrixKey, pyRotationMatrix);
        Py_DECREF(pyRotationMatrix);
        Py_DECREF(pyRotationMatrixKey);

        PyObject * pyTextFilterMode      = Py_BuildValue("i", pResults->results[i]->textFilterMode);
        PyObject * pyTextFilterModeKey   = Py_BuildValue("s", "TextFilterMode");
        PyDict_SetItem(pyIntermediateResult, pyTextFilterModeKey, pyTextFilterMode);
        Py_DECREF(pyTextFilterMode);
        Py_DECREF(pyTextFilterModeKey);

        PyObject * pyTFMIndex      = Py_BuildValue("i", pResults->results[i]->tfmIndex);
        PyObject * pyTFMIndexKey   = Py_BuildValue("s", "TFMIndex");
        PyDict_SetItem(pyIntermediateResult, pyTFMIndexKey, pyTFMIndex);
        Py_DECREF(pyTFMIndex);
        Py_DECREF(pyTFMIndexKey);

        PyObject * pyLocalizationMode      = Py_BuildValue("i", pResults->results[i]->localizationMode);
        PyObject * pyLocalizationModeKey   = Py_BuildValue("s", "LocalizationMode");
        PyDict_SetItem(pyIntermediateResult, pyLocalizationModeKey, pyLocalizationMode);
        Py_DECREF(pyLocalizationMode);
        Py_DECREF(pyLocalizationModeKey);

        PyObject * pyLMIndex      = Py_BuildValue("i", pResults->results[i]->lmIndex);
        PyObject * pyLMIndexKey   = Py_BuildValue("s", "LMIndex");
        PyDict_SetItem(pyIntermediateResult, pyLMIndexKey, pyLMIndex);
        Py_DECREF(pyLMIndex);
        Py_DECREF(pyLMIndexKey);

        PyObject * pyBinarizationMode      = Py_BuildValue("i", pResults->results[i]->binarizationMode);
        PyObject * pyBinarizationModeKey   = Py_BuildValue("s", "BinarizationMode");
        PyDict_SetItem(pyIntermediateResult, pyBinarizationModeKey, pyBinarizationMode);
        Py_DECREF(pyBinarizationMode);
        Py_DECREF(pyBinarizationModeKey);

        PyObject * pyBMIndex      = Py_BuildValue("i", pResults->results[i]->bmIndex);
        PyObject * pyBMIndexKey   = Py_BuildValue("s", "BMIndex");
        PyDict_SetItem(pyIntermediateResult, pyBMIndexKey, pyBMIndex);
        Py_DECREF(pyBMIndex);
        Py_DECREF(pyBMIndexKey);

        PyObject * pyImagePreprocessingMode      = Py_BuildValue("i", pResults->results[i]->imagePreprocessingMode);
        PyObject * pyImagePreprocessingModeKey   = Py_BuildValue("s", "ImagePreprocessingMode");
        PyDict_SetItem(pyIntermediateResult, pyImagePreprocessingModeKey, pyImagePreprocessingMode);
        Py_DECREF(pyImagePreprocessingMode);
        Py_DECREF(pyImagePreprocessingModeKey);

        PyObject * pyIPMIndex      = Py_BuildValue("i", pResults->results[i]->ipmIndex);
        PyObject * pyIPMIndexKey   = Py_BuildValue("s", "IPMIndex");
        PyDict_SetItem(pyIntermediateResult, pyIPMIndexKey, pyIPMIndex);
        Py_DECREF(pyIPMIndex);
        Py_DECREF(pyIPMIndex);

        PyObject * pyROIId      = Py_BuildValue("i", pResults->results[i]->roiId);
        PyObject * pyROIIdKey   = Py_BuildValue("s", "ROIId");
        PyDict_SetItem(pyIntermediateResult, pyROIIdKey, pyROIId);
        Py_DECREF(pyROIId);
        Py_DECREF(pyROIIdKey);

        PyObject * pyRegionPredetectionMode      = Py_BuildValue("i", pResults->results[i]->regionPredetectionMode);
        PyObject * pyRegionPredetectionModeKey   = Py_BuildValue("s", "RegionPredetectionMode");
        PyDict_SetItem(pyIntermediateResult, pyRegionPredetectionModeKey, pyRegionPredetectionMode);
        Py_DECREF(pyRegionPredetectionMode);
        Py_DECREF(pyRegionPredetectionModeKey);

        PyObject * pyRPMIndex      = Py_BuildValue("i", pResults->results[i]->rpmIndex);
        PyObject * pyRPMIndexKey   = Py_BuildValue("s", "RPMIndex");
        PyDict_SetItem(pyIntermediateResult, pyRPMIndexKey, pyRPMIndex);
        Py_DECREF(pyRPMIndex);
        Py_DECREF(pyRPMIndexKey);

        PyObject * pyGrayscaleTransformationMode      = Py_BuildValue("i", pResults->results[i]->grayscaleTransformationMode);
        PyObject * pyGrayscaleTransformationModeKey   = Py_BuildValue("s", "GrayscaleTransformationMode");
        PyDict_SetItem(pyIntermediateResult, pyRegionPredetectionModeKey, pyRegionPredetectionMode);
        Py_DECREF(pyRegionPredetectionMode);
        Py_DECREF(pyRegionPredetectionModeKey);

        PyObject * pyGTMIndex      = Py_BuildValue("i", pResults->results[i]->gtmIndex);
        PyObject * pyGTMIndexKey   = Py_BuildValue("s", "GTMIndex");
        PyDict_SetItem(pyIntermediateResult, pyGTMIndexKey, pyGTMIndex);
        Py_DECREF(pyGTMIndex);
        Py_DECREF(pyGTMIndexKey);

        PyObject * pyColourConversionMode      = Py_BuildValue("i", pResults->results[i]->colourConversionMode);
        PyObject * pyColourConversionModeKey   = Py_BuildValue("s", "ColourConversionMode");
        PyDict_SetItem(pyIntermediateResult, pyColourConversionModeKey, pyColourConversionMode);
        Py_DECREF(pyColourConversionMode);
        Py_DECREF(pyColourConversionModeKey);

        PyObject * pyCICMIndex      = Py_BuildValue("i", pResults->results[i]->cicmIndex);
        PyObject * pyCICMIndexKey   = Py_BuildValue("s", "CICMIndex");
        PyDict_SetItem(pyIntermediateResult, pyCICMIndexKey, pyCICMIndex);
        Py_DECREF(pyCICMIndex);
        Py_DECREF(pyCICMIndexKey);

        PyObject * pyColourClusteringMode      = Py_BuildValue("i", pResults->results[i]->colourClusteringMode);
        PyObject * pyColourClusteringModeKey   = Py_BuildValue("s", "ColourClusteringMode");
        PyDict_SetItem(pyIntermediateResult, pyColourClusteringModeKey, pyColourClusteringMode);
        Py_DECREF(pyColourClusteringMode);
        Py_DECREF(pyColourClusteringModeKey);

        PyObject * pyCCMIndex      = Py_BuildValue("i", pResults->results[i]->ccmIndex);
        PyObject * pyCCMIndexKey   = Py_BuildValue("s", "CCMIndex");
        PyDict_SetItem(pyIntermediateResult, pyCCMIndexKey, pyCCMIndex);
        Py_DECREF(pyCCMIndex);
        Py_DECREF(pyCCMIndexKey);

        PyObject * pyScaleDownRatio      = Py_BuildValue("i", pResults->results[i]->scaleDownRatio);
        PyObject * pyScaleDownRatioKey   = Py_BuildValue("s", "ScaleDownRatio");
        PyDict_SetItem(pyIntermediateResult, pyScaleDownRatioKey, pyScaleDownRatio);
        Py_DECREF(pyScaleDownRatio);
        Py_DECREF(pyScaleDownRatioKey);

        PyObject * pyFrameId      = Py_BuildValue("i", pResults->results[i]->frameId);
        PyObject * pyFrameIdKey   = Py_BuildValue("s", "FrameId");
        PyDict_SetItem(pyIntermediateResult, pyFrameIdKey, pyFrameId);
        Py_DECREF(pyFrameId);
        Py_DECREF(pyFrameIdKey);

        PyList_SetItem(pyIntermediateResults, i, pyIntermediateResult);
    }

    // DBR_FreeIntermediateResults(&pResults);

    return pyIntermediateResults;
}

static PyObject * CreatePyFrameDecodingParameters(FrameDecodingParameters * pParameters)
{
    PyObject * pyParameters = PyDict_New();
    if(pyParameters == NULL)
    { 
        return NULL;     
    }

    PyObject * pyMaxQueueLength             = Py_BuildValue("i", pParameters->maxQueueLength);
    PyObject * pyMaxQueueLengthKey          = Py_BuildValue("s", "MaxQueueLength");
    PyDict_SetItem(pyParameters, pyMaxQueueLengthKey, pyMaxQueueLength);
    Py_DECREF(pyMaxQueueLength);
    Py_DECREF(pyMaxQueueLengthKey);

    PyObject * pyMaxResultQueueLength             = Py_BuildValue("i", pParameters->maxResultQueueLength);
    PyObject * pyMaxResultQueueLengthKey          = Py_BuildValue("s", "MaxResultQueueLength");
    PyDict_SetItem(pyParameters, pyMaxResultQueueLengthKey, pyMaxResultQueueLength);
    Py_DECREF(pyMaxResultQueueLength);
    Py_DECREF(pyMaxResultQueueLengthKey);

    PyObject * pyWidth             = Py_BuildValue("i", pParameters->width);
    PyObject * pyWidthKey          = Py_BuildValue("s", "Width");
    PyDict_SetItem(pyParameters, pyWidthKey, pyWidth);
    Py_DECREF(pyWidth);
    Py_DECREF(pyWidthKey);

    PyObject * pyHeight             = Py_BuildValue("i", pParameters->height);
    PyObject * pyHeightKey          = Py_BuildValue("s", "Height");
    PyDict_SetItem(pyParameters, pyHeightKey, pyHeight);
    Py_DECREF(pyHeight);
    Py_DECREF(pyHeightKey);

    PyObject * pyStride             = Py_BuildValue("i", pParameters->stride);
    PyObject * pyStrideKey          = Py_BuildValue("s", "Stride");
    PyDict_SetItem(pyParameters, pyStrideKey, pyStride);
    Py_DECREF(pyStride);
    Py_DECREF(pyStrideKey);

    PyObject * pyImagePixelFormat             = Py_BuildValue("i", pParameters->imagePixelFormat);
    PyObject * pyImagePixelFormatKey          = Py_BuildValue("s", "ImagePixelFormat");
    PyDict_SetItem(pyParameters, pyImagePixelFormatKey, pyImagePixelFormat);
    Py_DECREF(pyImagePixelFormat);
    Py_DECREF(pyImagePixelFormatKey);

    PyObject * pyRegionTop             = Py_BuildValue("i", pParameters->region.regionTop);
    PyObject * pyRegionTopKey          = Py_BuildValue("s", "RegionTop");
    PyDict_SetItem(pyParameters, pyRegionTopKey, pyRegionTop);
    Py_DECREF(pyRegionTop);
    Py_DECREF(pyRegionTopKey);

    PyObject * pyRegionLeft             = Py_BuildValue("i", pParameters->region.regionLeft);
    PyObject * pyRegionLeftKey          = Py_BuildValue("s", "RegionLeft");
    PyDict_SetItem(pyParameters, pyRegionLeftKey, pyRegionLeft);
    Py_DECREF(pyRegionLeft);
    Py_DECREF(pyRegionLeftKey);

    PyObject * pyRegionRight             = Py_BuildValue("i", pParameters->region.regionRight);
    PyObject * pyRegionRightKey          = Py_BuildValue("s", "RegionRight");
    PyDict_SetItem(pyParameters, pyRegionRightKey, pyRegionRight);
    Py_DECREF(pyRegionRight);
    Py_DECREF(pyRegionRightKey);

    PyObject * pyRegionBottom             = Py_BuildValue("i", pParameters->region.regionBottom);
    PyObject * pyRegionBottomKey          = Py_BuildValue("s", "RegionBottom");
    PyDict_SetItem(pyParameters, pyRegionBottomKey, pyRegionBottom);
    Py_DECREF(pyRegionBottom);
    Py_DECREF(pyRegionBottomKey);

    PyObject * pyRegionMeasuredByPercentage             = Py_BuildValue("i", pParameters->region.regionMeasuredByPercentage);
    PyObject * pyRegionMeasuredByPercentageKey          = Py_BuildValue("s", "RegionMeasuredByPercentage");
    PyDict_SetItem(pyParameters, pyRegionMeasuredByPercentageKey, pyRegionMeasuredByPercentage);
    Py_DECREF(pyRegionMeasuredByPercentage);
    Py_DECREF(pyRegionMeasuredByPercentageKey);

    PyObject * pyThreshold             = Py_BuildValue("f", pParameters->threshold);
    PyObject * pyThresholdKey          = Py_BuildValue("s", "Threshold");
    PyDict_SetItem(pyParameters, pyThresholdKey, pyThreshold);
    Py_DECREF(pyThreshold);
    Py_DECREF(pyThresholdKey);

    PyObject * pyFPS             = Py_BuildValue("i", pParameters->fps);
    PyObject * pyFPSKey          = Py_BuildValue("s", "FPS");
    PyDict_SetItem(pyParameters, pyFPSKey, pyFPS);
    Py_DECREF(pyFPS);
    Py_DECREF(pyFPSKey);

	PyObject * pyAutoFilter = Py_BuildValue("i", pParameters->autoFilter);
	PyObject * pyAutoFilterKey = Py_BuildValue("s", "AutoFilter");
	PyDict_SetItem(pyParameters, pyAutoFilterKey, pyAutoFilter);
	Py_DECREF(pyAutoFilter);
	Py_DECREF(pyAutoFilterKey);

    return pyParameters;
}

FrameDecodingParameters CreateCFrameDecodingParameters(PyObject * pyParameters)
{
    FrameDecodingParameters parameters;
    parameters.maxQueueLength                       = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "MaxQueueLength")));
    parameters.maxResultQueueLength                 = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "MaxResultQueueLength")));
    parameters.width                                = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "Width")));
    parameters.height                               = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "Height")));
    parameters.stride                               = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "Stride")));
    parameters.imagePixelFormat                     = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "ImagePixelFormat")));
    parameters.region.regionBottom                  = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "RegionBottom")));
    parameters.region.regionLeft                    = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "RegionLeft")));
    parameters.region.regionRight                   = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "RegionRight")));
    parameters.region.regionTop                     = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "RegionTop")));
    parameters.region.regionMeasuredByPercentage    = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "RegionMeasuredByPercentage")));
    parameters.threshold                            = (float)PyFloat_AsDouble(PyDict_GetItem(pyParameters, Py_BuildValue("s", "Threshold")));
    parameters.fps                                  = PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "FPS")));
	parameters.autoFilter							= PyLong_AsLong(PyDict_GetItem(pyParameters, Py_BuildValue("s", "AutoFilter")));

    return parameters;
}

#pragma region DeprecatedFunctions
/**
 * Set Dynamsoft Barcode Reader license. This API will be deprecated in a future version.
 * To get valid license, please contact support@dynamsoft.com
 * Invalid license is acceptable. With an invalid license, SDK will return an imcomplete result.
 */
static PyObject *
initLicense(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pszLicense;
    if (!PyArg_ParseTuple(args, "s", &pszLicense))
    {
        return NULL;
    }

    int ret = DBR_InitLicense(self->hBarcode, pszLicense);
    return Py_BuildValue("i", ret);
}

/**
 * This function will be deprecated in a future version.
*/
static PyObject *createPyResults(TextResultArray *pResults, const char* encoding)
{
    if (!pResults)
    {
        printf("No barcode detected\n");
        return NULL;
    }
    // Get barcode results
    int count = pResults->resultsCount;

    // Create a Python object to store results
    PyObject *list = PyList_New(count);
    // printf("count: %d\n", count);
    PyObject *result = NULL;
    int i = 0;
    for (; i < count; i++)
    {
        LocalizationResult *pLocalizationResult = pResults->results[i]->localizationResult;
        int x1 = pLocalizationResult->x1;
        int y1 = pLocalizationResult->y1;
        int x2 = pLocalizationResult->x2;
        int y2 = pLocalizationResult->y2;
        int x3 = pLocalizationResult->x3;
        int y3 = pLocalizationResult->y3;
        int x4 = pLocalizationResult->x4;
        int y4 = pLocalizationResult->y4;
        PyObject *pyObject = NULL;
        if (encoding) {
            pyObject = PyList_New(10);
        #ifdef IS_PY3K
            PyObject *format = PyUnicode_FromString(pResults->results[i]->barcodeFormatString);
        #else
            PyObject *format = PyString_FromString(pResults->results[i]->barcodeFormatString);
        #endif
            PyList_SetItem(pyObject, 0, format);
            
            PyObject *result = PyUnicode_Decode(pResults->results[i]->barcodeBytes, pResults->results[i]->barcodeBytesLength, encoding, "strict");
            if (result == NULL) 
            {
                char *hex = (char*)malloc(pResults->results[i]->barcodeBytesLength * 3 + 1);
                ToHexString(pResults->results[i]->barcodeBytes, pResults->results[i]->barcodeBytesLength, hex);
                printf("Hex Data: %s\n", hex);
                free(hex);

                PyErr_SetString(PyExc_TypeError, "Incorrect character set! Failed to decode barcode results!");
                DBR_FreeTextResults(&pResults);
                return NULL;
            }
            PyList_SetItem(pyObject, 1, result);

            PyObject *x1_pyobj = Py_BuildValue("i", x1);
            PyList_SetItem(pyObject, 2, x1_pyobj);

            PyObject *y1_pyobj = Py_BuildValue("i", y1);
            PyList_SetItem(pyObject, 3, y1_pyobj);

            PyObject *x2_pyobj = Py_BuildValue("i", x2);
            PyList_SetItem(pyObject, 4, x2_pyobj);

            PyObject *y2_pyobj = Py_BuildValue("i", y2);
            PyList_SetItem(pyObject, 5, y2_pyobj);

            PyObject *x3_pyobj = Py_BuildValue("i", x3);
            PyList_SetItem(pyObject, 6, x3_pyobj);

            PyObject *y3_pyobj = Py_BuildValue("i", y3);
            PyList_SetItem(pyObject, 7, y3_pyobj);

            PyObject *x4_pyobj = Py_BuildValue("i", x4);
            PyList_SetItem(pyObject, 8, x4_pyobj);

            PyObject *y4_pyobj = Py_BuildValue("i", y4);
            PyList_SetItem(pyObject, 9, y4_pyobj);

        }
        else
            pyObject = Py_BuildValue("ssiiiiiiii", pResults->results[i]->barcodeFormatString, pResults->results[i]->barcodeText, x1, y1, x2, y2, x3, y3, x4, y4);
        
        PyList_SetItem(list, i, pyObject); // Add results to list

        // Print out PyObject if needed
        if (DEBUG)
        {
#if defined(IS_PY3K)
            PyObject *objectsRepresentation = PyObject_Repr(list);
            const char *s = PyUnicode_AsUTF8(objectsRepresentation);
            printf("Results: %s\n", s);
#else
            PyObject *objectsRepresentation = PyObject_Repr(list);
            const char *s = PyString_AsString(objectsRepresentation);
            printf("Results: %s\n", s);
#endif
        }
    }

    // Release memory
    DBR_FreeTextResults(&pResults);

    return list;
}

/**
 * This function will deprecate in a future version.
*/
void updateFormat(DynamsoftBarcodeReader *self, int format)
{
    // Update DBR params
    PublicRuntimeSettings pSettings = {0};
    DBR_GetRuntimeSettings(self->hBarcode, &pSettings);
    pSettings.barcodeFormatIds = format;
    char szErrorMsgBuffer[256];
    DBR_UpdateRuntimeSettings(self->hBarcode, &pSettings, szErrorMsgBuffer, 256);
}

/**
 * Decode barcode from a file. This API will be deprecated in a future version.
 */
static PyObject *
decodeFile(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
#if defined(_WIN32)
    printf("Windows\n");
#elif defined(__linux__)
    printf("Linux\n");
#elif defined(__APPLE__)
    printf("MacOS\n");
#else
    printf("Unknown Operating System.\n");
#endif

    char *pFileName; // File name
    int iFormat;     // Barcode formats
    char *templateName = NULL;
    char *encoding = NULL;
    if (!PyArg_ParseTuple(args, "si|ss", &pFileName, &iFormat, &templateName, &encoding))
    {
        return NULL;
    }

    updateFormat(self, iFormat);

    TextResultArray *pResults = NULL;

    // Barcode detection
    int ret = DBR_DecodeFile(self->hBarcode, pFileName, templateName ? templateName : "");
    if (ret)
    {
        printf("Detection error: %s\n", DBR_GetErrorString(ret));
    }
    DBR_GetAllTextResults(self->hBarcode, &pResults);

    // Wrap results
    PyObject *list = createPyResults(pResults, encoding);
    return list;
}

/**
 * Decode barcode from an image buffer. This API will be deprecated in a future version.
 */
static PyObject *
decodeBuffer(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *o;
    int iFormat;
    char *templateName = NULL;
    char *encoding = NULL;
    int imagePixelFormat = IPF_RGB_888;
    if (!PyArg_ParseTuple(args, "Oi|iss", &o, &iFormat, &imagePixelFormat, &templateName, &encoding))
        return NULL;

    updateFormat(self, iFormat);

#if defined(IS_PY3K)
    //Refer to numpy/core/src/multiarray/ctors.c
    Py_buffer *view;
    int nd;
    PyObject *memoryview = PyMemoryView_FromObject(o);
    if (memoryview == NULL)
    {
        PyErr_Clear();
        return NULL;
    }

    view = PyMemoryView_GET_BUFFER(memoryview);
    char *buffer = (char *)(view->buf);
    nd = (int)(view->ndim);
    int len = (int)(view->len);
    int stride = (int)(view->strides[0]);
    int channels = (int)(view->strides[1]);
    int width = (int)(view->strides[0] / view->strides[1]);
    int height = len / stride;
#else

    PyObject *ao = PyObject_GetAttrString(o, "__array_struct__");

    if ((ao == NULL) || !PyCObject_Check(ao))
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        return NULL;
    }

    PyArrayInterface *pai = (PyArrayInterface *)PyCObject_AsVoidPtr(ao);

    if (pai->two != 2)
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        Py_DECREF(ao);
        return NULL;
    }

    // Get image information
    char *buffer = (char *)pai->data;  // The address of image data
    int width = (int)pai->shape[1];    // image width
    int height = (int)pai->shape[0];   // image height
    int channels = (int)pai->shape[2];
    int stride = (int)pai->strides[0]; // image stride
#endif

    // Initialize Dynamsoft Barcode Reader
    TextResultArray *pResults = NULL;

    PyObject *list = NULL;
    int ret = DBR_DecodeBuffer(self->hBarcode, buffer, width, height, stride, imagePixelFormat, templateName ? templateName : "");
    if (ret)
    {
        printf("Detection error: %s\n", DBR_GetErrorString(ret));
    }
    // Wrap results
    DBR_GetAllTextResults(self->hBarcode, &pResults);
    list = createPyResults(pResults, encoding);

#if defined(IS_PY3K)
    Py_DECREF(memoryview);
#else
    Py_DECREF(ao);
#endif

    return list;
}

/**
 * Decode file stream. This API will be deprecated in a future version.
 */
static PyObject *
decodeFileStream(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
#if defined(_WIN32)
    printf("Windows\n");
#elif defined(__linux__)
    printf("Linux\n");
#elif defined(__APPLE__)
    printf("MacOS\n");
#else
    printf("Unknown Operating System.\n");
#endif

    PyObject *op; 
    int fileSize;
    int iFormat;     // Barcode formats
    char *templateName = NULL;
    char *encoding = NULL;
    if (!PyArg_ParseTuple(args, "Oii|ss", &op, &fileSize, &iFormat, &templateName, &encoding))
    {
        return NULL;
    }

    updateFormat(self, iFormat);

    TextResultArray *pResults = NULL;
    // https://docs.python.org/2/c-api/bytearray.html
    char *filestream = PyByteArray_AsString(op);
    // Barcode detection
    int ret = DBR_DecodeFileInMemory(self->hBarcode, filestream, fileSize, templateName ? templateName : "");
    if (ret)
    {
        printf("Detection error: %s\n", DBR_GetErrorString(ret));
    }
    DBR_GetAllTextResults(self->hBarcode, &pResults);

    // Wrap results
    PyObject *list = createPyResults(pResults, encoding);
    return list;
}

/**
 * Initializes barcode reader license from the license content on the client machine for offline verification. This API will be deprecated in a future version.
 *
 * @param pLicenseKey: The license key of Barcode Reader.
 * @param pLicenseContent: An encrypted string representing the license content (runtime number, expiry date, barcode type, etc.) obtained from the method DBR_OutputLicenseToString().
 *
 * @return Return 0 if the function operates successfully, otherwise call
 * 		   DBR_GetErrorString to get detail message.
 */
static PyObject *
initLicenseFromLicenseContent(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pszLicenseKey;
    char *pszLicenseContent;
    if (!PyArg_ParseTuple(args, "ss", &pszLicenseKey, &pszLicenseContent))
    {
        return NULL;
    }

    int ret = DBR_InitLicenseFromLicenseContent(self->hBarcode, pszLicenseKey, pszLicenseContent);
    return Py_BuildValue("i", ret);
}

/**
 * Outputs the license content as an encrypted string from the license server to be used for offline license verification. This API will be deprecated in a future version.
 *
 * @return if successful, return encypted string. Otherwise return error code. 
 */
static PyObject *
outputLicenseToString(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char content[512];
    int ret = DBR_OutputLicenseToString(self->hBarcode, content, 512);
    if (ret)
    {
        printf("%s\n", DBR_GetErrorString(ret));
        return Py_BuildValue("i", ret);
    }
    else
        return Py_BuildValue("s", content);
}

/**
 * Initializes barcode reader license from the license content on the client machine for offline verification. This API will be deprecated in a future version.
 *
 * @param pLicenseKey: The license key of Barcode Reader.
 * @param pLicenseContent: An encrypted string representing the license content (runtime number, expiry date, barcode type, etc.) obtained from the method DBR_OutputLicenseToString().
 *
 * @return Return 0 if the function operates successfully, otherwise call
 * 		   DBR_GetErrorString to get detail message.
 */
static PyObject *
initLicenseFromServer(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pszLicenseKey, *pLicenseServer;
    if (!PyArg_ParseTuple(args, "ss", &pLicenseServer, &pszLicenseKey))
    {
        return NULL;
    }

    int ret = DBR_InitLicenseFromServer(self->hBarcode, pLicenseServer, pszLicenseKey);
    return Py_BuildValue("i", ret);
}

/**
 * This function will be deprecated in a future version.
*/
static void setModeValue(DynamsoftBarcodeReader *self, PyObject *iter, char *mode)
{
    PublicRuntimeSettings pSettings = {0};
    DBR_GetRuntimeSettings(self->hBarcode, &pSettings);
    int index = 0;
    pSettings.furtherModes.grayscaleTransformationModes[0] = GTM_INVERTED;

    while (1)
    {
        PyObject *next = PyIter_Next(iter);
        if (!next)
        {
            break;
        }

        // Set attributes for different modes
        int attribute = PyLong_AsLong(next);
        if (!strcmp("grayscaleTransformationModes", mode))
        {
            // printf("Set grayscaleTransformationModes %d\n", attribute);
            pSettings.furtherModes.grayscaleTransformationModes[index] = attribute;
        }
        else if (!strcmp("colourClusteringModes", mode))
        {
            pSettings.furtherModes.colourClusteringModes[index] = attribute;
        }
        else if (!strcmp("colourConversionModes", mode))
        {
            pSettings.furtherModes.colourConversionModes[index] = attribute;
        }
        else if (!strcmp("regionPredetectionModes", mode))
        {
            pSettings.furtherModes.regionPredetectionModes[index] = attribute;
        }
        else if (!strcmp("imagePreprocessingModes ", mode))
        {
            pSettings.furtherModes.imagePreprocessingModes[index] = attribute;
        }
        else if (!strcmp("textureDetectionModes", mode))
        {
            pSettings.furtherModes.textureDetectionModes[index] = attribute;
        }
        else if (!strcmp("textFilterModes", mode))
        {
            pSettings.furtherModes.textFilterModes[index] = attribute;
        }
        else if (!strcmp("dpmCodeReadingModes", mode))
        {
            pSettings.furtherModes.dpmCodeReadingModes[index] = attribute;
        }
        else if (!strcmp("deformationResistingModes ", mode))
        {
            pSettings.furtherModes.deformationResistingModes[index] = attribute;
        }
        else if (!strcmp("barcodeComplementModes ", mode))
        {
            pSettings.furtherModes.barcodeComplementModes[index] = attribute;
        }
        else if (!strcmp("barcodeColourModes ", mode))
        {
            pSettings.furtherModes.barcodeColourModes[index] = attribute;
        }
        else if (!strcmp("textAssistedCorrectionMode", mode))
        {
            pSettings.furtherModes.textAssistedCorrectionMode = attribute;
        }

        ++index;
    }

    char szErrorMsgBuffer[256];
    DBR_UpdateRuntimeSettings(self->hBarcode, &pSettings, szErrorMsgBuffer, 256);
}

/**
 * Set modes for different scenarios. This API will be deprecated in a future version.
 *
 * @param mode: The mode name. E.g. dbr.GRAY_SCALE_TRANSFORMATION_MODE
 * @param values: A list of enumeration items. E.g. [dbr.GTM_INVERTED, dbr.GTM_ORIGINAL]
 *
 * @return Return NULL if failed.
 */
static PyObject *
setFurtherModes(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *mode;
    PyObject *value;
    if (!PyArg_ParseTuple(args, "sO", &mode, &value))
    {
        return NULL;
    }

    PyObject *iter = PyObject_GetIter(value);
    if (!iter)
    {
        printf("Please input a list\n");
        return NULL;
    }

    setModeValue(self, iter, mode);
    return Py_BuildValue("i", 0);
}

/**
 * Set public settings with JSON object. This API will be deprecated in a future version.
 *
 * @param json: the stringified JSON object.
 * 
 * @return Return 0 if the function operates successfully.
 */
static PyObject *
setParameters(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *json;
    if (!PyArg_ParseTuple(args, "s", &json))
    {
        return NULL;
    }

    char errorMessage[DEFAULT_MEMORY_SIZE];
    int ret = DBR_InitRuntimeSettingsWithString(self->hBarcode, json, CM_OVERWRITE, errorMessage, 256);
    if (ret) 
    {
        printf("Returned value: %d, error message: %s\n", ret, errorMessage);
        PyErr_SetString(PyExc_TypeError, "DBR_InitRuntimeSettingsWithString() failed");
        return NULL;
    }
    return Py_BuildValue("i", ret);
}

/**
 * Get public settings. This API will be deprecated in a future version.
 *
 * @return Return stringified JSON object.
 */
static PyObject *
getParameters(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char errorMessage[DEFAULT_MEMORY_SIZE];
    char pContent[DEFAULT_MEMORY_SIZE];

    int ret = DBR_OutputSettingsToString(self->hBarcode, pContent, DEFAULT_MEMORY_SIZE, "currentRuntimeSettings");
    // printf("pContent: %s\n, string len: %d", pContent, strlen(pContent));
    if (ret) 
    {
        printf("Returned value: %d, error message: %s\n", ret, errorMessage);
        PyErr_SetString(PyExc_TypeError, "DBR_OutputSettingsToString() failed");
        return NULL;
    }
    return Py_BuildValue("s", pContent);
}

/**
 * This function will deprecate in a future version.
*/
void onResultCallback(int frameId, TextResultArray *pResults, void *pUser)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)pUser;
    // Get barcode results
    int count = pResults->resultsCount;
    int i = 0;

    // https://docs.python.org/2/c-api/init.html
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    PyObject *list = PyList_New(count);
    for (; i < count; i++)
    {
        LocalizationResult *pLocalizationResult = pResults->results[i]->localizationResult;
        int x1 = pLocalizationResult->x1;
        int y1 = pLocalizationResult->y1;
        int x2 = pLocalizationResult->x2;
        int y2 = pLocalizationResult->y2;
        int x3 = pLocalizationResult->x3;
        int y3 = pLocalizationResult->y3;
        int x4 = pLocalizationResult->x4;
        int y4 = pLocalizationResult->y4;

        PyObject *pyObject = Py_BuildValue("ssiiiiiiii", pResults->results[i]->barcodeFormatString, pResults->results[i]->barcodeText, x1, y1, x2, y2, x3, y3, x4, y4);
        PyList_SetItem(list, i, pyObject); // Add results to list
    }

    PyObject *result = PyObject_CallFunction(self->py_callback, "O", list);
    if (result == NULL)
        return;
    Py_DECREF(result);

    PyGILState_Release(gstate);
    /////////////////////////////////////////////

    // Release memory
    DBR_FreeTextResults(&pResults);
}

/**
 * Read barcodes from continuous video frames. This API will be deprecated in a future version.
 */
static PyObject *
startVideoMode(PyObject *obj, PyObject *args)
{
    printf("Start the video mode\n");
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *callback = NULL;
    int maxListLength, maxResultListLength, width, height, stride, iFormat;
    int imagePixelFormat = IPF_RGB_888;
    if (!PyArg_ParseTuple(args, "iiiiiiO|i", &maxListLength, &maxResultListLength, &width, &height, &stride, &iFormat, &callback, &imagePixelFormat))
    {
        return NULL;
    }

    updateFormat(self, iFormat);

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }
    else
    {
        Py_XINCREF(callback);    /* Add a reference to new callback */
        Py_XDECREF(self->py_callback); /* Dispose of previous callback */
        self->py_callback = callback;
    }


    DBR_SetTextResultCallback(self->hBarcode, onResultCallback, self);

    int ret = DBR_StartFrameDecoding(self->hBarcode, maxListLength, maxResultListLength, width, height, stride, imagePixelFormat, "");
    return Py_BuildValue("i", ret);
}

/**
 * This API will be deprecated in a future version.
*/
static PyObject *
stopVideoMode(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
    printf("Stop the video mode\n");
    if (self->hBarcode)
    {
        int ret = DBR_StopFrameDecoding(self->hBarcode);
        return Py_BuildValue("i", ret);
    }

    return 0;
}

/**
 * This API will be deprecated in a future version.
*/
static PyObject *
appendVideoFrame(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *o;
    if (!PyArg_ParseTuple(args, "O", &o))
        return NULL;

#if defined(IS_PY3K)
    //Refer to numpy/core/src/multiarray/ctors.c
    Py_buffer *view;
    PyObject *memoryview = PyMemoryView_FromObject(o);
    if (memoryview == NULL)
    {
        PyErr_Clear();
        return NULL;
    }

    view = PyMemoryView_GET_BUFFER(memoryview);
    unsigned char *buffer = (unsigned char *)(view->buf);
    Py_DECREF(memoryview);
#else

    PyObject *ao = PyObject_GetAttrString(o, "__array_struct__");

    if ((ao == NULL) || !PyCObject_Check(ao))
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        return NULL;
    }

    PyArrayInterface *pai = (PyArrayInterface *)PyCObject_AsVoidPtr(ao);

    if (pai->two != 2)
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        Py_DECREF(ao);
        return NULL;
    }

    // Get image information
    unsigned char *buffer = (unsigned char *)pai->data; // The address of image data
    Py_DECREF(ao);
#endif

    int frameId = DBR_AppendFrame(self->hBarcode, buffer);
    return 0;
}

#pragma endregion

static PyObject * GetDBRVersion(PyObject *obj, PyObject *args)
{
	DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
	const char * version = DBR_GetVersion();
	return Py_BuildValue("s", version);
}

static PyObject * GetErrorString(PyObject *obj, PyObject *args)
{
	DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
	int errorCode = 0;
	if (!PyArg_ParseTuple(args, "i", &errorCode))
	{
		Py_RETURN_NONE;
	}

	const char* errorString = DBR_GetErrorString(errorCode);
	return Py_BuildValue("s", errorString);
}

static PyObject * InitLicense(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pszLicense;
    if (!PyArg_ParseTuple(args, "s", &pszLicense))
    {
		Py_RETURN_NONE;
    }

    int ret = DBR_InitLicense(self->hBarcode, pszLicense);
    const char* errorString = DBR_GetErrorString(ret);
    return Py_BuildValue("(i,s)", ret, errorString);
}

static PyObject * GetRuntimeSettings(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PublicRuntimeSettings settings;
    int errorCode = DBR_GetRuntimeSettings(self->hBarcode, &settings);

    PyObject * pySettings = CreatePyRuntimeSettings(settings);
	
    return pySettings;
}

static PyObject * UpdataRuntimeSettings(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
    PyObject *pyParameters;
    if (!PyArg_ParseTuple(args, "O", &pyParameters))
    {
		Py_RETURN_NONE;
    }
    
    PublicRuntimeSettings settings = CreateCRuntimeSettings(pyParameters);
    char szErrorMsgBuffer[256];
    int errorCode = DBR_UpdateRuntimeSettings(self->hBarcode, &settings, szErrorMsgBuffer, 256);
	return Py_BuildValue("(i,s)", errorCode, szErrorMsgBuffer);
}

static PyObject * ResetRuntimeSettings(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    DBR_ResetRuntimeSettings(self->hBarcode);

	Py_RETURN_NONE;
}

static PyObject * SetModeArgument(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
    char * pModesName;
    int index;
    char * pArgumentName;
    char * pArgumentValue;
    if (!PyArg_ParseTuple(args, "siss", &pModesName, &index, &pArgumentName, &pArgumentValue))
    {
		Py_RETURN_NONE;
    }
    char szErrorMsgBuffer[256];
    int errorCode = DBR_SetModeArgument(self->hBarcode, pModesName, index, pArgumentName, pArgumentValue, szErrorMsgBuffer, 256);
    return Py_BuildValue("(i,s)", errorCode, szErrorMsgBuffer);
}

static PyObject * GetModeArgument(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
    char * pModesName;
    int index;
    char * pArgumentName;
    if (!PyArg_ParseTuple(args, "sis", &pModesName, &index, &pArgumentName))
    {
		Py_RETURN_NONE;
    }
    char szErrorMsgBuffer[256];
    char pArgumentValue[512];
    int errorCode = DBR_GetModeArgument(self->hBarcode, pModesName, index, pArgumentName, pArgumentValue, 512, szErrorMsgBuffer, 256);
    if(errorCode != 0)
    {
		return Py_BuildValue("(i,s)", errorCode, szErrorMsgBuffer);
    }
    else
    {
		return Py_BuildValue("s", pArgumentValue);
    }
}

static PyObject * GetAllTextResults(PyObject *obj, PyObject *args)
{
	DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

	TextResultArray *pResults = NULL;
	DBR_GetAllTextResults(self->hBarcode, &pResults);

	if (pResults == NULL || pResults->resultsCount == 0)
	{
		Py_RETURN_NONE;
	}
	else
	{
		PyObject * pyTextResults = CreatePyTextResults(pResults);
		DBR_FreeTextResults(&pResults);
		if(pyTextResults == NULL)
		{
			Py_RETURN_NONE;
		}
		else
		{
			return pyTextResults;
		}
	}
}

static PyObject * GetAllIntermediateResults(PyObject *obj, PyObject *args)
{
	DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

	IntermediateResultArray * pIResults = NULL;
	DBR_GetIntermediateResults(self->hBarcode, &pIResults);

	if (pIResults == NULL || pIResults->resultsCount == 0)
		Py_RETURN_NONE;
	else
	{
		PyObject * pyIntermediateResults = CreatePyIntermediateResults(pIResults);
		DBR_FreeIntermediateResults(&pIResults);
		if (pyIntermediateResults == NULL)
		{
			Py_RETURN_NONE;
		}
		else
		{
			return pyIntermediateResults;
		}
	}
}

static PyObject * DecodeFile(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pFileName; // File name
    char *templateName = NULL;
    if (!PyArg_ParseTuple(args, "ss", &pFileName, &templateName))
    {
		Py_RETURN_NONE;
    }

    if(templateName == NULL)
    {
        templateName = "";
    }

    // Barcode detection

    int ret = DBR_DecodeFile(self->hBarcode, pFileName, templateName);
	return Py_BuildValue("i", ret);
}

static PyObject * DecodeBuffer(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *o;
    char *templateName = NULL;
    int width, height, stride;
    int imagePixelFormat = IPF_RGB_888;
    if (!PyArg_ParseTuple(args, "Oiiiis", &o, &height, &width, &stride, &imagePixelFormat, &templateName))
		Py_RETURN_NONE;

#if defined(IS_PY3K)
    //Refer to numpy/core/src/multiarray/ctors.c
    Py_buffer *view;
    // int nd;
    PyObject *memoryview = PyMemoryView_FromObject(o);
    if (memoryview == NULL)
    {
        PyErr_Clear();
		Py_RETURN_NONE;
    }

    view = PyMemoryView_GET_BUFFER(memoryview);
    char *buffer = (char *)(view->buf);

#else

    PyObject *ao = PyObject_GetAttrString(o, "__array_struct__");

    if ((ao == NULL) || !PyCObject_Check(ao))
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
		Py_RETURN_NONE;
    }

    PyArrayInterface *pai = (PyArrayInterface *)PyCObject_AsVoidPtr(ao);

    if (pai->two != 2)
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        Py_DECREF(ao);
		Py_RETURN_NONE;
    }

    // Get image information
    char *buffer = (char *)pai->data;  // The address of image data

#endif

    // Initialize Dynamsoft Barcode Reader

    if(templateName == NULL)
    {
        templateName = "";
    }
    int ret = DBR_DecodeBuffer(self->hBarcode, buffer, width, height, stride, imagePixelFormat, templateName);
#if defined(IS_PY3K)
	Py_DECREF(memoryview);
#else
	Py_DECREF(ao);
#endif
	return Py_BuildValue("i", ret);
}

static PyObject * DecodeFileStream(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *op; 
    int fileSize;
    char *templateName = NULL;
    if (!PyArg_ParseTuple(args, "Ois", &op, &fileSize, &templateName))
    {
		Py_RETURN_NONE;
    }

    // https://docs.python.org/2/c-api/bytearray.html
    char *filestream = PyByteArray_AsString(op);
    if(templateName == NULL)
    {
        templateName = "";
    }
    // Barcode detection
    int ret = DBR_DecodeFileInMemory(self->hBarcode, filestream, fileSize, templateName);
	return Py_BuildValue("i", ret);
}

static PyObject * GetLengthOfFrameQueue(PyObject *obj, PyObject *args)
{
	DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

	int length = DBR_GetLengthOfFrameQueue(self->hBarcode);
	return Py_BuildValue("i", length);
}

void OnResultCallback(int frameId, TextResultArray *pResults, void *pUser)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)pUser;
    // Get barcode results
    int count = pResults->resultsCount;
    int i = 0;

    // https://docs.python.org/2/c-api/init.html
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    PyObject * pyTextResults = CreatePyTextResults(pResults);
    if(pyTextResults != NULL)
    {
        PyObject * result = PyObject_CallFunction(self->py_callback, "O", pyTextResults);
        Py_DECREF(pyTextResults);
        if (result != NULL)
            Py_DECREF(result);
    }

    PyGILState_Release(gstate);
    /////////////////////////////////////////////

    // Release memory
    DBR_FreeTextResults(&pResults);
}

static PyObject * InitFrameDecodingParameters(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
    FrameDecodingParameters pSettings;
    DBR_InitFrameDecodingParameters(self->hBarcode, &pSettings);
    PyObject * frameParameters = CreatePyFrameDecodingParameters(&pSettings);
    return frameParameters;
}

static PyObject * StartVideoMode(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *pyParameters = NULL;
    PyObject *callback = NULL;
    char * templateName = NULL;
    if (!PyArg_ParseTuple(args, "OOs",&pyParameters, &callback, &templateName))
    {
		Py_RETURN_NONE;
    }

    if(!PyDict_Check(pyParameters))
    {
        printf("the first parameter should be a dictionary.");
		Py_RETURN_NONE;
    }
    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		Py_RETURN_NONE;
    }
    else
    {
        Py_XINCREF(callback);    /* Add a reference to new callback */
        Py_XDECREF(self->py_callback); /* Dispose of previous callback */
        self->py_callback = callback;
    }

    DBR_SetTextResultCallback(self->hBarcode, OnResultCallback, self);
    FrameDecodingParameters parameters = CreateCFrameDecodingParameters(pyParameters);
    // Py_DECREF(pyParameters);
    if(templateName == NULL)
    {
        templateName = "";
    }
    int ret = DBR_StartFrameDecodingEx(self->hBarcode, parameters, templateName);
    return Py_BuildValue("i", ret);
}

static PyObject * StopVideoMode(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;
    printf("Stop the video mode\n");
    if (self->hBarcode)
    {
        int ret = DBR_StopFrameDecoding(self->hBarcode);
        return Py_BuildValue("i", ret);
    }

    return 0;
}

static PyObject * AppendVideoFrame(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    PyObject *o;
    if (!PyArg_ParseTuple(args, "O", &o))
		Py_RETURN_NONE;

#if defined(IS_PY3K)
    //Refer to numpy/core/src/multiarray/ctors.c
    Py_buffer *view;
    PyObject *memoryview = PyMemoryView_FromObject(o);
    if (memoryview == NULL)
    {
        PyErr_Clear();
		Py_RETURN_NONE;
    }

    view = PyMemoryView_GET_BUFFER(memoryview);
    unsigned char *buffer = (unsigned char *)view->buf;
    Py_DECREF(memoryview);

#else

    PyObject *ao = PyObject_GetAttrString(o, "__array_struct__");

    if ((ao == NULL) || !PyCObject_Check(ao))
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
		Py_RETURN_NONE;
    }

    PyArrayInterface *pai = (PyArrayInterface *)PyCObject_AsVoidPtr(ao);

    if (pai->two != 2)
    {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        Py_DECREF(ao);
		Py_RETURN_NONE;
    }

    // Get image information
    unsigned char *buffer = (unsigned char *)pai->data; // The address of image data
    Py_DECREF(ao);

#endif

    int frameId = DBR_AppendFrame(self->hBarcode, buffer);
    return Py_BuildValue("i",frameId);
}

static PyObject * InitLicenseFromLicenseContent(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pszLicenseKey;
    char *pszLicenseContent;
    if (!PyArg_ParseTuple(args, "ss", &pszLicenseKey, &pszLicenseContent))
    {
		Py_RETURN_NONE;
    }

    int ret = DBR_InitLicenseFromLicenseContent(self->hBarcode, pszLicenseKey, pszLicenseContent);
	const char* errorString = DBR_GetErrorString(ret);
	return Py_BuildValue("(i,s)", ret, errorString);
}

static PyObject * OutputLicenseToString(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char* content = NULL;
    int ret = DBR_OutputLicenseToStringPtr(self->hBarcode, &content);
    if (ret)
    {
        return Py_BuildValue("i", ret);
    }
    else
    {
        PyObject * licenseString = Py_BuildValue("s", content);
        DBR_FreeLicenseString(&content);
        return licenseString;
    }
}

static PyObject * InitLicenseFromServer(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *pszLicenseKey, *pLicenseServer;
    if (!PyArg_ParseTuple(args, "ss", &pLicenseServer, &pszLicenseKey))
    {
		Py_RETURN_NONE;
    }

    int ret = DBR_InitLicenseFromServer(self->hBarcode, pLicenseServer, pszLicenseKey);
	const char* errorString = DBR_GetErrorString(ret);
	return Py_BuildValue("(i,s)", ret, errorString);
}

static PyObject * InitRuntimeSettingsByJsonString(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *json;
	int conflictMode;
    if (!PyArg_ParseTuple(args, "si", &json, &conflictMode))
    {
		Py_RETURN_NONE;
    }

    char errorMessage[512];
    int ret = DBR_InitRuntimeSettingsWithString(self->hBarcode, json, conflictMode, errorMessage, 512);
    return Py_BuildValue("(i,s)", ret, errorMessage);
}

static PyObject * InitRuntimeSettingsByJsonFile(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *jsonPath;
	int conflictMode;
    if (!PyArg_ParseTuple(args, "si", &jsonPath, &conflictMode))
    {
		Py_RETURN_NONE;
    }

    char errorMessage[512];
    int ret = DBR_InitRuntimeSettingsWithFile(self->hBarcode, jsonPath, conflictMode, errorMessage, 512);
	return Py_BuildValue("(i,s)", ret, errorMessage);
}

static PyObject * OutputSettingsToJsonString(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char * pContent = NULL;

    int ret = DBR_OutputSettingsToStringPtr(self->hBarcode, &pContent, "CurrentRuntimeSettings");
    PyObject * content = Py_BuildValue("s", pContent);
    DBR_FreeSettingsString(&pContent);
    return content;
}

static PyObject * OutputSettingsToJsonFile(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char * jsonPath;
    if (!PyArg_ParseTuple(args, "s", &jsonPath))
    {
		Py_RETURN_NONE;
    }

    int ret = DBR_OutputSettingsToFile(self->hBarcode, jsonPath, "CurrentRuntimeSettings");

	const char* errorString = DBR_GetErrorString(ret);
	return Py_BuildValue("(i,s)", ret, errorString);
}

static PyObject * AppendTplFileToRuntimeSettings(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *jsonPath;
    int conflictMode;
    if (!PyArg_ParseTuple(args, "si", &jsonPath, &conflictMode))
    {
		Py_RETURN_NONE;
    }

    char errorMessage[512];
    int ret = DBR_AppendTplFileToRuntimeSettings(self->hBarcode, jsonPath, conflictMode, errorMessage, 512);

    return Py_BuildValue("(i,s)", ret, errorMessage);
}

static PyObject * AppendTplStringToRuntimeSettings(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    char *json;
    int conflictMode;
    if (!PyArg_ParseTuple(args, "si", &json, &conflictMode))
    {
		Py_RETURN_NONE;
    }

    char errorMessage[512];
    int ret = DBR_AppendTplStringToRuntimeSettings(self->hBarcode, json, conflictMode, errorMessage, 512);

	return Py_BuildValue("(i,s)", ret, errorMessage);
}

static PyObject * GetAllTemplateNames(PyObject *obj, PyObject *args)
{
    DynamsoftBarcodeReader *self = (DynamsoftBarcodeReader *)obj;

    int count = DBR_GetParameterTemplateCount(self->hBarcode);
	if (count == 0)
		Py_RETURN_NONE;

    PyObject * nameList = PyList_New(count);
	if (nameList == NULL)
		Py_RETURN_NONE;
    for(int i = 0; i < count; ++i)
    {
        char templateName[256];
        DBR_GetParameterTemplateName(self->hBarcode, i, templateName, 256);
        PyObject * pyTemplateName = Py_BuildValue("s", templateName);
        PyList_SetItem(nameList, i, pyTemplateName);
    }
    return nameList;
}

static PyMemberDef dbr_members[] = {
    {NULL}
};

static PyMethodDef dbr_methods[] = {
    {"initLicense",                     initLicense,                        METH_VARARGS, NULL},
    {"decodeFile",                      decodeFile,                         METH_VARARGS, NULL},
    {"decodeBuffer",                    decodeBuffer,                       METH_VARARGS, NULL},
    {"startVideoMode",                  startVideoMode,                     METH_VARARGS, NULL},
    {"stopVideoMode",                   stopVideoMode,                      METH_VARARGS, NULL},
    {"appendVideoFrame",                appendVideoFrame,                   METH_VARARGS, NULL},
    {"initLicenseFromLicenseContent",   initLicenseFromLicenseContent,      METH_VARARGS, NULL},
    {"outputLicenseToString",           outputLicenseToString,              METH_VARARGS, NULL},
    {"initLicenseFromServer",           initLicenseFromServer,              METH_VARARGS, NULL},
    {"setFurtherModes",                 setFurtherModes,                    METH_VARARGS, NULL},
    {"setParameters",                   setParameters,                      METH_VARARGS, NULL},
    {"getParameters",                   getParameters,                      METH_VARARGS, NULL},
    {"decodeFileStream",                decodeFileStream,                   METH_VARARGS, NULL},
	{"GetDBRVersion",					GetDBRVersion,						METH_VARARGS, NULL},
	{"GetErrorString",                  GetErrorString,                     METH_VARARGS, NULL},
	{"GetAllTextResults",               GetAllTextResults,                  METH_VARARGS, NULL},
	{"GetAllIntermediateResults",       GetAllIntermediateResults,          METH_VARARGS, NULL},
	{"GetLengthOfFrameQueue",			GetLengthOfFrameQueue,				METH_VARARGS, NULL},
    {"InitLicense",                     InitLicense,                        METH_VARARGS, NULL},
    {"DecodeFile",                      DecodeFile,                         METH_VARARGS, NULL},
    {"DecodeBuffer",                    DecodeBuffer,                       METH_VARARGS, NULL},
    {"StartVideoMode",                  StartVideoMode,                     METH_VARARGS, NULL},
    {"StopVideoMode",                   StopVideoMode,                      METH_VARARGS, NULL},
    {"AppendVideoFrame",                AppendVideoFrame,                   METH_VARARGS, NULL},
    {"InitLicenseFromLicenseContent",   InitLicenseFromLicenseContent,      METH_VARARGS, NULL},
    {"OutputLicenseToString",           OutputLicenseToString,              METH_VARARGS, NULL},
    {"InitLicenseFromServer",           InitLicenseFromServer,              METH_VARARGS, NULL},
    {"InitRuntimeSettingsByJsonString", InitRuntimeSettingsByJsonString,    METH_VARARGS, NULL},
    {"OutputSettingsToJsonString",      OutputSettingsToJsonString,         METH_VARARGS, NULL},
    {"InitRuntimeSettingsByJsonFile",   InitRuntimeSettingsByJsonFile,      METH_VARARGS, NULL},
    {"OutputSettingsToJsonFile",        OutputSettingsToJsonFile,           METH_VARARGS, NULL},
    {"AppendTplFileToRuntimeSettings",  AppendTplFileToRuntimeSettings,     METH_VARARGS, NULL},
    {"AppendTplStringToRuntimeSettings",AppendTplStringToRuntimeSettings,   METH_VARARGS, NULL},
    {"GetAllTemplateNames",             GetAllTemplateNames,                METH_VARARGS, NULL},
    {"DecodeFileStream",                DecodeFileStream,                   METH_VARARGS, NULL},
    {"GetRuntimeSettings",              GetRuntimeSettings,                 METH_VARARGS, NULL},
    {"UpdataRuntimeSettings",           UpdataRuntimeSettings,              METH_VARARGS, NULL},
    {"ResetRuntimeSettings",            ResetRuntimeSettings,               METH_VARARGS, NULL},
    {"SetModeArgument",                 SetModeArgument,                    METH_VARARGS, NULL},
    {"GetModeArgument",                 GetModeArgument,                    METH_VARARGS, NULL},
    {"InitFrameDecodingParameters",     InitFrameDecodingParameters,        METH_VARARGS, NULL},
    {NULL,                              NULL,                               0,            NULL}
};

static PyMethodDef module_methods[] =
{
    {NULL}
};

static int DynamsoftBarcodeReader_clear(DynamsoftBarcodeReader *self)
{
    DBR_DestroyInstance(self->hBarcode);
    return 0;
}

static void DynamsoftBarcodeReader_dealloc(DynamsoftBarcodeReader *self)
{
#if defined(IS_PY3K)
    DynamsoftBarcodeReader_clear(self);
    Py_TYPE(self)->tp_free((PyObject *)self);
#else
    DynamsoftBarcodeReader_clear(self);
    self->ob_type->tp_free((PyObject *)self);
#endif
}

static PyObject * DynamsoftBarcodeReader_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    DynamsoftBarcodeReader *self;

    self = (DynamsoftBarcodeReader *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->hBarcode = DBR_CreateInstance();
        const char *versionInfo = DBR_GetVersion();
        printf("Dynamsoft Barcode Reader %s\n", versionInfo);
        if (!self->hBarcode)
        {
            printf("Cannot allocate memory!\n");
            return NULL;
        }
    }

    return (PyObject *)self;
}

static int DynamsoftBarcodeReader_init(DynamsoftBarcodeReader *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

static PyTypeObject DynamsoftBarcodeReaderType = {
    PyVarObject_HEAD_INIT(NULL, 0) "dbr.DynamsoftBarcodeReader", /* tp_name */
    sizeof(DynamsoftBarcodeReader),                              /* tp_basicsize */
    0,                                                           /* tp_itemsize */
    (destructor)DynamsoftBarcodeReader_dealloc,                  /* tp_dealloc */
    0,                                                           /* tp_print */
    0,                                                           /* tp_getattr */
    0,                                                           /* tp_setattr */
    0,                                                           /* tp_reserved */
    0,                                                           /* tp_repr */
    0,                                                           /* tp_as_number */
    0,                                                           /* tp_as_sequence */
    0,                                                           /* tp_as_mapping */
    0,                                                           /* tp_hash  */
    0,                                                           /* tp_call */
    0,                                                           /* tp_str */
    0,                                                           /* tp_getattro */
    0,                                                           /* tp_setattro */
    0,                                                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                    /*tp_flags*/
    "Dynamsoft Barcode Reader objects",                          /* tp_doc */
    0,                                                           /* tp_traverse */
    0,                                                           /* tp_clear */
    0,                                                           /* tp_richcompare */
    0,                                                           /* tp_weaklistoffset */
    0,                                                           /* tp_iter */
    0,                                                           /* tp_iternext */
    dbr_methods,                                                 /* tp_methods */
    dbr_members,                                                 /* tp_members */
    0,                                                           /* tp_getset */
    0,                                                           /* tp_base */
    0,                                                           /* tp_dict */
    0,                                                           /* tp_descr_get */
    0,                                                           /* tp_descr_set */
    0,                                                           /* tp_dictoffset */
    (initproc)DynamsoftBarcodeReader_init,                       /* tp_init */
    0,                                                           /* tp_alloc */
    DynamsoftBarcodeReader_new,                                  /* tp_new */
};

#if defined(IS_PY3K)
static int dbr_traverse(PyObject *m, visitproc visit, void *arg)
{
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int dbr_clear(PyObject *m)
{
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "dbr",
    "Extension with Dynamsoft Barcode Reader.",
    -1,
    NULL, NULL, NULL, NULL, NULL};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_dbr(void)

#else
#define INITERROR return
void initdbr(void)
#endif
{
    if (PyType_Ready(&DynamsoftBarcodeReaderType) < 0)
        INITERROR;

#if defined(IS_PY3K)
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("dbr", module_methods);
#endif
    if (module == NULL)
        INITERROR;

    Py_INCREF(&DynamsoftBarcodeReaderType);
    PyModule_AddObject(module, "DynamsoftBarcodeReader", (PyObject *)&DynamsoftBarcodeReaderType);
#if defined(IS_PY3K)
    return module;
#endif
}
