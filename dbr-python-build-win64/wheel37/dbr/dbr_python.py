import sys
import os
import cv2
import numpy
from enum import IntEnum
from dbr import DynamsoftBarcodeReader

#region Enum

class EnumErrorCode(IntEnum):
    """Error code"""

    DBR_OK = 0
    DBRERR_UNKNOWN = -10000
    DBRERR_NO_MEMORY = -10001
    DBRERR_NULL_POINTER = -10002
    DBRERR_LICENSE_INVALID = -10003
    DBRERR_LICENSE_EXPIRED = -10004
    DBRERR_FILE_NOT_FOUND = -10005
    DBRERR_FILETYPE_NOT_SUPPORTED = -10006
    DBRERR_BPP_NOT_SUPPORTED = -10007
    DBRERR_INDEX_INVALID = -10008
    DBRERR_BARCODE_FORMAT_INVALID = -10009
    DBRERR_CUSTOM_REGION_INVALID = -10010
    DBRERR_MAX_BARCODE_NUMBER_INVALID = -10011
    DBRERR_IMAGE_READ_FAILED = -10012
    DBRERR_TIFF_READ_FAILED = -10013
    DBRERR_QR_LICENSE_INVALID = -10016
    DBRERR_1D_LICENSE_INVALID = -10017
    DBRERR_DIB_BUFFER_INVALID = -10018
    DBRERR_PDF417_LICENSE_INVALID = -10019
    DBRERR_DATAMATRIX_LICENSE_INVALID = -10020
    DBRERR_PDF_READ_FAILED = -10021
    DBRERR_PDF_DLL_MISSING = -10022
    DBRERR_PAGE_NUMBER_INVALID = -10023
    DBRERR_CUSTOM_SIZE_INVALID = -10024
    DBRERR_CUSTOM_MODULESIZE_INVALID = -10025
    DBRERR_RECOGNITION_TIMEOUT = -10026
    DBRERR_JSON_PARSE_FAILED = -10030
    DBRERR_JSON_TYPE_INVALID = -10031
    DBRERR_JSON_KEY_INVALID = -10032
    DBRERR_JSON_VALUE_INVALID = -10033
    DBRERR_JSON_NAME_KEY_MISSING = -10034
    DBRERR_JSON_NAME_VALUE_DUPLICATED = -10035
    DBRERR_TEMPLATE_NAME_INVALID = -10036
    DBRERR_JSON_NAME_REFERENCE_INVALID = -10037
    DBRERR_PARAMETER_VALUE_INVALID = -10038
    DBRERR_DOMAIN_NOT_MATCHED = -10039
    DBRERR_RESERVEDINFO_NOT_MATCHED = -10040
    DBRERR_AZTEC_LICENSE_INVALID = -10041
    DBRERR_LICENSE_DLL_MISSING = -10042
    DBRERR_LICENSEKEY_NOT_MATCHED = -10043
    DBRERR_REQUESTED_FAILED = -10044
    DBRERR_LICENSE_INIT_FAILED = -10045
    DBRERR_PATCHCODE_LICENSE_INVALID = -10046
    DBRERR_POSTALCODE_LICENSE_INVALID = -10047
    DBRERR_DPM_LICENSE_INVALID = -10048
    DBRERR_FRAME_DECODING_THREAD_EXISTS = -10049
    DBRERR_STOP_DECODING_THREAD_FAILED = -10050
    DBRERR_SET_MODE_ARGUMENT_ERROR = -10051
    DBRERR_LICENSE_CONTENT_INVALID = -10052
    DBRERR_LICENSE_KEY_INVALID = -10053
    DBRERR_LICENSE_DEVICE_RUNS_OUT = -10054
    DBRERR_GET_MODE_ARGUMENT_ERROR = -10055
    DBRERR_IRT_LICENSE_INVALID = -10056
    DBRERR_MAXICODE_LICENSE_INVALID = -10057
    DBRERR_GS1_DATABAR_LICENSE_INVALID = -10058
    DBRERR_GS1_COMPOSITE_LICENSE_INVALID = -10059

class EnumBarcodeFormat(IntEnum):
    """ Describes the barcode types in BarcodeFormat group 1. """

    BF_ALL = -32505857
    BF_ONED = 0x000007FF
    BF_GS1_DATABAR = 0x0003F800
    BF_CODE_39 = 0x1
    BF_CODE_128 = 0x2
    BF_CODE_93 = 0x4
    BF_CODABAR = 0x8
    BF_ITF = 0x10
    BF_EAN_13 = 0x20
    BF_EAN_8 = 0x40
    BF_UPC_A = 0x80
    BF_UPC_E = 0x100
    BF_INDUSTRIAL_25 = 0x200
    BF_CODE_39_EXTENDED = 0x400
    BF_GS1_DATABAR_OMNIDIRECTIONAL = 0x800
    BF_GS1_DATABAR_TRUNCATED = 0x1000
    BF_GS1_DATABAR_STACKED = 0x2000
    BF_GS1_DATABAR_STACKED_OMNIDIRECTIONAL = 0x4000
    BF_GS1_DATABAR_EXPANDED = 0x8000
    BF_GS1_DATABAR_EXPANDED_STACKED = 0x10000
    BF_GS1_DATABAR_LIMITED = 0x20000
    BF_PATCHCODE = 0x00040000
    BF_PDF417 = 0x02000000
    BF_QR_CODE = 0x04000000
    BF_DATAMATRIX = 0x08000000
    BF_AZTEC = 0x10000000
    BF_MAXICODE = 0x20000000
    BF_MICRO_QR = 0x40000000
    BF_MICRO_PDF417 = 0x00080000
    BF_GS1_COMPOSITE = -2147483648
    BF_NULL = 0x00

class EnumBarcodeFormat_2(IntEnum):
    """ Describes the barcode types in BarcodeFormat group 2. """

    BF2_NULL = 0x00
    BF2_POSTALCODE = 0x01F00000
    BF2_NONSTANDARD_BARCODE = 0x01
    BF2_USPSINTELLIGENTMAIL = 0x00100000
    BF2_POSTNET = 0x00200000
    BF2_PLANET = 0x00400000
    BF2_AUSTRALIANPOST = 0x00800000
    BF2_RM4SCC = 0x01000000

class EnumBarcodeComplementMode(IntEnum):
    """ Describes the barcode complement mode. """

    BCM_AUTO = 0x01
    BCM_GENERAL = 0x02
    BCM_SKIP = 0x00

class EnumImagePixelFormat(IntEnum):
    """ Describes the image pixel format. """

    IPF_BINARY = 0
    IPF_BINARYINVERTED = 1
    IPF_GRAYSCALED = 2
    IPF_NV21 = 3
    IPF_RGB_565 = 4
    IPF_RGB_555 = 5
    IPF_RGB_888 = 6
    IPF_ARGB_8888 = 7
    IPF_RGB_161616 = 8
    IPF_ARGB_16161616 = 9

class BarcodeColourMode(IntEnum):
    """ Describes the barcode colour mode. """

    BICM_DARK_ON_LIGHT = 0x01
    BICM_LIGHT_ON_DARK = 0x02
    BICM_DARK_ON_DARK = 0x04
    BICM_LIGHT_ON_LIGHT = 0x08
    BICM_DARK_LIGHT_MIXED = 0x10
    BICM_DARK_ON_LIGHT_DARK_SURROUNDING = 0x20
    BICM_SKIP = 0x00

class EnumBinarizationMode(IntEnum):
    """ Describes the binarization mode. """

    BM_AUTO = 0x01
    BM_LOCAL_BLOCK = 0x02
    BM_SKIP = 0x00

class EnumColourClusteringMode(IntEnum):
    """ Describes the colour clustering mode. """

    CCM_AUTO = 0x01
    CCM_GENERAL_HSV = 0x02
    CCM_SKIP = 0x00

class EnumColourConversionMode(IntEnum):
    """ Describes the colour conversion mode. """

    CICM_GENERAL = 0x01
    CICM_SKIP = 0x00

class EnumDPMCodeReadingMode(IntEnum):
    """ Describes the DPM code reading mode. """

    DPMCRM_AUTO = 0x01
    DPMCRM_GENERAL = 0x02
    DPMCRM_SKIP = 0x00

class EnumConflictMode(IntEnum):
    """ Describes the conflict mode. """

    CM_IGNORE = 1
    CM_OVERWRITE = 2

class EnumImagePreprocessingMode(IntEnum):
    """ Describes the image preprocessing mode. """

    IPM_AUTO = 0x01
    IPM_GENERAL = 0x02
    IPM_GRAY_EQUALIZE = 0x04
    IPM_GRAY_SMOOTH = 0x08
    IPM_SHARPEN_SMOOTH = 0x10
    IPM_SKIP = 0x00

