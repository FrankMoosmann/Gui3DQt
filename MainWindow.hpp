/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   MainWindow.hpp
 *  \brief  Provides a modular framework for 2D and 3D data display
 *  \author Frank Moosmann (<moosmann@mrt.uka.de>),
 *  \date   2007-2009
 *  \copyright  Karlsruhe Institute of Technology (KIT)
 *              Institute of Measurement and Control Systems
 *              http://www.mrt.kit.edu
 *              
 *              This program is free software: you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              version 3 as published by the Free Software Foundation.
 *              Other licenses are available on demand.
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *              GNU General Public License for more details.
 *
 *              You should have received a copy of the GNU General Public License
 *              along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GUI3DQT_MAINWINDOW_HPP_
#define GUI3DQT_MAINWINDOW_HPP_

#include <list>

#define BOOST_FILESYSTEM_VERSION 2
#include <boost/filesystem.hpp>

#include <QtGui/QMainWindow>
#include <QtGui/QBoxLayout>
#include <QScrollBar>
#include <QGroupBox>

#include "MNavWidget.hpp"
#include "Visualizer.hpp"

namespace Ui { class MainWindowClass; } // forward declaration to avoid including the ui_ header


namespace Gui3DQt {

/*!
  \class MainWindow
  \brief Provides a modular framework for 2D and 3D data display.
  
  Further documentation should be added!
*/
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  enum GuiMode {GM_3D, GM_3D2D};
  enum VisualizerMode {VM_Plain, VM_Groupbox};
  
  MainWindow(GuiMode gMode = GM_3D2D, QWidget *parent = 0);
  virtual ~MainWindow();

  void                    registerVisualizer(Visualizer*, std::string title, VisualizerMode vMode = VM_Groupbox, bool active = true); //!< called from extern to register a new Visualizer (add to GUI and call their paint methods on redraws)
  MNavWidget*             getMNavWidget();

private:
  const static double     IMAGE_2D_ZOOM_FACTOR;
  
  // User Interface:
  Ui::MainWindowClass     *ui;
  QBoxLayout              *controlParentLayout; // Horizontal Layout that can contain several vertical layouts
  QBoxLayout              *controlLayout; // current Vertical Layout
  MNavWidget              *glWid;
	const GuiMode           guiMode;
  QImage                  image2D;
  unsigned int            addedWidgets;
  float                   currImgScaleFactor;
  void                    setScaledImage(float factor);
  void                    adjustScrollBar(QScrollBar *scrollBar, double factor);
  
  // Grabbing:
  boost::filesystem::path imageOutputDirectory;
  std::string             imageFilePattern;
  unsigned int            frameCounter;
  bool                    grabFrames;
  bool                    grabSingleFrame;
  std::string             getCurrentOutputFilename();

  // Drawing:
  typedef std::pair<Visualizer*,QGroupBox*> VisGroupbox;
  std::list<VisGroupbox>  visualizers;
  void                    paintGLOpaque(); // called by QGlMNavWidget during painting, calls itself respective method of all visualizers
  void                    paintGLTranslucent(); // called by QGlMNavWidget during painting, calls itself respective method of all visualizers
  void                    afterGLPaint(); // called by QGlMNavWidget after painting is finished
  void                    updateGUI(); // recalculates and sets captions of labels / menus

private slots:
  void                    set2DImage(QImage&);
  void                    viewChanged(int);

  void                    changeView2D3D();
  void                    showHideControlPanel();
  void                    setWhiteBackground();
  void                    setBlackBackground();
  void                    zoomIn2D();
  void                    zoomOut2D();

  void                    setImageOutputDir();
  void                    setImageFilePattern();
  void                    startStopGrabbing(bool grab);
  void                    startSingleGrab();
};

} // namespace

#endif // GUI3DQT_MAINWINDOW_HPP_
