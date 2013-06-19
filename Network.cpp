#include "Network.hpp"

#include <QDebug>

#include "Facade.hpp"


void Network::run() {
    stopFlag = false;

    //auto training_result = training( trainingData, trainigResult, numberOfNeurons, alpha, maxNumberOfEpoch, accuracy, &stopFlag );
    auto training_result = training(numberOfNeurons,alpha,maxNumberOfEpoch,trainigResult,trainingData,&stopFlag);

    const auto weight_hidden_output = std::get<0>(training_result);
    const auto weight_input_hidden = std::get<1>(training_result);
   // std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;
   // std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
    trainingError = std::get<2>(training_result);

    const auto bias = ones(size(testingData).first);
    const auto mu_test = mean( testingData );
    const auto sigma_test = stand( testingData );
    testingData = ( testingData - mu_test[ 0 ] ) / sigma_test[ 0 ];

    obtainedTestingResult = eval( merge( testingData, ones( testingData.size() ) ), weight_hidden_output, weight_input_hidden);
    //std::cerr << "obtainedTestingResult: " << obtainedTestingResult << std::endl;
   // exit(-1);
    testingError = test_error(trainigResult, obtainedTestingResult, testingResult);

    Facade::getInstance().processFinished();
}

std::vector<long double> Network::getTrainigResult() const
{
    return trainigResult;
}

void Network::setTrainigResult(const std::vector<long double> value)
{
    trainigResult = value;
}

std::vector<std::vector<long double> > Network::getTrainingData() const
{
    return trainingData;
}

void Network::setTrainingData(const std::vector<std::vector<long double> > value)
{
    trainingData = value;
}

std::vector<long double> Network::getTestingResult() const
{
    return testingResult;
}

void Network::setTestingResult(const std::vector<long double> value)
{
    testingResult = value;
}

std::vector<std::vector<long double> > Network::getTestingData() const
{
    return testingData;
}

void Network::setTestingData(const std::vector<std::vector<long double> > value)
{
    testingData = value;
}

std::vector<long double> Network::getObtainedTestingResult() const
{
    return obtainedTestingResult;
}

void Network::setObtainedTestingResult(const std::vector<long double> &value)
{
    obtainedTestingResult = value;
}

std::vector<long double> Network::getTestingError() const
{
    return testingError;
}

void Network::setTestingError(const std::vector<long double> &value)
{
    testingError = value;
}

std::size_t Network::getNumberOfNeurons() const
{
    return numberOfNeurons;
}

void Network::setNumberOfNeurons(const std::size_t &value)
{
    numberOfNeurons = value;
}

std::vector<long double> Network::getTrainingError() const
{
    return trainingError;
}

void Network::setTrainingError(const std::vector<long double> &value)
{
    trainingError = value;
}
