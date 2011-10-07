#include "Visualizer2.hpp"

#include <Gui3DQt/graphics.hpp>

Visualizer2::Visualizer2(QWidget *parent)
  : Gui3DQt::Visualizer(parent)
{
  glListIndex = glGenLists(1); // generate one display list
  ui.setupUi(this);
  connect( ui.pbRegenerate, SIGNAL(pressed()), this, SLOT(regenerate()) );
  regenerate();
}

Visualizer2::~Visualizer2()
{
  glDeleteLists(glListIndex, 1);
}


void Visualizer2::paintGLOpaque()
{
  glCallList(glListIndex+0); // call list at index 0
}

void Visualizer2::paintGLTranslucent()
{
}

double rand1010()
{
  double r = rand()%2000; // 0..2000
  return r/100.0 - 10.0; // -10..10
}

void Visualizer2::regenerate()
{
  // get parameters from GUI
  double x = ui.dsbX->value();
  double y = ui.dsbY->value();
  unsigned int nbp = ui.sbNbPts->value();
  
  // example on how to read background-color:
  GLdouble clr[4];
  glGetDoublev(GL_COLOR_CLEAR_VALUE, &(clr[0]));
  bool brightBackg = (clr[0]+clr[1]+clr[2])/3 > 0.5; // bright background -> 1, dark background -> 0
  
  // re-create list containing drawing commands
  glDeleteLists(glListIndex, 1);
  glListIndex = glGenLists(1); // generate a display list
  glNewList(glListIndex, GL_COMPILE);
  
  glPushMatrix();
  glTranslatef(x, y, 5.0); // shift to center
  if (brightBackg) // choose point color dependent on background
    glColor3f(0.5, 0.0, 0.0);
  else
    glColor3f(0.5, 1.0, 1.0);
  glPointSize(2); // size of rendered points
  glBegin(GL_POINTS);
  for (unsigned int i=0; i<nbp; ++i) { // render random points
    glVertex3f(rand1010(), rand1010(), rand1010());
  }
  glEnd();
  glPopMatrix(); // shift back
  
  glEndList();
  
  emit stateChanged();
}