class EnumIntermediateResultType(IntEnum):
    """ Describes the intermediate result type. """

    IRT_NO_RESULT = 0x00
    IRT_ORIGINAL_IMAGE = 0x01
    IRT_COLOUR_CLUSTERED_IMAGE = 0x02
    IRT_COLOUR_CONVERTED_GRAYSCALE_IMAGE = 0x04
    IRT_TRANSFORMED_GRAYSCALE_IMAGE = 0x08
    IRT_PREDETECTED_REGION = 0x10
    IRT_PREPROCESSED_IMAGE = 0x20
    IRT_BINARIZED_IMAGE = 0x40
    IRT_TEXT_ZONE = 0x80
    IRT_CONTOUR = 0x100
    IRT_LINE_SEGMENT = 0x200
    IRT_FORM = 0x400
    IRT_SEGMENTATION_BLOCK = 0x800
    IRT_TYPED_BARCODE_ZONE = 0x1000

class LocalizationMode(IntEnum):
    """ Describes the localization mode. """

    LM_AUTO = 0x01
    LM_CONNECTED_BLOCKS = 0x02
    LM_STATISTICS = 0x04
    LM_LINES = 0x08
    LM_SCAN_DIRECTLY = 0x10
    LM_STATISTICS_MARKS = 0x20
    LM_STATISTICS_POSTAL_CODE = 0x40
    LM_SKIP = 0x00

class EnumQRCodeErrorCorrectionLevel(IntEnum):
    """ Describes the QR Code error correction level. """

    QRECL_ERROR_CORRECTION_H = 0
    QRECL_ERROR_CORRECTION_L = 0
    QRECL_ERROR_CORRECTION_M = 0
    QRECL_ERROR_CORRECTION_Q = 0

class EnumRegionPredetectionMode(IntEnum):
    """ Describes the region predetection mode. """

    RPM_AUTO = 0x01
    RPM_GENERAL = 0x02
    RPM_GENERAL_RGB_CONTRAST = 0x04
    RPM_GENERAL_GRAY_CONTRAST = 0x08
    RPM_GENERAL_HSV_CONTRAST = 0x10
    RPM_SKIP = 0x00

class EnumDeformationResistingMode(IntEnum):
    """ Describes the deformation resisting mode. """

    DRM_AUTO = 0x01
    DRM_GENERAL = 0x02
    DRM_SKIP = 0x00

class EnumResultType(IntEnum):
    """ Describes the extended result type. """

    RT_STANDARD_TEXT = 0
    RT_RAW_TEXT = 1
    RT_CANDIDATE_TEXT = 2
    RT_PARTIAL_TEXT = 3

class EnumTerminatePhase(IntEnum):
    """ Describes the terminate phase. """

    TP_REGION_PREDETECTED = 0x01
    TP_IMAGE_PREPROCESSED = 0x02
    TP_IMAGE_BINARIZED = 0x04
    TP_BARCODE_LOCALIZED = 0x08
    TP_BARCODE_TYPE_DETERMINED = 0x10
    TP_BARCODE_RECOGNIZED = 0x20

class EnumTextAssistedCorrectionMode(IntEnum):
    """ Describes the text assisted correction mode. """

    TACM_AUTO = 0x01
    TACM_VERIFYING = 0x02
    TACM_VERIFYING_PATCHING = 0x04
    TACM_SKIP = 0x00

class EnumTextFilterMode(IntEnum):
    """ Describes the text filter mode. """
    
    TFM_AUTO = 0x01
    TFM_GENERAL_CONTOUR = 0x02
    TFM_SKIP = 0x00

class EnumIntermediateResultSavingMode(IntEnum):
    """ Describes the intermediate result saving mode. """

    IRSM_MEMORY = 0x01
    IRSM_FILESYSTEM = 0x02
    IRSM_BOTH = 0x04

class EnumTextResultOrderMode(IntEnum):
    """ Describes the text result order mode. """

    TROM_CONFIDENCE = 0x01
    TROM_POSITION = 0x02
    TROM_FORMAT = 0x04
    TROM_SKIP = 0x00
    
class EnumTextureDetectionMode(IntEnum):
    """ Describes the texture detection mode. """

    TDM_AUTO = 0X01
    TDM_GENERAL_WIDTH_CONCENTRATION = 0X02
    TDM_SKIP = 0x00

class EnumGrayscaleTransformationMode(IntEnum):
    """ Describes the grayscale transformation mode. """

    GTM_INVERTED = 0x01
    GTM_ORIGINAL = 0x02
    GTM_SKIP = 0x00

class EnumResultCoordinateType(IntEnum):
    """ Describes the result coordinate type. """

    RCT_PIXEL = 0x01
    RCT_PERCENTAGE = 0x02

class EnumIMResultDataType(IntEnum):
    """ Describes the intermediate result data type. """

    IMRDT_IMAGE = 0x01
    IMRDT_CONTOUR = 0x02
    IMRDT_LINESEGMENT = 0x04
    IMRDT_LOCALIZATIONRESULT = 0x08
    IMRDT_REGIONOFINTEREST = 0x10

class EnumScaleUpMode(IntEnum):
    """ Describes the scale up mode. """

    SUM_AUTO = 0x01
    SUM_LINEAR_INTERPOLATION = 0x02
    SUM_NEAREST_NEIGHBOUR_INTERPOLATION = 0x04
    SUM_SKIP = 0x00

class EnumAccompanyingTextRecognitionMode(IntEnum):
    """ Describes the accompanying text recognition mode. """

    ATRM_GENERAL = 0x01
    ATRM_SKIP = 0x00

#endregion

#region struct

class SamplingImageData:
    """ 
    Stores the sampling image data. 

    Attributes:
    -----------
    - bytes <bytearray> : The sampling image data in a byte array

    - width <int> : The width of the sampling image

    - height <int> : The height of the sampling image
    """

    def __init__(self, sampling_image_data):
        '''Init Function'''
        self.bytes = sampling_image_data["Bytes"]
        self.width = sampling_image_data["Width"]
        self.height = sampling_image_data["Height"]

class FrameDecodingParameters:
    """ 
    Defines a class to configure the frame decoding Parameters. 
    
    Attributes:
    -----------
    - max_queue_length <int> : The maximum number of frames waiting for decoding
        - Value range : [0,0x7fffffff]
        - Default value : 3

    - max_result_queue_length <int> : The maximum number of frames waiting results (text result/localization result) will be kept for further reference
        - Value range : [0,0x7fffffff]
        - Default value : 10

    - width <int> : The width of the frame image in pixels
        - Value range : [0,0x7fffffff]
        - Default value : 0

    - height <int> : The height of the frame image in pixels
        - Value range : [0,0x7fffffff]
        - Default value : 0

    - stride <int> : The stride (or scan width) of the frame image
        - Value range : [0,0x7fffffff]
        - Default value : 0

    - image_pixel_format <EnumImagePixelFormat> : The image pixel format used in the image byte array
        - Value range : A value of ImagePixelFormat Enumeration items
        - Default value : EnumImagePixelFormat.IPF_GRAYSCALED
    
    - region_top <int> : The region definition of the frame to calculate the internal indicator.The top-most coordinate or percentage of the region.
        - Value range : 
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100] 
        - Default value : 0

    - region_left <int> : The region definition of the frame to calculate the internal indicator.The left-most coordinate or percentage of the region
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100] 
        - Default value : 0

    - region_right <int> : The region definition of the frame to calculate the internal indicator.The right-most coordinate or percentage of the region
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100] 
        - Default value : 0

    - region_bottom <int> : The region definition of the frame to calculate the internal indicator.The bottom-most coordinate or percentage of the region
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100] 
        - Default value : 0

    - region_measured_by_percentage <int> : Sets whether or not to use percentage to measure the region size
        - Value range : [0,1]
        - Default value : 0
        - remarks : When it's set to 1, the values of Top, Left, Right, Bottom indicate percentage (from 0 to 100); Otherwise, they indicate coordinates. 0: not by percentage, 1: by percentage.

    - threshold <double> : The threshold used for filtering frames.
        - Value range : [0,1]
        - Default value : 0.1
        - Remarks : The SDK will calculate an inner indicator for each frame from append_frame(), if the change rate of the indicators between the current frame 
        and the history frames is larger than the given threshold, the current frame will not be added to the inner frame queue waiting for decoding.
    
    - fps <int> : The frequency of calling AppendFrame() per second.
        - Value range : [0,0x7fffffff]
        - Default value : 0
        - Remarks : 0 means the frequency will be calculated automatically by the SDK.

    - auto_filter <int> : Sets whether to filter frames automatically.
        - Value range : [0,1]
        - Default value : 1
        - Remarks : 0:Diable filtering frames automatically. 1:Enable filtering frames automatically.

    """

    def __init__(self, parameters):
        ''' Init Function '''
        self.max_queue_length = parameters["MaxQueueLength"]
        self.max_result_queue_length = parameters["MaxResultQueueLength"]
        self.width = parameters["Width"]
        self.height = parameters["Height"]
        self.stride = parameters["Stride"]
        self.image_pixel_format = parameters["ImagePixelFormat"]
        self.region_top = parameters["RegionTop"]
        self.region_left = parameters["RegionLeft"]
        self.region_right = parameters["RegionRight"]
        self.region_bottom = parameters["RegionBottom"]
        self.region_measured_by_percentage = parameters["RegionMeasuredByPercentage"]
        self.threshold = parameters["Threshold"]
        self.fps = parameters["FPS"]
        self.auto_filter = parameters["AutoFilter"]

    def update_parameters(self, parameters):
        ''' update frame decoding parameters '''
        parameters["MaxQueueLength"] = self.max_queue_length
        parameters["MaxResultQueueLength"] = self.max_result_queue_length
        parameters["Width"] = self.width
        parameters["Height"] = self.height
        parameters["Stride"] = self.stride
        parameters["ImagePixelFormat"] = self.image_pixel_format
        parameters["RegionTop"] = self.region_top
        parameters["RegionLeft"] = self.region_left
        parameters["RegionRight"] = self.region_right
        parameters["RegionBottom"] = self.region_bottom
        parameters["RegionMeasuredByPercentage"] = self.region_measured_by_percentage
        parameters["Threshold"] = self.threshold
        parameters["FPS"] = self.fps
        parameters["AutoFilter"] = self.auto_filter

