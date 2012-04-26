/*
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
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
#include "MNavWidget.hpp"

#include <QtGui>
#include <QtOpenGL>
#include <QSizePolicy>
#include <math.h>
#include <iostream>
#include "graphics.hpp"
#include <GL/glu.h>

#define DEFAULT_ZOOM_SENSITIVITY             0.002
#define DEFAULT_ROTATE_SENSITIVITY           0.30
#define DEFAULT_MOVE_SENSITIVITY             0.001
#define DEFAULT_MIN_ZOOM_RANGE               0.009
#define DEFAULT_CAMERA_FOV                   60.0
#define DEFAULT_MIN_CLIP_RANGE               0.1
#define DEFAULT_MAX_CLIP_RANGE               1000.0

#define DEFAULT_ZOOM_SENSITIVITY_2D          0.02
#define DEFAULT_ROTATE_SENSITIVITY_2D        0.0050
#define DEFAULT_MOVE_SENSITIVITY_2D          1.0

#define KEY_ROTATE_AMOUNT 5.0
#define KEY_MOVE_AMOUNT   10.0
#define KEY_ZOOM_AMOUNT   5.0


using namespace std;
using namespace Gui3DQt::Graphics;

namespace Gui3DQt {
  
MNavWidget::MNavWidget(QWidget *parent)
    : QGLWidget(parent)
{
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//  cout << "CREATE GLWIDGET..." << flush;

	userPaintGLOpaque = 0;
  userPaintGLTranslucent = 0;
	
  cam_state = IDLE;
  cam_pan = 0;
  cam_tilt = 0;
  cam_distance = 10.0;
  cam_x_offset = 0;
  cam_y_offset = 0;
  cam_z_offset = 0;
  cam_x_offset_2D = 0;
  cam_y_offset_2D = 0;
  cam_rotation_2D = 0;
  cam_zoom = 1;
  cam_warp_x = 1;
  cam_warp_y = 1;

  gui_mode = GUI_MODE_3D;

  zoom_sensitivity = DEFAULT_ZOOM_SENSITIVITY;
  rotate_sensitivity = DEFAULT_ROTATE_SENSITIVITY;
  move_sensitivity = DEFAULT_MOVE_SENSITIVITY;
  min_zoom_range = DEFAULT_MIN_ZOOM_RANGE;
  camera_fov = DEFAULT_CAMERA_FOV;
  min_clip_range = DEFAULT_MIN_CLIP_RANGE;
  max_clip_range = DEFAULT_MAX_CLIP_RANGE;

  zoom_sensitivity_2D = DEFAULT_ZOOM_SENSITIVITY_2D;
  rotate_sensitivity_2D = DEFAULT_ROTATE_SENSITIVITY_2D;
  move_sensitivity_2D = DEFAULT_MOVE_SENSITIVITY_2D;
//  cout << "GLWIDGET CREATED" << endl;
  
  setFocusPolicy(Qt::StrongFocus);
}

MNavWidget::~MNavWidget()
{
//    makeCurrent();
}

QSize MNavWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MNavWidget::sizeHint() const
{
    return QSize(400, 300);
}

void MNavWidget::setUserPaintGLOpaque(boost::function<void()> func)
{
	userPaintGLOpaque = func;
}

void MNavWidget::setUserPaintGLTranslucent(boost::function<void()> func)
{
  userPaintGLTranslucent = func;
}

void MNavWidget::setUserAfterPaint(boost::function<void()> func)
{
  userAfterPaint = func;
}


void MNavWidget::initializeGL()
{
//  cout << "INITIALIZE GL" << flush;
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE); // if enabled, surfaces can be flagged to be drawn only from one side
  //glShadeModel(GL_FLAT); // a polygon as only one color
  glShadeModel(GL_SMOOTH); // a polygon might have a color gradient if its vertices have a different color assigned
  float light_ambient[] = { 1, 1, 1, 1 }; // keine Richtung, d.h. kein räumlicher Eindruck -> nur kleine Werte, bzw Alpha klein, default: 0,0,0,1
//  float light_diffuse[] = { -1, -1, -1, 1 }; // Richtungslicht, oberflächenunabhängig, default: 1,1,1,1
  float light_diffuse[] = { 1, 1, 1, 1 }; // Richtungslicht, oberflächenunabhängig, default: 1,1,1,1
  float light_specular[] = { 1, 1, 1, 1 }; // Richtungslicht, sorgt für Glanzpunkte, default: 1,1,1,1
  float light_position[] = { 0, 0, 100, 0 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glDisable(GL_LIGHTING); // by default, disable lighting
  glEnable(GL_NORMALIZE); // calls to glNormal will result in normalized normal vectors
  glClearColor(0.0, 0.0, 0.0, 1.0); // set background color (red,gree,blue,alpha): black
  glClearDepth(1.0); // set maximum depth
//  cout << "...done" << endl;
}

void MNavWidget::paintGL()
{
	/* setup camera view */
  if(gui_mode == GUI_MODE_3D) {
	  float cpan, ctilt, camera_x, camera_y, camera_z;
	  cpan = cam_pan * M_PI / 180.0;
	  ctilt = cam_tilt * M_PI / 180.0;
	  camera_x = cam_distance * cos(cpan) * cos(ctilt);
	  camera_y = cam_distance * sin(cpan) * cos(ctilt);
	  camera_z = cam_distance * sin(ctilt);
    set_display_mode_3D(width(), height(), camera_fov, min_clip_range, max_clip_range);
	  glViewport(0, 0, (GLsizei)width(), (GLsizei)height());
	  gluLookAt(camera_x + cam_x_offset, camera_y + cam_y_offset, camera_z + cam_z_offset, cam_x_offset, cam_y_offset, cam_z_offset, 0, 0, 1);
  }
  else if(gui_mode == GUI_MODE_2D)  {
    set_display_mode_2D(width(), height());
    glTranslatef(width() / 2.0, height() / 2.0, 0.0);
    glScalef(cam_zoom, cam_zoom, 1.0);
    glRotatef(radians_to_degrees(cam_rotation_2D), 0, 0, 1);
    glScalef(cam_warp_x, cam_warp_y, 1);
    glTranslatef(-cam_x_offset_2D, -cam_y_offset_2D, 0.0);
  }

  /* clear window */
  glDepthMask(true);
