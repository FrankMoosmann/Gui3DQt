/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   graphics.hpp
 *  \brief  Provides functions to paint various things
 *  \author Benjamin Pitzer
 *  \date   2007
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

#ifndef GUI3DQT_GRAPHICS_HPP_
#define GUI3DQT_GRAPHICS_HPP_

namespace Gui3DQt {
namespace Graphics {
  
  /* util functions */
  double degrees_to_radians(double theta);
  double radians_to_degrees(double theta);
  double normalize_theta(double theta);
  
  /* display modes */
  void set_display_mode_2D(int w, int h);
  void set_display_mode_3D(int w, int h, float fov, float zNear, float zFar);
  
  /* draw strings */
  void draw_bitmap_string_2D(float x, float y, void *font, char *string) ;
  void draw_bitmap_string_centered_2D(float x, float y, void *font, char *string);
  void draw_bitmap_string_3D(float x, float y, float z, void *font, char *string) ;
  int bitmap_string_width(void *font, char *string);
  double stroke_text_width(void *font, float size, const char *string);
  void draw_stroke_text_2D(float x, float y, void *font, float size, char *string);
  void draw_stroke_text_centered_2D(float x, float y, void *font, float size,char *string);
  void draw_stroke_string(void *font, char *string);
  int stroke_string_width(void *font, char *string);
  
  /* draw other stuff, 2D at z=0 if no z is given */
  void draw_limit(double x, double y, double theta, double v);
  void draw_circle(double x, double y, double r, int filled);
  void draw_ellipse(double x, double y, double rx, double ry, int filled);
  void draw_diamond(double x, double y, double r);
  void draw_arrow(double x1, double y1, double x2, double y2, double head_width, double head_length);
  void draw_arrowhead(double x, double y, double angle);
  void draw_line(double x1, double y1, double x2, double y2);
  void draw_dashed_line(double x1, double y1,double x2, double y2,double stripe_len);
  void draw_coordinate_frame(double scale);
  void draw_bounding_box(double x, double y, double theta, double w, double l);
  void draw_nline_flag(double x, double y, double w, double h, int num_lines, char **line, int color, double camera_pan);
  void draw_observed_car(double x, double y, double theta, double w, double l, int id, double v, int draw_flag, double x_var, double y_var, int tracking_state, int lane, double confidence, int published, double camera_pan);
  void draw_distance_rings(double x, double y, double theta, int max_distance, int distance_increment);
  void draw_grid(double center_x, double center_y);
  void draw_cube_solid(float x1,float x2,float y1,float y2,float z1,float z2); // it is required that x1<x2, y1<y2, z1<z2!
  void draw_cube_cage(float x1,float x2,float y1,float y2,float z1,float z2);

}
}

#endif /*GUI3DQT_GRAPHICS_HPP_*/
