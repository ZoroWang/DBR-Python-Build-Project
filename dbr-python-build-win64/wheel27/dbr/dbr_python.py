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
    """ Stores the sampling image data. """

    def __init__(self, sampling_image_data):
        '''Init Function'''
        self.bytes = sampling_image_data["Bytes"]
        self.width = sampling_image_data["Width"]
        self.height = sampling_image_data["Height"]

class FrameDecodingParameters:
    """ Defines a struct to configure the frame decoding Parameters. """

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
    """" Runtime settings """

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
    """ Stores the OneD code details. """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.start_chars_bytes = detailed_result["StartCharsBytes"]
        self.stop_chars_bytes = detailed_result["StopCharsBytes"]
        self.check_digit_bytes = detailed_result["CheckDigitBytes"]

class QRCodeDetailedResult:
    """ Stores the QRCode details. """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.error_correction_level = detailed_result["ErrorCorrectionLevel"]
        self.versions = detailed_result["Version"]
        self.model = detailed_result["Model"]

class DataMatrixDetailedResult:
    """ Stores the DataMatrix details. """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.data_region_rows = detailed_result["DataRegionRows"]
        self.data_region_columns = detailed_result["DataRegionColumns"]
        self.data_region_number = detailed_result["DataRegionNumber"]

class PDFDetailedResult:
    """ Stores the PDF details. """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.error_correction_level = detailed_result["ErrorCorrectionLevel"]

class AztecDetailedResult:
    """ Stores the Aztec details. """

    def __init__(self, detailed_result):
        ''' Init Function '''
        self.module_size = detailed_result["ModuleSize"]
        self.rows = detailed_result["Rows"]
        self.columns = detailed_result["Columns"]
        self.layer_number = detailed_result["LayerNumber"]

class ExtendedResult:
    """ Stores the extended result. """

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
        self.result_type = extended_result["Clarity"]

class LocalizationResult:
    """ Stores the localization result. """

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
    """ Stores the text result. """

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
    """ Point """

    def __init__(self, point):
        ''' Init Function '''
        self.x = point["X"]
        self.y = point["Y"]

class ImageData:
    """ Image Data """

    def __init__(self, image_data):
        ''' Init Function '''
        self.bytes = image_data["Bytes"]
        self.width = image_data["Width"]
        self.height = image_data["Height"]
        self.stride = image_data["Stride"]
        self.image_pixel_format = image_data["ImagePixelFormat"]

class Contour:
    """ Contour """

    def __init__(self, contour):
        ''' Init Function '''
        points = contour["Points"]
        self.points = []
        for point in points:
            self.points.append(Point(point))

class LineSegment:
    """ Line Segment """

    def __init__(self, line_segment):
        ''' Init Function '''
        start_point = line_segment["StartPoint"]
        self.start_point = Point(start_point)
        end_point = line_segment["EndPoint"]
        self.end_point = Point(end_point)
        self.lines_confidence_coefficients = line_segment["LinesConfidenceCoefficients"]

class RegionOfInterest:
    """ Stores the region of interest. """

    def __init__(self, result):
        ''' Init Function '''
        self.roi_id = result["ROIId"]
        point = result["Point"]
        self.point = Point(point)
        self.width = result["Width"]
        self.height = result["Height"]

class IntermediateResult:
    """ Stores the intermediate result. """

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
    """ dbr-python """

    def __init__(self):
        ''' Init Function '''
        self.dbr = DynamsoftBarcodeReader()
        self.version = "dbr-python 7.3"
        self.dbr_version = self.dbr.GetDBRVersion()


    def get_error_string(self, error_code):
        ''' Get the detailed error message
            :param error_code <int> : Error code
            :return error_string <str> : The detailed error message
        '''
        error_string = self.dbr.GetErrorString(error_code)
        return error_string

#region License Functions

    def init_license(self, dbr_license):
        ''' Reads product key and activates the SDK. 
            :param dbr_license <string> : The product keys
            :return error       <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.InitLicense(dbr_license)
        return error


    def init_license_from_server(self, license_server, license_key):
        ''' Initializes barcode reader license and connects to the specified server for online verification. 
            :param license_server <str> : The name/IP of the license server.
            :param license_key    <str> : The license key.
            :return error       <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.InitLicenseFromServer(license_server, license_key)
        return error


    def init_license_from_license_content(self, license_key, license_content):
        ''' Initializes barcode reader license from the license content on the client machine for offline verification.
            :param license_key     <str> : The license key.
            :param license_content <str> : An encrypted string representing the license content (quota, expiration date, barcode type, etc.) obtained from the method dbr.output_license_to_string().
            :return error        <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.InitLicenseFromLicenseContent(self, license_key, license_content)
        return error


    def output_license_to_string(self):
        ''' Outputs the license content as an encrypted string from the license server to be used for offline license verification. 
            :return license_string  <str> : An encrypted string which stores the content of license.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        license_string = self.dbr.OutputLicenseToString()
        if type(license_string) is int:
            error_string = self.dbr.GetErrorString(license_string)
            raise BarcodeReaderError(error_string)
        else:
            return license_string