//  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  // draw first all opaque(non-transparent) objects with depth buffer writable
  glDisable(GL_BLEND); // disable transparency effects
  if (userPaintGLOpaque)
    userPaintGLOpaque();

  // then make the depth buffer read-only (i.e. depth-buffer is determined by opal objects only),
  // draw all translucent object in any order,
  // finally make the depth buffer writable again
  glDepthMask(false);
  glEnable(GL_BLEND); // enable transparent effects
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
//  glBlendFunc(GL_DST_COLOR, GL_SRC_ALPHA); //SRC-factor : existing pixels, DST-factor: about to draw 
  if (userPaintGLTranslucent)
		userPaintGLTranslucent();
  glDepthMask(true);

  glFlush();
  
  if (userAfterPaint)
    userAfterPaint();
}

void MNavWidget::resizeGL(int width, int height)
{
  (void)width; // avoids warning "unused parameter"
  (void)height; // avoids warning "unused parameter"
//    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
//    glMatrixMode(GL_MODELVIEW);
    //cout << "GL RESIZED to " << width << "," << height << endl;
//	  glViewport(0, 0, width, height);   -> react on this within the paint function!
//	  if (gui_mode == GUI_MODE_3D)
//	    graphics_set_display_mode_3D(width, height, camera_fov, min_clip_range, max_clip_range);
//	  else if (gui_mode == GUI_MODE_2D)
//	    graphics_set_display_mode_2D(width, height);
}

void MNavWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept(); // we handle the event, do not propagate to parents;
    last_mouse_x = event->x();
    last_mouse_y = event->y();
    switch (event->button()) {
    	case Qt::LeftButton:  cam_state = ROTATING; break;
    	case Qt::MidButton:   cam_state = MOVING; break;
    	case Qt::RightButton: cam_state = ZOOMING; break;
    	default:							cam_state = IDLE;
    }
}

void MNavWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept(); // we handle the event, do not propagate to parents;
  	cam_state = IDLE;
}

void MNavWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept(); // we handle the event, do not propagate to parents;
    int dx = event->x() - last_mouse_x;
    int dy = event->y() - last_mouse_y;

	  if (gui_mode == GUI_MODE_3D) {
	    switch (cam_state) {
	    	case ROTATING : rotate_camera(dx, dy); break; // if (event->buttons() & Qt::LeftButton)
	    	case MOVING : 	move_camera(dx, dy); break;
	    	case ZOOMING : 	zoom_camera(dy); break;
	    	default:				;
	    }
	  }
	  else if (gui_mode == GUI_MODE_2D) {
	    if(cam_state == ROTATING) {
	      //      if(y < height() / 2)
	      //        dx *= -1;
	      //if(x > width() / 2)
	      //  dy *= -1;
	      rotate_camera_2D(dx);
	    }
	    else if(cam_state == MOVING)
	      move_camera_2D(dx, dy);
	    else if(cam_state == ZOOMING)
	      zoom_camera_2D(dy);
	  }
	
	  last_mouse_x = event->x();
	  last_mouse_y = event->y();
	  updateGL();
}

void MNavWidget::keyPressEvent(QKeyEvent *event)
{
  double dx = 0, dy = 0;
  bool acceptKey = false;
  if (gui_mode == GUI_MODE_3D) {
  	switch (event->modifiers()) {
  		case Qt::NoModifier:
			  switch (event->key()) {
				  case Qt::Key_Left:  dx = KEY_MOVE_AMOUNT;  dy = 0;                break;
		      case Qt::Key_Right: dx = -KEY_MOVE_AMOUNT; dy = 0;                break;
		      case Qt::Key_Up:    dx = 0;                dy = KEY_MOVE_AMOUNT;  break;
		      case Qt::Key_Down:  dx = 0;                dy = -KEY_MOVE_AMOUNT; break;
		    }
	      if (dx != 0 || dy != 0) {
	        move_camera(dx, dy);
			  	acceptKey = true;
	      }
  			break;
  		case Qt::ControlModifier:
			  switch (event->key()) {
				  case Qt::Key_Left:  dx = -KEY_ROTATE_AMOUNT; dy = 0;                  break;
		      case Qt::Key_Right: dx = KEY_ROTATE_AMOUNT;  dy = 0;                  break;
		      case Qt::Key_Up:    dx = 0;                  dy = KEY_ROTATE_AMOUNT;  break;
		      case Qt::Key_Down:  dx = 0;                  dy = -KEY_ROTATE_AMOUNT; break;
		    }
		    if (dx != 0 || dy != 0) {
		      rotate_camera(dx, dy);
			  	acceptKey = true;
	      }
  			break;
  		case Qt::AltModifier:
			  switch (event->key()) {
		      case Qt::Key_Up:    dy = KEY_ZOOM_AMOUNT;  break;
		      case Qt::Key_Down:  dy = -KEY_ZOOM_AMOUNT; break;
	      }
	      if (dy != 0) {
	        zoom_camera(dy);
			  	acceptKey = true;
	      }
  			break;
  	} // end switch modifier 
  } // end GUI_MODE_3D
  if (gui_mode == GUI_MODE_2D) {
  	switch (event->modifiers()) {
  		case Qt::NoModifier:
			  switch (event->key()) {
				  case Qt::Key_Left:  dx = KEY_MOVE_AMOUNT;  dy = 0;                break;
		      case Qt::Key_Right: dx = -KEY_MOVE_AMOUNT; dy = 0;                break;
		      case Qt::Key_Up:    dx = 0;                dy = KEY_MOVE_AMOUNT;  break;
		      case Qt::Key_Down:  dx = 0;                dy = -KEY_MOVE_AMOUNT; break;
		    }
	      if (dx != 0 || dy != 0) {
	        move_camera_2D(dx, dy);
			  	acceptKey = true;
	      }
  			break;
  		case Qt::ControlModifier:
			  switch (event->key()) {
				  case Qt::Key_Left:  dx = -KEY_ROTATE_AMOUNT; break;
		      case Qt::Key_Right: dx = KEY_ROTATE_AMOUNT;  break;
		    }
		    if (dx != 0) {
		      rotate_camera_2D(dx);
			  	acceptKey = true;
	      }
  			break;
  		case Qt::AltModifier:
			  switch (event->key()) {
		      case Qt::Key_Up:    dy = KEY_ZOOM_AMOUNT;  break;
		      case Qt::Key_Down:  dy = -KEY_ZOOM_AMOUNT; break;
	      }
	      if (dy != 0) {
	        zoom_camera_2D(dy);
			  	acceptKey = true;
	      }
  			break;
  	} // end switch modifier 
  } // end GUI_MODE_2D
  if (acceptKey) {
  	event->accept();
  	updateGL();
  } else
    event->ignore();
}

