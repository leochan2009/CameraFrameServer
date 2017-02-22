CameraFrameServer is used to send video stream data to 3D Slicer. The OpenIGTLinkIF module is able to decode the video stream and reconstruct the video. The project is depending on OpenIGTLink protocol [OpenIGTLink Web Page](http://openigtlink.org/) and OpenH264 library [OpenH264 Web Page](http://www.openh264.org/) and  OpenCV(http://opencv.org/).

=======================
Build Instruction
-----------------
Build OpenIGTLink and Install OpenCV.

OpenIGTLink: https://github.com/leochan2009/OpenIGTLink branch: VideoStreamMerge 

Install a stable OpenCV from http://opencv.org/

OpenH264 is built within OpenIGTLink, when build OpenIGTLink, turn on the USE_H264 option.
For successful build of OpenH264, NASM needs to be installed for assembly code: workable version 2.10.06 or above, nasm can downloaded from http://www.nasm.us/ For Mac OSX 64-bit NASM needed to be below version 2.11.08 as nasm 2.11.08 will introduce error when using RIP-relative addresses in Mac OSX 64-bit.
If you have another NASM version installed, try to unlink the wrong version of NASM, and link to the version to below 2.11.08.
After check the nasm version with comman "which nasm" and "nasm -v".

## Linux / Mac OS X
First, obtain the source code from the repository using Git. To simply download the code, run the following command from a terminal:

    $ git clone https://github.com/leochan2009/CameraFrameServer.git

Then configure using CMake. The library requires CMake version higher than 2.8.

    $ mkdir CameraFrameServer-build
    $ cd CameraFrameServer-build
    $ ccmake ../CameraFrameServer
Set the directories for OpenIGTLink_DIR and OpenCV_DIR    
    $ make
    
## Windows
* Build OpenIGTLink with USE_H264 option turned on. 
  * At configuration, the source code for OpenH264 will be download, 
  * Build OpenH264, the source file is located at: C:\Devel\CameraFrameServer-build\OpenH264 Check the build step for OpenH264 at this web page [OpenH264 Web Page](http://www.openh264.org/)  
* Download the CameraFrameServer source code from Git repository.
  * URL of repository: https://github.com/leochan2009/CameraFrameServer.git
* Run CMake
  * Where is the source code: C:\Devel\CameraFrameServer
  * Where to build the binaries: C:\Devel\CameraFrameServer-build
  * Click "Configure" and select your compiler (usually just click "OK")
  * Message: "Build directory does not exit, should I create it?" - click "OK"
  * Set the directories for OpenIGTLink_DIR and OpenCV_DIR
  * Click "Configure"
  * close this CMake
* Start Visual C and compile the project (C:\Devel\CameraFrameServer-build\CameraFrameServer.sln)

VideoStreaming Configuration
-------
In the source code folder, there exist two files: ServerConfig.cfg and layer.cfg
The ServerConfig.cfg file configures the H264 codec and OpenIGTLink port number and data transmission variables
The layer.cfg file configures the layers of H264 codec.

### The parameters that are compulsory to be set carefully are:
* ServerConfig.cfg
  * TransportMethod -  This defines the UDP or TCP transportation (# 1 for UDP, 0 for TCP. It should be set to 0 for 3D Slicer modules.)
  * ServerPortNumber - The port number for TCP communication (18944 is the default slicer communication port)
  * SourceWidth   -    input video width
  * SourceHeight   -   input video height
  * LayerCfg    -      Layer configuration file, this parameter should point to the correct layer configuration file.
* Layer.cfg
  * FrameWidth   -     Output  frame width
  * FrameHeight   -    Output  frame height
  
### Optional parameters
If the TransportMethod is set to 1, then the following parameter needs to be set
* ServerConfig.cfg
  * ClientIPAddress  -  The IP of the client, server sends the UDP packet to the client with establish the connection.
  * ClientPortNumber -  Client port number should be different than the server port number.
  * NetWorkBandWidth -  In kbps/s, a parameter indicates the current network bandwidth
  * RCMode      -       0: quality mode;  1: bitrate mode;  # 2: buffer based mode,can't control bitrate; -1: rc off mode;
  * TargetBitrate	-   Unit: kbps, controlled by RCMode.
  
License
-------
The code is distributed as open source under [the new BSD license](http://www.opensource.org/licenses/bsd-license.php).

Acknowledgement
-------
* The project is based on the work of OpenIGTLink, OpenCV and OpenH264 Communities.
* Documentation is based on the README.md file from OpenIGTLink.