class PublicRuntimeSetting:
    """"
    Defines a struct to configure the barcode reading runtime settings. These settings control the barcode recognition process such as which barcode types to decode. 

    Attributes:
    -----------
    - terminate_phase <EnumTerminatePhase> : Sets the phase to stop the barcode reading algorithm.
        - Value range : Any one of the TerminatePhase Enumeration items
        - Default value : EnumTerminatePhase.TP_BARCODE_RECOGNIZED
        - Remarks : When the recognition result is not desired, you can set this parameter can be set to skip certain processing stages.

    - timeout <int> : Sets the maximum amount of time (in milliseconds) that should be spent searching for a barcode per page. It does not include the time taken to load/decode an image (TIFF, PNG, etc.) from disk into memory.
        - Value range : [0, 0x7fffffff]
        - Default value : 10000
        - Remarks : If you want to stop reading barcodes after a certain period of time, you can use this parameter to set a timeout.

    - max_algorithm_thread_count <int> : Sets the number of threads the image processing algorithm will use to decode barcodes.
        - Value range : [1, 4]
        - Default value : 4
        - Remarks : To keep a balance between speed and quality, the library concurrently runs four different threads for barcode decoding by default. 

    - expected_barcodes_count <int> :Sets the number of barcodes expected to be detected for each image.
        - Value range : [0, 0x7fffffff]
        - Default value : 0
        - Remarks : 
            - 0: means Unknown and it will find at least one barcode.
            - 1: try to find one barcode. If one barcode is found, the library will stop the localization process and perform barcode decoding.
            - n: try to find n barcodes. If the library only finds m (m<n) barcode, it will try different algorithms till n barcodes are found or all algorithms are tried.

    - barcode_format_ids <int> : Sets the formats of the barcode in BarcodeFormat group 1 to be read. Barcode formats in BarcodeFormat group 1 can be combined.
        - Value range : A combined value of BarcodeFormat Enumeration items
        - Default value : EnumBarcodeFormat.BF_ALL
        - Remarks : If the barcode type(s) are certain, specifying the barcode type(s) to be read will speed up the recognition process.

    - barcode_format_ids_2 <int> : Sets the formats of the barcode in BarcodeFormat group 2 to be read. Barcode formats in BarcodeFormat group 2 can be combined.
        - Value range : A combined value of BarcodeFormat_2 Enumeration items
        - Default value : EnumBarcodeFormat_2.BF2_NULL
        - Remarks : If the barcode type(s) are certain, specifying the barcode type(s) to be read will speed up the recognition process.

    - pdf_raster_dpi <int> : Sets the output image resolution.
        - Value range : [100, 600]
        - Default value : 300
        - Remarks :  When decoding barcodes from a PDF file using the DecodeFile method, the library will convert the PDF file to image(s) first, then perform barcode recognition.

    - scale_down_threshold <int> : Sets the threshold for the image shrinking.
        - Value range : [512, 0x7fffffff]
        - Default value : 2300
        - Remarks : If the shorter edge size is larger than the given threshold value, the library will calculate the required height and width of the barcode image and shrink the image to that size before localization. 
        Otherwise, the library will perform barcode localization on the original image.

    - binarization_modes <list[EnumBinarizationMode]> : Sets the mode and priority for binarization.
        - Value range : Each list item can be any one of the BinarizationMode Enumeration items.
        - Default value : [EnumBinarizationMode.BM_LOCAL_BLOCK, EnumBinarizationMode.BM_SKIP, EnumBinarizationMode.BM_SKIP, EnumBinarizationMode.BM_SKIP, EnumBinarizationMode.BM_SKIP, EnumBinarizationMode.BM_SKIP, EnumBinarizationMode.BM_SKIP, EnumBinarizationMode.BM_SKIP]
        - Remarks : If the barcode type(s) are certain, specifying the barcode type(s) to be read will speed up the recognition process.

    - localization_modes <list[EnumLocalizationMode]> : Sets the mode and priority for localization algorithms.
        - Value range : Each list item can be any one of the LocalizationMode Enumeration items
        - Default value : [EnumLocalizationMode.LM_CONNECTED_BLOCKS, EnumLocalizationMode.LM_SCAN_DIRECTLY, EnumLocalizationMode.LM_STATISTICS, EnumLocalizationMode.LM_LINES, EnumLocalizationMode.LM_SKIP, EnumLocalizationMode.LM_SKIP, EnumLocalizationMode.LM_SKIP, EnumLocalizationMode.LM_SKIP]
        - Remarks : If the barcode type(s) are certain, specifying the barcode type(s) to be read will speed up the recognition process.

    - colour_clustering_modes <list[EnumColourClusteringMode]> : Sets the mode and priority for colour categorization. Not supported yet.
        - Value range : Each list item can be any one of the ColourClusteringMode Enumeration items.
        - Default value : [EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP, EnumColourClusteringMode.CCM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - colour_conversion_modes <list[EnumColourConversionMode]> : Sets the mode and priority for converting a colour image to a grayscale image.
        - Value range : Each list item can be any one of the ColourConversionMode Enumeration items.
        - Default value : [EnumColourConversionMode.CICM_GENERAL, EnumColourConversionMode.CICM_SKIP, EnumColourConversionMode.CICM_SKIP, EnumColourConversionMode.CICM_SKIP, EnumColourConversionMode.CICM_SKIP, EnumColourConversionMode.CICM_SKIP, EnumColourConversionMode.CICM_SKIP, EnumColourConversionMode.CICM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - grayscale_transformation_modes <list[EnumGrayscaleTransformationMode]> : Sets the mode and priority for the grayscale image conversion.
        - Value range : Each list item can be any one of the GrayscaleTransformationMode Enumeration items.
        - Default value : [EnumGrayscaleTransformationMode.GTM_ORIGINAL, EnumGrayscaleTransformationMode.GTM_SKIP, EnumGrayscaleTransformationMode.GTM_SKIP, EnumGrayscaleTransformationMode.GTM_SKIP, EnumGrayscaleTransformationMode.GTM_SKIP, EnumGrayscaleTransformationMode.GTM_SKIP, EnumGrayscaleTransformationMode.GTM_SKIP, EnumGrayscaleTransformationMode.GTM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - region_predetection_modes <list[EnumRegionPredetectionMode]> : Sets the region pre-detection mode for barcodes search.
        - Value range : Each list item can be any one of the RegionPredetectionMode Enumeration items.
        - Default value : [EnumRegionPredetectionMode.RPM_GENERAL, EnumRegionPredetectionMode.RPM_SKIP, EnumRegionPredetectionMode.RPM_SKIP, EnumRegionPredetectionMode.RPM_SKIP, EnumRegionPredetectionMode.RPM_SKIP, EnumRegionPredetectionMode.RPM_SKIP, EnumRegionPredetectionMode.RPM_SKIP, EnumRegionPredetectionMode.RPM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.
        If the image is large and the barcode on the image is very small, it is recommended to enable region predetection to speed up the localization process and recognition accuracy.

    - image_preprocessing_modes <list[EnumImagePreprocessingMode]> : Sets the mode and priority for image preprocessing algorithms.
        - Value range : Each list item can be any one of the ImagePreprocessingMode Enumeration items.
        - Default value : [EnumImagePreprocessingMode.IPM_GENERAL, EnumImagePreprocessingMode.IPM_SKIP, EnumImagePreprocessingMode.IPM_SKIP, EnumImagePreprocessingMode.IPM_SKIP, EnumImagePreprocessingMode.IPM_SKIP, EnumImagePreprocessingMode.IPM_SKIP, EnumImagePreprocessingMode.IPM_SKIP, EnumImagePreprocessingMode.IPM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - texture_detection_modes <list[EnumTextureDetectionMode]> : Sets the mode and priority for texture detection.
        - Value range : Each list item can be any one of the TextureDetectionMode Enumeration items.
        - Default value : [EnumTextureDetectionMode.TDM_GENERAL_WIDTH_CONCENTRATION, EnumTextureDetectionMode.TDM_SKIP, EnumTextureDetectionMode.TDM_SKIP, EnumTextureDetectionMode.TDM_SKIP, EnumTextureDetectionMode.TDM_SKIP, EnumTextureDetectionMode.TDM_SKIP, EnumTextureDetectionMode.TDM_SKIP, EnumTextureDetectionMode.TDM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - text_filter_modes <list[EnumTextFilterMode]> : Sets the mode and priority for text filter.
        - Value range : Each list item can be any one of the TextFilterMode Enumeration items.
        - Default value : [EnumTextFilterMode.TFM_GENERAL_CONTOUR, EnumTextFilterMode.TFM_SKIP, EnumTextFilterMode.TFM_SKIP, EnumTextFilterMode.TFM_SKIP, EnumTextFilterMode.TFM_SKIP, EnumTextFilterMode.TFM_SKIP, EnumTextFilterMode.TFM_SKIP, EnumTextFilterMode.TFM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.
        If the image contains a lot of text, you can enable text filter to speed up the localization process.
    
    - dpm_code_reading_modes <list[EnumDPMCodeReadingMode]> : Sets the mode and priority for DPM code reading.
        - Value range : Each list item can be any one of the DPMCodeReadingMode Enumeration items.
        - Default value : [EnumDPMCodeReadingMode.DPMCRM_GENERAL, EnumDPMCodeReadingMode.DPMCRM_SKIP, EnumDPMCodeReadingMode.DPMCRM_SKIP, EnumDPMCodeReadingMode.DPMCRM_SKIP, EnumDPMCodeReadingMode.DPMCRM_SKIP, EnumDPMCodeReadingMode.DPMCRM_SKIP, EnumDPMCodeReadingMode.DPMCRM_SKIP, EnumDPMCodeReadingMode.DPMCRM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - deformation_resisting_modes <list[EnumDeformationResistingMode]> : Sets the mode and priority for deformation resisting.
        - Value range : Each list item can be any one of the DeformationResistingMode Enumeration items.
        - Default value : [EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP, EnumDeformationResistingMode.DRM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - barcode_complement_modes <list[EnumBarcodeComplementMode]> : Sets the mode and priority to complement the missing parts in the barcode.
        - Value range : Each list item can be any one of the BarcodeComplementMode Enumeration items.
        - Default value : [EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP, EnumBarcodeComplementMode.BCM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - barcode_colour_modes <list[EnumBarcodeColourMode]> : Sets the mode and priority for the barcode colour mode used to process the barcode zone.
        - Value range : Each list item can be any one of the BarcodeColourMode Enumeration items.
        - Default value : [EnumBarcodeColourMode.BICM_DARK_ON_LIGHT, EnumBarcodeColourMode.BICM_SKIP, EnumBarcodeColourMode.BICM_SKIP, EnumBarcodeColourMode.BICM_SKIP, EnumBarcodeColourMode.BICM_SKIP, EnumBarcodeColourMode.BICM_SKIP, EnumBarcodeColourMode.BICM_SKIP, EnumBarcodeColourMode.BICM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - accompanying_text_recognition_modes <list[EnumAccompanyingTextRecognitionMode]> : Sets the mode and priority to recognize accompanying text.
        - Value range : Each list item can be any one of the AccompanyingTextRecognitionMode Enumeration items.
        - Default value : [EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP, EnumAccompanyingTextRecognitionMode.ATRM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - text_result_order_modes <list[EnumTextResultOrderMode]> : Sets the mode and priority for the order of the text results returned.
        - Value range : Each list item can be any one of the TextResultOrderMode Enumeration items.
        - Default value : [EnumTextResultOrderMode.TROM_CONFIDENCE, EnumTextResultOrderMode.TROM_POSITION, EnumTextResultOrderMode.TROM_FORMAT, EnumTextResultOrderMode.TROM_SKIP, EnumTextResultOrderMode.TROM_SKIP, EnumTextResultOrderMode.TROM_SKIP, EnumTextResultOrderMode.TROM_SKIP, EnumTextResultOrderMode.TROM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - scale_up_modes <list[EnumScaleUpMode]> : Sets the mode and priority to control the sampling methods of scale-up for linear barcode with small module sizes.
        - Value range : Each list item can be any one of the ScaleUpMode Enumeration items.
        - Default value : [EnumScaleUpMode.SUM_AUTO, EnumScaleUpMode.SUM_SKIP, EnumScaleUpMode.SUM_SKIP, EnumScaleUpMode.SUM_SKIP, EnumScaleUpMode.SUM_SKIP, EnumScaleUpMode.SUM_SKIP, EnumScaleUpMode.SUM_SKIP, EnumScaleUpMode.SUM_SKIP]
        - Remarks : The list index represents the priority of the item. The smaller index is, the higher priority is.

    - text_assisted_correction_mode <list[EnumTextAssistedCorrectionMode]> : Sets the mode of text assisted correction for barcode decoding. Not supported yet.
        - Value range : Each list item can be any one of the TextAssistedCorrectionMode Enumeration items.
        - Default value : EnumTextAssistedCorrectionMode.TACM_VERIFYING

    - intermediate_result_saving_mode <list[EnumIntermediateResultSavingMode]> : Sets the mode for saving intermediate result.
        - Value range : Each list item can be any one of the IntermediateResultSavingMode Enumeration items.
        - Default value : EnumIntermediateResultSavingMode.IRSM_MEMORY

    - deblur_level <int> : Sets the degree of blurriness of the barcode.
        - Value range : [0, 9]
        - Default value : 9
        - Remarks : If you have a blurry image, you can set this property to a larger value. The higher the value set, the more effort the library will spend to decode images, but it may also slow down the recognition process.

    - intermediate_result_types <int> : Sets which types of intermediate result to be kept for further reference. Intermediate result types can be combined.
        - Value range : A combined value of IntermediateResultType Enumeration items
        - Default value : 0

    - result_coordinate_type <EnumResultCoordinateType> : Specifies the format for the coordinates returned.
        - Value range : Any one of the ResultCoordinateType Enumeration items
        - Default value : 0

    - return_barcode_zone_clarity <int> : Sets whether or not to return the clarity of the barcode zone.
        - Value range : [0,1]
        - Default value : 0
        - Remarks : 0 : Do not return the clarity of the barcode zone; 1 : Return the clarity of the batcode zone.

    - region_top <int> : The top-most coordinate or percentage of the region.
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100]
        - Default value : 0

    - region_bottom <int> : Sets whether or not to return the clarity of the barcode zone.
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100]
        - Default value : 0

    - region_left <int> : Sets whether or not to return the clarity of the barcode zone.
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100]
        - Default value : 0

    - region_right <int> : Sets whether or not to return the clarity of the barcode zone.
        - Value range :
            - if region_measured_by_percentage = 0, [0,0x7fffffff] 
            - if region_measured_by_percentage = 1, [0,100]
        - Default value : 0

    - region_measured_by_percentage <int> : Sets whether or not to return the clarity of the barcode zone.
        - Value range : [0,1]
        - Default value : 0
        - Remarks : When it's set to 1, the values of Top, Left, Right, Bottom indicate percentage (from 0 to 100); Otherwise, they indicate coordinates. 0: not by percentage; 1: by percentage.

    - min_barcode_text_length <int> : Sets the range of barcode text length for barcodes search.
        - Value range : [0, 0x7fffffff]
        - Default value : 0
        - Remarks : 0: means no limitation on the barcode text length.

    - min_result_confidence <int> : The minimum confidence of the result.
        - Value range : [0, 100]
        - Default value : 0
        - Remarks : 0: means no limitation on the result confidence.

    """

    def __init__(self, settings):
        '''Init Function'''
        self.terminate_phase = settings["TerminatePhase"]
        self.timeout = settings["Timeout"]
        self.max_algorithm_thread_count = settings["MaxAlgorithmThreadCount"]
        self.expected_barcodes_count = settings["ExpectedBarcodesCount"]
        self.barcode_format_ids = settings["BarcodeFormatIds"]
        self.barcode_format_ids_2 = settings["BarcodeFormatIds_2"]
        self.pdf_raster_dpi = settings["PDFRasterDPI"]
        self.scale_down_threshold = settings["ScaleDownThreshold"]
        self.binarization_modes = settings["BinarizationModes"]
        self.localization_modes = settings["LocalizationModes"]
        self.colour_clustering_modes = settings["ColourClusteringModes"]
        self.colour_conversion_modes = settings["ColourConversionModes"]
        self.grayscale_transformation_modes = settings["GrayscaleTransformationModes"]
        self.region_predetection_modes = settings["RegionPredetectionModes"]
        self.image_preprocessing_modes = settings["ImagePreprocessingModes"]
        self.texture_detection_modes = settings["TextureDetectionModes"]
        self.text_filter_modes = settings["TextFilterModes"]
        self.dpm_code_reading_modes = settings["DPMCodeReadingModes"]
        self.deformation_resisting_modes = settings["DeformationResistingModes"]
        self.barcode_complement_modes = settings["BarcodeComplementModes"]
        self.barcode_colour_modes = settings["BarcodeColourModes"]
        self.text_result_order_modes = settings["TextResultOrderModes"]
        self.text_assisted_correction_mode = settings["TextAssistedCorrectionMode"]
        self.deblur_level = settings["DeblurLevel"]
        self.intermediate_result_types = settings["IntermediateResultTypes"]
        self.intermediate_result_saving_mode = settings["IntermediateResultSavingMode"]
        self.result_coordinate_type = settings["ResultCoordinateType"]
        self.return_barcode_zone_clarity = settings["ReturnBarcodeZoneClarity"]
        self.region_top = settings["RegionTop"]
        self.region_bottom = settings["RegionBottom"]
        self.region_left = settings["RegionLeft"]
        self.region_right = settings["RegionRight"]
        self.region_measured_by_percentage = settings["RegionMeasuredByPercentage"]
        self.min_barcode_text_length = settings["MinBarcodeTextLength"]
        self.min_result_confidence = settings["MinResultConfidence"]
        self.scale_up_modes = settings["ScaleUpModes"]
        self.accompanying_text_recognition_modes = settings["AccompanyingTextRecognitionModes"]

    def update_settings(self, settings):
        '''update settings'''
        settings["TerminatePhase"] = self.terminate_phase 
        settings["Timeout"] = self.timeout 
        settings["MaxAlgorithmThreadCount"] = self.max_algorithm_thread_count
        settings["ExpectedBarcodesCount"] = self.expected_barcodes_count 
        settings["BarcodeFormatIds"] = self.barcode_format_ids
        settings["BarcodeFormatIds_2"] = self.barcode_format_ids_2 
        settings["PDFRasterDPI"] = self.pdf_raster_dpi
        settings["ScaleDownThreshold"] = self.scale_down_threshold
        settings["BinarizationModes"] = self.binarization_modes
        settings["LocalizationModes"] = self.localization_modes
        settings["ColourClusteringModes"] = self.colour_clustering_modes
        settings["ColourConversionModes"] = self.colour_conversion_modes
        settings["GrayscaleTransformationModes"] = self.grayscale_transformation_modes
        settings["RegionPredetectionModes"] = self.region_predetection_modes
        settings["ImagePreprocessingModes"] = self.image_preprocessing_modes
        settings["TextureDetectionModes"] = self.texture_detection_modes
        settings["TextFilterModes"] = self.text_filter_modes
        settings["DPMCodeReadingModes"] = self.dpm_code_reading_modes
        settings["DeformationResistingModes"] = self.deformation_resisting_modes
        settings["BarcodeComplementModes"] = self.barcode_complement_modes
        settings["BarcodeColourModes"] = self.barcode_colour_modes
        settings["TextResultOrderModes"] = self.text_result_order_modes
        settings["TextAssistedCorrectionMode"] = self.text_assisted_correction_mode
        settings["DeblurLevel"] = self.deblur_level
        settings["IntermediateResultTypes"] = self.intermediate_result_types
        settings["IntermediateResultSavingMode"] = self.intermediate_result_saving_mode
        settings["ResultCoordinateType"] = self.result_coordinate_type
        settings["ReturnBarcodeZoneClarity"] = self.return_barcode_zone_clarity
        settings["RegionTop"] = self.region_top
        settings["RegionBottom"] = self.region_bottom
        settings["RegionLeft"] = self.region_left
        settings["RegionRight"] = self.region_right
        settings["RegionMeasuredByPercentage"] = self.region_measured_by_percentage
        settings["MinBarcodeTextLength"] = self.min_barcode_text_length
        settings["MinResultConfidence"] = self.min_result_confidence
        settings["ScaleUpModes"] = self.scale_up_modes
        settings["AccompanyingTextRecognitionModes"] = self.accompanying_text_recognition_modes

