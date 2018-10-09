#include <stdio.h>
#include "gl/glut.h" 
#include "visuals.h"

int main(int argc, char* argv[])
{ 
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);

  glutInitWindowSize(600,400);
  glutInitWindowPosition(100,100);

  glutCreateWindow("Hliako_Systhma");
  

  Setup();
  
  glutDisplayFunc(Render);
  glutReshapeFunc(Resize);
  glutIdleFunc(Idle);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();
  return 0;	
}  
