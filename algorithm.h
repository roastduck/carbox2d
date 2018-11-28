#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QColor>
#include <QVector>
#include <QObject>
#include <Box2D/Box2D.h>

/**
 * Base class for all algorithm
 */
class Algorithm: public QObject {
    Q_OBJECT
public:
    Algorithm(int popSize_);
    virtual ~Algorithm();

    // Functions below are used to get car characteristics from the chromosome
    float getAxleAngle(const int index);
    int getWheelOn(const int index);
    float getWheelRadius(const int index);

    // Functions below are used to get color of each part
    QColor getColorAxle(const int index);
    QColor getColorCart(const int index);

    // Functions below manages call list, i.e. the OpenGL handler for a car
    unsigned int getCarCallListNumber();
    unsigned int getCarParentCallListNumber(const int parent);
    void setCarCallList(const unsigned int callListNumber);

    int getGenerationNum() { return generationNum; }
    int getCarNum() { return currentCar; }

    void setScoreAndTime(float score, float time);
    float getScore(const int index);
    int getTime(const int index);

    float getAvgScore(const int index);
    float getMaxScore(const int index);

signals:
    void freeCallListNumber(unsigned int);

protected:
    /**
     * Set parents' call lists of car #`index` to be `parentACL` and `parentBCL`.
     *
     * @param parentACL : Call list of Parent A
     * @param parentBCL : Call list of Parent B
     */
    void setParentCallLists(const int index, const int parentACL, const int parentBCL);

private:
    QColor getColor(const int index);

protected:
    static constexpr int CHROMOS_SIZE = 40;
    static constexpr int START_WHEELS_GEN = 16;
    static constexpr float WHEEL_PROB0 = 0.5; //(0:1]
    static constexpr float MIN_CART = 0.1;
    static constexpr float MAX_CART = 3;
    static constexpr float MIN_ANGLE = 0.08;
    static constexpr float MIN_WHEEL = 0.1;
    static constexpr float MAX_WHEEL = 1.5;
    static constexpr int RED = 0, GREEN = 1, BLUE = 2; ///< Indicies of the `colors` array
    const int popSize; ///< Population size, i.e. number of cars in one generation

    int generationNum; ///< Generation ID
    int currentCar; ///< Car ID in current generation

    float (*chromos)[CHROMOS_SIZE]; ///< Array of size popSize. Chromosome of each car.
    unsigned short int (*colors)[16][3]; ///< Array of size popSize. Color of each part of the car,
                                         ///< used to indicate which part is from which parent

    QVector<float> avgScore, maxScore;
    float *scores, *times; ///< Array of size popSize. Score and time of each simulation.

    unsigned int *callLists, (*parentsCallLists)[2]; ///< Arrays of size popSize. Call lists are OpenGL handlers
};

#endif // ALGORITHM_H
