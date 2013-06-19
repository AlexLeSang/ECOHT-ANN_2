#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <QObject>

#include <QPair>
#include <QList>
#include <QVector>

#include <QRunnable>

#include "ANNetwork.hpp"


/*!
 * \brief The Network class
 */
class Network : public QRunnable
{
public:
    static Network & getInstance() {
        static Network instance;
        return instance;
    }
    void run();

    void stop() {
        stopFlag = true;
    }

    qreal getAccuracy() const {
        return accuracy;
    }

    void setAccuracy(const qreal value) {
        accuracy = value;
        Q_ASSERT( accuracy < 1.0 );
    }

    qreal getAlpha() const {
        return alpha;
    }

    void setAlpha(const qreal value) {
        alpha = value;
    }

    quint32 getMaxNumberOfEpoch() const {
        return maxNumberOfEpoch;
    }

    void setMaxNumberOfEpoch(const quint32 value) {
        maxNumberOfEpoch = value;
    }


    std::vector<long double> getTrainigResult() const;
    void setTrainigResult(const std::vector<long double> value);

    std::vector<std::vector<long double> > getTrainingData() const;
    void setTrainingData(const std::vector<std::vector<long double> > value);

    std::vector<long double> getTestingResult() const;
    void setTestingResult(const std::vector<long double> value);

    std::vector<std::vector<long double> > getTestingData() const;
    void setTestingData(const std::vector<std::vector<long double> > value);

    std::vector<long double> getObtainedTestingResult() const;
    void setObtainedTestingResult(const std::vector<long double> &value);

    std::vector<long double> getTestingError() const;
    void setTestingError(const std::vector<long double> &value);

    std::size_t getNumberOfNeurons() const;
    void setNumberOfNeurons(const std::size_t &value);

    std::vector<long double> getTrainingError() const;
    void setTrainingError(const std::vector<long double> &value);

private:
    Network() : maxNumberOfEpoch( 50 ), accuracy( 1e-4 ), alpha( 1.0 )
    {
        setAutoDelete( false );
    }
    Network(const Network & rNetwork) = delete;
    Network & operator = (const Network & rNetwork) = delete;

private:
    std::size_t maxNumberOfEpoch;
    long double accuracy;
    long double alpha;
    std::size_t numberOfNeurons;

    volatile bool stopFlag;

    std::vector< long double > trainingError;

    std::vector< long double > trainigResult;
    std::vector< std::vector< long double > > trainingData;

    std::vector< long double > testingResult;
    std::vector< std::vector< long double > > testingData;

    std::vector< long double > obtainedTestingResult;
    std::vector< long double > testingError;
};


#endif // NETWORK_HPP