void MNavWidget::setCameraParams(double zoom_sensitivity, double rotate_sensitivity, double move_sensitivity, double min_zoom_range, double camera_fov, double min_clip_range, double max_clip_range)
{
  this->zoom_sensitivity = zoom_sensitivity;
  this->rotate_sensitivity = rotate_sensitivity;
  this->move_sensitivity = move_sensitivity;
  this->min_zoom_range = min_zoom_range;
  this->camera_fov = camera_fov;
  this->min_clip_range = min_clip_range;
  this->max_clip_range = max_clip_range;
}

void MNavWidget::set2DCameraParams(double zoom_sensitivity, double rotate_sensitivity, double move_sensitivity)
{
  this->zoom_sensitivity_2D = zoom_sensitivity;
  this->rotate_sensitivity_2D = rotate_sensitivity;
  this->move_sensitivity_2D = move_sensitivity;
}

void MNavWidget::setCameraPos(double pan, double tilt, double range, double x_offset, double y_offset, double z_offset)
{
  cam_pan = pan;
  cam_tilt = tilt;
  cam_distance = range;
  cam_x_offset = x_offset;
  cam_y_offset = y_offset;
  cam_z_offset = z_offset;
  paintGL();
}

void MNavWidget::getCameraPos(double &pan, double &tilt, double &range, double &x_offset, double &y_offset, double &z_offset)
{
  pan = cam_pan;
  tilt = cam_tilt;
  range = cam_distance;
  x_offset = cam_x_offset;
  y_offset = cam_y_offset;
  z_offset = cam_z_offset;
}

void MNavWidget::set2DCameraPos(double x_offset, double y_offset, double rotation, double zoom)
{
  cam_x_offset_2D = x_offset;
  cam_y_offset_2D = y_offset;
  cam_rotation_2D = rotation;
  cam_zoom = zoom;
  paintGL();
}

MNavWidget::gui_mode_t MNavWidget::get_mode(void)
{
  return gui_mode;
}

void MNavWidget::set_mode(MNavWidget::gui_mode_t mode)
{
  gui_mode = mode;
}

void MNavWidget::set_2D_warping(double warp_x, double warp_y)
{
  cam_warp_x = warp_x;
  cam_warp_y = warp_y;
}

void MNavWidget::recenter(void)
{
  cam_x_offset = 0;
  cam_y_offset = 0;
  cam_z_offset = 0;
}

