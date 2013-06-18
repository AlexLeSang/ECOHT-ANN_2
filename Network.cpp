#include "Network.hpp"

#include <QDebug>

#include "Facade.hpp"

void Network::run() {
    initLayers();
    training( trainingData, trainigResult );
    testing( testingData, testingResult );
    Facade::getInstance().processFinished();
}

QVector<long double> Network::getTrainigResult() const
{
    return trainigResult;
}

void Network::setTrainigResult(const QVector<long double> &value)
{
    trainigResult = value;
}

QVector<QVector<long double> > Network::getTrainingData() const
{
    return trainingData;
}

void Network::setTrainingData(const QVector<QVector<long double> > &value)
{
    trainingData = value;
}

QVector<long double> Network::getTestingResult() const
{
    return testingResult;
}

void Network::setTestingResult(const QVector<long double> &value)
{
    testingResult = value;
}

QVector<QVector<long double> > Network::getTestingData() const
{
    return testingData;
}

void Network::setTestingData(const QVector<QVector<long double> > &value)
{
    testingData = value;
}

QVector<long double> Network::getObtainedTestingResult() const
{
    return obtainedTestingResult;
}

void Network::setObtainedTestingResult(const QVector<long double> &value)
{
    obtainedTestingResult = value;
}

QVector<long double> Network::getTestingError() const
{
    return testingError;
}

void Network::setTestingError(const QVector<long double> &value)
{
    testingError = value;
}