class OnedDetailedResult:
    """
    Stores the OneD code details.
    
    Attributes:
    -----------
    - module_size <int> : The barcode module size (the minimum bar width in pixel) 

    - start_chars_bytes <bytearray> : The start chars in a byte array

    - stop_chars_bytes <bytearray> : The stop chars in a byte array

    - check_digit_bytes <bytearray> : The check digit chars in a byte array
    """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.start_chars_bytes = detailed_result["StartCharsBytes"]
        self.stop_chars_bytes = detailed_result["StopCharsBytes"]
        self.check_digit_bytes = detailed_result["CheckDigitBytes"]

class QRCodeDetailedResult:
    """
    Stores the QRCode details.
    
    Attributes:
    -----------
    - module_size <int> : The barcode module size (the minimum bar width in pixel) 

    - rows <int> : The row count of the barcode

    - columns <int> : The column count of the barcode

    - error_correction_level <EnumQRCodeErrorCorrectionLevel> : The error correction level of the barcode
    
    - versions <int> : The version of the QR Code
    
    - model <int> : Number of the models

    """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.error_correction_level = detailed_result["ErrorCorrectionLevel"]
        self.versions = detailed_result["Version"]
        self.model = detailed_result["Model"]

class DataMatrixDetailedResult:
    """
    Stores the DataMatrix details.
    
    Attributes:
    -----------
    - module_size <int> : The barcode module size (the minimum bar width in pixel) 

    - rows <int> : The row count of the barcode

    - columns <int> : The column count of the barcode

    - data_region_rows <int> : The data region row count of the barcode
    
    - data_region_columns <int> : The data region column count of the barcode
    
    - data_region_number <int> : The data region count

    """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.data_region_rows = detailed_result["DataRegionRows"]
        self.data_region_columns = detailed_result["DataRegionColumns"]
        self.data_region_number = detailed_result["DataRegionNumber"]

