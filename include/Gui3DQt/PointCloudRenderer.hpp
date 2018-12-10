#ifndef POINTCLOUDRENDERER_HPP
#define POINTCLOUDRENDERER_HPP

#include <vector>
#include <GL/glut.h>

namespace Gui3DQt {

/*! \class PointCloudRenderer
 *
 *  \brief Class for efficiently rendering huge point clouds with OpenGL, class also serves as storage
 *
 *  TODO: simply uses glVertexPointer/glColorPointer which limits this class to be only instanciated once! Hence, switch to ARB_vertex_buffer_object to bypass this limitation
 *  TODO: possible extension: template this class so that additional attributes can be stored along with the points/colors
 */
class PointCloudRenderer
{
public:
    PointCloudRenderer();
    ~PointCloudRenderer();

    struct GlVec3 { // 3D-coord
      GlVec3() : x(0), y(0), z(0) {};
      GlVec3(float px, float py, float pz) : x(px), y(py), z(pz) {};
      template <class DVec>
      GlVec3(const DVec &v) : x(v[0]), y(v[1]), z(v[2]) {};
      GLfloat x;
      GLfloat y;
      GLfloat z;
    };
    struct GlCol3 { // 3D-color
      GlCol3() : r(0), g(0), b(0) {};
      GlCol3(float cr, float cg, float cb) : r(cr), g(cg), b(cb) {};
      GLubyte r;
      GLubyte g;
      GLubyte b;
    };

    void render(GLfloat ptSize = 1, std::vector<GLuint> *indices = NULL);
    size_t size() const;
    void clear();
    void reserve(size_t number);
    void push_back(GlVec3 point, GlCol3 color);
    void push_back(float x, float y, float z, int r, int g, int b);
    GlVec3& pointAt(int index); //!< use to get or set point coordinate
    GlCol3& colorAt(int index); //!< use to get or set color

private:
    std::vector<GlVec3> point3d; // continuous memory-buffer for points to draw with OpenGL
    std::vector<GlCol3> color3f; // continuous memory-buffer for colors to draw with OpenGL
    std::vector<GLuint> indices; // list-like object containing which entries/indices of the OpenGL buffers to draw
};

} // end namespace

#endif // POINTCLOUDRENDERER_HPP
