#include "video_pince.hpp"

using namespace std;
using namespace cv;

void hello_world()
{
    cout << "Hello world" << endl;
}

void affichage()
{
}

void myFuncPanel(int value, void *userdata)
{
    // Thresholds pushed_threshold = *((Thresholds*)&userdata);
}

void init_control_panel(Thresholds *src)
{
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    //cvCreate trackbars in "Control" window
    cvCreateTrackbar("Canny thresh:", "Control", &src->thresh, 255);
    cvCreateTrackbar("  RGB / HSB    ", "Control", &src->buttonHSV, 1);
    cvCreateTrackbar("Low Hue/Blue   ", "Control", &src->iLowH, 255); //Hue (0 - 179)
    cvCreateTrackbar("High Hue/Blue  ", "Control", &src->iHighH, 255);
    cvCreateTrackbar("Low Sat/Green  ", "Control", &src->iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("High Sat/Green ", "Control", &src->iHighS, 255);
    cvCreateTrackbar("Low Bright/Red ", "Control", &src->iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("High Bright/Red", "Control", &src->iHighV, 255);
}

vector<cv::Point2f> color_detection(Mat imgOriginal, Thresholds test)
{
    Mat drawing, imgHSV, imgThresholded;
    ;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    if (test.buttonHSV == 0)
    {
        inRange(imgOriginal, Scalar(test.iLowH, test.iLowS, test.iLowV), Scalar(test.iHighH, test.iHighS, test.iHighV), imgThresholded); //Threshold the image
        //cout << "RGB = " << buttonHSV <<endl;
    }
    else
    {
        inRange(imgHSV, Scalar(test.iLowH, test.iLowS, test.iLowV), Scalar(test.iHighH, test.iHighS, test.iHighV), imgThresholded); //Threshold the image
        //cout << "HSV =" << buttonHSV <<endl;
    }

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //morphological closing (fill small holes in the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    namedWindow("Thresholded Image", CV_WINDOW_FREERATIO);
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Thresholded Image", imgThresholded);

    return thresh_callback(0, 0, imgThresholded, test.thresh, drawing);
}

vector<Point2f> thresh_callback(int, void *, Mat src, int thresh, Mat output)
{
    Mat canny_output, thresholded_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    //Canny( src, canny_output, thresh, thresh*2, 3 );
    //Detect edges using tresholded
    threshold(src, thresholded_output, thresh, 255, THRESH_BINARY);
    /// Find contours
    findContours(thresholded_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());

    for (unsigned int i = 0; i < contours.size(); i++)
    {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
        minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
    }

    /// Draw contours
    output = Mat::zeros(thresholded_output.size(), CV_8UC3);
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        Scalar contour_color = Scalar(255, 0, 0);
        drawContours(output, contours_poly, i, contour_color, 2, 8, hierarchy, 0, Point());
    }

    //for(unsigned int i = 0; i < center.size(); i++) {
        /*if(/*radius[i] < 3
        || center[i].x < 20 ||  src.cols - center[i].x < 20
        || center[i].y < 20 ||  src.rows - center[i].y < 20
        ) {
            center.erase(center.begin() + i);
            cout << "DELETE DUE TO RADIUS" << endl;
        }

    }*/
    cout << "size" << center.size() << endl;
    if (center.size() > 2) {
        bool continuer = true;
        for(unsigned int i = 0; i < center.size() && continuer; i++) {
            for(unsigned int j = 0; j < center.size(); j++) {
                if(abs(center[i].y - center[j].y) < 50 && abs(center[i].x - center[j].x) > 25) {

                    cout << "FOUND CENTER BY FILTRING" << endl;
                    Point center1 = center[i];
                    Point center2 = center[j];
                    center.clear();
                    center.push_back(center1);
                    center.push_back(center2);
                    continuer = false;
                    break;
                }
            }
        }
        
    }

    for(unsigned int i = 0; i < center.size(); i++)
        circle(output, center[i], 3, Scalar(0, 255, 0), 2, 8, 0);

    imshow("Contours", output);

    return center;

    /// Show in a window
}
