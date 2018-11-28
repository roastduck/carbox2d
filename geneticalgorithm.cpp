#include "geneticalgorithm.h"

#include "qglobal.h"
#include "qmath.h"

//public

int GeneticAlgorithm::getOffspringsCount(const int index) {
    return offspringsCount[index];
}

void GeneticAlgorithm::init() {
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < CHROMOS_SIZE; j++) {
            chromos[i][j] = float(qrand())/float(RAND_MAX);
        }
        unsigned short int red = qrand()%256;
        unsigned short int green = qrand()%256;
        unsigned short int blue = qrand()%256;
        for (int j = 0; j < 16; j++) {
            colors[i][j][RED] = red;
            colors[i][j][GREEN] = green;
            colors[i][j][BLUE] = blue;
        }
    }
    mutationRate = 0;
}

void GeneticAlgorithm::nextGeneration() {
    copyChromes();
    int max1 = 0;
    int max2 = 1;
    if (compareCar(scores[max2], times[max2], scores[max1], times[max1]))
        qSwap(max1, max2);
    float total = scores[0] + scores[0];
    for (int i = 2; i < POP_SIZE; i++) {
        total += scores[i];
        if (compareCar(scores[i], times[i], scores[max1], times[max1])) {
            qSwap(max1, max2);
            max1 = i;
        } else if (compareCar(scores[i], times[i], scores[max2], times[max2]))
            max2 = i;
    }
    bool newRecord = qAbs(scores[max1] - maxScore[maxScore.size() - 1]) >= 1;
    if (mutationRate < MAX_MUTATION_RATE && !newRecord)
        mutationRate += 0.5;
    else if (newRecord)
        mutationRate = 0;
    maxScore.push_back(scores[max1]);
    avgScore.push_back(total/POP_SIZE);
    copyChrome(max1, 0);
    setParentCallLists(0, max1);
    copyChrome(max2, 1);
    setParentCallLists(1, max2);
    int winners[POP_SIZE/2];
    bool queue[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++)
        queue[i] = true;
    for (int i = 0; i < POP_SIZE/2; i++) {
        int a = getRandomChrome(queue);
        int b = getRandomChrome(queue);
        winners[i] = compareCar(scores[a], times[a], scores[b], times[b])? a: b;
        offspringsCount[winners[i]]++;
    }
    crossover(winners[0], winners[1], 2, 3);
    for (int i = 0; i < POP_SIZE; i++)
        queue[i] = true;
    for (int i = 2; i < POP_SIZE/2; i++) {
        int parentA = winners[i];
        int parentB = getRandomChrome(queue, parentA);
        crossover(parentA, parentB, i*2, i*2 + 1);
        offspringsCount[parentB]++;
    }
    mutation();
    generationNum++;
    currentCar = 0;
}

//private

bool GeneticAlgorithm::compareCar(const float scoreA, const float timeA,
                                const int scoreB, const float timeB) {
    if (scoreA > scoreB)
        return true;
    if (scoreB > scoreA)
        return false;
    if (timeA <= timeB)
        return true;
    return false;
}

void GeneticAlgorithm::copyChrome(const int parent, const int offspring) {
    for (int j = 0; j < CHROMOS_SIZE; j++) {
        chromos[offspring][j] = oldChromes[parent][j];
    }
    for (int j = 0; j < 16; j++) {
        for (int channel = 0; channel < 3; channel++)
        colors[offspring][j][channel] = oldColors[parent][j][channel];
    }
}

void GeneticAlgorithm::copyChromes() {
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < CHROMOS_SIZE; j++) {
            oldChromes[i][j] = chromos[i][j];
        }
        for (int j = 0; j < 16; j++) {
            for (int channel = 0; channel < 3; channel++)
            oldColors[i][j][channel] = colors[i][j][channel];
        }
    }
}

void GeneticAlgorithm::crossover(const int parentA, const int parentB,
                                const int offspringA, const int offspringB) {
    int bend0 = qrand()%CHROMOS_SIZE;
    int bend1 = qrand()%CHROMOS_SIZE;
    if (bend0 > bend1)
        qSwap(bend0, bend1);
    for (int i = 0; i < CHROMOS_SIZE; i++) {
        if (i >= bend0 && i  <= bend1) {
            chromos[offspringA][i] = oldChromes[parentB][i];
            chromos[offspringB][i] = oldChromes[parentA][i];
            setColors(parentB, offspringA, parentA, offspringB, i);
        } else {
            chromos[offspringA][i] = oldChromes[parentA][i];
            chromos[offspringB][i] = oldChromes[parentB][i];
            setColors(parentA, offspringA, parentB, offspringB, i);
        }
    }
    setParentCallLists(offspringA, parentA, parentB);
    setParentCallLists(offspringB, parentB, parentA);
}

int GeneticAlgorithm::getRandomChrome(bool queue[], const int excluding) {
    int index = qrand()%POP_SIZE;
    while (!queue[index] || index == excluding) {
        index = (index + 1)%POP_SIZE;
    }
    queue[index] = false;
    return index;
}

void GeneticAlgorithm::mutation() {
    for (int i = 2; i < POP_SIZE; i++) {
        for (int j = 0; j < CHROMOS_SIZE; j++) {
            if (qrand()%1000 < mutationRate*10.0) {
                chromos[i][j] =  float(qrand())/float(RAND_MAX);
                int colorIndex = j < 16? j/2: (j - 16)/3;
                for (int channel = 0; channel < 3; channel++) {
                    colors[i][colorIndex][channel] = qrand()%256;
                }
            }
        }

    }
}

void GeneticAlgorithm::setColors(const int parentA, const int offspringA,
                                const int parentB, const int offspringB,
                                const int index) {
    int i = -1;
    if (index < 16) {
        if (index%2 == 0)
            i = index/2;
    } else {
        if ((index - 16)%3 == 0)
            i = (index - 15)/3 + 8;
    }
    if (i < 0)
        return;
    for (int channel = 0; channel < 3; channel++) {
        colors[offspringA][i][channel] = oldColors[parentA][i][channel];
        colors[offspringB][i][channel] = oldColors[parentB][i][channel];
    }
}

