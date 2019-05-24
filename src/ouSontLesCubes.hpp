#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/tracking.hpp>

#include "hanoi.hpp"
#include "utils.hpp"

struct Cube {
    int id;
    cv::Point position;
};

class TrouveurDeCubes {
public:
    TrouveurDeCubes();
    std::vector<Cube> ouSontLesCubes(cv::Mat & frame, cv::Mat & drawFrame);
private:
    cv::Ptr<cv::aruco::Dictionary> mDico;
    std::map<int, cv::Ptr<cv::Tracker>> mTrackers;
};