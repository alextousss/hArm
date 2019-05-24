#pragma once

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>

#include "ouSontLesCubes.hpp"
#include "hanoi.cpp"
#include "libowinew.hpp"
#include "video_pince.hpp"
#include "utils.hpp"


enum Etat { 
    STANDBY, 
    OUVERTURE, 
    MONTEE_1, 
    MOUVEMENT_HORIZONTAL_1, 
    DESCENTE_1, 
    FERMETURE, 
    MONTEE_2, 
    MOUVEMENT_HORIZONTAL_2, 
    DESCENTE_2 
};

void printEtat(Etat etat);


class BougeurDeCubes {
    
public:
    BougeurDeCubes();
    bool aFini();
    void programmerCoup(Coup mouvement);
    void update(cv::Mat frame, cv::Mat displayFrame);
    void remplirEtatDesColonnes(cv::Mat frame, cv::Mat displayFrame);

private:
    sf::Clock mLastStateChange;
    bool mFini;
    TrouveurDeCubes mTrouveur;
    Coup mCoupActuel;
    Etat mEtat;
    int mColonneActuelle;
    OwiCommander mCommander;
    // Tableau qui contient les colonnes. 
    // Les colonnes sont un tableau d'ID de cubes.
    // Le cube le plus en bas de la colonne a pour id 0
    std::vector<std::vector<Cube>> mContenuColonnes; 
    Thresholds mPinceThresholds;
};