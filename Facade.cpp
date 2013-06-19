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
    const auto err = networkRef.getTrainingError();
    QVector< long double > qe( err.size() );
    std::copy( err.cbegin(), err.cend(), qe.begin() );
    return qe;
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
        std::ofstream one("1.out");
        std::ofstream two("1.w");
        networkRef.save(one,two);
    } catch( std::exception e ){
        std::cerr << e.what() << std::endl;
        return;
    }
    // TODO save to file
//    preprocessorRef.saveFile( d );
}

void Facade::setNumberOfNeurons(const int val)
{
    networkRef.setNumberOfNeurons( val );
}

void Facade::startProcess()
{
    const auto data = preprocessorRef.getData();
   // std::cerr << "data: " << data << std::endl;
    const auto & x1 = data[ 0 ];
    const auto & x2 = data[ 1 ];
    const auto & y = data[ 2 ];

    const auto input = merge( x1, x2 );

    const auto training_testing_data = split( input, std::make_tuple( 0.6, 0.4 ) );

    const auto & trainingData = std::get<0>( training_testing_data );
    const auto & testingData = std::get<1>( training_testing_data );

    const auto test_training_result = split( y, std::make_tuple( 0.6, 0.4 ) );
    const auto & trainingResult = std::get<0>( test_training_result );
    const auto & testingResult = std::get<1>( test_training_result );


    Q_ASSERT( trainingData.size() == trainingResult.size() );
    Q_ASSERT( testingData.size() == testingData.size() );

    networkRef.setTrainingData( input );
    networkRef.setTrainigResult( y );

    networkRef.setTestingData( input );
    networkRef.setTestingResult( y );

    QThreadPool::globalInstance()->start( &networkRef );
}

void Facade::stopProcess()
{
    networkRef.stop();
}
