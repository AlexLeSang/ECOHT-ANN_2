#include "mainwindow.hpp"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stopButton->hide();

    ui->qwtPlot->setTitle("Error");
    ui->qwtPlot->setAxisTitle(ui->qwtPlot->xBottom, "Epoch");
    ui->qwtPlot->setAxisTitle(ui->qwtPlot->yLeft,"Error");
    ui->qwtPlot->setAxisAutoScale( ui->qwtPlot->xBottom, true );
    ui->qwtPlot->setAxisAutoScale( ui->qwtPlot->yLeft, true );
    ui->stopButton->setVisible( false );
    ui->saveButton->setVisible( false );
    zoom = new QwtPlotZoomer(ui->qwtPlot->canvas());
    zoom->setRubberBandPen(QPen(Qt::white));
    QPen pen = QPen( Qt::red );
    curve.setRenderHint( QwtPlotItem::RenderAntialiased );
    curve.setPen( pen );
    curve.attach( ui->qwtPlot );
    sendAlpha();

    // INFO connect ui to mainwindow
    {
        QObject::connect( ui->saveImageButton, SIGNAL( clicked() ),
                          this, SLOT( saveImage() ) );
       QObject::connect( ui->inputOpenButton, SIGNAL( clicked() ),
                          this, SLOT( openInputFile() ) );
        QObject::connect( ui->saveButton, SIGNAL( clicked() ),
                          this, SLOT( openOutputFile() ) );
        QObject::connect( ui->startButton, SIGNAL( clicked() ),
                          this, SLOT( start() ) );
        QObject::connect( ui->startButton, SIGNAL( clicked( bool ) ),
                          ui->stopButton, SLOT( setVisible(bool) ) );
        QObject::connect( ui->alphaMantiss, SIGNAL( valueChanged( double ) ),
                          this, SLOT( sendAlpha() ) );
        QObject::connect( ui->alphaDegree, SIGNAL( valueChanged( int ) ),
                          this, SLOT( sendAlpha() ) );
    }

    // INFO connectio ui to ui
    {
        QObject::connect(ui->startButton,SIGNAL(clicked()),ui->stopButton,SLOT(show()));
    }
//    /* INFO connection ui to facade
    {
        QObject::connect( ui->stopButton, SIGNAL( clicked() ),
                          &Facade::getInstance(), SLOT( stopProcess() ) );

        QObject::connect( ui->maxEpoch, SIGNAL( valueChanged(int) ),
                          &Facade::getInstance(), SLOT( setMaxNumberOfEpoh(int) ) );

        QObject::connect( ui->numberOfNeurons, SIGNAL( valueChanged(int) ),
                          &Facade::getInstance(), SLOT( setNumberOfNeurons(int) ) );

    }
//    */
//    /* INFO connection facade to ui
    {
        QObject::connect( &Facade::getInstance(), SIGNAL( processEnd() ),
                          ui->startButton, SLOT( show() ) );
        QObject::connect( &Facade::getInstance(), SIGNAL( processEnd() ),
                          ui->saveButton, SLOT( show() ) );
        QObject::connect( &Facade::getInstance(), SIGNAL( processEnd() ),
                          ui->stopButton, SLOT( hide() ) );
    }
//    */
//    /* INFO connection facade to main window
    {
//        QObject::connect( &Facade::getInstance(), SIGNAL( sendInitialLayerInfo(LayerDescription)),
//                          this, SLOT( setInitialLayerInfo( LayerDescription ) ) );
        QObject::connect( &Facade::getInstance(), SIGNAL( processEnd() ),
                          this, SLOT( displayResults() ) );
    }
//    */
//    /* INFO connection main window to facade
    {
        QObject::connect( this, SIGNAL( setInputFileName(QString) ),
                          &Facade::getInstance(), SLOT( setInputFileName(QString) ) );
        QObject::connect( this, SIGNAL( setOutputFileName(QString) ),
                          &Facade::getInstance(), SLOT( setOutputFileName(QString) ) );
        QObject::connect( this, SIGNAL( setAlpha( double ) ),
                          &Facade::getInstance(), SLOT( setAlhpa( double ) ) );
    }
//    */
}

/*!
 * \brief MainWindow::DisplayResults
 */
