#include "Visualizer1.hpp"

#include <stdlib.h>
#include <Gui3DQt/graphics.hpp>

Visualizer1::Visualizer1(QWidget *parent)
  : Gui3DQt::Visualizer(parent)
  , circ_radius1(1.0)
  , circ_radius2(1.0)
  {
  ui.setupUi(this);
  connect( ui.cbCubes, SIGNAL(stateChanged(int)), this, SLOT(update()) );
  connect( ui.cbCircle, SIGNAL(stateChanged(int)), this, SLOT(update()) );
  connect( ui.pbCircleRadius, SIGNAL(pressed()), this, SLOT(rand_radius()) );
}

Visualizer1::~Visualizer1()
{

}


void Visualizer1::paintGLOpaque()
{
  if (ui.cbCircle->checkState() == Qt::Checked) {
    glColor3ub(255, 0, 0);
    Gui3DQt::Graphics::draw_circle(0.0, 0.0, circ_radius1, 0); // empty
    glColor3ub(10, 10, 200);
    Gui3DQt::Graphics::draw_circle(0.0, 0.0, circ_radius2, 1); // filled
  }
}

void Visualizer1::paintGLTranslucent()
{
  if (ui.cbCubes->isChecked()) {
    // draw cubes with helper function
    Gui3DQt::Graphics::draw_cube_solid(10.0, 15.0, 10.0, 15.0, 0.2, 1.2);
    Gui3DQt::Graphics::draw_cube_cage(15.0, 20.0, 15.0, 20.0, 0.2, 1.2);
    // draw ellipsoid
    GLUquadricObj *quadric = gluNewQuadric();
    glColor4f(0.5, 0.0, 1.0, 0.5);
    gluQuadricDrawStyle(quadric, GLU_SILHOUETTE); // rendering style (GLU_POINT, GLU_LINE, GLU_FILL or GLU_SILHOUETTE), default: GLU_FILL
    glPushMatrix();
    glTranslatef(22.5,22.5,1.0); // move coordinate system to point
    gluSphere(quadric,2.5, 10, 30); // radius, slices, stacks;
    glPopMatrix();
    gluDeleteQuadric(quadric);
  }
}

void Visualizer1::update()
{
  emit stateChanged();
}

void Visualizer1::rand_radius()
{
  circ_radius1 = 10.0 + (double)(rand()%500)/100.0;
  circ_radius2 = 5.0 + (double)(rand()%500)/100.0;
  emit stateChanged();
}
