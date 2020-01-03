# Dynamsoft Barcode Reader

[Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/Products/Dynamic-Barcode-Reader.aspx) enables you to efficiently embed barcode reading functionality in your web, desktop and mobile application using just a few lines of code. This can save you months of added development time and extra costs. With our SDK, you can create high-speed and reliable barcode scanner software to meet your business needs.

## Environment
**Python 2/3**

## Supported Symbologies
- Linear Barcodes (1D)

    - Code 39 (including Code 39 Extended)
    - Code 93
    - Code 128
    - Codabar
    - Interleaved 2 of 5
    - EAN-8
    - EAN-13
    - UPC-A
    - UPC-E
    - Industrial 2 of 5

- 2D Barcodes:
    - QR Code (including Micro QR Code)
    - Data Matrix
    - PDF417 (including Micro PDF417)
    - Aztec Code
    - MaxiCode (mode 2-5)

- Patch Code
- GS1 DataBar (Omnidirectional,
Truncated, Stacked, Stacked
Omnidirectional, Limited,
Expanded, Expanded Stacked)
- GS1 Composite Code


## Installation

### Windows

```bash
pip install dbr
```

### Linux, macOS and Raspberry Pi

Build the extension from the [source code](https://github.com/dynamsoft-dbr/python-barcode/tree/master/src).

## Quick Usage

```python
from dbr import DynamsoftBarcodeReader
dbr = DynamsoftBarcodeReader()
dbr.initLicense('YOUR-LICENSE') # https://www.dynamsoft.com/CustomerPortal/Portal/Triallicense.aspx
results = dbr.DecodeFile(fileName)
textResults = results["TextResults"]
for textResult in textResults:
    print(textResult["BarcodeFormatString"])
    print(textResult["BarcodeText"])
```

## Functions

- InitLicense(license-key)
- DecodeFile(filename) 
- DecodeBuffer(frame-by-opencv-capture, height, width, stride)
- DecodeFileStream(fileStream, fileSize)
    ```
    Code Snippet:
        # The value returned by every decode method is a dictionary object, it includes two items: TextResults and IntermediateResults.And the two items are both list objects.
        results = DecodeFile(fileName)
        textResults = results["TextResults"]
        intermediateResults = results["IntermediateResults"]
        # Each item in textResults or intermediateResults is a dictionary object. 
        # if you want some individual results in textResult or intermediateReuslt, you can get all keys in textResult or intermediateReuslt and get the value by the key.
        for textResult in textResults:
            print(textResult.keys())
            print("BarcodeFormat:" + textResult["BarcodeFormatString"])
            print("BarcodeText:" + textResult["BarcodeText"])
    ```

    | TextReuslt            | Type                              |
    | --------------------- |-----------------------------------|
    | BarcodeFormat         | LONG                              |
    | BarcodeFormatString   | String                            |
    | BarcodeFormat_2       | LONG                              |
    | BarcodeFormatString_2 | String                            |
    | BarcodeText           | String                            |
    | BarcodeBytes          | ByteArray                         |
    | LocalizationResult    | Dictionary<LocalizationResult>    |
    | DetailedResult        | Dictionary<DetailedResult>        |
    | ExtendedResults       | List<ExtendedResult>              |

  
    | LocalizationResult        | Type              |
    | --------------------------|-------------------|
    | TerminatePhase            | LONG              |
    | BarcodeFormat             | LONG              |
    | BarcodeFormatString       | String            |
    | BarcodeFormat_2           | LONG              |
    | BarcodeFormatString_2     | String            |
    | X1                        | LONG              |
    | Y1                        | LONG              |
    | X2                        | LONG              |
    | Y2                        | LONG              |
    | X3                        | LONG              |
    | Y3                        | LONG              |
    | X4                        | LONG              |
    | Y4                        | LONG              |
    | Angle                     | LONG              |
    | ModuleSize                | LONG              |
    | PageNumber                | LONG              |
    | RegionName                | String            |
    | DocumentName              | String            |
    | ResultCoordinateType      | LONG              |
    | AccompanyingTextBytes     | ByteArray         |
    | Confidence                | LONG              |

    ```
    DetailedReuslt depends on BarcodeFormat:
    ```

    | DetailedReuslt                                | Type              |
    | ----------------------------------------------|-------------------|
    | ModuleSize(OneD,QR,DataMatrix,PDF417,AZTEC)   | LONG              |
    | StartCharsBytes(OneD)                         | ByteArray         |
    | StopCharsBytes(OneD)                          | ByteArray         |
    | CheckDigitBytes(Oned)                         | ByteArray         |
    | Rows(QR,DataMatrix,PDF417,AZTEC)              | LONG              |
    | Columns(QR,DataMatrix,PDF417,AZTEC)           | LONG              |
    | ErrorCorrectionLevel(QR,PDF417)               | LONG              |
    | version(QR)                                   | LONG              |
    | model(QR)                                     | LONG              |
    | DataRegionRows(DataMatrix)                    | LONG              |
    | DataRegionColumns(DataMatrix)                 | LONG              |
    | DataRegionNumber(DataMatrix)                  | LONG              |
    | LayerNumber(AZTEC)                            | LONG              |

    | ExtendedResult            | Type                      |
    | --------------------------|---------------------------|
    | ResultType                | LONG                      |
    | BarcodeFormat             | LONG                      |
    | BarcodeFormatString       | String                    |
    | BarcodeFormat_2           | LONG                      |
    | BarcodeFormatString_2     | String                    |
    | Confidence                | LONG                      |
    | Bytes                     | ByteArray                 |
    | AccompanyingTextBytes     | ByteArray                 |
    | Deformation               | LONG                      |
    | SamplingImage             | Dictionary<IMResultData>  |
    | Clarity                   | LONG                      |

    | SamplingImage            | Type              |
    | -------------------------|-------------------|
    | Bytes                    | ByteArray         |
    | Width                    | LONG              |
    | Height                   | LONG              |

    | IntermediateResult            | Type              |
    | ------------------------------|-------------------|
    | DataType                      | LONG              |
    | IMResults                     | List<IMResultData>|
    | ResultType                    | LONG              |
    | BarcodeComplementMode         | LONG              |
    | BCMIndex                      | LONG              |
    | DPMCodeReadingMode            | LONG              |
    | DPMCRMIndex                   | LONG              |
    | RotationMatrix                | List[9]<double>   |
    | TextFilterMode                | LONG              |
    | TFMIndex                      | LONG              |
    | LocalizationMode              | LONG              |
    | LMIndex                       | LONG              |
    | BinarizationMode              | LONG              |
    | BMIndex                       | LONG              |
    | ImagePreprocessingMode        | LONG              |
    | IPMIndex                      | LONG              |
    | ROIId                         | LONG              |
    | RegionPredetectionMode        | LONG              |
    | RPMIndex                      | LONG              |
    | GrayscaleTransformationMode   | LONG              |
    | GTMIndex                      | LONG              |
    | ColourConversionMode          | LONG              |
    | CICMIndex                     | LONG              |
    | ColourClusteringMode          | LONG              |
    | CCMIndex                      | LONG              |
    | ScaleDownRatio                | LONG              |
    | FrameId                       | LONG              |

    ```
    IMResultData depends on DataType:
    ```

    | IMResultData                                  | Type                          |
    | ----------------------------------------------|-------------------------------|
    | Bytes(IMRDT_IMAGE)                            | ByteArray                     |
    | Width(IMRDT_IMAGE,IMRDT_REGIONOFINTEREST)     | LONG                          |
    | Height(IMRDT_IMAGE,IMRDT_REGIONOFINTEREST)    | LONG                          |
    | Stride(IMRDT_IMAGE)                           | LONG                          |
    | Points(IMRDT_CONTOUR)                         | List<Point>                   |
    | StartPoint(IMRDT_LINESEGMENT)                 | Dictionary<Point>             |
    | EndPoint(IMRDT_LINESEGMENT)                   | Dictionary<Point>             |
    | LinesConfidenceCoefficients(IMRDT_LINESEGMENT)| List<LONG>                    |
    | ROIId(IMRDT_REGIONOFINTEREST)                 | LONG                          |
    | Point(IMRDT_REGIONOFINTEREST)                 | Dictionary<Point>             |
    | LocalizationRsult(IMRDT_LOCALIZATIONRESULT)   | Dictionary<LocalizationResult>|


    | Point                | Type              |
    | ---------------------|-------------------|
    | X                    | LONG              |
    | Y                    | LONG              |

- InitFrameDecodingParameters()
- StartVideoMode(frameDecodingParameters, callback)

    | FrameDecodingParameters     | Type              |
    | ----------------------------|-------------------|
    | MaxQueueLength              | LONG              |
    | MaxResultQueueLength        | LONG              |
    | Width                       | LONG              |
    | Height                      | LONG              |
    | Stride                      | LONG              |
    | ImagePixelFormat            | LONG              |
    | RegionBottom                | LONG              |
    | RegionLeft                  | LONG              |
    | RegionRight                 | LONG              |
    | RegionTop                   | LONG              |
    | RegionMeasuredByPercentage  | LONG              |
    | Threshold                   | Float             |
    | FPS                         | LONG              |

- StopVideoMode()
- AppendVideoFrame(frame-by-opencv-capture)
- InitLicenseFromLicenseContent(license-key, license-content)
- OutputLicenseToString()
- InitLicenseFromServer(license-key, license-server)
- InitRuntimeSettingsByJsonString(jsonTemplateString)
- OutputSettingsToJsonString()
- InitRuntimeSettingsByJsonFile(jsonTmeplateFile)
- OutputSettingsToJsonFile(outputJsonFile)
- AppendTplStringToRuntimeSettings(jsonTemplateString, conflictMode)
- AppendTplFileToRuntimeSettings(jsonTmeplateFile, conflictMode)

    ```
    conflictMode = dbr.CM_IGNORE or dbr.CM_OVERWRITE
    ```

- GetAllTemplateNames()
- GetRuntimeSettings()
- UpdataRuntimeSettings(settings)

    ```
    Code Snippet:
    # if you want to modify some values in RuntimeSettings, you can refer to the following code.
    # Attention: before using the UpdataRuntimeSettings() method, you must use the GetRuntimeSettings() method to get the current runtime settings.
    settings = GetRuntimeSettings()
    settings["BarcodeFormatIds"] = dbr.BF_ONED | dbr.BF_GS1_DATABAR
    settings["ExpectedBarcodesCount"] = 10
    settings["BinarizationModes"] = [dbr.BM_LOCAL_BLOCK, 0,0,0,0,0,0,0]
    settings["LocalizationModes"] = [dbr.LM_CONNECTED_BLOCKS, 0,0,0,0,0,0,0]
    settings["IntermediateResultSavingMode"] = dbr.IRSM_BOTH
    settings["IntermediateResultTypes"] = dbr.IRT_ORIGINAL_IMAGE | dbr.IRT_BINARIZED_IMAGE
    errorCode = UpdataRuntimeSettings(settings)
    ```

    | RuntimeSettings               | Type              |
    | ------------------------------|-------------------|
    | TerminatePhase                | LONG              |
    | Timeout                       | LONG              |
    | MaxAlgorithmThreadCount       | LONG              |
    | ExpectedBarcodesCount         | LONG              |
    | BarcodeFormatIds              | LONG              |
    | BarcodeFormatIds_2            | LONG              |
    | PDFRasterDPI                  | LONG              |
    | ScaleDownThreshold            | LONG              |
    | BinarizationModes             | List[8]           |
    | LocalizationModes             | List[8]           |
    | ColourClusteringModes         | List[8]           |
    | ColourConversionModes         | List[8]           |
    | GrayscaleTransformationModes  | List[8]           |
    | RegionPredetectionModes       | List[8]           |
    | ImagePreprocessingModes       | List[8]           |
    | TextureDetectionModes         | List[8]           |
    | TextFilterModes               | List[8]           |
    | DPMCodeReadingModes           | List[8]           |
    | DeformationResistingModes     | List[8]           |
    | BarcodeComplementModes        | List[8]           |
    | BarcodeColourModes            | List[8]           |
    | TextResultOrderModes          | List[8]           |
    | TextAssistedCorrectionMode    | LONG              |
    | DeblurLevel                   | LONG              |
    | IntermediateResultTypes       | LONG              |
    | IntermediateResultSavingMode  | LONG              |
    | ResultCoordinateType          | LONG              |
    | ReturnBarcodeZoneClarity      | LONG              |
    | RegionTop                     | LONG              |
    | RegionBottom                  | LONG              |
    | RegionLeft                    | LONG              |
    | RegionRight                   | LONG              |
    | RegionMeasuredByPercentage    | LONG              |
    | MinBarcodeTextLength          | LONG              |
    | MinResultConfidence           | LONG              |

- ResetRuntimeSettings()
- SetModeArgument(modesName, index, argumentName, argumentValue)
- GetModeArgument(modesName, index, argumentName)
    ```
    Code Snippet:
    errorCode = SetModeArgument("BinarizationModes", 0, "BlockSizeX", "3")
    argumentValue = GetModeArgument("BinarizationModes", 0, "BlockSizeX")
    ```

    | ModesName                     | ArgumentName                  |
    | ------------------------------|-------------------------------|
    | BarcodeColourModes            | LightReflection               |
    | BinarizationModes             | BlockSizeX                    |
    | BinarizationModes             | BlockSizeY                    |
    | BinarizationModes             | EnableFillBinaryVacancy       |
    | BinarizationModes             | ImagePreprocessingModesIndex  |
    | BinarizationModes             | ThreshValueCoefficient        |
    | ColourClusteringModes         | Sensitivity                   |
    | ColourConversionModes         | BlueChannelWeight             |
    | ColourConversionModes         | GreenChannelWeight            |
    | ColourConversionModes         | RedChannelWeight              |
    | DeformationResistingModes     | Level                         |
    | ImagePreprocessingModes       | Sensitivity                   |
    | ImagePreprocessingModes       | SmoothBlockSizeX              |
    | ImagePreprocessingModes       | SmoothBlockSizeY              |
    | ImagePreprocessingModes       | SharpenBlockSizeX             |
    | ImagePreprocessingModes       | SharpenBlockSizeY             |
    | IntermediateResultSavingMode  | FolderPath                    |
    | IntermediateResultSavingMode  | RecordsetSizeOfLatestImages   |
    | LocalizationModes             | ScanStride                    |
    | RegionPredetectionModes       | MinImageDimension             |
    | RegionPredetectionModes       | Sensitivity                   |
    | TextAssistedCorrectionMode    | BottomTextPercentageSize      |
    | TextAssistedCorrectionMode    | LeftTextPercentageSize        |
    | TextAssistedCorrectionMode    | RightTextPercentageSize       |
    | TextAssistedCorrectionMode    | TopTextPercentageSize         |
    | TextFilterModes               | MinImageDimension             |
    | TextFilterModes               | Sensitivity                   |
    | TextureDetectionModes         | Sensitivity                   |


## Deprecated Functions

- initLicense(license-key)
- decodeFile(filename, format) 
- decodeBuffer(frame-by-opencv-capture, format)
- decodeFileStream(fileStream, fileSize, format)
- startVideoMode(max_buffer, max_results, video_width, video_height, stride, format, callback)
- stopVideoMode()
- appendVideoFrame(frame-by-opencv-capture)
- initLicenseFromLicenseContent(license-key, license-content)
- outputLicenseToString()
- initLicenseFromServer(license-key, license-server)
- setFurtherModes(mode, [values])
- setParameters(json-string)
- getParameters()


## License Key
Get the [free trial license](https://www.dynamsoft.com/CustomerPortal/Portal/Triallicense.aspx).

## Contact 
support@dynamsoft.com




    