void MainWindow::displayResults()
{
//    ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotCurve, false );
//    ui->qwtPlot->replot();

//    const auto errorVector = Facade::getInstance().getErrors();
//    QVector < QPointF > points( errorVector.size() );
//    quint32 counter = 0;
//    auto pointsIt = points.begin();
//    // Create a
//    for ( auto errorIt = errorVector.constBegin(); errorIt != errorVector.constEnd(); ++ errorIt, ++ pointsIt, ++ counter ) {
//        (*pointsIt) = QPointF( counter, (*errorIt) );
//    }
//    QwtPointSeriesData * data = new QwtPointSeriesData(points);
//    curve.setData(data);
//    curve.attach( ui->qwtPlot );
//    ui->qwtPlot->replot();

    ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotCurve, false );
    ui->qwtPlot->replot();

    const auto errorVector = Facade::getInstance().getErrors();
    QVector < QPointF > points( errorVector.size() );
    quint32 counter = 0;
    auto pointsIt = points.begin();
    // Create a
    for ( auto errorIt = errorVector.constBegin(); errorIt != errorVector.constEnd(); ++ errorIt, ++ pointsIt, ++ counter ) {
        (*pointsIt) = QPointF( counter, (*errorIt) );
    }
    QwtPointSeriesData * data = new QwtPointSeriesData(points);
    curve.setData(data);
 //   curve.setSamples( QPolygonF ( points ) );
    curve.attach( ui->qwtPlot );
    ui->qwtPlot->replot();
}


/*!
 * \brief MainWindow::saveImage
 */
void MainWindow::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"), "image.png", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap pixmap = QPixmap::grabWidget(ui->qwtPlot);

    pixmap.save(filename);
}

/*!
 * \brief MainWindow::changeLayers
 * \param layersNumber
 */

/*!
 * \brief MainWindow::showResults
 * \param data
 */
void MainWindow::showResults(const Dataset & data)
{
    const quint32 tableWidth = data.first().first.size() + data.first().second.size();
    const quint32 inputLength = data.first().first.size();
    //ui->resultsTable->setColumnCount( tableWidth );
    //ui->resultsTable->setRowCount( data.size() );

    QStringList labelsList;
    for ( qint32 i=1; i <= data.first().first.size(); ++i ){
        labelsList.append( QString( "x" ) + QString::number( i ) );
    }

    for ( qint32 i=1; i <= data.first().second.size(); ++i ){
        labelsList.append( QString( "y" ) + QString::number( i ) );
    }

   //ui->resultsTable->setHorizontalHeaderLabels( labelsList );

    for ( auto it = data.constBegin(); it != data.constEnd(); ++it ){
        for ( auto itInputs = (*it).first.constBegin(); itInputs != (*it).first.constEnd(); ++itInputs ){
            QTableWidgetItem *item = new QTableWidgetItem( QString::number( *itInputs ));
    //        ui->resultsTable->setItem( it - data.constBegin(), itInputs - (*it).first.constBegin(), item );
        }

        for ( auto itOutputs = (*it).second.constBegin(); itOutputs != (*it).second.constEnd(); ++itOutputs ){
            QTableWidgetItem *item = new QTableWidgetItem( QString::number( *itOutputs ));
    //        ui->resultsTable->setItem( it - data.constBegin(), itOutputs - (*it).second.constBegin() + inputLength, item );
        }
    }
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() == Qt::ControlModifier && (e->key() == Qt::Key_S || e->key() == Qt::Key_Z || e->key() == Qt::Key_X) && ui->stopButton->isHidden()){
        this->start();
    }
}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    Facade::getInstance().stopProcess();
    delete zoom;
    delete ui;
}

/*!
 * \brief MainWindow::openFile
 */
void MainWindow::openInputFile()
{
    const QString fileName = QFileDialog::getOpenFileName( this, tr("Open data file"), "", tr("Data files (*.dat)"));
    if(fileName.isNull()) return;
    ui->currentFileName->setText( fileName );
    emit setInputFileName( fileName );
}

/*!
 * \brief MainWindow::openOutputFile
 */
void MainWindow::openOutputFile()
{
    const QString fileName = QFileDialog::getOpenFileName( this, tr("Open result file"), "", tr("Result files (*.res)"));
    if(fileName.isNull()) return;
    emit setOutputFileName( fileName );
}

/*!
 * \brief MainWindow::start
 */
void MainWindow::start()
{
    Facade::getInstance().startProcess();
    ui->stopButton->show();
}

/*!
 * \brief MainWindow::sendAlpha
 */
void MainWindow::sendAlpha()
{
    const double mantis = ui->alphaMantiss->value();
    const int deg = ui->alphaDegree->value();
    emit setAlpha( mantis * pow10( deg ) );
}
