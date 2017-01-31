#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "igtlOSUtil.h"
#include "igtl_util.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
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
  //------------------------------------------------------------
  // Prepare server socket
  igtl::ServerSocket::Pointer serverSocket;
  serverSocket = igtl::ServerSocket::New();
  int r = serverSocket->CreateServer(18944);
  
  if (r < 0)
  {
    std::cerr << "Cannot create a server socket." << std::endl;
    exit(0);
  }
  
  
  igtl::Socket::Pointer socket;
  
  while (1)
  {
    //------------------------------------------------------------
    // Waiting for Connection
    socket = serverSocket->WaitForConnection(1000);
    
    if (socket.IsNotNull()) // if client connected
    {
      //------------------------------------------------------------
      // loop
      for (int i = 0; i < 100; i ++)
      {
        cv::Mat frame;
        cap >> frame;
        cv::Mat yuvImg;
        cv::cvtColor(frame, yuvImg, CV_BGR2RGB); // initialize the yuvImg.data, otherwize will be null
        //------------------------------------------------------------
        // size parameters
        int   size[]     = {frame.cols, frame.rows, 1};       // image dimension
        float spacing[]  = {1.0, 1.0, 1.0};     // spacing (mm/pixel)
        int   svsize[]   = {frame.rows, frame.cols, 1};       // sub-volume size
        int   svoffset[] = {0, 0, 0};           // sub-volume offset
        int   scalarType = igtl::ImageMessage::TYPE_UINT8;// scalar type
        
        //------------------------------------------------------------
        // Create a new IMAGE type message
        int endian = igtl::ImageMessage::ENDIAN_BIG;
        if (igtl_is_little_endian())
        {
          endian = igtl::ImageMessage::ENDIAN_LITTLE;
        }
        igtl::ImageMessage::Pointer imgMsg = igtl::ImageMessage::New();
        imgMsg->SetNumComponents(3);
        imgMsg->SetDimensions(size);
        imgMsg->SetSpacing(spacing);
        imgMsg->SetEndian(endian);
        imgMsg->SetScalarType(scalarType);
        imgMsg->SetDeviceName("ImagerClient");
        imgMsg->SetSubVolume(svsize, svoffset);
        imgMsg->AllocateScalars();
        
        //------------------------------------------------------------
        // Set image data (See GetTestImage() bellow for the details)
        memcpy(imgMsg->GetScalarPointer(),yuvImg.data, imgMsg->GetImageSize());
        
        //------------------------------------------------------------
        // Get randome orientation matrix and set it.
        igtl::Matrix4x4 matrix;
        igtl::IdentityMatrix(matrix);
        imgMsg->SetMatrix(matrix);
        
        //------------------------------------------------------------
        // Pack (serialize) and send
        imgMsg->Pack();
        socket->Send(imgMsg->GetPackPointer(), imgMsg->GetPackSize());
        
        igtl::Sleep(200); // wait
      }
    }
  }
  
  //------------------------------------------------------------
  // Close connection (The example code never reachs to this section ...)
  
  socket->CloseSocket();
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