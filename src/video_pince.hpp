#ifndef VIDEO_PINCE
#define VIDEO_PINCE

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

struct Thresholds
{
    int buttonHSV = 1;
    int iLowH = 94;    int iHighH = 150;
    int iLowS = 150;    int iHighS = 255;
    int iLowV = 0;    int iHighV = 255;
    int thresh = 100;    //int max_thresh = 255;
};

void hello_world();
std::vector<cv::Point2f> thresh_callback(int, void* , cv::Mat src, int thresh, cv::Mat output);
void affichage();

void init_control_panel(Thresholds *src);
std::vector<cv::Point2f> color_detection(cv::Mat imgOriginal, Thresholds test);

class Video_pince
{
    public:
};


#endif
