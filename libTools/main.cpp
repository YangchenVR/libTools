#include <igl/colon.h>
#include <igl/harmonic.h>
#include <igl/readOBJ.h>
#include <igl/readDMAT.h>
#include <igl/viewer/Viewer.h>
#include <algorithm>
#include <iostream>
#include "VR_Global_Define.h"

#define MyMeshPath "D:/MyWorkspace/MyMesh/OBJ/"
YC::MyDenseVector purple(80.0 / 255.0, 64.0 / 255.0, 255.0 / 255.0);
YC::MyDenseVector gold(255.0 / 255.0, 228.0 / 255.0, 58.0 / 255.0);


bool deformation_field = false;
YC::MyMatrix V, U, V_disp;
YC::MyVector Z;
YC::MyIntMatrix F;
YC::MyIntVector b;
YC::MyInt g_iVtxSize = 0;

void computeVertexDisp(const YC::MyVector& dispVec, YC::MyMatrix& dispMat, const YC::MyInt nVtxSize)
{
}

bool pre_draw(igl::viewer::Viewer & viewer)
{
  using namespace Eigen;
  // Determine boundary conditions
  if(viewer.core.is_animating)
  {
    
  }
  
  viewer.data.set_vertices(U);
  viewer.data.compute_normals();
  return false;
}

bool key_down(igl::viewer::Viewer &viewer, unsigned char key, int mods)
{
  switch(key)
  {
    case ' ':
      viewer.core.is_animating = !viewer.core.is_animating;
      return true;
    case 'D':
    case 'd':
      deformation_field = !deformation_field;
      return true;
  }
  return false;
}

int main(int argc, char *argv[])
{
  using namespace Eigen;
  using namespace std;
  igl::readOBJ(MyMeshPath "Bunny_3w.obj", V, F);
  U=V;
  

  // Pseudo-color based on selection
  YC::MyMatrix C(F.rows(), 3);
  for(int f = 0;f<F.rows();f++)
  {
	  C.row(f) = gold;
  }

  // Plot the mesh with pseudocolors
  igl::viewer::Viewer viewer;
  viewer.data.set_mesh(U, F);
  viewer.core.show_lines = true;
  viewer.core.orthographic = true;
  viewer.data.set_colors(C);
  viewer.core.trackball_angle = Eigen::Quaternionf(sqrt(2.0),0,sqrt(2.0),0);
  viewer.core.trackball_angle.normalize();
  viewer.callback_pre_draw = &pre_draw;
  viewer.callback_key_down = &key_down;
  //viewer.core.is_animating = true;
  viewer.core.animation_max_fps = 30.;
  cout<<
    "Press [space] to toggle deformation."<<endl<<
    "Press 'd' to toggle between biharmonic surface or displacements."<<endl;
  viewer.launch();
}
