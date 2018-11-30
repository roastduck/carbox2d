#ifndef EXTERNALALGORITHM_H
#define EXTERNALALGORITHM_H

#include "algorithm.h"

class ExternalAlgorithm: public Algorithm {
    Q_OBJECT
public:
    ExternalAlgorithm();
    virtual ~ExternalAlgorithm();

    void nextGeneration() override;

private:
    void readChromos();
    void writeScores();

	pid_t pid = 0;
	int inpipefd[2];
	int outpipefd[2];
};

#endif // EXTERNALALGORITHM_H