class PDFDetailedResult:
    """ 
    Stores the PDF details.
    
    Attributes:
    -----------
    - module_size <int> : The barcode module size (the minimum bar width in pixel) 

    - rows <int> : The row count of the barcode

    - columns <int> : The column count of the barcode

    - error_correction_level <EnumQRCodeErrorCorrectionLevel> : The error correction level of the barcode
    
    """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.error_correction_level = detailed_result["ErrorCorrectionLevel"]

class AztecDetailedResult:
    """
    Stores the Aztec details.

    Attributes:
    -----------
    - module_size <int> : The barcode module size (the minimum bar width in pixel) 

    - rows <int> : The row count of the barcode

    - columns <int> : The column count of the barcode

    - layer_number <int> : A negative number (-1, -2, -3, -4) specifies a compact Aztec code. A positive number (1, 2, .. 32) specifies a normal (full-rang) Aztec code.

    """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.layer_number = detailed_result["LayerNumber"]

class ExtendedResult:
    """
    Stores the extended result.
    
    Attributes:
    -----------
    - result_type <EnumResultType> : Extended result type

    - barcode_format <EnumBarcodeFormat> : Barcode type in BarcodeFormat group 1

    - barcode_format_string <str> : Barcode type in BarcodeFormat group 1 as string

    - barcode_format_2 <EnumBarcodeFormat_2> : Barcode type in BarcodeFormat group 2

    - barcode_format_string_2 <str> : Barcode type in BarcodeFormat group 2 as string

    - confidence <EnumResultType> : The confidence of the result

    - bytes <bytearray> : The content in a byte array

    - accompanying_text_bytes <bytearray> : The accompanying text content in a byte array

    - deformation <int> : The deformation value

    - detailed_result <class> : One of the following: OnedDetailedResult, PDFDetailedResult, DataMatrixDetailedResult, AztecDetailedResult, QRCodeDetailedResult

    - sampling_image <class SamplingImageData> : The sampling image info

    - clarity <int> : The clarity of the barcode zone in percentage.

    """

    def __init__(self, extended_result):
        ''' Init Function '''
        self.result_type = extended_result["ResultType"]
        self.barcode_format = extended_result["BarcodeFormat"]
        self.barcode_format_string = extended_result["BarcodeFormatString"]
        self.barcode_format_2 = extended_result["BarcodeFormat_2"]
        self.barcode_format_string_2 = extended_result["BarcodeFormatString_2"]
        self.confidence = extended_result["Confidence"]
        self.bytes = extended_result["Bytes"]
        self.accompanying_text_bytes = extended_result["AccompanyingTextBytes"]
        self.deformation = extended_result["Deformation"]

        detailed_result = extended_result["DetailedResult"]
        is_oned = self.barcode_format & EnumBarcodeFormat.BF_ONED
        if is_oned != 0:
            self.detailed_result = OnedDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_QR_CODE:
            self.detailed_result = QRCodeDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_DATAMATRIX:
            self.detailed_result = DataMatrixDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_PDF417:
            self.detailed_result = PDFDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_AZTEC:
            self.detailed_result = AztecDetailedResult(detailed_result)
        else:
            self.detailed_result = None

        sampling_image = extended_result["SamplingImage"]
        self.sampling_image = SamplingImageData(sampling_image)
        self.clarity = extended_result["Clarity"]

