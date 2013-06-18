#include "Preprocessor.hpp"
#include "Facade.hpp"
#include <fstream>
/*!
 * \brief Preprocessor::Preprocessor
 */
Preprocessor::Preprocessor() : percentageOfTest( defaultPercent ) {}

Preprocessor &Preprocessor::getInstance()
{
    static Preprocessor instance;
    return instance;
}

std::vector< std::vector<long double> > Preprocessor::getData() const
{
    std::vector<std::vector<long double> > res(cache[0].first.size() + cache[0].second.size());
    for( std::size_t i = 0; i < res.size(); ++i ){
        res[i].resize(cache.size());
    }
    for( auto i = 0; i < cache.size(); ++i){
        unsigned int a = 0;
        for( auto j = 0; j < cache[i].first.size(); ++j ){
            res[j][i]=  cache[i].first[j];
            a = j;
        }
        for( auto j = 0; j < cache[i].second.size(); ++j ){
            res[j + a][i] = cache[i].second[j];
        }
    }
    return res;
}


void Preprocessor::readFile()
{
    QFile inputFile( fileNameIn );
    if ( !inputFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        throw FileOpeningErrorException( "Preprocessor::readFile" );
    }

    QTextStream inputStream( &inputFile );
    qint32 numberOfInputs;
    qint32 numberOfOutputs;
    inputStream >> numberOfInputs;
    if(numberOfInputs == 0){
        throw UnexpectedFileEndException( "Preprocessor::readFile" );
    }
    inputStream.readLine();

    cache.clear();
    while ( !inputStream.atEnd() ) {
        QString nextLine;
        nextLine = inputStream.readLine().trimmed().simplified();
        if ( nextLine.isEmpty() ){
            continue;
        }
        QTextStream line( &nextLine );
        QVector < qreal > inputParameters( numberOfInputs );

        for ( auto itInputs = inputParameters.begin(); itInputs != inputParameters.end(); ++itInputs ) {
            qreal tmp;
            if( line.atEnd() ) {
                throw UnexpectedFileEndException( "Preprocessor::readFile" );
            }
            line >> tmp;

            (*itInputs) = tmp;
        }

        QVector < qreal > outputParameters;
        while( !line.atEnd() ) {
            qreal tmp;
            line >> tmp;
            outputParameters.append( tmp );

        }

        cache.append( DataSample( inputParameters, outputParameters ) );
    }

    numberOfOutputs = cache.first().second.size();
  //  std::random_shuffle(cache.begin(),cache.end());
}

/*!
 * \brief Preprocessor::writeFile
 * \param data
 */
void Preprocessor::writeFile( const std::vector<std::vector<long double> >& data )
{
    std::ofstream outputFile( fileNameOut.toStdString());

    for( auto i =0 ; i < data[0].size(); ++i){
        for(auto j = 0; j < data.size(); ++j){
            outputFile << data[j][i];
            if ( j+1 == data.size())
                outputFile << std::endl;
        }

    }

}

/*!
 * \brief Preprocessor::setInputFileName
 * \param fileName
 */
void Preprocessor::setInputFileName(const QString & fileName )
{
    // TODO handle an exception
    // TODO parse and get number of inputs
    // TODO call method of the facade and set number of inputs
    // TODO call method of the facade and set number of outputs
    if ( fileName == fileNameIn ) {
        QFileInfo f( fileNameIn );
        if( !f.exists() ){
            throw FileNotExistsException( "Preprocessor::setFileName" );
        }
        if ( f.lastModified() != lastModified ){
            lastModified = f.lastModified();
            readFile();
           // splitData();
        }
        return;
    }
    else {
        QFileInfo f( fileName );
        if( !f.exists() ){
            throw FileNotExistsException( "Preprocessor::setFileName" );
        }
        lastModified = f.lastModified();
        fileNameIn = fileName;
        readFile();
       // splitData();
    }
}

/*!
 * \brief Preprocessor::setOutputFileName
 * \param fileName
 */
void Preprocessor::setOutputFileName(const QString &fileName)
{
    fileNameOut = fileName;
}

/*!
 * \brief Preprocessor::setPercentageOfTest
 * \param value
 */
void Preprocessor::setPercentageOfTest(const quint32 value)
{
    percentageOfTest = value;
}

/*!
 * \brief Preprocessor::saveFile
 * \param data
 */
void Preprocessor::saveFile(const std::vector<std::vector< long double> > &data)
{
    writeFile(data);
}

/*!
 * \brief Preprocessor::flush
 */
void Preprocessor::flush()
{
    readFile();
}

