#include "vaVersLesCubes.hpp"

using namespace cv;
using namespace std;

#define IMGWIDTH 640
#define IMGHEIGHT 480


#define YTOTOP (IMGHEIGHT / 1.7f)
#define PINCEOFFSET_Y (IMGHEIGHT/5)
#define PINCEOFFSET_X (IMGWIDTH/32)


bool compareTwoCubes(Cube a, Cube b) { 
    return (a.position.y > b.position.y); 
} 


float maxYOfColonnes(vector<vector<Cube>> colonnes) {
    float maxY = 0;
    for(unsigned int i = 0; i < colonnes.size(); i++) {
        for(unsigned int j = 0; j < colonnes[i].size(); j++) {
            if(colonnes[i][j].position.y > maxY)
                maxY = colonnes[i][j].position.y;
        }
    }
    return maxY;
}


float coordYDeTarget(Mat frame, int target) {
    switch(target) {
        case 0: return  0.30f * frame.cols; break;
        case 1: return 0.50f * frame.cols; break;
        case 2: return 0.70f * frame.cols; break;
        default:
            cout << "target must be between 0 and 2" << endl;
    }
}



void printEtat(Etat etat) {
    switch(etat) {
        case STANDBY: 
            cout << "STANDBY"; 
            break; 
        case OUVERTURE: 
            cout << "OUVERTURE"; 
            break; 
        case MONTEE_1: 
            cout << "MONTEE_1"; 
            break; 
        case MOUVEMENT_HORIZONTAL_1: 
            cout << "MOUVEMENT_HORIZONTAL_1"; 
            break; 
        case DESCENTE_1: 
            cout << "DESCENTE_1"; 
            break; 
        case FERMETURE: 
            cout << "FERMETURE"; 
            break; 
        case MONTEE_2: 
            cout << "MONTEE_2"; 
            break; 
        case MOUVEMENT_HORIZONTAL_2: 
            cout << "MOUVEMENT_HORIZONTAL_2"; 
            break; 
        case DESCENTE_2: 
            cout << "DESCENTE_2"; 
            break; 
    }
}

BougeurDeCubes::BougeurDeCubes() {
    mFini = false;
    mCoupActuel = {0, 0};
    mEtat = STANDBY;
    mColonneActuelle = 0;
    init_control_panel(&mPinceThresholds);
    for(unsigned int i = 0; i < 3; i++) {
        mContenuColonnes.push_back(vector<Cube>(0));
    }
    mLastStateChange.restart();
}

bool BougeurDeCubes::aFini() {
    return mFini;
}


void BougeurDeCubes::programmerCoup(Coup mouvement) {
    mFini = false;
    mCoupActuel = mouvement;
}



void BougeurDeCubes::remplirEtatDesColonnes(Mat frame, Mat displayFrame) {
    mContenuColonnes.clear();
    vector<Cube> cubes = mTrouveur.ouSontLesCubes(frame, displayFrame);
    for(unsigned int i = 0; i < 3; i++) {
        mContenuColonnes.push_back(vector<Cube>(0));
    }
    for(unsigned int i = 0; i < cubes.size(); i++) {
        if(cubes[i].position.x < float(frame.cols) * 0.33f) {
            mContenuColonnes[0].push_back(cubes[i]);
        } else if (cubes[i].position.x < float(frame.cols) * 0.66f) {
            mContenuColonnes[1].push_back(cubes[i]);
        } else {
            mContenuColonnes[2].push_back(cubes[i]);
        }
    }

    for(unsigned int i = 0; i < 3; i++) {
        sort(mContenuColonnes[i].begin(), mContenuColonnes[i].end(), compareTwoCubes);
    }

    for(unsigned int i = 0; i < mContenuColonnes.size(); i++) {
        cout << i << " -> ";
        for(unsigned int j = 0; j <mContenuColonnes[i].size(); j++) {
            cout << mContenuColonnes[i][j].id << "\t";
        }
        cout << endl;
    }

}

