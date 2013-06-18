#include <iostream>
#include "Facade.hpp"
#include "Preprocessor.hpp"

/*!
 * \brief Facade::Facade
 */
Facade::Facade() :
    networkRef( Network::getInstance() ),
    preprocessorRef( Preprocessor::getInstance() )
{}

/*!
 * \brief Facade::getInstance
 * \return
 */
Facade &Facade::getInstance()
{
    static Facade instance;
    return instance;
}

/*!
 * \brief Facade::processFinished
 */
void Facade::processFinished()
{
    emit processEnd();
}

/*!
 * \brief Facade::getErrors
 * \return
 */
QVector< long double >  Facade::getErrors() const
{
    return networkRef.getTrainingError();
}


/*!
 * \brief Facade::setAlhpa
 * \param val
 */
void Facade::setAlhpa( double val )
{
    networkRef.setAlpha( val );
}

/*!
 * \brief Facade::setAccuracy
 * \param val
 */
void Facade::setAccuracy(double val)
{
    networkRef.setAccuracy( val );
}

/*!
 * \brief Facade::setMaxNumberOfEpoh
 * \param val
 */
void Facade::setMaxNumberOfEpoh(int val)
{
    networkRef.setMaxNumberOfEpoch( val );
}

void Facade::setTrainingDataPercent(int val)
{
    preprocessorRef.setPercentageOfTest( static_cast<quint32>(val) );
}

/*!
 * \brief Facade::setInputFileName
 * \param fileName
 */
void Facade::setInputFileName(const QString fileName)
{
    try{
        preprocessorRef.setInputFileName( fileName );
    }catch(std::exception e){
        std::cerr << e.what() << std::endl;
    }
}

void Facade::setOutputFileName(const QString fileName)
{
    try{
        preprocessorRef.setOutputFileName( fileName );
    } catch( std::exception e ){
        std::cerr << e.what() << std::endl;
        return;
    }
    // TODO
//    preprocessorRef.saveFile( d );
}

void Facade::setNumberOfNeurons(const quint32 & val)
{
    networkRef.setNumberOfNeurons( val );
    emit sendNumberOfNeurons( val );
}

void Facade::startProcess()
{
    // TODO get data, split and set to network

//    networkRef.setTrainingData( trainingData );
//    networkRef.setTrainigResult( trainingResult );

//    networkRef.setTestingData( testingData );
//    networkRef.setTestingResult( testingResult );

    QThreadPool::globalInstance()->start( &networkRef );
}

void Facade::stopProcess()
{
    networkRef.stop();
}
