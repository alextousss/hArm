#include "ouSontLesCubes.hpp"

using namespace cv;
using namespace std;

TrouveurDeCubes::TrouveurDeCubes() {
    mDico = aruco::getPredefinedDictionary(aruco::DICT_ARUCO_ORIGINAL);
    //mTrackerAlgorithm = TrackerCSRT::create();

}

vector<Cube> TrouveurDeCubes::ouSontLesCubes(Mat &  frame, Mat & drawFrame) {
    vector<Cube> detected;
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners;
    cv::aruco::detectMarkers(frame, mDico, corners, ids);

    vector<int> goodIDS;
    goodIDS.push_back(236);
    goodIDS.push_back(907);
    goodIDS.push_back(290);


    // Si au moins un des marqueurs est détecté
    if (ids.size() > 0)
        cv::aruco::drawDetectedMarkers(drawFrame, corners, ids);
    for (unsigned int i = 0; i < corners.size(); i++) {
        for(unsigned int j = 0; j < goodIDS.size(); j++) {
            if(ids[i] == goodIDS[j]) {
                detected.push_back({ids[i], {midpoint(corners[i][0], corners[i][2])}});
            }
        }
    }

    #define MARGIN 16
    #define RADIUS 16

    for(unsigned int i=0; i < detected.size(); i++) {
        Rect2d roi(Point(detected[i].position.x-RADIUS-MARGIN/2, detected[i].position.y-RADIUS-MARGIN/2), Size(RADIUS*2+MARGIN, RADIUS*2+MARGIN));
        mTrackers[detected[i].id] = TrackerCSRT::create();
        mTrackers[detected[i].id]->init(frame,roi);
    }

    vector<Cube> output;

    for ( map<int, Ptr<Tracker>>::iterator it = mTrackers.begin(); it != mTrackers.end(); it++ ) {
        Rect2d roi;
        it->second->update(frame, roi);
        if(roi.y > 320)
            output.push_back({it->first, (roi.br() + roi.tl())*0.5});
        rectangle( drawFrame, roi, Scalar( 255, 0, 0 ), 2, 1 );
    }
    
    return output;
}
