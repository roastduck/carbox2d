#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "algorithm.h"

class GeneticAlgorithm: public Algorithm {
    Q_OBJECT
public:
    GeneticAlgorithm() : Algorithm(POP_SIZE) {}

    int getOffspringsCount(const int index);
    void init();

private:
    static constexpr int POP_SIZE = 32;
    static constexpr int MAX_MUTATION_RATE = 5.0;

    void nextGeneration() override;

    bool compareCar(const float scoreA, const float timeA, const int scoreB,
                    const float timeB);
    void copyChrome(const int parent, const int offspring);
    void copyChromes();
    void crossover(const int parentA, const int parentB, const int offspringA,
                    const int offspringB);
    int getRandomChrome(bool queue[], const int excluding = -1);
    void mutation();
    void setColors(const int parentA, const int offspringA, const int  parentB,
                    const int offspringB, const int index);

    float oldChromes[POP_SIZE][CHROMOS_SIZE];
    float mutationRate;
    unsigned short int oldColors[POP_SIZE][16][3];
    int offspringsCount[POP_SIZE];
};

#endif // GENETICALGORITHM_H
