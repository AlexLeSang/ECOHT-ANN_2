#ifndef FACADE_HPP
#define FACADE_HPP

#include <QObject>

#include "Network.hpp"
#include "Preprocessor.hpp"

class Network;

/*!
 * \brief The Facade class
 */
class Facade : public QObject
{
    Q_OBJECT

public:
    static Facade & getInstance();
    void processFinished();

    QVector<long double> getErrors() const;

public slots:
    void setAlhpa( double val );
    void setAccuracy( double val );
    void setMaxNumberOfEpoh( int val );
    void setNumberOfNeurons(const int val );
    void setTrainingDataPercent(int val);

    void setInputFileName( const QString fileName );
    void setOutputFileName( const QString fileName );

    void startProcess();
    void stopProcess();

signals:
    void processEnd();

private:
    Facade();

private:
    Network & networkRef;
    Preprocessor & preprocessorRef;
};

#endif // FACADE_HPP