class LocalizationResult:
    """
    Stores the localization result.

    Attributes:
    -----------
    - terminate_phase <EnumTerminatePhase> : The terminate phase of localization result

    - barcode_format <EnumBarcodeFormat> : Barcode type in BarcodeFormat group 1

    - barcode_format_string <str> : Barcode type in BarcodeFormat group 1 as string

    - barcode_format_2 <EnumBarcodeFormat_2> : Barcode type in BarcodeFormat group 2

    - barcode_format_string_2 <str> : Barcode type in BarcodeFormat group 2 as string

    - localization_points <tuple> : The 4 localization points

    - angle <int> : The angle of a barcode. Values range is from 0 to 360

    - module_size <int> : The barcode module size (the minimum bar width in pixel)

    - page_number <int> : The page number the barcode located in. The index is 0-based

    - region_name <int> : The region name the barcode located in

    - document_name <int> : The document name

    - result_coordinate_type <EnumResultCoordinateType> : The coordinate type

    - accompanying_text_bytes <bytearray> : The accompanying text content in a byte array

    - confidence <int> : The confidence of the localization result

    """

    def __init__(self, localization_result):
        ''' Init Function '''
        self.terminate_phase = localization_result["TerminatePhase"]
        self.barcode_format = localization_result["BarcodeFormat"]
        self.barcode_format_string = localization_result["BarcodeFormatString"]
        self.barcode_format_2 = localization_result["BarcodeFormat_2"]
        self.barcode_format_string_2 = localization_result["BarcodeFormatString_2"]
        x1 = localization_result["X1"]
        y1 = localization_result["Y1"]
        x2 = localization_result["X2"]
        y2 = localization_result["Y2"]
        x3 = localization_result["X3"]
        y3 = localization_result["Y3"]
        x4 = localization_result["X4"]
        y4 = localization_result["Y4"]
        self.localization_points = [(x1,y1),(x2,y2),(x3,y3),(x4,y4)]
        self.angle = localization_result["Angle"]
        self.module_size = localization_result["ModuleSize"]
        self.page_number = localization_result["PageNumber"]
        self.region_name = localization_result["RegionName"]
        self.document_name = localization_result["DocumentName"]
        self.result_coordinate_type = localization_result["ResultCoordinateType"]
        self.accompanying_text_bytes = localization_result["AccompanyingTextBytes"]
        self.confidence = localization_result["Confidence"]

class TextResult:
    """
    Stores the text result.
    
    Attributes:
    -----------
    - barcode_format <EnumBarcodeFormat> : Barcode type in BarcodeFormat group 1

    - barcode_format_string <str> : Barcode type in BarcodeFormat group 1 as string

    - barcode_format_2 <EnumBarcodeFormat_2> : Barcode type in BarcodeFormat group 2

    - barcode_format_string_2 <str> : Barcode type in BarcodeFormat group 2 as string

    - barcode_text <str> : The barcode text

    - barcode_bytes <bytearray> : The barcode content in a byte array

    - localization_result <class LocalizationResult> : The corresponding localization result

    - detailed_result <class> : One of the following: OnedDetailedResult, PDFDetailedResult, DataMatrixDetailedResult, AztecDetailedResult, QRCodeDetailedResult

    - extended_results <class ExtendedResult> : The extended result list

    """

    def __init__(self, text_result):
        ''' Init Function '''
        self.barcode_format = text_result["BarcodeFormat"]
        self.barcode_format_string = text_result["BarcodeFormatString"]
        self.barcode_format_2 = text_result["BarcodeFormat_2"]
        self.barcode_format_string_2 = text_result["BarcodeFormatString_2"]
        self.barcode_text = text_result["BarcodeText"]
        self.barcode_bytes = text_result["BarcodeBytes"]

        localization_result = text_result["LocalizationResult"]
        self.localization_result = LocalizationResult(localization_result)

        detailed_result = text_result["DetailedResult"]
        is_oned = self.barcode_format & EnumBarcodeFormat.BF_ONED
        if is_oned != 0:
            self.detailed_result = OnedDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_QR_CODE:
            self.detailed_result = QRCodeDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_DATAMATRIX:
            self.detailed_result = DataMatrixDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_PDF417:
            self.detailed_result = PDFDetailedResult(detailed_result)
        elif self.barcode_format == EnumBarcodeFormat.BF_AZTEC:
            self.detailed_result = AztecDetailedResult(detailed_result)
        else:
            self.detailed_result = None

        extended_results = text_result["ExtendedResults"]
        if type(extended_results) is list:
            self.extended_results = []
            length = len(extended_results)
            for extended_result in extended_results:
                self.extended_results.append(ExtendedResult(extended_result))
        else:
            self.extended_results = None

class Point:
    """
    Point
    
    Attributes:
    -----------
    - x <int> : The X coordinate of the point

    - y <int> : The Y coordinate of the point

    """

    def __init__(self, point):
        ''' Init Function '''
        self.x = point["X"]
        self.y = point["Y"]

class ImageData:
    """
    Image Data
    
    Attributes:
    -----------
    - bytes <bytearray> : The image data content in a byte array

    - width <bytearray> : The width of the image in pixels

    - height <bytearray> : The height of the image in pixels

    - stride <bytearray> : The image data content in a byte array

    - image_pixel_format <bytearray> : The image data content in a byte array
    """

    def __init__(self, image_data):
        ''' Init Function '''
        self.bytes = image_data["Bytes"]
        self.width = image_data["Width"]
        self.height = image_data["Height"]
        self.stride = image_data["Stride"]
        self.image_pixel_format = image_data["ImagePixelFormat"]

class Contour:
    """
    Contour

    Attributes:
    -----------
    - points <list[class Point]> : The points list

    """

    def __init__(self, contour):
        ''' Init Function '''
        points = contour["Points"]
        self.points = []
        for point in points:
            self.points.append(Point(point))

class LineSegment:
    """ 
    Line Segment 
    
    Attributes:
    -----------
    - start_point <class Point> : The start point of the line segment

    - end_point <class Point> : The end point of the line segment

    - lines_confidence_coefficients <list[int]> : The end point of the line segment

    """

    def __init__(self, line_segment):
        ''' Init Function '''
        start_point = line_segment["StartPoint"]
        self.start_point = Point(start_point)
        end_point = line_segment["EndPoint"]
        self.end_point = Point(end_point)
        self.lines_confidence_coefficients = line_segment["LinesConfidenceCoefficients"]

class RegionOfInterest:
    """ 
    Stores the region of interest. 
    
    Attributes:
    -----------
    - roi_id <int> : The ID generated by the SDK

    - point <class Point> : The left top point of the region

    - width <int> : The width of the region

    - height <int> : The height of the region

    """

    def __init__(self, result):
        ''' Init Function '''
        self.roi_id = result["ROIId"]
        point = result["Point"]
        self.point = Point(point)
        self.width = result["Width"]
        self.height = result["Height"]

