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
#include "graphics.hpp"

#define _USE_MATH_DEFINES

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

namespace Gui3DQt {
namespace Graphics {
    
const double PI_2 = M_PI/2;

const unsigned int COLOR_PALETTE_SIZE = 13;
unsigned char colorpalette[COLOR_PALETTE_SIZE][3] = {
  {239,230,0},
  {230,0,230},
  {0,230,230},
  {230,0,0},
  {128,51,128},
  {51,128,128},
  {255,51,51},
  {51,255,51},
  {51,51,255},
  {51,179,204},
  {128,255,51},
  {255,128,51},
  {51,128,255}
};



double degrees_to_radians(double theta)  //inline
{
  return (theta * M_PI / 180.0);
}
double radians_to_degrees(double theta)  //inline
{
  return (theta * 180.0 / M_PI);
}
double normalize_theta(double theta)  //inline
{
  int multiplier;

  if (theta >= -M_PI && theta < M_PI)
    return theta;

  multiplier = (int)(theta / (2*M_PI));
  theta = theta - multiplier*2*M_PI;
  if (theta >= M_PI)
    theta -= 2*M_PI;
  if (theta < -M_PI)
    theta += 2*M_PI;

  return theta;
}




void set_display_mode_2D(int w, int h)
{
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLfloat)w, 0.0, (GLfloat)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void set_display_mode_3D(int w, int h, float fovy, float zNear, float zFar)
{
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy, w / (float)h, zNear, zFar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}



void draw_bitmap_string_2D(float x, float y, void *font, char *string)
{
  char *c;

  glPushMatrix();
  glRasterPos2d(x, y);
  for(c = string; *c != '\0'; c++)
    glutBitmapCharacter(font, *c);
  glPopMatrix();
}

int bitmap_string_width(void *font, char *string)
{
  int width = 0;
  char *c;

  for(c = string; *c != '\0'; c++)
    width += glutBitmapWidth(font, *c);
  return width;
}

void draw_bitmap_string_3D(float x, float y, float z, void *font, char *string)
{
  char *c;

  glPushMatrix();
  glRasterPos3d(x, y, z);
  glBitmap(0, 0, 0, 0, -bitmap_string_width(font, string) / 2.0, 0, NULL);

  for(c = string; *c != '\0'; c++)
    glutBitmapCharacter(font, *c);
  glPopMatrix();
}

void draw_bitmap_string_centered_2D(float x, float y, void *font, char *string)
{
  char *c;
  int vert_offset = -5;

  glPushMatrix();
  glRasterPos2d(x, y);
  if(font == GLUT_BITMAP_HELVETICA_18)
    vert_offset = -7;
  else if(font == GLUT_BITMAP_HELVETICA_12)
    vert_offset = -4;
  else if(font == GLUT_BITMAP_HELVETICA_10)
    vert_offset = -3;
  glBitmap(0, 0, 0, 0,
           -bitmap_string_width(font, string) / 2.0, vert_offset, NULL);
  for(c = string; *c != '\0'; c++)
    glutBitmapCharacter(font, *c);
  glPopMatrix();
}

void draw_stroke_string(void *font, char *string)
{
  char *c;

  for(c = string; *c != '\0'; c++)
    glutStrokeCharacter(font, *c);
}

int stroke_string_width(void *font, char *string)
{
  int width = 0;

  char *c;

  for(c = string; *c != '\0'; c++)
    width += glutStrokeWidth(font, *c);
  return width;
}

double stroke_text_width(void *font, float size, const char *string)
{
  double w = 0;
  int i;

  for(i = 0; i < (int)strlen(string); i++)
    w += glutStrokeWidth(font, string[i]);
  return w * size / 100.0;
}

void draw_stroke_text_2D(float x, float y, void *font, float size, char *string)
{
  char *c;

  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(size / 100.0, size / 100.0, size / 100.0);
  for(c = string; *c != '\0'; c++)
    glutStrokeCharacter(font, *c);
  glPopMatrix();
}

void draw_stroke_text_centered_2D(float x, float y, void *font, float size, char *string)
{
  double w, h;

  w = stroke_text_width(font, size, string);
  h = stroke_text_width(font, size, (char*)" ");
  draw_stroke_text_2D(x - w / 2, y - h / 2, font, size, string);
}

void draw_diamond(double x, double y, double r)
{
  glBegin(GL_POLYGON);
  glVertex2f(x + r, y);
  glVertex2f(x, y + r);
  glVertex2f(x - r, y);
  glVertex2f(x, y - r);
  glEnd();
}

void draw_arrow(double x1, double y1, double x2, double y2, double head_width, double head_length)
{
  double angle, ct, st;

  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();

  angle = atan2(y2 - y1, x2 - x1);
  ct = cos(angle);
  st = sin(angle);
  glBegin(GL_POLYGON);
  glVertex2f(x2, y2);
  glVertex2f(x2 - head_length * ct + head_width * st,
             y2 - head_length * st - head_width * ct);
  glVertex2f(x2 - head_length * ct - head_width * st,
             y2 - head_length * st + head_width * ct);
  glEnd();
}

void draw_arrowhead(double x, double y, double angle)
{
  double ct, st, l = 2, l2 = 0.5;

  ct = cos(angle);
  st = sin(angle);
  glBegin(GL_POLYGON);
  glVertex2f(x, y);
  glVertex2f(x - l * ct + l2 * st, y - l * st - l2 * ct);
  glVertex2f(x - l * ct - l2 * st, y - l * st + l2 * ct);
  glEnd();
}


void draw_circle(double x, double y, double r, int filled)
{
  int i;
  double angle;

  if(filled)
    glBegin(GL_TRIANGLE_FAN);
  else
    glBegin(GL_LINE_LOOP);

  for(i = 0; i < 20; i++) {
    angle = i / 20.0 * M_PI * 2;
    glVertex2f(x + r * cos(angle), y + r * sin(angle));
  }
  glEnd();
}

void draw_ellipse(double x, double y, double rx, double ry, int filled)
{
  int i;
  double angle;

  if(filled)
    glBegin(GL_TRIANGLE_FAN);
  else
    glBegin(GL_LINE_LOOP);

  for(i = 0; i < 20; i++) {
    angle = i / 20.0 * M_PI * 2;
    glVertex2f(x + rx * cos(angle), y + ry * sin(angle));
  }
  glEnd();
}

void draw_line(double x1, double y1, double x2, double y2)
{
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void draw_dashed_line(double x1, double y1, double x2,
                      double y2, double stripe_len)
{
  double frac, dx, dy, x, y;
  int i;

  frac = stripe_len / hypot(x2 - x1, y2 - y1);
  dx = frac * (x2 - x1);
  dy = frac * (y2 - y1);
  x = x1;
  y = y1;
  glBegin(GL_LINES);
  for(i = 0; i < (int)floor(1 / frac); i++) {
    if(i % 2 == 0) {
      glVertex2f(x, y);
      glVertex2f(x + dx, y + dy);
    }
    x += dx;
    y += dy;
  }
  glEnd();
}

// Draw an X-Y-Z Frame. The red arrow corresponds to the X-Axis,
// green to the Y-Axis, and blue to the Z-Axis.
void draw_coordinate_frame(double scale)
{
  double a_axisThicknessScale = scale;
  const int a_modifyMaterialState = 0;

  //glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  // Triangle vertices:
  static int nTriangles = 8;

  static float triangle_vertices[72] = {
    0.000000f,0.040000f,-0.800000f,0.028284f,0.028284f,-0.800000f,
    0.000000f,0.000000f,-1.000000f,0.028284f,0.028284f,-0.800000f,
    0.040000f,0.000000f,-0.800000f,0.000000f,0.000000f,-1.000000f,
    0.040000f,0.000000f,-0.800000f,0.028284f,-0.028284f,-0.800000f,
    0.000000f,0.000000f,-1.000000f,0.028284f,-0.028284f,-0.800000f,
    0.000000f,-0.040000f,-0.800000f,0.000000f,0.000000f,-1.000000f,
    0.000000f,-0.040000f,-0.800000f,-0.028284f,-0.028284f,-0.800000f,
    0.000000f,0.000000f,-1.000000f,-0.028284f,-0.028284f,-0.800000f,
    -0.040000f,0.000000f,-0.800000f,0.000000f,0.000000f,-1.000000f,
    -0.040000f,0.000000f,-0.800000f,-0.028284f,0.028284f,-0.800000f,
    0.000000f,0.000000f,-1.000000f,-0.028284f,0.028284f,-0.800000f,
    0.000000f,0.040000f,-0.800000f,0.000000f,0.000000f,-1.000000f
  };

  // Triangle normals:
  static float triangle_normals[72] = {
    0.000000f,0.980581f,-0.196116f,0.693375f,0.693375f,-0.196116f,
    0.357407f,0.862856f,-0.357407f,0.693375f,0.693375f,-0.196116f,
    0.980581f,0.000000f,-0.196116f,0.862856f,0.357407f,-0.357407f,
    0.980581f,0.000000f,-0.196116f,0.693375f,-0.693375f,-0.196116f,
    0.862856f,-0.357407f,-0.357407f,0.693375f,-0.693375f,-0.196116f,
    0.000000f,-0.980581f,-0.196116f,0.357407f,-0.862856f,-0.357407f,
    0.000000f,-0.980581f,-0.196116f,-0.693375f,-0.693375f,-0.196116f,
    -0.357407f,-0.862856f,-0.357407f,-0.693375f,-0.693375f,-0.196116f,
    -0.980581f,0.000000f,-0.196116f,-0.862856f,-0.357407f,-0.357407f,
    -0.980581f,0.000000f,-0.196116f,-0.693375f,0.693375f,-0.196116f,
    -0.862856f,0.357407f,-0.357407f,-0.693375f,0.693375f,-0.196116f,
    0.000000f,0.980581f,-0.196116f,-0.357407f,0.862856f,-0.357407f
  };

  // Quad vertices:
  static int nQuads = 16;

  static float quad_vertices[192] = {
    0.000000f,0.010000f,0.000000f,0.007000f,0.007000f,0.000000f,
    0.007000f,0.007000f,-0.800000f,0.000000f,0.010000f,-0.800000f,
    0.000000f,-0.010000f,0.000000f,-0.007000f,-0.007000f,0.000000f,
    -0.007000f,-0.007000f,-0.800000f,0.000000f,-0.010000f,-0.800000f,
    -0.007000f,-0.007000f,0.000000f,-0.010000f,0.000000f,0.000000f,
    -0.010000f,0.000000f,-0.800000f,-0.007000f,-0.007000f,-0.800000f,
    -0.010000f,0.000000f,0.000000f,-0.007000f,0.007000f,0.000000f,
    -0.007000f,0.007000f,-0.800000f,-0.010000f,0.000000f,-0.800000f,
    -0.007000f,0.007000f,0.000000f,0.000000f,0.010000f,0.000000f,
    0.000000f,0.010000f,-0.800000f,-0.007000f,0.007000f,-0.800000f,
    0.007000f,0.007000f,0.000000f,0.010000f,0.000000f,0.000000f,
    0.010000f,0.000000f,-0.800000f,0.007000f,0.007000f,-0.800000f,
    0.010000f,0.000000f,0.000000f,0.007000f,-0.007000f,0.000000f,
    0.007000f,-0.007000f,-0.800000f,0.010000f,0.000000f,-0.800000f,
    0.007000f,-0.007000f,0.000000f,0.000000f,-0.010000f,0.000000f,
    0.000000f,-0.010000f,-0.800000f,0.007000f,-0.007000f,-0.800000f,
    -0.007000f,0.007000f,-0.800000f,-0.028284f,0.028284f,-0.800000f,
    -0.040000f,0.000000f,-0.800000f,-0.010000f,0.000000f,-0.800000f,
    -0.010000f,0.000000f,-0.800000f,-0.040000f,0.000000f,-0.800000f,
    -0.028284f,-0.028284f,-0.800000f,-0.007000f,-0.007000f,-0.800000f,
    -0.007000f,-0.007000f,-0.800000f,-0.028284f,-0.028284f,-0.800000f,
    0.000000f,-0.040000f,-0.800000f,0.000000f,-0.010000f,-0.800000f,
    0.000000f,-0.010000f,-0.800000f,0.000000f,-0.040000f,-0.800000f,
    0.028284f,-0.028284f,-0.800000f,0.007000f,-0.007000f,-0.800000f,
    0.028284f,-0.028284f,-0.800000f,0.040000f,0.000000f,-0.800000f,
    0.010000f,0.000000f,-0.800000f,0.007000f,-0.007000f,-0.800000f,
    0.040000f,0.000000f,-0.800000f,0.028284f,0.028284f,-0.800000f,
    0.007000f,0.007000f,-0.800000f,0.010000f,0.000000f,-0.800000f,
    0.007000f,0.007000f,-0.800000f,0.028284f,0.028284f,-0.800000f,
    0.000000f,0.040000f,-0.800000f,0.000000f,0.010000f,-0.800000f,
    0.000000f,0.010000f,-0.800000f,0.000000f,0.040000f,-0.800000f,
    -0.028284f,0.028284f,-0.800000f,-0.007000f,0.007000f,-0.800000f
  };

  // Quad normals:
  static float quad_normals[192] = {
    0.000000f,1.000000f,0.000000f,0.707107f,0.707107f,0.000000f,
    0.707107f,0.707107f,0.000000f,0.000000f,1.000000f,0.000000f,
    0.000000f,-1.000000f,0.000000f,-0.707107f,-0.707107f,0.000000f,
    -0.707107f,-0.707107f,0.000000f,0.000000f,-1.000000f,0.000000f,
    -0.707107f,-0.707107f,0.000000f,-1.000000f,0.000000f,0.000000f,
    -1.000000f,0.000000f,0.000000f,-0.707107f,-0.707107f,0.000000f,
    -1.000000f,0.000000f,0.000000f,-0.707107f,0.707107f,0.000000f,
    -0.707107f,0.707107f,0.000000f,-1.000000f,0.000000f,0.000000f,
    -0.707107f,0.707107f,0.000000f,0.000000f,1.000000f,0.000000f,
    0.000000f,1.000000f,0.000000f,-0.707107f,0.707107f,0.000000f,
    0.707107f,0.707107f,0.000000f,1.000000f,0.000000f,0.000000f,
    1.000000f,0.000000f,0.000000f,0.707107f,0.707107f,0.000000f,
    1.000000f,0.000000f,0.000000f,0.707107f,-0.707107f,0.000000f,
    0.707107f,-0.707107f,0.000000f,1.000000f,0.000000f,0.000000f,
    0.707107f,-0.707107f,0.000000f,0.000000f,-1.000000f,0.000000f,
    0.000000f,-1.000000f,0.000000f,0.707107f,-0.707107f,0.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f,
    0.000000f,0.000000f,1.000000f,0.000000f,0.000000f,1.000000f
  };

  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Set up nice color-tracking
  if (a_modifyMaterialState)
  {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  int k;
  for(k=0; k<3; k++) {

    glPushMatrix();

    // Rotate to the appropriate axis
    if (k==0) {
      glRotatef(-90.0,0,1,0);
      glColor3f(1.0f,0.0f,0.0f);
    }
    else if (k==1) {
      glRotatef(90.0,1,0,0);
      glColor3f(0.0f,1.0f,0.0f);
    }
    else {
      glRotatef(180.0,1,0,0);
      glColor3f(0.0f,0.0f,1.0f);
    }

    glScaled(a_axisThicknessScale,a_axisThicknessScale,scale);

    glVertexPointer(3, GL_FLOAT, 0, triangle_vertices);
    glNormalPointer(GL_FLOAT, 0, triangle_normals);
    glDrawArrays(GL_TRIANGLES, 0, nTriangles*3);

    glVertexPointer(3, GL_FLOAT, 0, quad_vertices);
    glNormalPointer(GL_FLOAT, 0, quad_normals);
    glDrawArrays(GL_QUADS, 0, nQuads*4);

    glPopMatrix();
  }

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_bounding_box(double x, double y, double theta, double w, double l)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);

  glLineWidth(2);

  glPushMatrix();
  glTranslatef(x, y, 0);
  glRotatef(radians_to_degrees(theta), 0, 0, 1);

  /* draw car outline */
  glBegin(GL_POLYGON);
  glVertex2f(l / 2, w / 2);
  glVertex2f(l / 2, -w / 2);
  glVertex2f(-l / 2, -w / 2);
  glVertex2f(-l / 2, w / 2);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex2f(l / 2, w / 2);
  glVertex2f(l / 2, -w / 2);
  glVertex2f(-l / 2, -w / 2);
  glVertex2f(-l / 2, w / 2);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(l  / 4.0, 0);
  glVertex2f(l * 3 / 4.0, 0);
  glEnd();
  glPopMatrix();
  glLineWidth(1);

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
}

void draw_nline_flag(double x, double y, double w, double h, int num_lines, char **line, int color, double camera_pan)
{
  int i;
  float dh;

  glPushMatrix();
  glTranslatef(x, y, 0.5);
//  gui3D.camera_pose.pan
  glRotatef(camera_pan + 90, 0, 0, 1);
  glBegin(GL_LINE_STRIP);
  glColor3f(0, 0, 0);
//  glColor3f(colorpalette[color][0],colorpalette[color][1],colorpalette[color][2]);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 2.0);
  glEnd();
  glTranslatef(-w / 2, 0, 2);
  glBegin(GL_POLYGON);
  glColor4f(1, 1, 1, 0.7);
  glColor4f(colorpalette[color][0],colorpalette[color][1],colorpalette[color][2], 0.7);
  glVertex3f(-w / 2.0, 0, 0);
  glVertex3f(-w / 2.0, 0, h);
  glVertex3f(w / 2.0, 0, h);
  glVertex3f(w / 2.0, 0, 0);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glColor3f(colorpalette[color][0],colorpalette[color][1],colorpalette[color][2]);
//  glColor3f(0, 0, 0);
  glVertex3f(-w / 2.0, 0, 0);
  glVertex3f(-w / 2.0, 0, h);
  glVertex3f(w / 2.0, 0, h);
  glVertex3f(w / 2.0, 0, 0);
  glEnd();

  glColor3f(1, 1, 1);
  glTranslatef(0, -0.1, 0);
  glRotatef(90, 1, 0, 0);

  dh = h / (double)num_lines;
  for(i = 0; i < num_lines; i++)
    if(line[num_lines - i - 1] != NULL)
      draw_stroke_text_2D(-w  / 2.0 + 0.20 * dh, (i + 0.2) * dh,
          GLUT_STROKE_ROMAN, 0.6 * dh,
          line[num_lines - i - 1]);
  glPopMatrix();
}

void draw_observed_car(double x, double y, double theta,
            double w, double l, int id, double v,int draw_flag,
            double x_var, double y_var, int tracking_state,
            int lane, double confidence, int published, double camera_pan)
{
  (void)tracking_state;
  (void)lane;
  (void)camera_pan;
  const int num_lines = 5;
  char line1[100], line2[100], line3[100], line4[100], line5[100], line6[100], line7[100];
  char *text[7];
//  int color = abs(id) % 13;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);

  glLineWidth(2);

  glPushMatrix();
  glTranslatef(x, y, 0);
  glRotatef(radians_to_degrees(theta), 0, 0, 1);

  if (published)
    glColor4f(0, 1, 0, 0.7);
  else
    glColor4f(1, 1, 1, 0.7);

  /* draw car outline */
  glBegin(GL_POLYGON);
  glVertex2f(l / 2, w / 2);
  glVertex2f(l / 2, -w / 2);
  glVertex2f(-l / 2, -w / 2);
  glVertex2f(-l / 2, w / 2);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex2f(l / 2, w / 2);
  glVertex2f(l / 2, -w / 2);
  glVertex2f(-l / 2, -w / 2);
  glVertex2f(-l / 2, w / 2);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(l  / 4.0, 0);
  glVertex2f(l * 3 / 4.0, 0);
  glEnd();
  glPopMatrix();

  /* pose accuracy */
  glPushMatrix();
  glTranslatef(x, y, 0);
  glColor4f(0.6, 0.6, 0, 0.5);
  draw_ellipse(0, 0, sqrt(x_var), sqrt(y_var), 0);
  glPopMatrix();

  /* draw info */
  if(draw_flag) {

    glPushMatrix();
    glTranslatef(0, 0, 1.0);
    char buf[255];
    sprintf(buf, "%d", (int)confidence);
    glColor3f(1, 0, 0);
    draw_stroke_text_centered_2D(x,y,GLUT_STROKE_ROMAN, 2, buf);
    glPopMatrix();

    sprintf(line1, "ID:  %d", id);
    sprintf(line2, "X:   %.2f", x);
    sprintf(line3, "Y:   %.2f", y);
    sprintf(line4, "VEL: %.2f m/s", v);
    sprintf(line5, "DIR: %.2f", theta);
    text[0] = line1;
    text[1] = line2;
    text[2] = line3;
    text[3] = line4;
    text[4] = line5;
    text[5] = line6;
    text[6] = line7;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(radians_to_degrees(theta-PI_2), 0, 0, 1);
    glColor3f(0, 0, 0);
    glTranslatef(0, -0.1, 0);

    int i;
    float dh = l / 2.5 / (double)num_lines;

    for (i = 0; i < num_lines; i++)
      if (text[num_lines - i - 1] != NULL)
        draw_stroke_text_2D(-w / 2.0+ 0.20 * dh, (i + 0.2) * dh,
        GLUT_STROKE_ROMAN, 0.6 * dh, text[num_lines - i - 1]);
    glPopMatrix();
  }

  glLineWidth(1);

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
}

void draw_observed_car_old(double x, double y, double theta,
            double w, double l, int id, double v,int draw_flag,
            double x_var, double y_var, int tracking_state,
            int lane, double confidence, int published, double camera_pan)
{
  (void)tracking_state; // currently not used
  char line1[100], line2[100], line3[100], line4[100], line5[100], line6[100], line7[100];
  char *text[7];
  int color = abs(id) % 13;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);

  glLineWidth(2);

  glPushMatrix();
  glTranslatef(x, y, 0);
  glRotatef(radians_to_degrees(theta), 0, 0, 1);

  if (published)
    glColor3f(0, 1, 0);
  else
    glColor3f(1, 1, 0);

  /* draw car outline */
  glBegin(GL_POLYGON);
  glVertex2f(l / 2, w / 2);
  glVertex2f(l / 2, -w / 2);
  glVertex2f(-l / 2, -w / 2);
  glVertex2f(-l / 2, w / 2);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex2f(l / 2, w / 2);
  glVertex2f(l / 2, -w / 2);
  glVertex2f(-l / 2, -w / 2);
  glVertex2f(-l / 2, w / 2);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(l  / 4.0, 0);
  glVertex2f(l * 3 / 4.0, 0);
  glEnd();
  glPopMatrix();

  /* draw info flag */
  if(draw_flag) {
    sprintf(line1, "ID:    %d", id);
    sprintf(line2, "VEL:   %.1f MPS", v);
    sprintf(line3, "DIR:   %.2f", theta);
    sprintf(line4, "X_VAR: %f", x_var);
    sprintf(line5, "Y_VAR: %f", y_var);
    sprintf(line4, "X:     %f", x);
    sprintf(line5, "Y:     %f", y);
    sprintf(line6, "LANE: %d", lane);
    sprintf(line7, "CONF: %f", confidence);
    text[0] = line1;
    text[1] = line2;
    text[2] = line3;
    text[3] = line4;
    text[4] = line5;
    text[5] = line6;
    text[6] = line7;
    draw_nline_flag(x, y, 2.5, 1.0, 7, text, color, camera_pan);
  }
  glLineWidth(1);

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
}


void draw_limit(double x, double y, double theta, double v)
{
  char str[50];
  double angle;

  glPushMatrix();
  glTranslatef(x, y, 0);
  sprintf(str, "%.1f", v);
  angle = normalize_theta(theta);

  if(angle > 0 && angle < M_PI) {
    glRotatef(radians_to_degrees(angle) - 90, 0, 0, 1);
    glTranslatef(-stroke_string_width(GLUT_STROKE_ROMAN, str) * 0.005 -
                 1, -0.2, 0);
  }
  else {
    glRotatef(radians_to_degrees(angle) + 90, 0, 0, 1);
    glTranslatef(1, -0.2, 0);
  }
  glScalef(0.005, 0.005, 1);
  draw_stroke_string(GLUT_STROKE_ROMAN, str);
  glPopMatrix();
}

void draw_scale(int window_width, double map_zoom)
{
  char str[50];

  glBegin(GL_LINES);
  glVertex2f(window_width - 220, 17);
  glVertex2f(window_width - 20, 17);
  glVertex2f(window_width - 220, 22);
  glVertex2f(window_width - 220, 12);
  glVertex2f(window_width - 20, 22);
  glVertex2f(window_width - 20, 12);
  glEnd();
  glPushMatrix();
  sprintf(str, "%.2f meters", 200 / map_zoom);
  glTranslatef(window_width - 120 -
               stroke_string_width(GLUT_STROKE_ROMAN, str) *
               0.15 / 2.0, 22, 0);
  glScalef(0.15, 0.15, 1);
  draw_stroke_string(GLUT_STROKE_ROMAN, str);
  glPopMatrix();
}

void draw_distance_rings(double x, double y, double theta,
                                        int max_distance, int distance_increment)
{
  int i,j;
  char buf[255];
  float angle;
  glLineWidth(0.5);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);

