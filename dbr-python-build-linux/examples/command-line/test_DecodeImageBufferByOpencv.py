import os
import cv2
import sys
sys.path.append('../')
from dbr import DynamsoftBarcodeReader
dbr = DynamsoftBarcodeReader()

def InitLicense(license):
    dbr.InitLicense(license)

def DecodeBuffer(imageByOpencv):
    try:
        results = dbr.DecodeBuffer(image, image.shape[0], image.shape[1], image.strides[0])
        textResults = results["TextResults"]
        # intermediateResults = results["IntermediateResults"]
        print(len(textResults))
        for textResult in textResults:
            print(textResult["BarcodeFormatString"])
            print(textResult["BarcodeText"])
            localizationResult = textResult["LocalizationResult"]
            x1 = localizationResult["X1"]
            y1 = localizationResult["Y1"]
            x2 = localizationResult["X2"]
            y2 = localizationResult["Y2"]
            x3 = localizationResult["X3"]
            y3 = localizationResult["Y3"]
            x4 = localizationResult["X4"]
            y4 = localizationResult["Y4"]
            localizationPoints = [(x1,y1),(x2,y2),(x3,y3),(x4,y4)]
            print(localizationPoints)
    except Exception as err:
        print(err)


if __name__ == "__main__":

#you can change the following two variables' value to your own value.
    licenseKey = "Input your own license"
    fileName = r"Please input your own image path"

    InitLicense(licenseKey)
    image = cv2.imread(fileName)
    DecodeBuffer(fileName)