#endregion

#region Runtime Settings Functions

    def get_runtime_settings(self):
        ''' Get current runtime settings 
            :return runtime_settings <class PublicRuntimeSetting> : The PublicRuntimeSetting object of current runtime settings.
        '''
        cp_settings = self.dbr.GetRuntimeSettings()
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
        cp_settings = self.dbr.GetRuntimeSettings()
        try:
            settings.update_settings(cp_settings)
        except KeyError as ke:
            print(ke)
        error = self.dbr.UpdataRuntimeSettings(cp_settings)
        error_code = error[0]
        error_message = error[1]
        if error_code != EnumErrorCode.DBR_OK:
            raise BarcodeReaderError(error_message)


    def reset_runtime_settings(self):
        ''' Resets all parameters to default values. '''
        self.dbr.ResetRuntimeSettings()


    def set_mode_argument(self, modes_name, index, argument_name, argument_value):
        ''' Sets the optional argument for a specified mode in Modes(Mode) parameters. 
            :param modes_name     <str> : The modes(mode) parameter name to set argument.
            :param index          <int> : The array index of modes parameter to indicate a specific mode.
            :param argument_name  <str> : The name of the argument to set.
            :param argument_value <str> : The value of the argument to set.
            :return error         <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.SetModeArgument(modes_name, index, argument_name, argument_value)
        return error


    def get_mode_argument(self, modes_name, index, argument_name):
        ''' Gets the optional argument for a specified mode in Modes(Mode) parameters. 
            :param modes_name      <str> : The modes(mode) parameter name to get argument.
            :param index           <int> : The array index of modes parameter to indicate a specific mode.
            :param argument_name   <str> : The name of the argument to get.
            :return argument_value <str> : The value of the argument to get.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        return_value = self.dbr.GetModeArgument(modes_name, index, argument_name)
        if type(return_value) is str:
            return return_value
        else:
            error_code = return_value[0]
            error_message = return_value[1]
            raise BarcodeReaderError(error_message)

#endregion

