import numpy as np
import cv2
import time
import os
import ext

def rescale_frame(frame, percent=75):
    width = int(frame.shape[1] * percent/ 100)
    height = int(frame.shape[0] * percent/ 100)
    dim = (width, height)
    return cv2.resize(frame, dim, interpolation =cv2.INTER_AREA)

def pos_bottom_left(frame, grid=29):
    height = int(frame.shape[0])
    dim = (10, int(height * (grid/30)))
    return dim

def face_detect():
    # Find OpenCV version
    (major_ver, minor_ver, subminor_ver) = (cv2.__version__).split('.')
    print('OpenCV Major version: ', major_ver)
    print('OpenCV Minor version: ', minor_ver)
    print('OpenCV Subminor version: ', subminor_ver)
    #https://github.com/Itseez/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml
    face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

    cap = cv2.VideoCapture(0)

    while 1:
        # fps calculation from https://www.learnopencv.com/how-to-find-frame-rate-or-frames-per-second-fps-in-opencv-python-cpp/
        if int(major_ver)  < 3 :
            fps = cap.get(cv2.cv.CV_CAP_PROP_FPS)
        else :
            fps = cap.get(cv2.CAP_PROP_FPS)

        ret, frame = cap.read()
        img = rescale_frame(frame, percent=50)

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)

        bColor = (255,0,0)
        gColor = (0,255,0)
        rColor = (0,0,255)
        for (x,y,w,h) in faces:
            if (len(faces)%3 == 0):
                cv2.rectangle(img,(x,y),(x+w,y+h),rColor,2)
            if (len(faces)%3 == 1):
                cv2.rectangle(img,(x,y),(x+w,y+h),gColor,2)
            if (len(faces)%3 == 2):
                cv2.rectangle(img,(x,y),(x+w,y+h),bColor,2)

        #print("Estimated FPS : ", format(fps));
        cv2.putText(img, "Video Capture FPS : " + format(fps), pos_bottom_left(img), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 2, 8)
        #Get CPU load
        cpu_load = ext.cpu_mac()
        mem_load = ext.mem_mac()
        cv2.putText(img, "CPU: " + format(cpu_load) + "%", pos_bottom_left(img, 27), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 2, 8)
        cv2.putText(img, "Memory: " + format(mem_load) + "%", pos_bottom_left(img, 25), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 2, 8)

        cv2.imshow('Detected face',img)
        k = cv2.waitKey(30) & 0xff
        if (k == 27) or (k == 13):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__': face_detect()
