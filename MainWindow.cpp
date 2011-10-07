#include "MainWindow.hpp"

#include <iostream>
#include <GL/glut.h>
#include <QInputDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>

#include "ui_MainWindow.h"

using namespace std;
namespace fs = boost::filesystem;

namespace Gui3DQt {
  
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////     Constructor / Destructor        ///////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(GuiMode gMode, QWidget *parent)
  : QMainWindow(parent)
    ,guiMode(gMode)
    ,image2D(0,0, QImage::Format_RGB16)
    ,addedWidgets(0)
    ,currImgScaleFactor(1.0)
    ,imageOutputDirectory(QDir::homePath().toStdString())
    ,imageFilePattern("img*.png")
    ,frameCounter(0)
    ,grabFrames(false)
    ,grabSingleFrame(false)
{
  // create GUI and update labelings
  ui = new Ui::MainWindowClass();
  ui->setupUi(this);
  if (guiMode == GM_3D) { // remove view of 2D image
    QList<int> vSizes; vSizes.push_back(100); vSizes.push_back(0);
    ui->splitter2D3D->setSizes( vSizes );
    ui->tabWidget->removeTab(1);
    ui->actionChangeView->setEnabled(false);
  }
  QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(viewChanged(int)));
  ui->tabWidget->setCurrentIndex(0);
  updateGUI();
  // manually setup Layout for visualizers
  controlParentLayout = new QHBoxLayout();
  controlParentLayout->setSpacing(0);
  controlParentLayout->setContentsMargins(0,0,0,0);
  ui->controlsPlaceholder->setLayout(controlParentLayout);
  controlLayout = new QVBoxLayout();
  controlLayout->setSpacing(0);
  controlLayout->setContentsMargins(0,0,0,0);
  controlParentLayout->addLayout(controlLayout);
  // manually setup OpenGL Widget
  QGridLayout *glLayout = new QGridLayout();
  glLayout->setSpacing(0);
  ui->tab3D->setLayout(glLayout);
  glWid = new MNavWidget();
  glLayout->addWidget(glWid);
  // connect GUI to slots in this class
  QObject::connect(ui->actionChangeView, SIGNAL(triggered()), this, SLOT(changeView2D3D()));
  QObject::connect(ui->actionShowHideControlPanel, SIGNAL(triggered()), this, SLOT(showHideControlPanel()));
  QObject::connect(ui->actionWhiteBackground, SIGNAL(triggered()), this, SLOT(setWhiteBackground()));
  QObject::connect(ui->actionBlackBackground, SIGNAL(triggered()), this, SLOT(setBlackBackground()));
  QObject::connect(ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn2D()));
  QObject::connect(ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut2D()));
  QObject::connect(ui->actionSetOutputDirectory, SIGNAL(triggered()), this, SLOT(setImageOutputDir()));
  QObject::connect(ui->actionSetFilePattern, SIGNAL(triggered()), this, SLOT(setImageFilePattern()));
  QObject::connect(ui->actionGrab, SIGNAL(toggled(bool)), this, SLOT(startStopGrabbing(bool)));
  QObject::connect(ui->actionShot, SIGNAL(triggered()), this, SLOT(startSingleGrab()));

  glWid->setUserPaintGLOpaque(boost::bind( &MainWindow::paintGLOpaque, this));
  glWid->setUserPaintGLTranslucent(boost::bind( &MainWindow::paintGLTranslucent, this));
  glWid->setUserAfterPaint(boost::bind( &MainWindow::afterGLPaint, this));
  //glWid->setCameraParams(0.001, 0.3, 0.001, 0.009, 60, 1, 1000); //zoom_sensitivity, rotate_sensitivity, move_sensitivity, min_zoom_range, camera_fov, min_clip_range, max_clip_range
  glWid->setCameraPos(180.0, 89.99, 100.0, 0, 0, 0); //pan, tilt, range, x_offset, y_offset, z_offset
}