class IntermediateResult:
    """ 
    Stores the intermediate result. 
    
    Attributes:
    -----------
    - data_type <EnumIMResultDataType> : The data type of the intermediate result

    - results <list[class]> : One of the following types: List of class Contour, List of class ImageData, List of class LineSegment, List of class LocalizationResult, List of class RegionOfInterest

    - result_type <EnumIntermediateResultType> : Intermediate result type

    - barcode_complement_mode <EnumBarcodeComplementMode> : The BarcodeComplementMode used when generating the current intermediate result

    - bcm_index <int> : The list index of current used ColourClusteringMode in the ColourClusteringModes setting

    - deformation_resisting_mode <EnumDeformationResistingMode> : The DeformationResistingMode used when generating the current intermediate result

    - drm_index <int> : The list index of current used DeformationResistingMode in the DeformationResistingModes setting

    - dpm_code_reading_mode <EnumDPMCodeReadingMode> : The DPMCodeReadingMode used when generating the current intermediate result

    - dpmcrm_index <int> : The list index of current used DPMCodeReadingMode in the DPMCodeReadingModes setting

    - text_filter_mode <EnumTextFilterMode> : The TextFilterMode used when generating the current intermediate result

    - tfm_index <int> : The list index of current used TextFilterMode in the TextFilterModes setting

    - localization_mode <EnumLocalizationMode> : The LocalizationMode used when generating the current intermediate result

    - lm_index <int> : The list index of current used LocalizationMode in the LocalizationModes setting

    - binarization_mode <EnumBinarizationMode> : The BinarizationMode used when generating the current intermediate result

    - bm_index <int> : The list index of current used BinarizationMode in the BinarizationModes setting

    - image_preprocessing_mode <EnumImagePreprocessingMode> : The ImagePreprocessingMode used when generating the current intermediate result

    - ipm_index <int> : The list index of current used ImagePreprocessingMode in the ImagePreprocessingModes setting

    - region_predetection_mode <EnumRegionPredetectionMode> : The RegionPredetectionMode used when generating the current intermediate result

    - rpm_index <int> : The list index of current used RegionPredetectionMode in the RegionPredetectionModes setting

    - grayscale_transformation_mode <EnumGrayscaleTransformationMode> : The GrayscaleTransformationMode used when generating the current intermediate result

    - gtm_index <int> : The list index of current used GrayscaleTransformationMode in the GrayscaleTransformationModes setting

    - colour_conversion_mode <EnumColourConversionMode> : The ColourConversionMode used when generating the current intermediate result

    - cicm_index <int> : The list index of current used ColourConversionMode in the ColourConversionModes setting

    - colour_clustering_mode <EnumColourClusteringMode> : The ColourClusteringMode used when generating the current intermediate result

    - ccm_index <int> : The list index of current used ColourClusteringMode in the ColourClusteringModes setting

    - ccm_index <int> : The list index of current used ColourClusteringMode in the ColourClusteringModes setting

    - rotation_matrix <list[double]> : The rotation matrix

    - roi_id <int> : The ID of the ROI (Region Of Interest) generated by the SDK. -1 means the original image.

    - scale_down_ratio <int> : The scale down ratio

    - frame_id <int> : The ID of the operated frame

    """

    def __init__(self, intermediate_result):
        ''' Init Function '''
        self.data_type = intermediate_result["DataType"]

        im_results = intermediate_result["IMResults"]
        if type(im_results) is list:
            self.results = []
            if self.data_type == EnumIMResultDataType.IMRDT_IMAGE:
                for im_result in im_results:
                    self.results.append(ImageData(im_result))
            elif self.data_type == EnumIMResultDataType.IMRDT_CONTOUR:
                for im_result in im_results:
                    self.results.append(Contour(im_result))
            elif self.data_type == EnumIMResultDataType.IMRDT_LINESEGMENT:
                for im_result in im_results:
                    self.results.append(LineSegment(im_result))
            elif self.data_type == EnumIMResultDataType.IMRDT_LOCALIZATIONRESULT:
                for im_result in im_results:
                    self.results.append(LocalizationResult(im_result))
            elif self.data_type == EnumIMResultDataType.IMRDT_REGIONOFINTEREST:
                for im_result in im_results:
                    self.results.append(RegionOfInterest(im_result))
            else:
                pass
        else:
            self.results = None

        self.result_type = intermediate_result["ResultType"]
        self.barcode_complement_mode = intermediate_result["BarcodeComplementMode"]
        self.bcm_index = intermediate_result["BCMIndex"]
        self.deformation_resisting_mode = intermediate_result["DeformationResistingMode"]
        self.drm_index = intermediate_result["DRMIndex"]
        self.dpm_code_reading_mode = intermediate_result["DPMCodeReadingMode"]
        self.dpmcrm_index = intermediate_result["DPMCRMIndex"]
        self.text_filter_mode = intermediate_result["TextFilterMode"]
        self.tfm_index = intermediate_result["TFMIndex"]
        self.localization_mode = intermediate_result["LocalizationMode"]
        self.lm_index = intermediate_result["LMIndex"]
        self.binarization_mode = intermediate_result["BinarizationMode"]
        self.bm_index = intermediate_result["BMIndex"]
        self.image_preprocessing_mode = intermediate_result["ImagePreprocessingMode"]
        self.ipm_index = intermediate_result["IPMIndex"]
        self.region_predetection_mode = intermediate_result["RegionPredetectionMode"]
        self.rpm_index = intermediate_result["RPMIndex"]
        self.grayscale_transformation_mode = intermediate_result["GrayscaleTransformationMode"]
        self.gtm_index = intermediate_result["GTMIndex"]
        self.colour_conversion_mode = intermediate_result["ColourConversionMode"]
        self.cicm_index = intermediate_result["CICMIndex"]
        self.colour_clustering_mode = intermediate_result["ColourClusteringMode"]
        self.ccm_index = intermediate_result["CCMIndex"]
        self.rotation_matrix = intermediate_result["RotationMatrix"]
        self.roi_id = intermediate_result["ROIId"]
        self.scale_down_ratio = intermediate_result["ScaleDownRatio"]
        self.frame_id = intermediate_result["FrameId"]

#endregion

#region Exception Class

class BarcodeReaderError(Exception):
    """ Custom Exception """

    def __init__(self, error_info):
        super().__init__(self)
        self.error_info = error_info
    def __str__(self):
        return self.error_info

#endregion

#region Main Region 

class BarcodeReader():
    """ 
    dbr-python 
    
    Attributes:
    -----------
    - version <str> : The dbr-python version
    - dbr_version <str> : The DynamsoftBarcodeReader version

    Methods:
    -----------
    - Common Functions
        - get_error_string(error_code)
    - License Functions
        - init_license(dbr_license)
        - init_license_from_server(license_server, license_key)
        - init_license_from_license_content(license_key, license_content)
        - output_license_to_string()
    - Runtime Settings Functions
        - get_runtime_settings()
        - update_runtime_settings(settings)
        - reset_runtime_settings()
        - set_mode_argument(modes_name, index, argument_name, argument_value)
        - get_mode_argument(modes_name, index, argument_name)
    - Template Settings Funtions
        - init_runtime_settings_with_string(json_string, conflict_mode=EnumConflictMode.CM_OVERWRITE)
        - init_runtime_settings_with_file(json_file, conflict_mode=EnumConflictMode.CM_OVERWRITE)
        - append_template_string_to_runtime_settings(json_string, conflict_mode)
        - append_template_file_to_runtime_settings(json_file, conflict_mode)
        - output_settings_to_json_string()
        - output_settings_to_json_file(save_file_path)
        - get_all_template_names()
    - Image Decoding Functions
        - decode_file(image_file_name, template_name="")
        - decode_buffer(image, height, width, stride, image_pixel_format=EnumImagePixelFormat.IPF_RGB_888, template_name="")
        - decode_file_stream(file_stream, file_size, template_name="")
        - get_all_intermediate_results()
    - Frame Decoding Functions
        - init_frame_decoding_parameters()
        - start_video_mode(frame_decoding_parameters, call_back_func, template_name="")
        - append_video_frame(video_frame)
        - stop_video_mode()
        - get_length_of_frame_queue()

    """

    def __init__(self):
        ''' Init Function '''
        self.__dbr = DynamsoftBarcodeReader()
        self.version = "dbr-python 7.3"
        self.dbr_version = self.__dbr.GetDBRVersion()


    def get_error_string(self, error_code):
        ''' Get the detailed error message
            :param error_code <int> : Error code
            :return error_string <str> : The detailed error message
        '''
        error_string = self.__dbr.GetErrorString(error_code)
        return error_string

#region License Functions

    def init_license(self, dbr_license):
        ''' Reads product key and activates the SDK. 
            :param dbr_license <string> : The product keys
            :return error       <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.InitLicense(dbr_license)
        return error


    def init_license_from_server(self, license_server, license_key):
        ''' Initializes barcode reader license and connects to the specified server for online verification. 
            :param license_server <str> : The name/IP of the license server.
            :param license_key    <str> : The license key.
            :return error       <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.InitLicenseFromServer(license_server, license_key)
        return error


    def init_license_from_license_content(self, license_key, license_content):
        ''' Initializes barcode reader license from the license content on the client machine for offline verification.
            :param license_key     <str> : The license key.
            :param license_content <str> : An encrypted string representing the license content (quota, expiration date, barcode type, etc.) obtained from the method output_license_to_string().
            :return error        <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.InitLicenseFromLicenseContent(self, license_key, license_content)
        return error


    def output_license_to_string(self):
        ''' Outputs the license content as an encrypted string from the license server to be used for offline license verification. 
            :return license_string  <str> : An encrypted string which stores the content of license.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        license_string = self.__dbr.OutputLicenseToString()
        if type(license_string) is int:
            error_string = self.__dbr.GetErrorString(license_string)
            raise BarcodeReaderError(error_string)
        else:
            return license_string

#endregion

