#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <QtCore>

#include "Exceptions.hpp"
#include <algorithm>
#include <vector>

/*!
 * \brief DataSample
 */
typedef QPair < QVector < qreal >, QVector < qreal > >  DataSample;

/*!
 * \brief Dataset
 */
typedef QVector< DataSample > Dataset;


constexpr quint32 defaultPercent = 30;

/*!
 * \brief SplittedDataSet
 */
typedef QVector < QVector < long double > > SplittedDataSet;

typedef QVector < long double > SplittedResult;

class Preprocessor
{
public:
    static Preprocessor & getInstance();

    std::vector<std::vector<long double> > getData() const;


    void setInputFileName(const QString & fileName);
    void setOutputFileName(const QString & fileName);
    void setPercentageOfTest( const quint32 value );

    void saveFile(const std::vector<std::vector<long double> > &data);
    void flush();

private:
    Preprocessor();

    void readFile();
    void writeFile(const std::vector<std::vector<long double> > &data);

private:
    QString fileNameIn;
    QString fileNameOut;
    Dataset cache;
    quint32 percentageOfTest;
    QDateTime lastModified;
};

#endif // PREPROCESSOR_HPP
