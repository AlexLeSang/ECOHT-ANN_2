#include "Network.hpp"

#include "Layer.hpp"

#include <QDebug>

#include "Facade.hpp"

void Network::run() {
    initLayers();
    training( trainingData, trainigResult );
    testing( testingData, testingResult );
    Facade::getInstance().processFinished();
}

void Network::training(const Data &dataSet, const Result &desiredResult)
{
    // TODO
}

void Network::testing(const Data &data, const Result &desiredResult)
{
    // TODO
}