#region Runtime Settings Functions

    def get_runtime_settings(self):
        ''' Get current runtime settings 
            :return runtime_settings <class PublicRuntimeSetting> : The PublicRuntimeSetting object of current runtime settings.
        '''
        cp_settings = self.__dbr.GetRuntimeSettings()
        try:
            settings = PublicRuntimeSetting(cp_settings)
            return settings
        except KeyError as ke:
            print(ke)


    def update_runtime_settings(self, settings):
        ''' Update runtime settings with a PublicRuntimeSetting object
            :param settings <class PublicRuntimeSetting> : a PublicRuntimeSetting object.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        cp_settings = self.__dbr.GetRuntimeSettings()
        try:
            settings.update_settings(cp_settings)
        except KeyError as ke:
            print(ke)
        error = self.__dbr.UpdataRuntimeSettings(cp_settings)
        error_code = error[0]
        error_message = error[1]
        if error_code != EnumErrorCode.DBR_OK:
            raise BarcodeReaderError(error_message)


    def reset_runtime_settings(self):
        ''' Resets all parameters to default values. '''
        self.__dbr.ResetRuntimeSettings()


    def set_mode_argument(self, modes_name, index, argument_name, argument_value):
        ''' Sets the optional argument for a specified mode in Modes(Mode) parameters. 
            :param modes_name     <str> : The modes(mode) parameter name to set argument.
            :param index          <int> : The array index of modes parameter to indicate a specific mode.
            :param argument_name  <str> : The name of the argument to set.
            :param argument_value <str> : The value of the argument to set.
            :return error         <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.SetModeArgument(modes_name, index, argument_name, argument_value)
        return error


    def get_mode_argument(self, modes_name, index, argument_name):
        ''' Gets the optional argument for a specified mode in Modes(Mode) parameters. 
            :param modes_name      <str> : The modes(mode) parameter name to get argument.
            :param index           <int> : The array index of modes parameter to indicate a specific mode.
            :param argument_name   <str> : The name of the argument to get.
            :return argument_value <str> : The value of the argument to get.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        return_value = self.__dbr.GetModeArgument(modes_name, index, argument_name)
        if type(return_value) is str:
            return return_value
        else:
            error_code = return_value[0]
            error_message = return_value[1]
            raise BarcodeReaderError(error_message)

#endregion

#region Template Settings Funtions

    def init_runtime_settings_with_string(self, json_string, conflict_mode=EnumConflictMode.CM_OVERWRITE):
        ''' Initializes runtime settings with the parameters obtained from a JSON string. 
            :param json_string <str> : A JSON string that represents the content of the settings.
            :param conflict_mode(optional) <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.InitRuntimeSettingsByJsonString(json_string, conflict_mode)
        return error


    def init_runtime_settings_with_file(self, json_file, conflict_mode=EnumConflictMode.CM_OVERWRITE):
        ''' Initializes runtime settings with the parameters obtained from a JSON file. 
            :param json_file <str> : A JSON template file' path.
            :param conflict_mode(optional) <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.InitRuntimeSettingsByJsonFile(json_file, conflict_mode)
        return error


    def append_template_string_to_runtime_settings(self, json_string, conflict_mode):
        ''' Appends a new template string to the current runtime settings. 
            :param json_string <str> : A JSON string that represents the content of the settings.
            :param conflict_mode <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.AppendTplStringToRuntimeSettings(json_string, conflict_mode)
        return error


    def append_template_file_to_runtime_settings(self, json_file, conflict_mode):
        ''' Appends a new template file to the current runtime settings. 
            :param json_file <str> : A JSON template file' path.
            :param conflict_mode <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.AppendTplFileToRuntimeSettings(json_file, conflict_mode)
        return error


    def output_settings_to_json_string(self):
        ''' Outputs runtime settings to a json string.
            :return settings_string <str> : The output string which stores the contents of current settings.
        '''
        settings_string = self.__dbr.OutputSettingsToJsonString()
        return settings_string


    def output_settings_to_json_file(self, save_file_path):
        ''' Outputs runtime settings and save them into a settings file (JSON file).
            :param save_file_path <str> : The path of the output file which stores current settings.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.__dbr.OutputSettingsToJsonFile(save_file_path)
        return error


    def get_all_template_names(self):
        ''' Gets all parameter template names.
            :return template_names <list[str]> : all parameter template names
        '''
        template_names = self.__dbr.GetAllTemplateNames()
        return template_names

#endregion

#region Image Decoding Functions

    def decode_file(self, image_file_name, template_name=""):
        ''' Decodes barcodes in the specified image file.
            :param image_file_name           <str> : A string defining the file name.
            :param template_name(optional)   <str> : The template name.
            :return text_results <list[class TextResult]> : All text results.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        error_code = self.__dbr.DecodeFile(image_file_name, template_name)
        if error_code == EnumErrorCode.DBR_OK or error_code == EnumErrorCode.DBRERR_LICENSE_EXPIRED or error_code == EnumErrorCode.DBRERR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_RECOGNITION_TIMEOUT or \
            error_code == EnumErrorCode.DBRERR_1D_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_QR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PDF417_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_AZTEC_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_DATAMATRIX_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_GS1_DATABAR_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_GS1_COMPOSITE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_MAXICODE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PATCHCODE_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_POSTALCODE_LICENSE_INVALID :
            cp_text_results = self.__dbr.GetAllTextResults()
            
            text_results = []
            if type(cp_text_results) is list:
                for cp_text_result in cp_text_results:
                    text_results.append(TextResult(cp_text_result))
                return text_results
            else:
                return None
        else:
            error_message = self.__dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)
    
    
    def decode_buffer(self, image, height, width, stride, image_pixel_format=EnumImagePixelFormat.IPF_RGB_888, template_name=""):
        ''' Decodes barcodes from the memory buffer containing image pixels in defined format. 
            :param image <class numpy.ndarray> : The image which is processed by opencv.( image = cv2.imread('image_name') )
            :param height <int> : The height of the image in pixels.(height = image.shape[0])
            :param width <int> : The width of the image in pixels.(width = image.shape[1])
            :param stride <int> : The stride (or scan width) of the image.(stride = image.strides[0])
            :param image_pixel_format(optional) <EnumImagePixelFormat> : The image pixel format used in the image byte array. Default value = EnumImagePixelFormat.IPF_RGB_888.
            :param template_name(optional) <str> : The template name.
            :return text_results <list[class TextResult]> : All text results.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        error_code = self.__dbr.DecodeBuffer(image, height, width, stride, image_pixel_format, template_name)
        if error_code == EnumErrorCode.DBR_OK or error_code == EnumErrorCode.DBRERR_LICENSE_EXPIRED or error_code == EnumErrorCode.DBRERR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_RECOGNITION_TIMEOUT or \
            error_code == EnumErrorCode.DBRERR_1D_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_QR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PDF417_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_AZTEC_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_DATAMATRIX_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_GS1_DATABAR_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_GS1_COMPOSITE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_MAXICODE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PATCHCODE_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_POSTALCODE_LICENSE_INVALID :
            cp_text_results = self.__dbr.GetAllTextResults()
            text_results = []
            if type(cp_text_results) is list:
                for cp_text_result in cp_text_results:
                    text_results.append(TextResult(cp_text_result))
                return text_results
            else:
                return None
        else:
            error_message = self.__dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def decode_file_stream(self, file_stream, file_size, template_name=""):
        ''' Decodes barcodes from an image file in memory.
            :param file_stream <bytearray> : The image file bytes in memory.
            :param file_size <int> : The length of the file bytes in memory.( len(file_stream) )
            :param template_name(optional) <str> : The template name. 
            :return text_results <list[class TextResult]> : All text results.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        error_code = self.__dbr.DecodeFileStream(file_stream, file_size, template_name)
        if error_code == EnumErrorCode.DBR_OK or error_code == EnumErrorCode.DBRERR_LICENSE_EXPIRED or error_code == EnumErrorCode.DBRERR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_RECOGNITION_TIMEOUT or \
            error_code == EnumErrorCode.DBRERR_1D_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_QR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PDF417_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_AZTEC_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_DATAMATRIX_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_GS1_DATABAR_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_GS1_COMPOSITE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_MAXICODE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PATCHCODE_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_POSTALCODE_LICENSE_INVALID :
            cp_text_results = self.__dbr.GetAllTextResults()
            text_results = []
            if type(cp_text_results) is list:
                for cp_text_result in cp_text_results:
                    text_results.append(TextResult(cp_text_result))
                return text_results
            else:
                return None
        else:
            error_message = self.__dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def get_all_intermediate_results(self):
        ''' Returns intermediate results containing the original image, the colour clustered image, the binarized image, contours, lines, text blocks, etc. 
            :return intermediate_results <liset[class IntermediateResult]>
        '''
        cp_intermediate_results = self.__dbr.GetAllIntermediateResults()
        if cp_intermediate_results == None:
            return None
        else:
            intermediate_results = []
            for cp_intermediate_result in cp_intermediate_results:
                intermediate_results.append(IntermediateResult(cp_intermediate_result))
            return intermediate_results


#endregion

#region Frame Decoding Functions

    def init_frame_decoding_parameters(self):
        ''' Init frame decoding parameters.
            :return frame_decoding_parameters <class FrameDecodingParameters> : The frame decoding parameters.
        '''
        cp_frame_decoding_parameters = self.__dbr.InitFrameDecodingParameters()
        try:
            frame_decoding_parameters = FrameDecodingParameters(cp_frame_decoding_parameters)
            return frame_decoding_parameters
        except KeyError as ke:
            print(ke)


    def start_video_mode(self, frame_decoding_parameters, call_back_func, template_name=""):
        ''' Starts a new thread to decode barcodes from the inner frame queue.
            :param frame_decoding_parameters <class FrameDecodingParameters> : The frame decoding parameters. You can get it by init_frame_decoding_parameters(), then modify its parameters' value.
            :param call_back_func <function pointer>
            :param template_name(optional) <str> : The template name.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        cp_frame_decoding_parameters = self.__dbr.InitFrameDecodingParameters()
        try:
            frame_decoding_parameters.update_parameters(cp_frame_decoding_parameters)
        except KeyError as ke:
            print(ke)
        error_code = self.__dbr.StartVideoMode(cp_frame_decoding_parameters, call_back_func, template_name)
        if error_code != EnumErrorCode.DBR_OK:
            error_message = self.__dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def append_video_frame(self, video_frame):
        ''' Appends a video frame to the inner frame queue. 
            :param video_frame : Gets by opencv.
            :return frame_id <int>
        '''
        frame_id = self.__dbr.AppendVideoFrame(video_frame)
        return frame_id


    def stop_video_mode(self):
        ''' Stops the frame decoding thread created by start_video_mode().
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        error_code = self.__dbr.StopVideoMode()
        if error_code != EnumErrorCode.DBR_OK:
            error_message = self.__dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def get_length_of_frame_queue(self):
        ''' Gets current length of the inner frame queue. 
            :return frame_queue_length <int> : The length of the inner frame queue.
        '''
        frame_queue_length = self.__dbr.GetLengthOfFrameQueue()
        return frame_queue_length

#endregion

#endregion