void MNavWidget::recenter_2D(void)
{
  cam_x_offset_2D = 0;
  cam_y_offset_2D = 0;
}

void MNavWidget::pick_point(int mouse_x, int mouse_y, double *scene_x, double *scene_y)
{
  double cx = width() / 2.0;
  double cy = height() / 2.0;
  double pan = degrees_to_radians(-90.0 - cam_pan);
  double tilt = degrees_to_radians(90.0 - cam_tilt);
  double d = cam_distance;
  double f = cy / tan(degrees_to_radians(camera_fov / 2.0));

  // from Matlab
  double px = (mouse_x - cx) * cos(tilt) * d /
    (cos(tilt) * f + sin(tilt) * mouse_y - sin(tilt) * cy);
  double py = -(mouse_y - cy) * d /
    (cos(tilt) * f + sin(tilt) * mouse_y - sin(tilt) * cy);

  // rotate by pan, add offset
  *scene_x =  px * cos(pan) + py * sin(pan) + cam_x_offset;
  *scene_y = -px * sin(pan) + py * cos(pan) + cam_y_offset;
}

void MNavWidget::get_2D_position(int x, int y, double *xout, double *yout)
{
  double dx, dy, ctheta, stheta;
	double GLUI_x_offset = 0;
	double GLUI_y_offset = 0;
  dx = (x - width() / 2.0 - GLUI_x_offset) /
    cam_zoom;
  dy = (width() / 2.0 - y + GLUI_y_offset) /
    cam_zoom;
  ctheta = cos(-cam_rotation_2D);
  stheta = sin(-cam_rotation_2D);
  *xout = cam_x_offset_2D + ctheta * dx - stheta * dy;
  *yout = cam_y_offset_2D + stheta * dx + ctheta * dy;
}

void MNavWidget::rotate_camera(double dx, double dy)
{
  cam_pan -= dx * rotate_sensitivity;
  cam_tilt += dy * rotate_sensitivity;
  if(cam_tilt < 0)
    cam_tilt = 0;
  else if(cam_tilt > 89.0)
    cam_tilt = 89.0;
}

void MNavWidget::zoom_camera(double dy)
{
  cam_distance -= dy * zoom_sensitivity * cam_distance;
  if(cam_distance < min_zoom_range)
    cam_distance = min_zoom_range;
}

void MNavWidget::move_camera(double dx, double dy)
{
  cam_x_offset += -dy * cos(degrees_to_radians(cam_pan)) *  move_sensitivity * cam_distance;
  cam_y_offset += -dy * sin(degrees_to_radians(cam_pan)) *  move_sensitivity * cam_distance;
  cam_x_offset +=  dx * cos(degrees_to_radians(cam_pan - 90.0)) *  move_sensitivity * cam_distance;
  cam_y_offset +=  dx * sin(degrees_to_radians(cam_pan - 90.0)) *  move_sensitivity * cam_distance;
}

void MNavWidget::move_camera_2D(double dx, double dy)
{
  double tempx, tempy;

  tempx = dx / (float)cam_zoom;
  tempy = -dy / (float)cam_zoom;

  cam_x_offset_2D -=
    (tempx * cos(-cam_rotation_2D) -
     tempy * sin(-cam_rotation_2D)) /
    cam_warp_x * move_sensitivity_2D;
  cam_y_offset_2D -=
    (tempx * sin(-cam_rotation_2D) +
     tempy * cos(-cam_rotation_2D)) /
    cam_warp_y * move_sensitivity_2D;
}

void MNavWidget::rotate_camera_2D(double dx)
{
  cam_rotation_2D += dx * rotate_sensitivity_2D;
}

void MNavWidget::zoom_camera_2D(double dx)
{
  cam_zoom += dx * zoom_sensitivity_2D *
    cam_zoom;
  if(cam_zoom > 1e7)
    cam_zoom = 1e7;
  if(cam_zoom < 1e-12)
    cam_zoom = 1e-12;
}

} // namespace
