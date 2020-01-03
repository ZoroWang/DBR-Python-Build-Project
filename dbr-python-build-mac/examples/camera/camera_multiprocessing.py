import cv2
from dbr import DynamsoftBarcodeReader
dbr = DynamsoftBarcodeReader()
import time
import os
from multiprocessing import Process, Queue

import sys
sys.path.append('../')
# import config


def clear_queue(queue):
    try:
        while True:
            queue.get_nowait()
    except:
        pass
    queue.close()
    queue.join_thread()


def dbr_run(frame_queue, finish_queue):
    dbr.InitLicense("Input your own license")
    while finish_queue.qsize() == 0:
        try:
            inputframe = frame_queue.get_nowait()
            
            results = dbr.DecodeBuffer(inputframe, inputframe.shape[0], inputframe.shape[1], inputframe.strides[0])
            # textResults is a list object, the following program will output each whole text result.
            # if you just want some individual results in textResult, you can get all keys in text result and get the value by the key.
            textResults = results["TextResults"]
            intermediateResults = results["IntermediateResults"]
            for textResult in textResults:
                # print(textResult["BarcodeFormat"])
                print(textResult["BarcodeFormatString"])
                print(textResult["BarcodeText"])
                # print(textResult["BarcodeBytes"])
                # # LocalizationResult is a dictionary object, you can use the same method as textResult to get the key-value. 
                # print(textResult["LocalizationResult"])
                # # DetailedResult is a dictionary object, you can use the same method as textResult to get the key-value. 
                # print(textResult["DetailedResult"])
                # # ExtendedResults is a list object , and each item of it is a dictionary object.
                # extendedResults = textResult["ExtendedResults"]
                # for extendedResult in extendedResults:
                #     print(extendedResult)
            # intermediateResults is a list object, the following program will output each whole intermediate result.
            # if you just want some individual results in intermediateResult, you can get all keys in intermediateResult and get the value by the key.
            # for intermediateResult in intermediateResults:
            #     print(intermediateResult)
            #     print(intermediateResult.keys())
        except:
            pass

    print("Detection is done.")
    clear_queue(frame_queue)
    clear_queue(finish_queue)


def get_time():
    localtime = time.localtime()
    capturetime = time.strftime("%Y%m%d%H%M%S", localtime)
    return capturetime


def read_barcode():
    frame_queue = Queue(4)
    finish_queue = Queue(1)

    dbr_proc = Process(target=dbr_run, args=(
        frame_queue, finish_queue))
    dbr_proc.start()

    vc = cv2.VideoCapture(0)
    # vc.set(5, 30)  #set FPS
    vc.set(3, 640) #set width
    vc.set(4, 480) #set height

    if vc.isOpened():  # try to get the first frame
        rval, frame = vc.read()
    else:
        return

    windowName = "Barcode Reader"
    base = 2
    count = 0
    while True:
        cv2.imshow(windowName, frame)
        rval, frame = vc.read()

        count %= base
        if count == 0:
            try:
                frame_queue.put_nowait(frame)
            except:
                try:
                    while True:
                        frame_queue.get_nowait()
                except:
                    pass

        count += 1

        # 'ESC' for quit
        key = cv2.waitKey(20)
        if key == 27:
            finish_queue.put(True)

            dbr_proc.join()
            break

    cv2.destroyWindow(windowName)


if __name__ == "__main__":
    print("OpenCV version: " + cv2.__version__)
    read_barcode()
