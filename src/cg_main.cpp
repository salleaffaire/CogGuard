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
#include "cg_media_segment.hpp"

#include "cg_profile.hpp"
#include "cg_test.hpp"

// *******************************
// Add -l curldll
// *******************************
//#include <curl/curl.h>

#define CG_START_VIDEO_SOCKET 1
#define CG_VIDEO_SOCKET_PORT  8901

int
main(int argc, char *argv[])
{
   cg_configuration cg_config;
   cg_config.mVideoSocket = (CG_START_VIDEO_SOCKET == 1) ? true : false;
   cg_config.mVideoSocketPort = CG_VIDEO_SOCKET_PORT;

   // cg_init(&cg_config);

   // TestDeque();

   // Create and initialize a profiler
   cg_profiler pr;


// Test task
#if 0
   cg_test_task aTestTask;
   aTestTask.start();

   char key;
   std::cin >> key;

   aTestTask.stop();
   aTestTask.wait();

#endif

   CCanny<unsigned char> _cannyOperator;

#if 1

   VideoCapture cap(0);
   std::cout << "Capturing from camera 0 ...\n";

   if (!cap.isOpened()) return -1;

   // Get Input Video Size
   Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
                 (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

   std::cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
             << " of nr#: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

   namedWindow("Input", 1);
   namedWindow("Output", 1);

   Mat         src, dst;
   vector<Mat> spl;

   // Inputs: 3 components
   std::vector<cg_image<unsigned char> *> in_image_vec(3);
   in_image_vec[0] = cg_create_raw_8u(S.width, S.height);
   in_image_vec[1] = cg_create_raw_8u(S.width, S.height);
   in_image_vec[2] = cg_create_raw_8u(S.width, S.height);

   // Absolute difference between bg and current image
   cg_image<unsigned char> *diff_image = cg_create_raw_8u(S.width, S.height);

   // Create algorithms and configure
   cg_future_algorithm_background_estimation *bg = new cg_future_algorithm_background_estimation(S.width, S.height);
   bg->set_alpha(256);

   // Create an output image
   dst.create(S.height, S.width, CV_8UC1);

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

      // Run the algorithm
      // -------------------------------------------------------------------
      pr.start();
      bg->run(&in_image_vec);
      bg->wait();
      bg->set_alpha(6);

      cg_pixel_process_image(*(in_image_vec[0]),
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
      imshow("Input", src);
      imshow("Output", dst);

      if (waitKey(30) >= 0) break;
   }

   bg->stop();

   delete bg;

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

