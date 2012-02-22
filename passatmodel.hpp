#ifndef PASSATMODEL_H
#define PASSATMODEL_H

#include <GL/gl.h>
#include <GL/glu.h>

namespace Gui3DQt {
  namespace PassatModel {

    //! A maximum of this number of passat models can exist with different colors
    const unsigned int PASSAT_MODEL_COUNT = 12;
    
    //! The following function can be used to explicitly set the color of a specific model
    void setColor(unsigned int model_index, double r, double g, double b);

    /*! Draws a passat model (with default color if not explicitly specified before)
     *  The geometrical center will be at 0/0/0. To change that, use
     *  glPushMatrix(), glTranslatef(), glRotatef(), draw(), glPopMatrix()
     *  Some default translation parameters are given below
     */
    void draw(unsigned int model_index = 0, double wheel_angle_rad = 0., double velodyne_angle_rad = 0.);

    const double translate_to_velobase_x = 0.08;
    const double translate_to_velobase_y = 0.0;
    const double translate_to_velobase_z = -0.7;
    const double translate_to_rearaxle_x = 1.33; //glTranslatef(0.1, 0, -1.0);
    const double translate_to_rearaxle_y = 0.0; //VISU_3D_SHIFTUP = 1.9f;
    const double translate_to_rearaxle_z = 0.55;
    const double translate_to_groundcenter_x = 0.0; //glTranslatef(0.1, 0, -1.0);
    const double translate_to_groundcenter_y = 0.0;
    const double translate_to_groundcenter_z = 0.88;
    
  }
}

#endif
