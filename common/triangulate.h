#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include <common/global.h>

class Triangulate
{
public:

  // triangulate a contour/polygon, places results in STL vector
  // as series of triangles.
  static vec4List Process(const vec4List &contour);

  // compute area of a contour/polygon
  static float Area(const vec4List &contour);

  // decide if point Px/Py is inside triangle defined by
  // (Ax,Ay) (Bx,By) (Cx,Cy)
  static bool InsideTriangle(float Ax, float Ay,
                      float Bx, float By,
                      float Cx, float Cy,
                      float Px, float Py);


private:
  static bool Snip(const vec4List &contour,int u,int v,int w,int n,int *V);

};

#endif // TRIANGULATE_H
