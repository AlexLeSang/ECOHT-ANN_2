#include "Network.hpp"

#include <QDebug>

#include "Facade.hpp"

#include "ANNetwork.hpp"

void Network::run() {
    stopFlag = false;


    // TODO network

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

QVector<long double> Network::getTrainingError() const
{
    return trainingError;
}

void Network::setTrainingError(const QVector<long double> &value)
{
    trainingError = value;
}

quint32 Network::getNumberOfNeurons() const
{
    return numberOfNeurons;
}

void Network::setNumberOfNeurons(const quint32 &value)
{
    numberOfNeurons = value;
}