#region Settings Template Funtions

    def init_runtime_settings_with_string(self, json_string, conflict_mode=EnumConflictMode.CM_OVERWRITE):
        ''' Initializes runtime settings with the parameters obtained from a JSON string. 
            :param json_string <str> : A JSON string that represents the content of the settings.
            :param conflict_mode(optional) <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.InitRuntimeSettingsByJsonString(json_string, conflict_mode)
        return error


    def init_runtime_settings_with_file(self, json_file, conflict_mode=EnumConflictMode.CM_OVERWRITE):
        ''' Initializes runtime settings with the parameters obtained from a JSON file. 
            :param json_file <str> : A JSON template file' path.
            :param conflict_mode(optional) <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.InitRuntimeSettingsByJsonFile(json_file, conflict_mode)
        return error


    def append_template_string_to_runtime_settings(self, json_string, conflict_mode):
        ''' Appends a new template string to the current runtime settings. 
            :param json_string <str> : A JSON string that represents the content of the settings.
            :param conflict_mode <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.AppendTplStringToRuntimeSettings(json_string, conflict_mode)
        return error


    def append_template_file_to_runtime_settings(self, json_file, conflict_mode):
        ''' Appends a new template file to the current runtime settings. 
            :param json_file <str> : A JSON template file' path.
            :param conflict_mode <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters 
                from previous template setting or to overwrite previous settings with the new template.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.AppendTplFileToRuntimeSettings(json_file, conflict_mode)
        return error


    def output_settings_to_json_string(self):
        ''' Outputs runtime settings to a json string.
            :return settings_string <str> : The output string which stores the contents of current settings.
        '''
        settings_string = self.dbr.OutputSettingsToJsonString()
        return settings_string


    def output_settings_to_json_file(self, save_file_path):
        ''' Outputs runtime settings and save them into a settings file (JSON file).
            :param save_file_path <str> : The path of the output file which stores current settings.
            :return error <tuple> : error_code = error[0], error_message = error[1], if error_code != EnumErrorCode.DBR_OK, 
                you can get the detailed error message by error_message.
        '''
        error = self.dbr.OutputSettingsToJsonFile(save_file_path)
        return error


    def get_all_template_names(self):
        ''' Gets all parameter template names.
            :return template_names <list[str]> : all parameter template names
        '''
        template_names = self.dbr.GetAllTemplateNames()
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
        error_code = self.dbr.DecodeFile(image_file_name, template_name)
        if error_code == EnumErrorCode.DBR_OK or error_code == EnumErrorCode.DBRERR_LICENSE_EXPIRED or error_code == EnumErrorCode.DBRERR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_RECOGNITION_TIMEOUT or \
            error_code == EnumErrorCode.DBRERR_1D_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_QR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PDF417_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_AZTEC_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_DATAMATRIX_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_GS1_DATABAR_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_GS1_COMPOSITE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_MAXICODE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PATCHCODE_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_POSTALCODE_LICENSE_INVALID :
            cp_text_results = self.dbr.GetAllTextResults()
            text_results = []
            if type(cp_text_results) is list:
                for cp_text_result in cp_text_results:
                    text_results.append(TextResult(cp_text_result))
                return text_results
            else:
                return None
        else:
            error_message = self.dbr.GetErrorString(error_code)
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
        error_code = self.dbr.DecodeBuffer(image, height, width, stride, image_pixel_format, template_name)
        if error_code == EnumErrorCode.DBR_OK or error_code == EnumErrorCode.DBRERR_LICENSE_EXPIRED or error_code == EnumErrorCode.DBRERR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_RECOGNITION_TIMEOUT or \
            error_code == EnumErrorCode.DBRERR_1D_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_QR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PDF417_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_AZTEC_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_DATAMATRIX_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_GS1_DATABAR_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_GS1_COMPOSITE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_MAXICODE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PATCHCODE_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_POSTALCODE_LICENSE_INVALID :
            cp_text_results = self.dbr.GetAllTextResults()
            text_results = []
            if type(cp_text_results) is list:
                for cp_text_result in cp_text_results:
                    text_results.append(TextResult(cp_text_result))
                return text_results
            else:
                return None
        else:
            error_message = self.dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def decode_file_stream(self, file_stream, file_size, template_name=""):
        ''' Decodes barcodes from an image file in memory.
            :param file_stream <bytearray> : The image file bytes in memory.
            :param file_size <int> : The length of the file bytes in memory.( len(file_stream) )
            :param template_name(optional) <str> : The template name. 
            :return text_results <list[class TextResult]> : All text results.
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        error_code = self.dbr.DecodeFileStream(file_stream, file_size, template_name)
        if error_code == EnumErrorCode.DBR_OK or error_code == EnumErrorCode.DBRERR_LICENSE_EXPIRED or error_code == EnumErrorCode.DBRERR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_RECOGNITION_TIMEOUT or \
            error_code == EnumErrorCode.DBRERR_1D_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_QR_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PDF417_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_AZTEC_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_DATAMATRIX_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_GS1_DATABAR_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_GS1_COMPOSITE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_MAXICODE_LICENSE_INVALID or error_code == EnumErrorCode.DBRERR_PATCHCODE_LICENSE_INVALID or \
            error_code == EnumErrorCode.DBRERR_POSTALCODE_LICENSE_INVALID :
            cp_text_results = self.dbr.GetAllTextResults()
            text_results = []
            if type(cp_text_results) is list:
                for cp_text_result in cp_text_results:
                    text_results.append(TextResult(cp_text_result))
                return text_results
            else:
                return None
        else:
            error_message = self.dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def get_all_intermediate_results(self):
        ''' Returns intermediate results containing the original image, the colour clustered image, the binarized image, contours, lines, text blocks, etc. 
            :return intermediate_results <liset[class IntermediateResult]>
        '''
        cp_intermediate_results = self.dbr.GetAllIntermediateResults()
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
        cp_frame_decoding_parameters = self.dbr.InitFrameDecodingParameters()
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
        cp_frame_decoding_parameters = self.dbr.InitFrameDecodingParameters()
        try:
            frame_decoding_parameters.update_parameters(cp_frame_decoding_parameters)
        except KeyError as ke:
            print(ke)
        error_code = self.dbr.StartVideoMode(cp_frame_decoding_parameters, call_back_func, template_name)
        if error_code != EnumErrorCode.DBR_OK:
            error_message = self.dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def append_video_frame(self, video_frame):
        ''' Appends a video frame to the inner frame queue. 
            :param video_frame : Gets by opencv.
            :return frame_id <int>
        '''
        frame_id = self.dbr.AppendVideoFrame(video_frame)
        return frame_id


    def stop_video_mode(self):
        ''' Stops the frame decoding thread created by start_video_mode().
            :exception BarcodeReaderError : If error happens, this function will throw a BarcodeReaderError exception that can report the detailed error message.
        '''
        error_code = self.dbr.StopVideoMode()
        if error_code != EnumErrorCode.DBR_OK:
            error_message = self.dbr.GetErrorString(error_code)
            raise BarcodeReaderError(error_message)


    def get_length_of_frame_queue(self):
        ''' Gets current length of the inner frame queue. 
            :return frame_queue_length <int> : The length of the inner frame queue.
        '''
        frame_queue_length = self.dbr.GetLengthOfFrameQueue()
        return frame_queue_length

#endregion

#endregion