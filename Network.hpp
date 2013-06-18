#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <QObject>

#include <QPair>
#include <QList>
#include <QVector>

#include <QRunnable>

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


    QVector<long double> getTrainigResult() const;
    void setTrainigResult(const QVector<long double> &value);

    QVector<QVector<long double> > getTrainingData() const;
    void setTrainingData(const QVector<QVector<long double> > &value);

    QVector<long double> getTestingResult() const;
    void setTestingResult(const QVector<long double> &value);

    QVector<QVector<long double> > getTestingData() const;
    void setTestingData(const QVector<QVector<long double> > &value);

    QVector<long double> getObtainedTestingResult() const;
    void setObtainedTestingResult(const QVector<long double> &value);

    QVector<long double> getTestingError() const;
    void setTestingError(const QVector<long double> &value);

    QVector<long double> getTrainingError() const;
    void setTrainingError(const QVector<long double> &value);

    quint32 getNumberOfNeurons() const;
    void setNumberOfNeurons(const quint32 &value);

private:
    Network() : maxNumberOfEpoch( 50 ), accuracy( 1e-4 ), alpha( 1.0 )
    {
        setAutoDelete( false );
    }
    Network(const Network & rNetwork) = delete;
    Network & operator = (const Network & rNetwork) = delete;

private:
    quint32 maxNumberOfEpoch;
    qreal accuracy;
    qreal alpha;
    quint32 numberOfNeurons;

    volatile bool stopFlag;

    QVector< long double > trainingError;

    QVector< long double > trainigResult;
    QVector< QVector< long double > > trainingData;

    QVector< long double > testingResult;
    QVector< QVector< long double > > testingData;

    QVector< long double > obtainedTestingResult;
    QVector < long double > testingError;
};


#endif // NETWORK_HPP
