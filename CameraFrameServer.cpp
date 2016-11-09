#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "VideoStreamIGTLinkServer.h"

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
  char *configFile[]={(char *)"",argv[1]};
  VideoStreamIGTLinkServer* VideoStreamServer = new VideoStreamIGTLinkServer(2, configFile);
  VideoStreamServer->InitializeEncoderAndServer();
  if(VideoStreamServer->GetInitializationStatus())
  {
    VideoStreamServer->SetWaitSTTCommand(false);
    VideoStreamServer->SetInputFramePointer(yuvImg.data);
    VideoStreamServer->StartServer();
    while(1)
    {
      if(VideoStreamServer->GetServerConnectStatus())
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
          if (!VideoStreamServer->GetInitializationStatus())
          {
            VideoStreamServer->InitializeEncoderAndServer();
          }
          int iEncFrames = VideoStreamServer->EncodeSingleFrame();
          if (iEncFrames == cmResultSuccess)
          {
            int frameType = VideoStreamServer->GetVideoFrameType();
            VideoStreamServer->SendIGTLinkMessage();
          }
          int iFrameIdx =0;
          iFrameIdx++;
        }
      }
    }
  }
  else
  {
    std::cerr<<"Initialization of encoder or server failed!"<<std::endl;
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