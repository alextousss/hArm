#include <vector>
#include <chrono> 

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "vaVersLesCubes.hpp"
#include "ouSontLesCubes.hpp"
#include "hanoi.hpp"


using namespace std::chrono; 
using namespace cv;
using namespace std;




int main(int argc, char **argv) {
    BougeurDeCubes bougeur;



    // Contient les coups à jouer, de telle colonne à telle colonne
    vector<Coup> coups;


    coups.push_back({0, 1});
    coups.push_back({0, 2});
    coups.push_back({0, 1});
    cout << "before incap" << endl;
    cv::VideoCapture inCap;
    cout << "Open capture ... ";
    inCap.open(0);
    cout << "done" << endl;

    cout << "Set FourCC ... ";
    inCap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'P', 'E', 'G'));
    cout << "done" << endl;

    cout << "Set max FPS ... ";
    inCap.set(CV_CAP_PROP_FPS, 30);
    cout << "done" << endl;

    cout << "Set width ... ";
    inCap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cout << "done" << endl;

    cout << "Set height ... ";
    inCap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cout << "done" << endl;


    bool firstIteration = true;

    Mat frame, drawFrame;
    for(;;)  {
        inCap >> frame;
        if(frame.empty()) break;
        frame.copyTo(drawFrame);


        if (firstIteration) {
            bougeur.programmerCoup(coups[coups.size() - 1]);
            coups.pop_back();
        }
        bougeur.update(frame, drawFrame);

        if(bougeur.aFini()) {
            bougeur.programmerCoup(coups[coups.size() - 1]);
            coups.pop_back();
        }


        cv::imshow("vidoooooo", drawFrame);
        char key = (char) cv::waitKey(1);
        if (key == 27)
            break;

        firstIteration = false;
    }

    inCap.release();
}