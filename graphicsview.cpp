#include "graphicsview.h"
#include <__common.h>

#include <QMenu>
#include <QGraphicsLineItem>
#include <iostream>
#include <fstream>
#include <QDir>

GraphicsView::GraphicsView(QObject *parent) : QGraphicsView(){
    //QGraphicsScene *canvasScene = new QGraphicsScene;
    //canvasView = new QGraphicsView(canvasScene);
    //this->setScene(canvasScene); // here you associate the scene with the view
    //setSceneRect(0, 0, 200, 200); // override your sizeGS() call
    //mousePos.reserve(exMaxPolygonSize);
    //canvasView->setMouseTracking(true);
    //canvasView->setRenderHints( QPainter::Antialiasing );    

    parent      = parent;
    originalW   = 0;
    originalH   = 0;
    //qDebug() << "constructor is ok";

    //Prepare GV
    QGraphicsScene* scene = new QGraphicsScene();
    this->setScene(scene);

    this->setMouseTracking(true);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    emit signalMouseReleased(e);

    //qDebug() << "Signal mouse pressed emmited";
}


void GraphicsView::mousePressEvent(QMouseEvent *e)
{

    emit signalMousePressed(e);


    //Click context
    //..
    if(e->button() == Qt::RightButton)
    {
        QAction *request = showContextMenuLine(e->screenPos().toPoint());
        if(request!=0)
        {
            if(request->text()=="Display information")
                funcDisplayPixelProperties(e);

            if(request->text()=="Save view")
                this->save(_PATH_CUSTOM_GV_DEFAULT);

            if( request->text()=="Slide Display Wavelength" )
                funcDisplayWavelength(e);

            if( request->text()=="Slide Remark Pixel" )
                emit signalPixeleSelected(e);

            if( request->text()=="Display Wavelength" )
                funcDiffractionDisplayWavelength(e);

            //if(request->text()=="By wavelenght")
                //funcTestCalibration();

            /*
            if(request->text()=="rightShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 1);
            if(request->text()=="rightDrawWavelenght")
                //funcShowWavelenLines(1);
            if(request->text()=="rightMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 1);
            if(request->text()=="rightMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 1);

            if(request->text()=="upShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 2);
            if(request->text()=="upDrawWavelenght")
                //funcShowWavelenLines(2);
            if(request->text()=="upMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 2);
            if(request->text()=="upMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 2);

            if(request->text()=="leftShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 3);
            if(request->text()=="leftDrawWavelenght")
                //funcShowWavelenLines(3);
            if(request->text()=="leftMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 3);
            if(request->text()=="leftMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 3);

            if(request->text()=="downShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 4);
            if(request->text()=="downDrawWavelenght")
                //funcShowWavelenLines(4);
            if(request->text()=="downMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 4);
            if(request->text()=="downMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 4);
            */

        }
    }



}

/*
void GraphicsView::funcShowWavelenLines(int type){

    //Gets camera calibration from .XML
    //..
    lstDoubleAxisCalibration calSett;
    funcGetCalibration(&calSett);
    //funcPrintCalibration(&calibSettings);

    //Gets linear regression
    //..
    float a, b;
    switch(type){
    case 1:
        a = calSett.rightLinRegA;
        b = calSett.rightLinRegB;
        break;
    case 2:
        a = calSett.upLinRegA;
        b = calSett.upLinRegB;
        break;
    case 3:
        a = calSett.leftLinRegA;
        b = calSett.leftLinRegB;
        break;
    case 4:
        a = calSett.downLinRegA;
        b = calSett.downLinRegB;
        break;
    }

    //Calculates line positions
    //..
    int rS, rX, gX, bX, offset;
    offset = 0;
    rS = round( a + (b*0.0) );//it is a
    rX = round( a + (b*(float)_RED_WAVELENGHT) );
    gX = round( a + (b*(float)_GREEN_WAVELENGHT) );
    bX = round( a + (b*(float)_BLUE_WAVELENGHT) );

    rS += offset;
    rX += offset;
    gX += offset;
    bX += offset;

    //bX = 927;
    //gX = 981;
    //rX = 1016;

    //qDebug() << "W: " << this->scene()->width();
    //qDebug() << "H: " << this->scene()->height();
    //qDebug() << "rX: " << rX;
    //qDebug() << "gX: " << gX;
    //qDebug() << "bX: " << bX;

    //Draw lines
    //..
    //red

    QGraphicsLineItem *sourceLine   = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );
    QGraphicsLineItem *redLine      = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );
    QGraphicsLineItem *greenLine    = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );
    QGraphicsLineItem *blueLine     = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );

    sourceLine->setPen(QPen(Qt::magenta));
    redLine->setPen(QPen(Qt::red));
    greenLine->setPen(QPen(Qt::green));
    blueLine->setPen(QPen(Qt::blue));

    this->scene()->addItem(sourceLine);
    this->scene()->addItem(redLine);
    this->scene()->addItem(greenLine);
    this->scene()->addItem(blueLine);

    sourceLine->setX(rS);
    redLine->setX(rX);
    greenLine->setX(gX);
    blueLine->setX(bX);


    sourceLine->setToolTip("origin = 0nm");
    redLine->setToolTip(QString::number(_RED_WAVELENGHT) + "nm");
    greenLine->setToolTip(QString::number(_GREEN_WAVELENGHT) + "nm");
    blueLine->setToolTip(QString::number(_BLUE_WAVELENGHT) + "nm");

    this->scene()->update();
    this->update();

}
*/