MainWindow::~MainWindow()
{
  delete ui;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////          public methods             ///////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void MainWindow::registerVisualizer(Visualizer *vis, string title, VisualizerMode visuMode)
{
  QSize vs = vis->sizeHint();
  addedWidgets += vs.height();
  if (addedWidgets > 1200) {
    controlLayout = new QVBoxLayout();//(QBoxLayout::TopToBottom);
    controlLayout->setSpacing(0);
    controlLayout->setContentsMargins(0,0,0,0);
    controlLayout->addStretch(1); // spacer at bottom of widget
    controlParentLayout->addLayout(controlLayout);
    addedWidgets = vs.height();
  }
  QWidget *wAdd = NULL;
  QGroupBox *frame = NULL;
  switch (visuMode) {
    case VM_Plain:
      wAdd = vis;
      break;
    case VM_Groupbox:
      frame = new QGroupBox();
      frame->setTitle(QString(title.c_str()));
      frame->setCheckable(true);
      frame->setChecked(true);
      QLayout *frameL = new QVBoxLayout();
      frame->setLayout(frameL);
      frameL->setSpacing(0);
      frameL->setContentsMargins(0,0,0,0);
      frameL->addWidget(vis);
      QObject::connect( frame, SIGNAL(toggled(bool)), glWid, SLOT(updateGL()) );
      wAdd = frame;
      break;
  }
  controlLayout->insertWidget(controlLayout->count()-1, wAdd); // insert before spacer
  QObject::connect( vis, SIGNAL(stateChanged()), glWid, SLOT(updateGL()) );
  if (guiMode == GM_3D2D)
    QObject::connect( vis, SIGNAL(redraw2D(QImage&)), this, SLOT(set2DImage(QImage&)) );
  visualizers.push_back(VisGroupbox(vis,frame));
  vis->show();
}

MNavWidget* MainWindow::getMNavWidget()
{
  return glWid;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////          private methods            ///////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

string MainWindow::getCurrentOutputFilename()
{
  fs::path fullName = fs::path("");
  do {
    frameCounter++;
    string fName = imageFilePattern;
    string number = (boost::format("%1$05d") % frameCounter).str();
    boost::replace_first(fName, "*", number);
    fullName = fs::path(imageOutputDirectory / fName);
  } while (exists(fullName));
  return fullName.string();
}

void MainWindow::paintGLOpaque()
{
  for (list<VisGroupbox>::iterator i=visualizers.begin(); i!=visualizers.end(); i++) {
    if ((i->second == NULL) || (i->second->isChecked()))
      i->first->paintGLOpaque();
  }
}

void MainWindow::paintGLTranslucent()
{
  for (list<VisGroupbox>::iterator i=visualizers.begin(); i!=visualizers.end(); i++) {
    if ((i->second == NULL) || (i->second->isChecked()))
      i->first->paintGLTranslucent();
  }
}

void MainWindow::afterGLPaint()
{
  // store frame if grabbing is active
  if ((grabFrames || grabSingleFrame) && (ui->tabWidget->currentIndex() == 0)) {
    grabSingleFrame = false;
    QImage frame = glWid->grabFrameBuffer(); // bool withAlpha = false
    string filename = getCurrentOutputFilename();
    cout << "store frame as " << filename << endl;
    frame.save(QString(filename.c_str()));
  }
}

void MainWindow::updateGUI()
{
  string outputDirText = "Set Output Directory [" + imageOutputDirectory.string() + "]";
  ui->actionSetOutputDirectory->setText(outputDirText.c_str());
  string outputPattern = "Set File Pattern [" + imageFilePattern + "]";
  ui->actionSetFilePattern->setText(outputPattern.c_str());
}

void MainWindow::setScaledImage(float factor)
{
  currImgScaleFactor *= factor;
//  ui->imgPlaceholder->resize(currImgScaleFactor * ui->imgPlaceholder->pixmap()->size());
  ui->imgPlaceholder->setPixmap(QPixmap::fromImage(image2D.scaled(image2D.size()*currImgScaleFactor)));
  adjustScrollBar(ui->imgScrollArea->horizontalScrollBar(), factor);
  adjustScrollBar(ui->imgScrollArea->verticalScrollBar(), factor);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
  scrollBar->setValue(int(factor * scrollBar->value() + ((factor-1) * scrollBar->pageStep()/2)));
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////           private slots             ///////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void MainWindow::viewChanged(int tab)
{
  bool view3D = (tab == 0) ? true : false;
  ui->actionWhiteBackground->setEnabled(view3D);
  ui->actionBlackBackground->setEnabled(view3D);
  ui->actionZoomIn->setEnabled(!view3D);
  ui->actionZoomOut->setEnabled(!view3D);
}

void MainWindow::set2DImage(QImage& img)
{
  image2D = img;
//  ui->imgPlaceholder->setPixmap(QPixmap::fromImage(img));
  setScaledImage(1.0);
  ui->imgPlaceholder2->setPixmap(QPixmap::fromImage(img));
  // store frame if grabbing is active
  if (((grabFrames) || (grabSingleFrame)) && (ui->tabWidget->currentIndex() == 1)) {
    grabSingleFrame = false;
    string filename = getCurrentOutputFilename();
    cout << "store frame as " << filename << endl;
    img.save(QString(filename.c_str()));
  }
}

void MainWindow::setWhiteBackground()
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //alpha=1.0 -> full overwrite of colors
  //glClear is called in QGlMNavWidget before each paint, to make the change effective 
}

void MainWindow::setBlackBackground()
{
	glClearColor(0.0, 0.0, 0.0, 1.0); //alpha=1.0 -> full overwrite of colors
  //glClear is called in QGlMNavWidget before each paint, to make the change effective 
}

void MainWindow::zoomIn2D()
{
  setScaledImage(IMAGE_2D_ZOOM_FACTOR);
}

void MainWindow::zoomOut2D()
{
  setScaledImage(1/IMAGE_2D_ZOOM_FACTOR);
}

void MainWindow::changeView2D3D()
{
  ui->tabWidget->setCurrentIndex(1-ui->tabWidget->currentIndex());
}

void MainWindow::showHideControlPanel()
{
  ui->controlsPlaceholder->setVisible(!ui->controlsPlaceholder->isVisible());
}

void MainWindow::setImageOutputDir()
{
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  dialog.setDirectory(imageOutputDirectory.string().c_str());
  //dialog.setLabelText(tr("Set Output Directory"));
  if (dialog.exec()) {
    imageOutputDirectory = fs::path(dialog.selectedFiles().at(0).toStdString());
    updateGUI();
  }
}

void MainWindow::setImageFilePattern()
{
  bool ok;
  QString text = QInputDialog::getText(this, tr("Set Image File Pattern"), tr("* will be replaces by numbers"), QLineEdit::Normal, QString(imageFilePattern.c_str()), &ok);
  if (ok && !text.isEmpty())
    imageFilePattern = text.toStdString();
  updateGUI();
}

void MainWindow::startStopGrabbing(bool grab)
{
  grabFrames = grab;
  if (grabFrames)
    statusBar()->showMessage(tr("Grabbing Activated"));
  else
    statusBar()->showMessage(tr(""));
}

void MainWindow::startSingleGrab()
{
  // store immediately (otherwise the next image will be stored)
  if (ui->tabWidget->currentIndex() == 1) {
    string filename = getCurrentOutputFilename();
    cout << "store frame as " << filename << endl;
    image2D.save(QString(filename.c_str()));
    grabSingleFrame = false;
  } else {
    grabSingleFrame = true;
    afterGLPaint();
  }
}


} // namespace
