# Welcome to use Dynamsoft Barcode Reader - Python Edition

![markdown](https://dynamsoft.github.io/styleguide/assets/img-icon/logo-dynamsoft-whiteBg-190x47.png "Dynamsoft")  | ![markdown](https://dynamsoft.github.io/styleguide/assets/img-icon/logo-dbr-88x88.png "dbr")

**[Dynamsoft's Barcode Reader SDK](https://www.dynamsoft.com/Products/Dynamic-Barcode-Reader.aspx) enables you to efficiently embed barcode reading functionality in your web, desktop or mobile application using just a few lines of code. This can save you months of added development time and extra costs. With our SDK, you can create high-speed and reliable barcode scanner software to meet your business needs.**

## Dynamsoft Barcode Reader - Python Edition

*Dynamsoft Barcode Reader - Python Edition contains all the features of Dynamsoft Barcode Reader. It brings convenience for customers whos develop by Python. What's more, We store some examples in [Github](https://github.com/dynamsoft-dbr/python-barcode) to help you how to use it. If you have any problems, please contact us, we will provide technical support. *

### Contact US
<support@dynamsoft.com>

### Version
** 7.3 **
>Dynamsoft Barcode Reader Python SDK 7.3 is a milestone release, which means that dbr-python is a complete product.

### Supported Platforms
**Windows x64**
**Linux**
**Mac OS**

### Installation

>pip install dbr


### Supported Symbologies
*Depending on the needs of your business, we have you covered. We support all major barcodes symbologies across a variety of industries such as government, finance, retail, warehouse inventory, and healthcare.*

- **Linear Barcodes (1D)** :
    - Code 39 *(including Code 39 Extended)*
	- Code 93
	- Code 128
	- Codabar
	- Interleaved 2 of 5
	- EAN-8
	- EAN-13
	- UPC-A
	- UPC-E
	- Industrial 2 of 5
- **2D Barcodes** :
	- QR Code *(including Micro QR Code)*
	- Data Matrix
	- PDF417 *(including Micro PDF417)*
	- Aztec Code
	- MaxiCode *(mode 2-5)*
- **Patch Code**
- **GS1 Composite Code**
- **GS1 DataBar** :
	- Omnidirectional
	- Truncated
	- Stacked
	- Stacked Omnidirectional
	- Limited
	- Expanded
	- Expanded Stacked
- **Postal Codes** :
	- USPS Intelligent Mail
	- Postnet
	- Planet
	- Australian Post
	- UK Royal Mail

### New Features
>1. In version 7.3, we made significant changes to the packaging structure of the Python SDK. We added the Python layer based on the previous CPython layer encapsulation. Now the Python layer is the interface layer, which is responsible for direct interaction with developers. The CPython layer is the middle payer, which is responsible for the transformation between the C interface and the Python interface.

>2. The Python layer now makes full use of Python's object-oriented features. It encapsulates enumerations, structures and methods, also optimizes exception throwing and handling.

>3. Added new samples to help you get started with our Python SDK.

### Interfaces

#### Enum Interfaces

```
- EnumAccompanyingTextRecognitionMode : Describes the accompanying text recognition mode.
- EnumBarcodeComplementMode           : Describes the barcode complement mode.
- EnumBarcodeColourMode               : Describes the barcode colour mode.
- EnumBarcodeFormat                   : Describes the barcode types in BarcodeFormat group 1.
- EnumBarcodeFormat_2                 : Describes the barcode types in BarcodeFormat group 2.
- EnumBinarizationMode                : Describes the binarization mode.
- EnumColourClusteringMode            : Describes the colour clustering mode.
- EnumColourConversionMode            : Describes the colour conversion mode.
- EnumConflictMode                    : Describes the conflict mode.
- EnumDeformationResistingMode        : Describes the deformation resisting mode.
- EnumDPMCodeReadingMode              : Describes the DPM code reading mode.
- EnumErrorCode                       : Describes error code
- EnumGrayscaleTransformationMode     : Describes the grayscale transformation mode.
- EnumImagePixelFormat                : Describes the image pixel format.
- EnumImagePreprocessingMode          : Describes the image preprocessing mode.
- EnumIMResultDataType                : Describes the intermediate result data type.
- EnumIntermediateResultSavingMode    : Describes the intermediate result saving mode.
- EnumIntermediateResultType          : Describes the intermediate result type.
- EnumLocalizationMode                : Describes the localization mode.
- EnumQRCodeErrorCorrectionLevel      : Describes the QR Code error correction level.
- EnumRegionPredetectionMode          : Describes the region predetection mode.
- EnumResultCoordinateType            : Describes the result coordinate type.
- EnumResultType                      : Describes the extended result type.
- EnumScaleUpMode                     : Describes the scale up mode.
- EnumTerminatePhase                  : Describes the terminate phase.
- EnumTextAssistedCorrectionMode      : Describes the text assisted correction mode.
- EnumTextFilterMode                  : Describes the text filter mode.
- EnumTextResultOrderMode             : Describes the text result order mode.
- EnumTextureDetectionMode            : Describes the texture detection mode.
```

#### Struct Interfaces

- **SamplingImageData** : Stores the sampling image data.
	- ***Attributes*** :
	```
		- bytes  : The sampling image data in a byte array.
		- width  : The width of the sampling image.
		- height : The height of the sampling image.
	```


- **FrameDecodingParameters** : Defines a class to configure the frame decoding Parameters.
	- ***Attributes*** :
	```
		- max_queue_length              : The maximum number of frames waiting for decoding.
		- max_result_queue_length       : The maximum number of frames waiting results (text result/localization result) will be kept for further reference.
		- width                         : The width of the frame image in pixels.
		- height                        : The height of the frame image in pixels.
		- stride                        : The stride (or scan width) of the frame image.
		- image_pixel_format            : The image pixel format used in the image byte array.
		- region_top                    : The region definition of the frame to calculate the internal indicator.The top-most coordinate or percentage of the region.
		- region_left                   : The region definition of the frame to calculate the internal indicator.The left-most coordinate or percentage of the region.
		- region_right                  : The region definition of the frame to calculate the internal indicator.The right-most coordinate or percentage of the region.
		- region_bottom                 : The region definition of the frame to calculate the internal indicator.The bottom-most coordinate or percentage of the region.
		- region_measured_by_percentage : Sets whether or not to use percentage to measure the region size.
		- threshold                     : The threshold used for filtering frames.
		- fps                           : The frequency of calling AppendFrame() per second.
		- auto_filter                   : Sets whether to filter frames automatically.
	```


- **PublicRuntimeSetting** : Defines a struct to configure the barcode reading runtime settings. These settings control the barcode recognition process such as which barcode types to decode.
	- ***Attributes*** :
	```
		- terminate_phase : Sets the phase to stop the barcode reading algorithm.
		- timeout         : Sets the maximum amount of time (in milliseconds) that should be spent searching for a barcode per page. It does not include the time taken to load/decode an image (TIFF, PNG, etc.) from disk into memory.
		- max_algorithm_thread_count : Sets the number of threads the image processing algorithm will use to decode barcodes.
		- expected_barcodes_count    : Sets the number of barcodes expected to be detected for each image.
		- barcode_format_ids         : Sets the formats of the barcode in BarcodeFormat group 1 to be read. Barcode formats in BarcodeFormat group 1 can be combined.
		- barcode_format_ids_2       : Sets the formats of the barcode in BarcodeFormat group 2 to be read. Barcode formats in BarcodeFormat group 2 can be combined.
		- pdf_raster_dpi             : Sets the output image resolution.
		- scale_down_threshold       : Sets the threshold for the image shrinking.
		- binarization_modes         : Sets the mode and priority for binarization.
		- localization_modes         : Sets the mode and priority for localization algorithms.
		- colour_clustering_modes    : Sets the mode and priority for colour categorization. Not supported yet.
		- colour_conversion_modes    : Sets the mode and priority for converting a colour image to a grayscale image.
		- grayscale_transformation_modes : Sets the mode and priority for the grayscale image conversion.
		- region_predetection_modes      : Sets the region pre-detection mode for barcodes search.
		- image_preprocessing_modes      : Sets the mode and priority for image preprocessing algorithms.
		- texture_detection_modes        : Sets the mode and priority for texture detection.
		- text_filter_modes              : Sets the mode and priority for text filter.
		- dpm_code_reading_modes         : Sets the mode and priority for DPM code reading.
		- deformation_resisting_modes    : Sets the mode and priority for deformation resisting.
		- barcode_complement_modes       : Sets the mode and priority to complement the missing parts in the barcode.
		- barcode_colour_modes           : Sets the mode and priority for the barcode colour mode used to process the barcode zone.
		- accompanying_text_recognition_modes : Sets the mode and priority to recognize accompanying text.
		- text_result_order_modes             : Sets the mode and priority for the order of the text results returned.
		- scale_up_modes                      : Sets the mode and priority to control the sampling methods of scale-up for linear barcode with small module sizes.
		- text_assisted_correction_mode       : Sets the mode of text assisted correction for barcode decoding. Not supported yet.
		- intermediate_result_saving_mode     : Sets the mode for saving intermediate result.
		- deblur_level                        : Sets the degree of blurriness of the barcode.
		- intermediate_result_types           : Sets which types of intermediate result to be kept for further reference. Intermediate result types can be combined.
		- result_coordinate_type              : Specifies the format for the coordinates returned.
		- return_barcode_zone_clarity         : Sets whether or not to return the clarity of the barcode zone.
		- region_top    : The top-most coordinate or percentage of the region.
		- region_bottom : The bottom-most coordinate or percentage of the region.
		- region_left   : The left-most coordinate or percentage of the region.
		- region_right  : The right-most coordinate or percentage of the region.
		- region_measured_by_percentage : Sets whether or not to use percentage to measure the region size.
		- min_barcode_text_length       : Sets the range of barcode text length for barcodes search.
		- min_result_confidence         : The minimum confidence of the result.
	```

- **OnedDetailedResult** : Stores the OneD code details.
	- ***Attributes*** :
	```
		- module_size       : The barcode module size (the minimum bar width in pixel)
		- start_chars_bytes : The start chars in a byte array
		- stop_chars_bytes  : The stop chars in a byte array
		- check_digit_bytes : The check digit chars in a byte array
	```

- **QRCodeDetailedResult** : Stores the QRCode details.
	- ***Attributes*** :
	```
		- module_size            : The barcode module size (the minimum bar width in pixel)
		- rows                   : The row count of the barcode
		- columns                : The column count of the barcode
		- error_correction_level : The error correction level of the barcode
		- versions               : The version of the QR Code
		- model                  : Number of the models
	```

- **DataMatrixDetailedResult** : Stores the DataMatrix details.
	- ***Attributes*** :
	```
		- module_size         : The barcode module size (the minimum bar width in pixel)
		- rows                : The row count of the barcode
		- columns             : The column count of the barcode
		- data_region_rows    : The data region row count of the barcode
		- data_region_columns : The data region column count of the barcode
		- data_region_number  : The data region count
	```

- **PDFDetailedResult** : Stores the PDF details.
	- ***Attributes*** :
	```
		- module_size            : The barcode module size (the minimum bar width in pixel)
		- rows                   : The row count of the barcode
		- columns                : The column count of the barcode
		- error_correction_level : The error correction level of the barcode
	```

- **AztecDetailedResult** : Stores the Aztec details.
	- ***Attributes*** :
	```
		- module_size  : The barcode module size (the minimum bar width in pixel)
		- rows         : The row count of the barcode
		- columns      : The column count of the barcode
		- layer_number : A negative number (-1, -2, -3, -4) specifies a compact Aztec code. A positive number (1, 2, .. 32) specifies a normal (full-rang) Aztec code.
	```

- **ExtendedResult** : Stores the extended result.
	- ***Attributes*** :
	```
		- result_type             : Extended result type
		- barcode_format          : Barcode type in BarcodeFormat group 1
		- barcode_format_string   : Barcode type in BarcodeFormat group 1 as string
		- barcode_format_2        : Barcode type in BarcodeFormat group 2
		- barcode_format_string_2 : Barcode type in BarcodeFormat group 2 as string
		- confidence              : The confidence of the result
		- bytes                   : The content in a byte array
		- accompanying_text_bytes : The accompanying text content in a byte array
		- deformation             : The deformation value
		- detailed_result         : One of the following: OnedDetailedResult, PDFDetailedResult, DataMatrixDetailedResult, AztecDetailedResult, QRCodeDetailedResult
		- sampling_image          : The sampling image info
		- clarity                 : The clarity of the barcode zone in percentage.
	```

- **LocalizationResult** : Stores the localization result.
	- ***Attributes*** :
	```
		- terminate_phase         : The terminate phase of localization result
		- barcode_format          : Barcode type in BarcodeFormat group 1
		- barcode_format_string   : Barcode type in BarcodeFormat group 1 as string
		- barcode_format_2        : Barcode type in BarcodeFormat group 2
		- barcode_format_string_2 : Barcode type in BarcodeFormat group 2 as string
		- localization_points     : Extended result type
		- result_type             : The 4 localization points
		- angle                   : The angle of a barcode. Values range is from 0 to 360
		- module_size             : The barcode module size (the minimum bar width in pixel)
		- page_number             : The page number the barcode located in. The index is 0-based
		- region_name             : The region name the barcode located in
		- document_name           : The document name
		- result_coordinate_type  : The coordinate type
		- accompanying_text_bytes : The accompanying text content in a byte array
		- confidence              : The confidence of the localization result
	```

- **TextResult** : Stores the text result.
	- ***Attributes*** :
	```
		- barcode_format          : Barcode type in BarcodeFormat group 1
		- barcode_format_string   : Barcode type in BarcodeFormat group 1 as string
		- barcode_format_2        : Barcode type in BarcodeFormat group 2
		- barcode_format_string_2 : Barcode type in BarcodeFormat group 2 as string
		- barcode_text            : The barcode text
		- barcode_bytes           : The barcode content in a byte array
		- localization_result     : The corresponding localization result
		- detailed_result         : One of the following: OnedDetailedResult, PDFDetailedResult, DataMatrixDetailedResult, AztecDetailedResult, QRCodeDetailedResult
		- extended_results        : The extended result list
	```

- **Point** : Stores an x- and y-coordinate pair in two-dimensional space.
	- ***Attributes*** :
	```
		- x : The X coordinate of the point
		- y : The Y coordinate of the point
	```

- **ImageData** : Stores the image data.
	- ***Attributes*** :
	```
		- bytes              : The image data content in a byte array
		- width              : The width of the image in pixels
		- height             : The height of the image in pixels
		- stride             : The image data content in a byte array
		- image_pixel_format : The image data content in a byte array
	```

- **Contour** : Stores the contour.
	- ***Attributes*** :
	```
		- points : The points list
	```

- **LineSegment** : Stores line segment data.
	- ***Attributes*** :
	```
		- start_point                   : The start point of the line segment
		- end_point                     : The end point of the line segment
		- lines_confidence_coefficients : The end point of the line segment
	```

- **RegionOfInterest** : Stores the region of interest.
	- ***Attributes*** :
	```
		- roi_id : The ID generated by the SDK
		- point  : The left top point of the region
		- width  : The width of the region
		- height : The height of the region
	```

- **IntermediateResult** : Stores the intermediate result.
	- ***Attributes*** :
	```
		- data_type                     : The data type of the intermediate result
		- results                       : One of the following types: List of class Contour, List of class ImageData, List of class LineSegment, List of class LocalizationResult, List of class RegionOfInterest
		- result_type                   : Intermediate result type
		- barcode_complement_mode       : The BarcodeComplementMode used when generating the current intermediate result
		- bcm_index                     : The list index of current used ColourClusteringMode in the ColourClusteringModes setting
		- deformation_resisting_mode    : The DeformationResistingMode used when generating the current intermediate result
		- drm_index                     : The list index of current used DeformationResistingMode in the DeformationResistingModes setting
		- dpm_code_reading_mode         : The DPMCodeReadingMode used when generating the current intermediate result
		- dpmcrm_index                  : The list index of current used DPMCodeReadingMode in the DPMCodeReadingModes setting
		- text_filter_mode              : The TextFilterMode used when generating the current intermediate result
		- tfm_index                     : The list index of current used TextFilterMode in the TextFilterModes setting
		- localization_mode             : The LocalizationMode used when generating the current intermediate result
		- lm_index                      : The list index of current used LocalizationMode in the LocalizationModes setting
		- binarization_mode             : The BinarizationMode used when generating the current intermediate result
		- bm_index                      : The list index of current used BinarizationMode in the BinarizationModes setting
		- image_preprocessing_mode      : The ImagePreprocessingMode used when generating the current intermediate result
		- ipm_index                     : The list index of current used ImagePreprocessingMode in the ImagePreprocessingModes setting
		- region_predetection_mode      : The RegionPredetectionMode used when generating the current intermediate result
		- rpm_index                     : The list index of current used RegionPredetectionMode in the RegionPredetectionModes setting
		- grayscale_transformation_mode : The GrayscaleTransformationMode used when generating the current intermediate result
		- gtm_index                     : The list index of current used GrayscaleTransformationMode in the GrayscaleTransformationModes setting
		- colour_conversion_mode        : The ColourConversionMode used when generating the current intermediate result
		- cicm_index                    : The list index of current used ColourConversionMode in the ColourConversionModes setting
		- colour_clustering_mode        : The ColourClusteringMode used when generating the current intermediate result
		- ccm_index                     : The list index of current used ColourClusteringMode in the ColourClusteringModes setting
		- rotation_matrix               : The rotation matrix
		- roi_id                        : The ID of the ROI (Region Of Interest) generated by the SDK. -1 means the original image.
		- scale_down_ratio              : The scale down ratio
		- frame_id                      : The ID of the operated frame
	```

#### Exception Class Interface
- **BarcodeReaderError**

#### Main Class Interface
- **BarcodeReader** : Defines a class that provides functions for decoding barcodes in images. This is the main interface for recognizing barcodes.
	- ***Attributes*** :
	```
		- version     : The Dynamsoft Barcode Reader - Python Edition version
		- dbr_version : The Dynamsoft Barcode Reader version
	```
	- ***Methods*** :
	```
		- get_error_string(error_code)
			@description Get the detailed error message by error code
			@param  error_code   <int> : Error code
			@return error_string <str> : The detailed error message
		- init_license(dbr_license)
			@description Reads product key and activates the SDK.
			@param  dbr_license <str>   : The product keys
			@return error       <tuple> : error_code = error[0], error_message = error[1].
		- init_license_from_server(license_server, license_key)
			@description Initializes barcode reader license and connects to the specified server for online verification.
			@param  license_server <str>   : The name/IP of the license server.
			@param  license_key    <str>   : The license key.
			@return error          <tuple> : error_code = error[0], error_message = error[1].
		- init_license_from_license_content(license_key, license_content)
			@description Initializes barcode reader license from the license content on the client machine for offline verification.
			@param  license_key     <str>   : The license key.
			@param  license_content <str>   : An encrypted string representing the license content (quota, expiration date, barcode type, etc.) obtained from the method output_license_to_string().
			@return error           <tuple> : error_code = error[0], error_message = error[1].
		- output_license_to_string()
			@description Outputs the license content as an encrypted string from the license server to be used for offline license verification.
			@return license_string  <str> : An encrypted string which stores the content of license.
			@exception BarcodeReaderError
		- get_runtime_settings()
			@description Get current runtime settings.
			@return runtime_settings <class PublicRuntimeSetting> : The PublicRuntimeSetting object of current runtime settings.
		- update_runtime_settings(settings)
			@description Update runtime settings with a PublicRuntimeSetting object.
			@param settings <class PublicRuntimeSetting> : a PublicRuntimeSetting object.
			@exception BarcodeReaderError
		- reset_runtime_settings(settings)
			@description Resets all parameters to default values.
		- set_mode_argument(modes_name, index, argument_name, argument_value)
			@description Sets the optional argument for a specified mode in Modes(Mode) parameters.
			@param  modes_name     <str>   : The modes(mode) parameter name to set argument.
			@param  index          <int>   : The array index of modes parameter to indicate a specific mode.
			@param  argument_name  <str>   : The name of the argument to set.
			@param  argument_value <str>   : The value of the argument to set.
			@return error          <tuple> : error_code = error[0], error_message = error[1].
		- get_mode_argument(modes_name, index, argument_name)
			@description Gets the optional argument for a specified mode in Modes(Mode) parameters.
			@param  modes_name      <str> : The modes(mode) parameter name to get argument.
			@param  index           <int> : The array index of modes parameter to indicate a specific mode.
			@param  argument_name   <str> : The name of the argument to get.
			@return argument_value  <str> : The value of the argument to get.
			@exception BarcodeReaderError
		- init_runtime_settings_with_string(json_string, conflict_mode=EnumConflictMode.CM_OVERWRITE)
			@description Initializes runtime settings with the parameters obtained from a JSON string.
			@param  json_string     <str> : A JSON string that represents the content of the settings.
			@param  conflict_mode   <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters from previous template setting or to overwrite previous settings with the new template.
			@return error           <tuple> : error_code = error[0], error_message = error[1].
		- init_runtime_settings_with_file(json_file, conflict_mode=EnumConflictMode.CM_OVERWRITE)
			@description Initializes runtime settings with the parameters obtained from a JSON file.
			@param json_file        <str> : A JSON template file.
			@param conflict_mode    <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters from previous template setting or to overwrite previous settings with the new template.
			@return error           <tuple> : error_code = error[0], error_message = error[1].
		- append_template_string_to_runtime_settings(json_string, conflict_mode)
			@description Appends a new template string to the current runtime settings.
			@param json_string      <str> : A JSON string that represents the content of the settings.
			@param conflict_mode    <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters from previous template setting or to overwrite previous settings with the new template.
			@return error           <tuple> : error_code = error[0], error_message = error[1].
		- append_template_file_to_runtime_settings(json_file, conflict_mode)
			@description Appends a new template file to the current runtime settings.
			@param json_file        <str> : A JSON template file.
			@param conflict_mode    <EnumConflictMode> : The parameter setting mode, which decides whether to inherit parameters from previous template setting or to overwrite previous settings with the new template.
			@return error           <tuple> : error_code = error[0], error_message = error[1].
		- output_settings_to_json_string()
			@description Outputs runtime settings to a json string.
			@return settings_string <str> : The output string which stores the contents of current settings.
		- output_settings_to_json_file(save_file_path)
			@description Outputs runtime settings and save them into a settings file (JSON file).
			@param save_file_path   <str> : The path of the output file which stores current settings.
			@return error           <tuple> : error_code = error[0], error_message = error[1].
		- get_all_template_names()
			@description Gets all parameter template names.
			@return template_names  <list[str]> : all parameter template names
		- decode_file(image_file_name, template_name="")
			@description Decodes barcodes in the specified image file.
			@param  image_file_name  <str> : A string defining the file name.
			@param  template_name    <str> : The template name.
			@return text_results     <list[class TextResult]> : All text results.
			@exception BarcodeReaderError
		- decode_buffer(image, image_pixel_format=EnumImagePixelFormat.IPF_RGB_888, template_name="")
			@description Decodes barcodes from the memory buffer containing image pixels in defined format.
			@param image              <class numpy.ndarray> : The image which is processed by opencv.
			@param image_pixel_format <EnumImagePixelFormat> : The image pixel format used in the image byte array.
			@param template_name      <str> : The template name.
			@return text_results      <list[class TextResult]> : All text results.
			@exception BarcodeReaderError
		- decode_file_stream(file_stream, template_name="")
			@description Decodes barcodes from an image file in memory.
			@param file_stream <bytearray> : The image file bytes in memory.
			@return text_results     <list[class TextResult]> : All text results.
			@exception BarcodeReaderError
		- get_all_intermediate_results()
			@description Returns intermediate results containing the original image, the colour clustered image, the binarized image, contours, lines, text blocks, etc.
			@return intermediate_results      <liset[class IntermediateResult]> : All intermediate results.
		- init_frame_decoding_parameters()
			@description Init frame decoding parameters.
			@return frame_decoding_parameters <class FrameDecodingParameters> : The frame decoding parameters.
		- start_video_mode(frame_decoding_parameters, call_back_func, template_name="")
			@description Starts a new thread to decode barcodes from the inner frame queue.
			@param frame_decoding_parameters <class FrameDecodingParameters> : The frame decoding parameters. You can get it by init_frame_decoding_parameters(), then modify its attributes value.
			@param call_back_func            <function pointer> : Sets callback function to process text results generated during frame decoding.
			@param template_name             <str> : The template name.
			@exception BarcodeReaderError
		- append_video_frame(video_frame)
			@description Appends a video frame to the inner frame queue.
			@param video_frame : Gets by opencv.
			@return frame_id <int> : Current frame id.
		- stop_video_mode()
			@description Stops the frame decoding thread created by start_video_mode().
			@exception BarcodeReaderError
		- get_length_of_frame_queue()
			@description Gets current length of the inner frame queue.
			@return frame_queue_length <int> : The length of the inner frame queue.
	```

### Others

- **Code Snippet**

```
import os
import sys
import cv2
import json
from typing import List
from dbr import *

# you can change the following variables' value to your own value.
license_key = "Input your own license"
#license_server = "Input the name/IP of the license server"
json_file = r"Please input your own template path"
image = r"Please input your own image path"

reader = BarcodeReader()

reader.init_license(license_key)
#reader.init_license_from_server(license_server, license_key)
#license_content = reader.output_license_to_string()
#reader.init_license_from_license_content(license_key, license_content)

error = reader.init_runtime_settings_with_file(json_file)
if error[0] != EnumErrorCode.DBR_OK:
    print(error[1])

if sys.version_info.major == 3 and sys.version_info.minor >= 6:

    try:
        text_results:List[TextResult] = reader.decode_file(image)

        if text_results != None:
            for text_result in text_results:
                print("Barcode Format :")
                print(text_result.barcode_format_string)
                print("Barcode Text :")
                print(text_result.barcode_text)
                print("Localization Points : ")
                print(text_result.localization_result.localization_points)
                print("-------------")
    except BarcodeReaderError as bre:
        print(bre)

else:
    try:
        text_results = reader.decode_file(image)

        if text_results != None:
            for text_result in text_results:
                print("Barcode Format :")
                print(text_result.barcode_format_string)
                print("Barcode Text :")
                print(text_result.barcode_text)
                print("Localization Points : ")
                print(text_result.localization_result.localization_points)
                print("-------------")
    except BarcodeReaderError as bre:
        print(bre)
```

- **Mode Argument List**

>***You can refer to this list when you use set_mode_argument() or get_mode_argument().***

| Mode Parameter Name | Argument Name | Argument Description
| --------------------| ------------- | ---------------------
| AccompanyingTextRecognitionModes | RegionBottom | Specifies the y-coordinate of the bottom-right corner of the region in percentage. This value is relative to the top-left corner of the barcode.  <br> <b>Value Range : </b><br>[-255, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for ATRM_GENERAL<br>If RegionLeft, RegionTop, RegionRight and RegionBottom are all equal to 0, the accompanying text zone will be detected automatically by the SDK.<br>For more info on how to set a custom area for accompanying texts, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/SetCustomeAreaForAccompanyingTexts.html)|
| AccompanyingTextRecognitionModes | RegionLeft | Specifies the x-coordinate of the top-left corner of the region in percentage. This value is relative to the top-left corner of the barcode. <br> <b>Value Range : </b><br>[-255, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for ATRM_GENERAL<br>If RegionLeft, RegionTop, RegionRight and RegionBottom are all equal to 0, the accompanying text zone will be detected automatically by the SDK.<br>For more info on how to set a custom area for accompanying texts, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/SetCustomeAreaForAccompanyingTexts.html)|
| AccompanyingTextRecognitionModes | RegionRight | Specifies the x-coordinate of the bottom-right corner of the region in percentage. This value is relative to the top-left corner of the barcode. <br> <b>Value Range : </b><br>[-255, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for ATRM_GENERAL<br>If RegionLeft, RegionTop, RegionRight and RegionBottom are all equal to 0, the accompanying text zone will be detected automatically by the SDK.<br>For more info on how to set a custom area for accompanying texts, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/SetCustomeAreaForAccompanyingTexts.html)|
| AccompanyingTextRecognitionModes | RegionTop | Specifies the y-coordinate of the top-left corner of the region in percentage. This value is relative to the top-left corner of the barcode. <br> <b>Value Range : </b><br>[-255, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for ATRM_GENERAL<br>If RegionLeft, RegionTop, RegionRight and RegionBottom are all equal to 0, the accompanying text zone will be detected automatically by the SDK.<br>For more info on how to set a custom area for accompanying texts, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/SetCustomeAreaForAccompanyingTexts.html)|
| BarcodeColourModes | LightReflection | Sets if there is light reflection on the barocde zone. <br> <b>Value Range : </b><br>[0, 1] <br> <b>Default Value : </b><br>1 <br> <b>Remarks: </b><br>Valid for BICM_DARK_LIGHT_MIXED; BICM_DARK_ON_DARK; BICM_DARK_ON_LIGHT; BICM_LIGHT_ON_DARK;BICM_LIGHT_ON_LIGHT; BICM_DARK_ON_LIGHT_DARK_SURROUNDING<br>0: no light reflection<br>1: has light reflection|
| BinarizationModes | BlockSizeX | Sets the horizontal block size for the binarization process. <br> <b>Value Range : </b><br>[0, 1000] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for BM_LOCAL_BLOCK<br>0: the block size used for binarization will be set to a value which is calculated automatically.<br>N: <br>1 <= N <= 3: the block size used for binarization will be set to 3. <br>N > 3: the block size used for binarization will be set to N.<br>Block size refers to the size of a pixel neighborhood used to calculate a threshold value for the pixel.<br>An appropriate value for binarizationBlockSize can help generate a high quality binary image and increase the accuracy of barcode localization.|
| BinarizationModes | BlockSizeY | Sets the vertical block size for the binarization process. <br> <b>Value Range : </b><br>[0, 1000] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for BM_LOCAL_BLOCK<br>0: the block size used for binarization will be set to a value which is calculated automatically.<br>N: <br>1 <= N <= 3: the block size used for binarization will be set to 3. <br>N > 3: the block size used for binarization will be set to N.<br>Block size refers to the size of a pixel neighborhood used to calculate a threshold value for the pixel.<br>An appropriate value for binarizationBlockSize can help generate a high quality binary image to increase the accuracy of barcode localization.|
| BinarizationModes | EnableFillBinaryVacancy | Sets whether to enable binary vacancy filling. <br> <b>Value Range : </b><br>[0, 1] <br> <b>Default Value : </b><br>1 <br> <b>Remarks: </b><br>Valid for BM_LOCAL_BLOCK<br>0 - disable, 1 - enable<br>For barcodes with a large module size, there might be a vacant area in the position detection pattern after binarization. The vacant area may result in decoding failure. Setting this to True will fill in the vacant area with black and may help improve the decoding success rate. Better accuracy for images with a large module size.|
| BinarizationModes | ImagePreprocessingModesIndex | The index of a specific image preprocessing mode in the ImagePreprocessingModes parameter which the current binarization mode is applied to. <br> <b>Value Range : </b><br>[-1, 0x7fffffff] <br> <b>Default Value : </b><br>-1 <br> <b>Remarks: </b><br>Valid for BM_LOCAL_BLOCK<br>-1: The current binarization mode is applied to all modes in parameter ImagePreprocessingModes.|
| BinarizationModes | ThreshValueCoefficient | Constant subtracted from the mean or weighted mean. Normally, it is positive but may be zero or negative as well. <br> <b>Value Range : </b><br>[-255, 255] <br> <b>Default Value : </b><br>10 <br> <b>Remarks: </b><br>Valid for BM_LOCAL_BLOCK|
| ColourClusteringModes | Sensitivity | Sets the sensitivity used for colour categorization. <br> <b>Value Range : </b><br>[1, 9] <br> <b>Default Value : </b><br>5 <br> <b>Remarks: </b><br>Valid for CCM_GENERAL_HSV<br>A higher level means less colours will be clustered as the same colour.|
| ColourConversionModes | BlueChannelWeight | Sets the weight value of Blue Colour Channel used for converting a colour image to a grayscale image. <br> <b>Value Range : </b><br>[-1, 1000] <br> <b>Default Value : </b><br>-1 <br> <b>Remarks: </b><br>Valid for CICM_GENERAL<br>-1: The weight value will be set automatically by the SDK.|
| ColourConversionModes | GreenChannelWeight | Sets the weight value of Green Colour Channel used for converting a colour image to a grayscale image. <br> <b>Value Range : </b><br>[-1, 1000] <br> <b>Default Value : </b><br>-1 <br> <b>Remarks: </b><br>Valid for CICM_GENERAL<br>-1: The weight value will be set automatically by the SDK.|
| ColourConversionModes | RedChannelWeight | Sets the weight value of Red Colour Channel used for converting a colour image to a grayscale image. <br> <b>Value Range : </b><br>[-1, 1000] <br> <b>Default Value : </b><br>-1 <br> <b>Remarks: </b><br>Valid for CICM_GENERAL<br>-1: The weight value will be set automatically by the SDK.|
| DeformationResistingModes | Level | Sets the effort level used for deformation resisting.Not supported yet. <br> <b>Value Range : </b><br>[1, 9] <br> <b>Default Value : </b><br>5 <br> <b>Remarks: </b><br>Valid for DRM_GENERAL<br>A larger value means the library will take more effort to resist deformation.|
| ImagePreprocessingModes | Sensitivity | Sets the sensitivity used for gray equalization. <br> <b>Value Range : </b><br>[1, 9] <br> <b>Default Value : </b><br>5 <br> <b>Remarks: </b><br>Valid for IPM_GRAY_EQUALIZE<br>If you have an image with a low level of contrast, you can set the property to a larger value.<br>A larger value means a higher possibility that gray equalization will be activated.<br>This may cause adverse effect on images with a high level of contrast.|
| ImagePreprocessingModes | SmoothBlockSizeX | Sets the horizontal block size for the smoothing process. <br> <b>Value Range : </b><br>[3, 1000] <br> <b>Default Value : </b><br>3 <br> <b>Remarks: </b><br>Valid for IPM_GRAY_SMOOTH;IPM_SHARPEN_SMOOTH<br>Block size refers to the size of a pixel neighborhood used to calculate the threshold for the pixel.<br>An appropriate value can help increase the accuracy of barcode localization.|
| ImagePreprocessingModes | SmoothBlockSizeY | Sets the vertical block size for the smoothing process. <br> <b>Value Range : </b><br>[3, 1000] <br> <b>Default Value : </b><br>3 <br> <b>Remarks: </b><br>Valid for IPM_GRAY_SMOOTH;IPM_SHARPEN_SMOOTH<br>Block size refers to the size of a pixel neighborhood used to calculate the threshold for the pixel.<br>An appropriate value can help increase the accuracy of barcode localization.|
| ImagePreprocessingModes | SharpenBlockSizeX | Sets the horizontal block size for the sharpening process. <br> <b>Value Range : </b><br>[3, 1000] <br> <b>Default Value : </b><br>3 <br> <b>Remarks: </b><br>Valid for IPM_SHARPEN_SMOOTH<br>Block size refers to the size of a pixel neighborhood used to calculate the threshold for the pixel.<br>An appropriate value can help increase the accuracy of barcode localization.|
| ImagePreprocessingModes | SharpenBlockSizeY | Sets the vertical block size for the sharpening process. <br> <b>Value Range : </b><br>[3, 1000] <br> <b>Default Value : </b><br>3 <br> <b>Remarks: </b><br>Valid for IPM_SHARPEN_SMOOTH<br>Block size refers to the size of a pixel neighborhood used to calculate the threshold for the pixel.<br>An appropriate value can help increase the accuracy of barcode localization.|
| IntermediateResultSavingMode | FolderPath | Sets the path of the output folder which stores intermediate results. <br> <b>Value Range : </b><br>A string value representing the folder path with max length 480. <br> <b>Default Value : </b><br>"" <br> <b>Remarks: </b><br>Valid for IRSM_FILESYSTEM;IRSM_BOTH<br>"": The library path.<br>Please make sure the path exists and your application has the appropriate permissions for saving the results.|
| IntermediateResultSavingMode | RecordsetSizeOfLatestImages | Sets the maximum count of recordset to store the latest images' intermediate results. <br> <b>Value Range : </b><br>[0,0x7fffffff] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>0 means no limitation on the count of recordset. <br>When the count exceeds, the old recordset will be replaced by the new one.|
| LocalizationModes | ScanStride | Sets the stride in pixels between scans when searching for barcodes. <br> <b>Value Range : </b><br>[0, 0x7fffffff] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for LM_SCAN_DIRECTLY<br>0: means automatically set by the library.<br>When the set value is greater than half the width or height of the current image, the actual processing is 0.|
| LocalizationModes | ScanDirection | Sets the scan direction when searching barcode. <br> <b>Value Range : </b>[0,2] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>0: Both vertical and horizontal direction <br>1: Vertical direction <br>2: Horizontal direction.|
| RegionPredetectionModes | MinImageDimension | Sets the minimum image dimension (in pixels) to pre-detect barcode regions. <br> <b>Value Range : </b><br>[262144, 0x7fffffff] <br> <b>Default Value : </b><br>262144 <br> <b>Remarks: </b><br>Valid for RPM_GENERAL_GRAY_CONTRAST;RPM_GENERAL_HSV_CONTRAST;RPM_GENERAL_RGB_CONTRAST<br>If the image dimension is larger than the given value, the library will enable the feature of pre-detecting barcode regions. Otherwise, it will skip this step when searching for barcodes.|
| RegionPredetectionModes | Sensitivity | Sets the sensitivity used for region predetection algorithm. <br> <b>Value Range : </b><br>[1, 9] <br> <b>Default Value : </b><br>1 <br> <b>Remarks: </b><br>Valid for RPM_GENERAL_GRAY_CONTRAST;RPM_GENERAL_HSV_CONTRAST;RPM_GENERAL_RGB_CONTRAST<br>A larger value means the library will take more effort to detect regions.|
| ScaleUpModes | AcuteAngleWithXThreshold | Sets the acute angle threshold for scale-up. <br> <b>Value Range : </b><br>[-1, 90] <br> <b>Default Value : </b><br>-1 <br> <b>Remarks: </b><br>Valid for SUM_LINEAR_INTERPOLATION, SUM_NEAREST_NEIGHBOUR_INTERPOLATION<br>-1 : means automatically set by the library. <br>If the module size of the barcode is smaller than the ModuleSizeThreshold and the acute angle with X of the barcode is larger than the AcuteAngleWithXThreshold, the barcode will be enlarged to N times (N=1,2,3...) till N * modulesize >= TargetModuleSize. <br>For more info on how to custom scale up rules, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/EnableScaleUp.html)|
| ScaleUpModes | ModuleSizeThreshold | Sets the module size threshold for scale-up. <br> <b>Value Range : </b><br>[0, 0x7fffffff] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for SUM_LINEAR_INTERPOLATION, SUM_NEAREST_NEIGHBOUR_INTERPOLATION<br>0 : means automatically set by the library. <br>If the module size of the barcode is smaller than the ModuleSizeThreshold and the acute angle with X of the barcode is larger than the AcuteAngleWithXThreshold, the barcode will be enlarged to N times (N=1,2,3...) till N * modulesize >= TargetModuleSize. <br>For more info on how to custom scale up rules, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/EnableScaleUp.html)|
| ScaleUpModes | TargetModuleSize | Sets the target module size for scale-up. <br> <b>Value Range : </b><br>[0, 0x7fffffff] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for SUM_LINEAR_INTERPOLATION, SUM_NEAREST_NEIGHBOUR_INTERPOLATION<br>0 : means automatically set by the library. <br>If the module size of the barcode is smaller than the ModuleSizeThreshold and the acute angle with X of the barcode is larger than the AcuteAngleWithXThreshold, the barcode will be enlarged to N times (N=1,2,3...) till N * modulesize >= TargetModuleSize. <br>For more info on how to custom scale up rules, please refer to [this article](https://www.dynamsoft.com/help/Barcode-Reader/devguide/HowTo/EnableScaleUp.html)|
| TextAssistedCorrectionMode | BottomTextPercentageSize | Sets the percentage of the bottom accompanying text zone comparing to the barcode zone. <br> <b>Value Range : </b><br>[0, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for TACM_VERIFYING;TACM_VERIFYING_PATCHING<br>255: The accompanying text zone will be detected automatically by the SDK.|
| TextAssistedCorrectionMode | LeftTextPercentageSize | Sets the percentage of the left accompanying text zone comparing to the barcode zone. <br> <b>Value Range : </b><br>[0, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for TACM_VERIFYING;TACM_VERIFYING_PATCHING<br>255: The accompanying text zone will be detected automatically by the SDK.|
| TextAssistedCorrectionMode | RightTextPercentageSize | Sets the percentage of the right accompanying text zone comparing to the barcode zone. <br> <b>Value Range : </b><br>[0, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for TACM_VERIFYING;TACM_VERIFYING_PATCHING<br>255: The accompanying text zone will be detected automatically by the SDK.|
| TextAssistedCorrectionMode | TopTextPercentageSize | Sets the percentage of the top accompanying text zone comparing to the barcode zone. <br> <b>Value Range : </b><br>[0, 255] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for TACM_VERIFYING;TACM_VERIFYING_PATCHING<br>255: The accompanying text zone will be detected automatically by the SDK.|
| TextFilterModes | MinImageDimension | Sets the minimum image dimension (in pixels) to filter the text. <br> <b>Value Range : </b><br>[65536, 0x7fffffff] <br> <b>Default Value : </b><br>65536 <br> <b>Remarks: </b><br>Valid for TFM_GENERAL_CONTOUR<br>If the image dimension is larger than the given value, the library will enable the text filtering feature. Otherwise, it will skip this step when doing barcode recognition.<br>The feature can speed up the recognition process.|
| TextFilterModes | Sensitivity | Sets the sensitivity used for text filtering. <br> <b>Value Range : </b><br>[0, 9] <br> <b>Default Value : </b><br>0 <br> <b>Remarks: </b><br>Valid for TFM_GENERAL_CONTOUR<br>0: means automatically set by the library.<br>A larger value means the library will take more effort to filter text.|
| TextureDetectionModes | Sensitivity | Sets the sensitivity used for texture detection. <br> <b>Value Range : </b><br>[1, 9] <br> <b>Default Value : </b><br>5 <br> <b>Remarks: </b><br>Valid for TDM_GENERAL_WIDTH_CONCENTRATION<br>A larger value means the library will take more effort to detect texture.|