/*
void GraphicsView::funcShowWavelen(QPoint pos, int type){
    float wavelenght;
    int val;
    if(type<1 && type>4)
    {
        funcShowMsg("ERROR -> funcShowWavelen", "Type wrong");
        return (void)NULL;
    }
    if( type==1 || type==3 ){
        val = pos.x();
    }
    else
    {
        val = pos.y();
    }
    wavelenght = funcCalcWavelen( val, type );
    funcShowMsg("max avelenght",QString::number(wavelenght) +" | x: "+QString::number(pos.x()));
}
*/

/*
void GraphicsView::identifyMinimumWavelenght(QPoint pos, int type){
    float wavelenght;
    wavelenght = funcCalcWavelen( pos.x(), type );
    funcShowMsg("min wavelenght",QString::number(wavelenght));
}

void GraphicsView::identifyMaximumWavelenght(QPoint pos, int type){
    float wavelenght;
    wavelenght = funcCalcWavelen( pos.x(), type );
    funcShowMsg("max avelenght",QString::number(wavelenght));
}
*/

/*
float GraphicsView::funcCalcWavelen( int pixX, int type ){
    //Gets camera calibration from .XML
    //..
    lstDoubleAxisCalibration calSett;
    funcGetCalibration(&calSett);
    //funcPrintCalibration(&calibSettings);

    //It calculates wavelenght
    //..
    float a, b;
    switch( type )
    {
        case 1:
            a = calSett.rightLinRegA;
            b = calSett.rightLinRegB;
            break;
        case 2:
            a = calSett.upLinRegA;
            b = calSett.upLinRegB;
            break;
        case 3:
            a = calSett.leftLinRegA;
            b = calSett.leftLinRegB;
            break;
        case 4:
            a = calSett.downLinRegA;
            b = calSett.downLinRegB;
            break;
    }
    return ((float)pixX - a) / b;
}
*/

void GraphicsView::save(QString fileName){
    this->disableScrolls();
    QPixmap tmpPix = QPixmap::grabWidget(this);
    tmpPix.save(fileName);
    this->enableScrolls();
    funcShowMsg(" ","GV seved successfully");
}

