#include "algorithm.h"

Algorithm::Algorithm(int popSize_) : popSize(popSize_) {
    chromos = new float[popSize][CHROMOS_SIZE];
    colors = new unsigned short int[popSize][16][3];
    scores = new float[popSize];
    times = new float[popSize];
    callLists = new unsigned int[popSize];
    parentsCallLists = new unsigned int[popSize][2];

    maxScore.push_back(0);
    avgScore.push_back(0);
}

Algorithm::~Algorithm() {
    delete[] chromos;
    delete[] colors;
    delete[] scores;
    delete[] times;
}

float Algorithm::getAxleAngle(const int index) {
    return chromos[currentCar][START_WHEELS_GEN + index * 3 + 1] * M_PI * 2;
}

int Algorithm::getWheelOn(const int index) {
    float chromo = chromos[currentCar][START_WHEELS_GEN + index*3];
    if (chromo > WHEEL_PROB0)
        return -1;
    return int(chromo/WHEEL_PROB0*7+0.5);
}

float Algorithm::getWheelRadius(const int index) {
    return chromos[currentCar][START_WHEELS_GEN + index*3 + 2]*
            (MAX_WHEEL - MIN_WHEEL) + MIN_WHEEL;
}

QColor Algorithm::getColorAxle(const int index) {
    return getColor(index + 8);
}

QColor Algorithm::getColorCart(const int index) {
    return getColor(index);
}

unsigned int Algorithm::getCarCallListNumber() {
    for (int i = 0; i < popSize; i++) {
        for (int j = 0; j < 2; j++)
            if (parentsCallLists[i][j] == callLists[currentCar])
                return 0;
    }
    return callLists[currentCar];
}

unsigned int Algorithm::getCarParentCallListNumber(const int parent) {
    if (parent)
        return parentsCallLists[currentCar][1];
    return parentsCallLists[currentCar][0];
}

void Algorithm::setCarCallList(const unsigned int callListNumber) {
    callLists[currentCar] = callListNumber;
}

void Algorithm::setScoreAndTime(float score, float time) {
    scores[currentCar] = score;
    times[currentCar] = time;
}

float Algorithm::getScore(const int index) {
    if (generationNum && !currentCar)
        return scores[index];
    if (currentCar <= index)
        return -1;
    return scores[index];
}

int Algorithm::getTime(const int index) {
    return times[index];
}

float Algorithm::getAvgScore(const int index) {
    return avgScore[index];
}

float Algorithm::getMaxScore(const int index) {
    return maxScore[index];
}

void Algorithm::setParentCallLists(const int index, const int parentACL,
                                        const int parentBCL) {
    parentsCallLists[index][0] = parentACL;
    parentsCallLists[index][1] = parentBCL;
}

QColor Algorithm::getColor(const int index) {
    return QColor(
        colors[currentCar][index][RED],
        colors[currentCar][index][GREEN],
        colors[currentCar][index][BLUE]
    );
}

