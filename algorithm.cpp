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

    for (int i = 0; i < popSize; i++) {
        callLists[i] = 0;
        parentsCallLists[i][0] = 0;
        parentsCallLists[i][1] = 0;
    }

    currentCar = -1;
    generationNum = 0;
}

Algorithm::~Algorithm() {
    for (int i = 0; i < popSize; i++)
        for (int j = 0; j < 2; j++)
            if (parentsCallLists[i][j])
                emit freeCallListNumber(parentsCallLists[i][j]);
    delete[] chromos;
    delete[] colors;
    delete[] scores;
    delete[] times;
}

void Algorithm::nextCar() {
    currentCar++;
    if (currentCar >= popSize)
        nextGeneration();
}

float Algorithm::getMagnitude(const int index) {
    return chromos[currentCar][index * 2 + 1] * (MAX_CART - MIN_CART) + MIN_CART;
}

float Algorithm::getCartAngle(const int index) {
    float angles[8];
    float angleSum = 0;
    for (int j = 0; j < 8; j++) {
        angles[j] = chromos[currentCar][j*2]*(1 - MIN_ANGLE) + MIN_ANGLE;
        angleSum += angles[j];
    }
    float angle = 0;
    for (int j = 0; j <= index; j++)
        angle += angles[j] / angleSum * M_PI * 2;
    return angle;
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

void Algorithm::setParentCallLists(const int index, const int parentA, const int parentB) {
    if (parentsCallLists[index][0])
        emit freeCallListNumber(parentsCallLists[index][0]);
    if (parentsCallLists[index][1])
        emit freeCallListNumber(parentsCallLists[index][1]);
    parentsCallLists[index][0] = callLists[parentA];
    parentsCallLists[index][1] = callLists[parentB];
}

void Algorithm::setParentCallLists(const int index, const int parent) {
    if (parentsCallLists[index][0])
        emit freeCallListNumber(parentsCallLists[index][0]);
    if (parentsCallLists[index][1])
        emit freeCallListNumber(parentsCallLists[index][1]);
    parentsCallLists[index][0] = callLists[parent];
    parentsCallLists[index][1] = 0;
}

QColor Algorithm::getColor(const int index) {
    return QColor(
        colors[currentCar][index][RED],
        colors[currentCar][index][GREEN],
        colors[currentCar][index][BLUE]
    );
}