void GraphicsView::disableScrolls(){
    this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

void GraphicsView::enableScrolls(){
    this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
}

void GraphicsView::funcDisplayPixelProperties(QMouseEvent *e)
{
    //Translate to Real Coordinates
    int canvasW, canvasH, imgW, imgH;
    //QPixmap tmpPix(_PATH_DISPLAY_IMAGE);
    imgW    = this->originalW;
    imgH    = this->originalH;
    canvasW = this->width();
    canvasH = this->height();

    double tmpPos[2];
    tmpPos[0] = round( ((float)e->x()/(float)canvasW)*imgW );
    tmpPos[1] = round( ((float)e->y()/(float)canvasH)*imgH );

    //Get the position clicked into scene
    //..
    QString tmpProp;
    QPointF tmpPoint = this->mapToScene(e->pos().x(),e->pos().y());
    tmpProp.append("CanvasW: "+QString::number(canvasW)+"\n");
    tmpProp.append("CanvasH: "+QString::number(canvasH)+"\n");
    tmpProp.append("imgW: "+QString::number(imgW)+"\n");
    tmpProp.append("imgH: "+QString::number(imgH)+"\n");
    tmpProp.append("x: "+QString::number(tmpPoint.x())+"\n");
    tmpProp.append("y: "+QString::number(tmpPoint.y())+"\n");
    tmpProp.append("realX: "+QString::number(tmpPos[0])+"\n");
    tmpProp.append("realY: "+QString::number(tmpPos[1])+"\n");
    funcShowMsg("Pixel properties", tmpProp);
}

void GraphicsView::funcDisplayWavelength(QMouseEvent *e)
{
    //......................................
    // Get Calibration Parameters
    //......................................
    structAnalysePlotSaved slideFluorescent;
    if( funcReadAnalysePlot(&slideFluorescent) == _ERROR )
    {
        funcShowMsg("ERROR","Calibration Not Found or Wrong");
        return (void)false;
    }

    QString tmpParam;
    double wavelengthRed, wavelengthGreen, wavelengthBlue;
    tmpParam = readFileParam(_PATH_SETTINGS_RED_WAVELEN);
    wavelengthRed   = tmpParam.toDouble(0);
    tmpParam        = readFileParam(_PATH_SETTINGS_GREEN_WAVELEN);
    wavelengthGreen = tmpParam.toDouble(0);
    tmpParam        = readFileParam(_PATH_SETTINGS_BLUE_WAVELEN);
    wavelengthBlue  = tmpParam.toDouble(0);

    //......................................
    //Calculate Linear Regression
    //......................................
    double X[3];
    double Y[3];
    X[0]    = (double)slideFluorescent.red;
    X[1]    = (double)slideFluorescent.green;
    X[2]    = (double)slideFluorescent.blue;
    Y[0]    = wavelengthRed;
    Y[1]    = wavelengthGreen;
    Y[2]    = wavelengthBlue;
    linearRegresion linReg = funcLinearRegression(X,Y,3);

    //......................................
    //Translate x in canvas int x in image
    //......................................
    float x = ( (double)e->x() / (double)slideFluorescent.canvasW ) * (double)slideFluorescent.originalW;

    //......................................
    //Calculate Wavelength to that X (pixel)
    //......................................
    float pixelsWavelength = linReg.a + ( linReg.b * x );
    //qDebug() << "X: " << X[0] << ", " << X[1] << ", " << X[2];
    //qDebug() << "Y: " << Y[0] << ", " << Y[1] << ", " << Y[2];
    //qDebug() << "x(): " << (float)e->x();

    //......................................
    //Display Wavelength
    //......................................
    funcShowMsg("Wavelength Calculated",QString::number(pixelsWavelength)+"nm");

}

void GraphicsView::funcDiffractionDisplayWavelength(QMouseEvent *e)
{
    //------------------------------------
    //Load Slit Calibration
    //------------------------------------
    QString selectedCalibPath;
    selectedCalibPath = readAllFile( _PATH_SLIDE_ACTUAL_CALIB_PATH ).trimmed();
    if( selectedCalibPath.isEmpty() )
    {
        funcShowMsgERROR("Please, define calibration file source.");
        return (void)false;
    }
    structSlideCalibration slitCalibration;
    if( funcReadSlideCalib( selectedCalibPath, &slitCalibration ) != _OK )
    {
        funcShowMsgERROR("Loading Slit Calibration: "+selectedCalibPath,this);
        return (void)false;
    }

    //------------------------------------
    //Calc and display wavelength
    //------------------------------------
    double wavelength, tmpDist;
    tmpDist = ((double)e->x() / (double)this->width()) * (double)originalW;

    /*
    qDebug() << "e.x: " << e->x()
             << " this->width(): " << this->width()
             << " originalW: " << originalW
             << "tmpDist: " << tmpDist;
    exit(0);*/

    wavelength  = funcDist2Wave((double)tmpDist,slitCalibration.dist2WaveLR,slitCalibration.polyDist2Wave);
    wavelength += slitCalibration.originWave;

    //qDebug() << "slitCalibration.polyDist2Wave1: " << slitCalibration.polyDist2Wave1;
    //qDebug() << "slitCalibration.polyDist2Wave2: " << slitCalibration.polyDist2Wave2;
    //qDebug() << "slitCalibration.polyDist2Wave3: " << slitCalibration.polyDist2Wave3;
    //exit(0);
    //wavelength += tmpOffset;

    QString tmpMessage;
    tmpMessage.append("realX(");
    tmpMessage.append(QString::number(tmpDist));
    tmpMessage.append(") -> ");
    tmpMessage.append(QString::number(wavelength));
    tmpMessage.append("nm");
    //tmpMessage.append(" tmpOffset: ");
    //tmpMessage.append(QString::number(tmpOffset));
    funcShowMsg("Wavelength",tmpMessage);
}


/*
void GraphicsView::funcTestCalibration()
{

    //Gets camera calibration from .XML
    //..
    strDiffPix *diffPix = (strDiffPix *)malloc(sizeof(strDiffPix));
    lstDoubleAxisCalibration calSett;
    funcGetCalibration(&calSett);
    //funcPrintCalibration(&calibSettings);

    //Draw contour
    //..
    int col;

    //Horizontal lines
    for(col=calSett.squarePixX; col<calSett.squarePixX+calSett.squarePixW; col++){
        diffPix->x = col;
        diffPix->y = calSett.squarePixY;
        funcSourcePixToDiffPix( diffPix, &calSett );
        scene()->addEllipse(diffPix->x,diffPix->y,1,1,QPen(Qt::white));
        scene()->addEllipse(diffPix->rightX,diffPix->rightY,1,1,QPen(Qt::red));
        //scene()->addEllipse(diffPix->upX,diffPix->upY,1,1,QPen(Qt::green));
        //scene()->addEllipse(diffPix->leftX,diffPix->leftY,1,1,QPen(Qt::blue));
        //scene()->addEllipse(diffPix->downX,diffPix->downY,1,1,QPen(Qt::magenta));
        qDebug() << "x: " << diffPix->x;
        qDebug() << "y: " << diffPix->y;

        qDebug() << "rightX: " << diffPix->rightX;
        qDebug() << "rightY: " << diffPix->rightY;

        exit(0);

    }
}
*/

QAction *GraphicsView::showContextMenuLine(QPoint pos){

    //..........................................
    //Main menu
    //..........................................
    QMenu *xmenu = new QMenu();
    xmenu->addAction( "Display information" );
    xmenu->addAction( "Save view" );

    //..........................................
    //Slide
    //..........................................
    xmenu->addSeparator();
    QMenu* submenuSlide = xmenu->addMenu( "Slide" );
    submenuSlide->addAction( "Slide Display Wavelength" );
    submenuSlide->addAction( "Slide Remark Pixel" );

    //..........................................
    //Diffraction
    //..........................................
    xmenu->addSeparator();
    QMenu* submenuDiffraction = xmenu->addMenu( "Diffraction" );
    submenuDiffraction->addAction( "Display Wavelength" );



    /*
    xmenu->addSeparator();
    QMenu* submenu2  = xmenu->addMenu( "Identify" );
    QMenu* submenu21 = submenu2->addMenu( "Right" );
    QMenu* submenu22 = submenu2->addMenu( "Up" );
    QMenu* submenu23 = submenu2->addMenu( "Left" );
    QMenu* submenu24 = submenu2->addMenu( "Down" );

    submenu21->addAction( "rightShowWavelenght" );
    submenu21->addAction( "rightDrawWavelenght" );
    submenu21->addAction( "rightMinWavelenght" );
    submenu21->addAction( "rightMaxWavelenght" );

    submenu22->addAction( "upDrawWavelenght" );
    submenu22->addAction( "upShowWavelenght" );
    submenu22->addAction( "upMinWavelenght" );
    submenu22->addAction( "upMaxWavelenght" );

    submenu23->addAction( "leftDrawWavelenght" );
    submenu23->addAction( "leftShowWavelenght" );
    submenu23->addAction( "leftMinWavelenght" );
    submenu23->addAction( "leftMaxWavelenght" );

    submenu24->addAction( "downDrawWavelenght" );
    submenu24->addAction( "downShowWavelenght" );
    submenu24->addAction( "downMinWavelenght" );
    submenu24->addAction( "downMaxWavelenght" );
    */

    //submenu2->addAction( "Green centroid" );
    //submenu2->addAction( "Blue centroid" );
    //submenu2->addSeparator();
    //submenu2->addAction( "Source centroid" );


    return xmenu->exec(pos);
}



void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
    emit signalMouseMove(e);
}