void BougeurDeCubes::update(Mat frame, Mat displayFrame) {
    line(displayFrame, Point(float(frame.cols) * 0.33f, 0), Point(float(frame.cols) * 0.33f, frame.rows), Scalar(255, 255, 255));
    line(displayFrame, Point(float(frame.cols) * 0.66f, 0), Point(float(frame.cols) * 0.66f, frame.rows), Scalar(255, 255, 255));

    printEtat(mEtat);
    cout << endl;
    if(mFini || mCoupActuel.to == mCoupActuel.from) { // Si on a fini notre mouvement actuel, stop it!
        return;
    }
    cout << "COUP ACTUEL : " << mCoupActuel.from << "->" << mCoupActuel.to << endl;
    int cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    vector<Point2f> pincePositions = color_detection(displayFrame, mPinceThresholds);
    if(pincePositions.size() != 2) {
        return;
    }

    remplirEtatDesColonnes(frame, displayFrame);


    Cube cible;
    bool cibleEstTo = 
        (mEtat == STANDBY || mEtat == OUVERTURE || mEtat == MONTEE_1 || mEtat == MOUVEMENT_HORIZONTAL_1 || mEtat == DESCENTE_1 || mEtat == FERMETURE);
    unsigned int target = cibleEstTo ? mCoupActuel.from : mCoupActuel.to;
    if(mContenuColonnes.size() && mContenuColonnes[target].size()) {
        cible =  cibleEstTo ? mContenuColonnes[target].back() : mContenuColonnes[target][0];
        if(!cibleEstTo) {
            cible.position.y = maxYOfColonnes(mContenuColonnes) - 50;
            // Si le seul qu'il y a dans la colonne c'est le cube en train d'être déplacé
            // Alors on utilise une valeur a la con
            if(mContenuColonnes[target].size() == 1)  {
                cible.position.x = coordYDeTarget(frame, target);
            }
        }
    }
    else {
        cible = {0, Point2f( 
            coordYDeTarget(frame, target),  
            maxYOfColonnes(mContenuColonnes)
        )};
    }
    circle(displayFrame, cible.position, 3, Scalar(255,0,255), 2 ,8 ,0 );

    Point pincePos = midpoint(pincePositions[0], pincePositions[1]);
    pincePos.y += PINCEOFFSET_Y;
    pincePos.x += PINCEOFFSET_X;

    circle(displayFrame, pincePos, 3, Scalar(255,0,255), 2 ,8 ,0 );
    int largeurPince = abs(pincePositions[1].x - pincePositions[0].x);
    if(largeurPince == 0) {
        return;
    }
    cout << "larguer : " << largeurPince << endl;

    switch (mEtat) {

        case STANDBY: {
            mEtat = OUVERTURE;
            mLastStateChange.restart();
        }
        break;

        case OUVERTURE: {
            cmd[2] = -1;
            if(mLastStateChange.getElapsedTime().asMilliseconds()  > 1000)
                mEtat = MONTEE_1;

            /*if(largeurPince > 100)
                mEtat = MONTEE_1;*/
        }
        break;

        case MONTEE_1: {
            cmd[1] = 1;
            if(pincePos.y < YTOTOP) {
                mEtat = MOUVEMENT_HORIZONTAL_1;
            }
        }
        break;


        case MOUVEMENT_HORIZONTAL_1: {
            cmd[0] = pincePos.x < cible.position.x ? -1 : 1;
            if(abs(cible.position.x - pincePos.x) < 10) {
                mEtat = DESCENTE_1;
            }
        }
        break;


        case DESCENTE_1: {
            cmd[1] = -1;
            cmd[0] = pincePos.x < cible.position.x ? -1 : 1;
            if(cible.position.y < pincePos.y) {
                mEtat = FERMETURE;
                cmd[1] = 1;
                mLastStateChange.restart();
            }
        }
        break;

        case FERMETURE: {
            cmd[2] = 1;
            cout << mLastStateChange.getElapsedTime().asMilliseconds() << endl;
            if(mLastStateChange.getElapsedTime().asMilliseconds() > 1300)
                mEtat = MONTEE_2;

            /*if(largeurPince < 70)
                mEtat = MONTEE_2;*/
        }
        break;

        case MONTEE_2: {
            cmd[1] = 1;
            if(pincePos.y < YTOTOP) {
                mEtat = MOUVEMENT_HORIZONTAL_2;
            }
        }
        break;


        case MOUVEMENT_HORIZONTAL_2: {
            cmd[0] = pincePos.x < cible.position.x ? -1 : 1;
            if(abs(cible.position.x - pincePos.x) < 10) {
                mEtat = DESCENTE_2;
            }
        }
        break;


        case DESCENTE_2: {
            cmd[1] = -1;
            cmd[0] = pincePos.x < cible.position.x ? -1 : 1;

            if(cible.position.y < pincePos.y) {
                /*int n = mContenuColonnes[mCoupActuel.from].size(); 
                if(n != 0) {
                    Cube moved = mContenuColonnes[mCoupActuel.from][n-1];
                    mContenuColonnes[mCoupActuel.from].pop_back();
                    mContenuColonnes[mCoupActuel.to].push_back(moved);
                }*/

                mEtat = STANDBY;
                mFini = true;
            }
        }
        break;


    }
    mCommander.setCMD(cmd);
}


