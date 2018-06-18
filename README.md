# Face Detection with Haar Cascade in Python Using OpenCV

This program detects faces from a video input (webcam) and then shows the video output with the estimated fps (frames per second). The program do require OpenCV package for Python to be installed.

Use this package to encrypt messages and embed in QR code, and decode the message back.

#### Build for Linux and OSX:
[![Build Status](https://travis-ci.org/somdipdey/Encrypted_QR_Code.svg?branch=master)](https://travis-ci.org/somdipdey/Encrypted_QR_Code)
[![License: MIT](https://img.shields.io/badge/License-MIT-red.svg)](https://github.com/somdipdey/Encrypted_QR_Code/blob/master/LICENSE)

## Dependencies For MAC OS

	$ [sudo] pip install opencv-python
  $ [sudo] pip install matplotlib
	$ [sudo] pip install numpy

## Dependencies For Ubuntu, Debian

	$ [sudo] apt-get install libopencv-dev python-opencv
  $ [sudo] apt-get install python3-matplotlib
	$ [sudo] apt-get install python-numpy

#### Note:

For other dependencies based on a particular OS please refer to that OS's install command for the aforementioned packages.

## Installation For MAC OS, Ubuntu, Debian

### Step 1:
  	$ git clone https://github.com/somdipdey/FaceDetection_with_HaarCascade_in_Python_OpenCV.git
  	$ cd FaceDetection_with_HaarCascade_in_Python_OpenCV
  	$ [sudo] python setup.py build
    $ [sudo] python setup.py install

### Alternate Step To Install This Program
    $ pip install git+https://github.com/somdipdey/FaceDetection_with_HaarCascade_in_Python_OpenCV.git
    $ [sudo] python setup.py build
    $ [sudo] python setup.py install

#### Note:

You need to use the setup.py build and install commands to build the C extension program to be used in your python program.

## Execute/Run The Program
    $ python detect.py

#### Note:

You can pass extra parameter as True in order to write out the FPS, Compute_FPS, CPU_LOAD, MEMORY_LOAD to an audit.txt file as comma separated values. Example:

    $ python detect.py True

The above command will write the FPS, Compute_FPS, CPU_LOAD, MEMORY_LOAD to an audit.txt in the same directory as the program.

Here, FPS is the Frames Per Second of the Video Capture Input (webcam), whereas CPU_LOAD is the Frames Per Second of the computed program after several compute operations are performed. CPU_LOAD is the current load of the CPU in Percentage and MEMORY_LOAD is the current Memory (RAM) load in percentage.

## Result/Output

<img width="350" alt="Face Detection Using OpenCV and Resource Monitoring" src="https://user-images.githubusercontent.com/8515608/41542192-4a3686c6-730c-11e8-9895-2f56a91d8af2.png">
