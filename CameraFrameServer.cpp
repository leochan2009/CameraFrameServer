#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "api/svc/codec_api.h"
#include "api/svc/codec_def.h"
#include "api/svc/codec_app_def.h"
#include "utils/BufferedData.h"
#include "utils/FileInputStream.h"
#include "api/sha1.c"
#include "igtl_header.h"
#include "igtl_video.h"


#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
#include "igtlVideoMessage.h"
#include "igtlServerSocket.h"
#include "igtlMultiThreader.h"
#include "VideoStreamIGTLinkServer.h"


void* ThreadFunction(void* ptr);
int   SendVideoData(igtl::Socket::Pointer& socket, igtl::VideoMessage::Pointer& videoMsg);

typedef struct {
  cv::VideoCapture cap;
  int   nloop;
  igtl::MutexLock::Pointer glock;
  igtl::Socket::Pointer socket;
  int   interval;
  int   stop;
  bool  useCompression;
} ThreadData;

std::string     videoFile = "";

void CalculateFrameRate(cv::VideoCapture cap);

int main(int argc, char* argv[])
{
  //------------------------------------------------------------
  // Parse Arguments
  
  if (argc != 2) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <port>"    << std::endl;
    std::cerr << "    <port>     : Port # (18944 in Slicer default)"   << std::endl;
    exit(0);
  }
  
  int    port     = 18944;
  cv::VideoCapture cap;
  cap.open(0);
  //CalculateFrameRate(cap);
  //cap.release();
  if (!cap.isOpened())
  {
    cap.open(0);
  }
  cv::Mat frame;
  cap >> frame;
  cv::Mat yuvImg;
  cv::cvtColor(frame, yuvImg, CV_BGR2YUV_I420); // initialize the yuvImg.data, otherwize will be null
  char *configFile[]={"",argv[1]};
  VideoStreamIGTLinkServer* VideoStreamServer = new VideoStreamIGTLinkServer(2,configFile);
  VideoStreamServer->bConfigFile = true;
  VideoStreamServer->InitializeEncoder();
  VideoStreamServer->SetInputFramePointer(yuvImg.data);
  VideoStreamServer->StartServer(port);
  while(1)
  {
    if(!VideoStreamServer->stop)
    {
      cap >> frame;
      if(frame.empty()){
        std::cerr<<"frame is empty"<<std::endl;
        break;
      }
      else
      {
        cv::imshow("", frame);
        cv::waitKey(10);
        cv::cvtColor(frame, yuvImg, CV_BGR2YUV_I420);
        if (!VideoStreamServer->InitializationDone)
        {
          VideoStreamServer->InitializeEncoder();
        }
        int iEncFrames = VideoStreamServer->encodeSingleFrame();
        if (iEncFrames == cmResultSuccess)
        {
          VideoStreamServer->SendIGTLinkMessage();
        }
        int iFrameIdx =0;
        iFrameIdx++;
      }
    }
  }
  VideoStreamServer->~VideoStreamIGTLinkServer();
  return 1;
}

void CalculateFrameRate(cv::VideoCapture cap)
{
  double fps = cap.get(CV_CAP_PROP_FPS);
  // If you do not care about backward compatibility
  // You can use the following instead for OpenCV 3
  // double fps = video.get(CAP_PROP_FPS);
  std::cout << "Frames per second using video.get(CV_CAP_PROP_FPS) : " << fps << std::endl;
  // Number of frames to capture
  int num_frames = 60;
  
  // Start and end times
  time_t start, end;
  
  // Variable for storing video frames
  cv::Mat frame;
  
  std::cout << "Capturing " << num_frames << " frames" << std::endl ;
  
  // Start time
  time(&start);
  
  // Grab a few frames
  for(int i = 0; i < num_frames; i++)
  {
    cap >> frame;
  }
  
  // End Time
  time(&end);
  
  // Time elapsed
  double seconds = difftime (end, start);
  std::cout << "Time taken : " << seconds << " seconds" << std::endl;
  
  // Calculate frames per second
  fps  = num_frames / seconds;
  std::cout << "Estimated frames per second : " << fps << std::endl;
  
  // Release video
  cap.release();
}