  // draw radius
  glPushMatrix();
  glRotatef(radians_to_degrees(theta), 0, 0, 1);
  glTranslatef(x,y,0);
  glLineWidth(2.0);
  for(i = distance_increment; i <= max_distance; i += distance_increment) {
    glColor4f(0.5, 0.5, 0.5, 1.0);
    sprintf(buf,"%d",i);
    draw_stroke_text_2D(i, 0, GLUT_STROKE_ROMAN, 3.0, buf);
    glBegin(GL_LINE_LOOP);
    for(j = 0; j < 100; j++) {
      angle = j / 100.0 * M_PI * 2;
      glVertex3f(i * cos(angle), i * sin(angle), 0);
    }
    glEnd();
  }
  glPopMatrix();
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
}

void draw_grid(double center_x, double center_y)
{
  // ++++++++ draw grid ++++++++++++
  //glEnable(GL_LINE_SMOOTH); // nur bei breiten linien von Bedeutung
  glColor3f(0.4, 0.4, 0.4);
  glLineWidth(0.5);
  glBegin(GL_LINES);
  for (int grid_x = -100; grid_x < 100; grid_x++) {
    glVertex3f(grid_x - center_x, -100 - center_y, 0);
    glVertex3f(grid_x - center_x,  100 - center_y, 0);
  }
  for (int grid_y = -100; grid_y < 100; grid_y++) {
    glVertex3f(-100 - center_x, grid_y - center_y, 0);
    glVertex3f( 100 - center_x, grid_y - center_y, 0);
  }
  glEnd();

  // ++++++++ draw circles ++++++++++++
//  glColor3f(0.6, 0.6, 0.6);
//  glPushMatrix();
//  //glRotatef(kogmo_radians_to_degrees(robot_pose.yaw), 0, 0, 1);
//  //glTranslatef(velodyne_frame->config.laser_offset.x, velodyne_frame->config.laser_offset.y, velodyne_frame->config.laser_offset.z);
//  for(int i = 10; i <= 80; i += 10) {
//    glColor3f(0.5, 0.5, 0.5);
//    glBegin(GL_LINE_LOOP);
//    for(int j = 0; j < 100; j++) {
//      double angle = j / 100.0 * M_PI * 2;
//      glVertex3f(i * cos(angle), i * sin(angle), -1.71);
//    }
//    glEnd();
//  }
//  glPopMatrix();
  
}


// it is required that x1<x2, y1<y2, z1<z2!
void draw_cube_solid(float x1,float x2,float y1,float y2,float z1,float z2)
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glBegin(GL_QUADS);  //front side is defined as side where points are in counter-clock-wise direction - only with GL_QUADS, here it's 1,2,4,3 - 3,4,6,5
    glVertex3f(x1, y1, z1); //x,y,z
    glVertex3f(x2, y1, z1); //x,y,z
    glVertex3f(x2, y2, z1); //x,y,z
    glVertex3f(x1, y2, z1); //x,y,z

