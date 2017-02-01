#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vp9/common/vp9_common.h"

#include "tools_common.h"
#include "video_writer.h"

std::string     videoFile = "";

void CalculateFrameRate(cv::VideoCapture cap);
void usage_exit(void) {};

static int encode_frame(vpx_codec_ctx_t *codec, vpx_image_t *img,
                        int frame_index, int flags, VpxVideoWriter *writer) {
  int got_pkts = 0;
  vpx_codec_iter_t iter = NULL;
  const vpx_codec_cx_pkt_t *pkt = NULL;
  const vpx_codec_err_t res =
  vpx_codec_encode(codec, img, frame_index, 1, flags, VPX_DL_GOOD_QUALITY);
  if (res != VPX_CODEC_OK) die_codec(codec, "Failed to encode frame");
  pkt = vpx_codec_get_cx_data(codec, &iter);
  while (pkt != NULL) {
    got_pkts = 1;
    
    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      const int keyframe = (pkt->data.frame.flags & VPX_FRAME_IS_KEY) != 0;
      printf(keyframe ? "K" : ".");
      fflush(stdout);
    }
  }
  
  return got_pkts;
}

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
  vpx_codec_ctx_t codec;
  vpx_codec_enc_cfg_t cfg;
  vpx_image_t raw;
  vpx_codec_err_t res;
  VpxVideoInfo info;
  VpxVideoWriter *writer = NULL;
  const VpxInterface *encoder = NULL;
  const int fps = 30;
  vp9_zero(info);
  encoder = get_vpx_encoder_by_name("vp9");
  if (!encoder) die("Unsupported codec.");
  
  info.codec_fourcc = encoder->fourcc;
  info.frame_width = frame.cols;
  info.frame_height = frame.rows;
  info.time_base.numerator = 1;
  info.time_base.denominator = fps;
  /*if (info.frame_width <= 0 || info.frame_height <= 0 ||
      (info.frame_width % 2) != 0 || (info.frame_height % 2) != 0) {
    die("Invalid frame size: %dx%d", info.frame_width, info.frame_height);
  }*/
  
  if (!vpx_img_alloc(&raw, VPX_IMG_FMT_I420, info.frame_width,
                     info.frame_height, 1)) {
    std::cerr<<"Failed to allocate image.";
  }
  
  printf("Using %s\n", vpx_codec_iface_name(encoder->codec_interface()));
  
  res = vpx_codec_enc_config_default(encoder->codec_interface(), &cfg, 0);
  if (res) die_codec(&codec, "Failed to get default codec config.");
  
  cfg.g_w = info.frame_width;
  cfg.g_h = info.frame_height;
  cfg.g_timebase.num = info.time_base.numerator;
  cfg.g_timebase.den = info.time_base.denominator;
  cfg.g_lag_in_frames = 0;
  
  //writer = vpx_video_writer_open(argv[4], kContainerIVF, &info);
  //if (!writer) die("Failed to open %s for writing.", argv[4]);
  
  //if (!(infile = fopen(argv[3], "rb")))
    //die("Failed to open %s for reading.", argv[3]);
  
  if (vpx_codec_enc_init(&codec, encoder->codec_interface(), &cfg, 0))
    std::cerr<<"Failed to initialize encoder.";//die_codec(&codec, "Failed to initialize encoder");
  
  if (vpx_codec_control_(&codec, VP9E_SET_LOSSLESS, 1))
    std::cerr<<"Failed to use lossless mode";//die_codec(&codec, "Failed to use lossless mode");
  
  int iFrameIdx = 0;
  while(1)
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
      memcpy(raw.planes[0],yuvImg.data, info.frame_width*info.frame_height);
      memcpy(raw.planes[1],yuvImg.data+info.frame_width*info.frame_height, info.frame_width*info.frame_height/4);
      memcpy(raw.planes[2],yuvImg.data+info.frame_width*info.frame_height/4*5, info.frame_width*info.frame_height/4);
      encode_frame(&codec, &raw, iFrameIdx, 0, writer);
      
      iFrameIdx++;
    }
  }
  // Flush encoder.
  while (encode_frame(&codec, NULL, -1, 0, writer)) {
  }
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