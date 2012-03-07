/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   MNavWidget.h
 *  \brief  Provides a user-customized OpenGL Widget for QT with easy mouse navigation etc.
 *  \author Frank Moosmann (<moosmann@mrt.uka.de>),
 *          Benjamin Pitzer
 *  \date   2006-2009
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
#ifndef GUI3DQT_MNAVWIDGET_HPP_
#define GUI3DQT_MNAVWIDGET_HPP_

#include <QtGui/QWidget>
#include <QtOpenGL/QGLWidget>
#include <boost/function.hpp>

namespace Gui3DQt {
  
/*!
  \class MNavWidget
  \brief This is a user-customized OpenGL Widget for QT with easy mouse navigation etc.
  
  This Widget is derived from QGLWidget and thus intergrates seemlessly into QT.
  It offers the following functionality in addition:
  - nice 3D mouse navigation
  - 2D/3D switching context
  - registration of user-defined paint functions which are called when a repaint is initiated
*/
class MNavWidget : public QGLWidget
{
    Q_OBJECT

    typedef enum { GUI_MODE_3D, GUI_MODE_2D } gui_mode_t;
    typedef enum { IDLE, ROTATING, MOVING, ZOOMING } camera_state_t;

public:
    MNavWidget(QWidget *parent = 0);
    virtual ~MNavWidget();

    virtual QSize minimumSizeHint() const; // inherited from QWidget
    virtual QSize sizeHint() const; // inherited from QWidget

    void setUserPaintGLTranslucent(boost::function<void()> func); //!< If registered, this function is called after enabling "transparent" mode
    void setUserPaintGLOpaque(boost::function<void()> func); //!< If registered, this function is called after enabling "opoaque" mode
    void setUserAfterPaint(boost::function<void()> func); //!< If registered, this function is called after rendering is finished (e.g. for frame grabbing etc)

    void setCameraParams(double zoom_sensitivity, double rotate_sensitivity, double move_sensitivity, double min_zoom_range, double camera_fov, double min_clip_range, double max_clip_range);
    void set2DCameraParams(double zoom_sensitivity, double rotate_sensivitity, double move_sensitivity);
    void setCameraPos(double pan, double tilt, double range, double x_offset, double y_offset, double z_offset);
    void getCameraPos(double &pan, double &tilt, double &range, double &x_offset, double &y_offset, double &z_offset);
    void set2DCameraPos(double x_offset, double y_offset, double rotation, double zoom);
    gui_mode_t get_mode(void);
    void set_mode(gui_mode_t mode);
    void set_2D_warping(double warp_x, double warp_y);
    void recenter(void);
    void recenter_2D(void);
    void pick_point(int mouse_x, int mouse_y, double *scene_x, double *scene_y);
    void get_2D_position(int x, int y, double *xout, double *yout);
    
protected: // access only by derived classes
    virtual void initializeGL(); // inherited from QGLWidget
    virtual void paintGL(); // inherited from QGLWidget
    virtual void resizeGL(int width, int height); // inherited from QGLWidget
    virtual void mousePressEvent(QMouseEvent *event); // inherited from QWidget
    virtual void mouseReleaseEvent(QMouseEvent *event); // inherited from QWidget
    virtual void mouseMoveEvent(QMouseEvent *event); // inherited from QWidget
    virtual void keyPressEvent(QKeyEvent *event); // inherited from QWidget

private:

    boost::function<void()> userPaintGLTranslucent;
    boost::function<void()> userPaintGLOpaque;
    boost::function<void()> userAfterPaint;
    
    void rotate_camera(double dx, double dy);
    void zoom_camera(double dy);
    void move_camera(double dx, double dy);
    void move_camera_2D(double dx, double dy);
    void rotate_camera_2D(double dx);
    void zoom_camera_2D(double dx);

    camera_state_t cam_state;
    float cam_pan, cam_tilt, cam_distance;
    float cam_x_offset, cam_y_offset, cam_z_offset;
    float cam_x_offset_2D, cam_y_offset_2D, cam_rotation_2D, cam_zoom, cam_warp_x, cam_warp_y;

    int last_mouse_x, last_mouse_y;
    int last_passive_mouse_x,last_passive_mouse_y;
  
    double zoom_sensitivity;
    double rotate_sensitivity;
    double move_sensitivity;
    double min_zoom_range;
    double camera_fov;
    double min_clip_range;
    double max_clip_range;
  
    double zoom_sensitivity_2D;
    double rotate_sensitivity_2D;
    double move_sensitivity_2D;
    gui_mode_t gui_mode;

};

} // namespace

#endif // GUI3DQT_MNAVWIDGET_HPP_

