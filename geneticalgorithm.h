#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "algorithm.h"

class GeneticAlgorithm: public Algorithm {
    Q_OBJECT
public:
    GeneticAlgorithm();

private:
    static constexpr int POP_SIZE = 32;
    static constexpr int MAX_MUTATION_RATE = 5.0;

    void nextGeneration() override;

    bool compareCar(const float scoreA, const float timeA, const int scoreB,
                    const float timeB);
    void copyChromo(const int parent, const int offspring);
    void crossover(const int parentA, const int parentB, const int offspringA,
                    const int offspringB);
    int getRandomChromo(bool queue[], const int excluding = -1);
    void mutation();
    void setColors(const int parentA, const int offspringA, const int  parentB,
                    const int offspringB, const int index);

    float mutationRate;
};

#endif // GENETICALGORITHM_H
