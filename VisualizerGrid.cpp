#include "VisualizerGrid.hpp"
#include "graphics.hpp"

namespace Gui3DQt {
  
VisualizerGrid::VisualizerGrid(QWidget *parent)
    : Visualizer(parent)
{
}

VisualizerGrid::~VisualizerGrid()
{

}


void VisualizerGrid::paintGLOpaque()
{
  Graphics::draw_grid(0,0);
  Graphics::draw_coordinate_frame(1.0);
}

void VisualizerGrid::paintGLTranslucent()
{
}

void VisualizerGrid::update()
{
  emit stateChanged();
}

} // namespace