    glVertex3f(x1, y2, z1); //x,y,z
    glVertex3f(x2, y2, z1); //x,y,z
    glVertex3f(x2, y2, z2); //x,y,z
    glVertex3f(x1, y2, z2); //x,y,z

    glVertex3f(x1, y2, z2); //x,y,z
    glVertex3f(x2, y2, z2); //x,y,z
    glVertex3f(x2, y1, z2); //x,y,z
    glVertex3f(x1, y1, z2); //x,y,z

    glVertex3f(x1, y1, z2); //x,y,z
    glVertex3f(x2, y1, z2); //x,y,z
    glVertex3f(x2, y1, z1); //x,y,z
    glVertex3f(x1, y1, z1); //x,y,z

    glVertex3f(x1, y1, z1); //x,y,z
    glVertex3f(x1, y2, z1); //x,y,z
    glVertex3f(x1, y2, z2); //x,y,z
    glVertex3f(x1, y1, z2); //x,y,z

    glVertex3f(x2, y2, z1); //x,y,z
    glVertex3f(x2, y1, z1); //x,y,z
    glVertex3f(x2, y1, z2); //x,y,z
    glVertex3f(x2, y2, z2); //x,y,z
  glEnd();
  glDisable(GL_CULL_FACE);
}

void draw_cube_cage(float x1,float x2,float y1,float y2,float z1,float z2)
{
	glBegin(GL_LINE_STRIP);
	  glVertex3f(x1, y1, z1); //x,y,z
	  glVertex3f(x1, y2, z1); //x,y,z
	  glVertex3f(x1, y2, z2); //x,y,z
	  glVertex3f(x1, y1, z2); //x,y,z
	  glVertex3f(x1, y1, z1); //x,y,z
	  glVertex3f(x2, y1, z1); //x,y,z
	  glVertex3f(x2, y2, z1); //x,y,z
	  glVertex3f(x2, y2, z2); //x,y,z
	  glVertex3f(x2, y1, z2); //x,y,z
	  glVertex3f(x2, y1, z1); //x,y,z
  glEnd();
	glBegin(GL_LINES);
	  glVertex3f(x1, y1, z2); //x,y,z
	  glVertex3f(x2, y1, z2); //x,y,z
	  glVertex3f(x1, y2, z2); //x,y,z
	  glVertex3f(x2, y2, z2); //x,y,z
	  glVertex3f(x1, y2, z1); //x,y,z
	  glVertex3f(x2, y2, z1); //x,y,z
  glEnd();
}


    
} // end namespace
} // end namespace

