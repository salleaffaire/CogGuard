/*
 * co_main.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: luc.martel
 */

#include <iostream>
#include <string.h>

// OpenCV
#include <cv.h>
#include <highgui.h>
using namespace cv;
#include "cg_opencv_if.hpp"


// A Web Server
#include "cg.hpp"

// Image class
#include "cg_image.hpp"

// Algorithms
#include "cg_algorithm_bg.hpp"
#include "cg_canny.hpp"

// Media streaming classes
#include "cg_media_stream_memory.hpp"
#include "cg_media_stream_producer_identity.hpp"
#include "cg_media_segment.hpp"

#include "cg_profile.hpp"
#include "cg_test.hpp"

// *******************************
// Add -l curldll
// *******************************
//#include <curl/curl.h>

// *******************************
// Using FFMPEG
// *******************************


#define CG_START_VIDEO_SOCKET 1
#define CG_VIDEO_SOCKET_PORT  8901

#define CG_WRITE_BACK         0

// *******************************
// Callbacks
// *******************************
void on_trackbar( int, void* )
{
}

int
main(int argc, char *argv[])
{
   cg_configuration cg_config;
   cg_config.mVideoSocket = (CG_START_VIDEO_SOCKET == 1) ? true : false;
   cg_config.mVideoSocketPort = CG_VIDEO_SOCKET_PORT;

   // cg_init(&cg_config);

   // TestDeque();


// Test task
#if 0
   cg_test_task aTestTask;
   aTestTask.start();

   char key;
   std::cin >> key;

   aTestTask.stop();
   aTestTask.wait();

#endif


// Test Media Streams
#if 0
   // Test stream file
   std::string filename_in = "test_stream.dat";
   std::string filename_out = "test_stream_out.dat";
   // Media Stream (Input)
   cg_media_stream_in_memory stream_in(filename_in);
   cg_media_stream_out_memory stream_out(filename_out, 12);

   // Create an identity stream producer
   cg_media_stream_producer_identity stream_producer;

   // Configure stream producer
   stream_producer.set_input_stream(&stream_in, 0);
   stream_producer.set_output_stream(&stream_out);

   stream_producer.run();

   stream_out.close();
#endif


   // Test Algorithm classes
   // -----------------------------------------------------------------------------
   CCanny<unsigned char> _cannyOperator;

   // Real-time algorithm
   // -----------------------------------------------------------------------------
#if 1

   const int alpha_slider_max = 256;
   int       alpha_slider     = 6;

   VideoCapture cap(0);
   std::cout << "Capturing from camera 0 ...\n";

   if (!cap.isOpened()) return -1;


   int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
   // Transform from int to char via Bitwise operators
   char EXT[] = {(char)((ex & 0XFF) >> 0),
                 (char)((ex & 0XFF00) >> 8),
                 (char)((ex & 0XFF0000) >> 16),
                 (char)((ex & 0XFF000000) >> 24),
                 0};
   std::cout << "FOUR CC code = " << EXT << std::endl;

   // Get Input Video Size
   Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
                 (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

   std::cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
             << " of nr#: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

   int fps = cap.get(CV_CAP_PROP_FPS);
   std::cout << "Input frame FPS: " << fps << " f/s" << std::endl;

#if (CG_WRITE_BACK == 1)
   std::string outputfilename = "video.avi";

   VideoWriter outputVideo;
   outputVideo.open(outputfilename.c_str(), ex=-1, 30, S, true);

   if (!outputVideo.isOpened())
   {
       std::cout  << "Could not open the output video for write: " << outputfilename << std::endl;
       return -1;
   }
#endif

   // Create windows
   namedWindow("Input", 1);
   namedWindow("Output", 1);
   namedWindow("Debug", 1);

   /// Create Trackbars
   char TrackbarName[50];
   sprintf(TrackbarName, "Alpha");

   createTrackbar(TrackbarName, "Output", &alpha_slider, alpha_slider_max, on_trackbar);

   // OpenCV Structures
   // -------------------------------------------------------------
   // INPUT
   Mat         src, dst, deb;
   vector<Mat> spl;

   // OUTPUT
   dst.create(S.height, S.width, CV_8UC1);
   deb.create(S.height, S.width, CV_8UC1);

   // CogniGuard Structures
   // -------------------------------------------------------------
   // INPUT
   std::vector<cg_image<unsigned char> *> in_image_vec(3);
   in_image_vec[0] = cg_create_raw_8u(S.width, S.height);
   in_image_vec[1] = cg_create_raw_8u(S.width, S.height);
   in_image_vec[2] = cg_create_raw_8u(S.width, S.height);

   // PROCESSED INPUT
   // Storing the processed source here
   cg_image<unsigned char> *in_source = cg_create_raw_8u(S.width, S.height);

   // OUTPUT
   // Absolute difference between bg and current image
   cg_image<unsigned char> *diff_image = cg_create_raw_8u(S.width, S.height);
   // -------------------------------------------------------------

   // CogniGuard Algorithms
   // -------------------------------------------------------------
   // Create algorithms and configure
   cg_future_algorithm_background_estimation *bg = new cg_future_algorithm_background_estimation(S.width, S.height);
   bg->set_alpha(256);

   // Create and initialize a profiler
   cg_profiler pr;

   // Forever
   for (unsigned int iteration;;++iteration)
   {
      // Capture to src
      cap >> src;

      // process - extract only the correct channel
      split(src, spl);

      // Attaching to the OpenCV buffers
      //in_image_r->get_plane(0).attach(src.rows*src.cols, src.data);
      //in_image_g->get_plane(0).attach(src.rows*src.cols, src.data);
      //in_image_b->get_plane(0).attach(src.rows*src.cols, src.data);
      cg_attach(in_image_vec[0], &spl[0], 0);
      cg_attach(in_image_vec[1], &spl[1], 0);
      cg_attach(in_image_vec[2], &spl[2], 0);

      // Fliter source image
      // -------------------------------------------------------------------
      cg_o1_filter_process_image(*(in_image_vec[1]),
                                 *in_source,
                                 *[](unsigned char *in[]) -> unsigned char {
                                    unsigned int t = 0;
                                    t += (1**(in[0]+0) + 2**(in[0]+1) + 1**(in[0]+2));
                                    t += (2**(in[1]+0) + 4**(in[1]+1) + 2**(in[1]+2));
                                    t += (1**(in[2]+0) + 2**(in[2]+1) + 1**(in[2]+2));

                                    // Divided by 16
                                    return (unsigned char)(t >> 4);
      });

      // -------------------------------------------------------------------
      cg_copy(&deb, in_source, 0);

      // -------------------------------------------------------------------

      // Run the algorithm
      // -------------------------------------------------------------------
      pr.start();
      bg->run(&in_image_vec);
      bg->wait();
      bg->set_alpha(alpha_slider);

      // Use the GREEN channel - less noisy
      cg_pixel_process_image(*(in_image_vec[1]),
                             *(bg->get_bg()),
                             *(diff_image),
                             *[](unsigned char in1, unsigned char in2) -> unsigned char {
                                int t = in1 - in2;
                                t = (t < 0) ? -t : t;
                                t = (t > 64) ? 255 : 0;
                                return (unsigned char)(t);
      });
      unsigned int pr_time = pr.stop();
      std::cout << "Processing time = " << pr_time << "  seconds." << std::endl;
      // -------------------------------------------------------------------


      if ((iteration % 8) == 0)
      {
         // std::cout << "Copy output" << std::endl;
         //cg_copy(&dst, bg->get_bg(), 0);
         cg_copy(&dst, diff_image, 0);
      }

      // Do nothing
      //for(int i = 0; i < 3; ++i)
      //if (i != 0)
      //   spl[i] = Mat::zeros(S, spl[0].type());
      //merge(spl, res);

      // Show
      imshow("Input",  src);
      imshow("Output", dst);
      imshow("Debug",  deb);

#if (CG_WRITE_BACK == 1)
      outputVideo << dst;
#endif

      if (waitKey(30) >= 0) break;
   }

   bg->stop();

   delete bg;

   delete diff_image;

   delete in_source;

   delete in_image_vec[0];
   delete in_image_vec[1];
   delete in_image_vec[2];

#endif

   // Test image classes
#if 1
   //cg_image<unsigned char> in_image_0 = cg_create_raw_8u(640, 480);
   //cg_image<unsigned char> in_image_1 = cg_create_raw_8u(641, 480);
   //cg_image<unsigned char> in_image_2 = cg_create_raw_8u(642, 480);
   //cg_image<unsigned char> in_image_3 = cg_create_raw_8u(643, 480);

   //cg_alg_background_extimation bg_estimator;

   //unsigned char key;
   //std::cin >> key;

   // std::cout << in_image.get_width() << std::endl;
   // std::cout << (unsigned int)&in_image << std::endl;

#endif

   // Test curl
#if 0
   CURL *curl;
   CURLcode res;

   curl = curl_easy_init();
   curl_easy_cleanup(curl);
#endif

   std::cout << "CG Done !!" << std::endl;

   return 0;
}

