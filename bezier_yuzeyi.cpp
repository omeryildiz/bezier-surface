/*
Künye		: Ömer YILDIZ
Okul No		: 2009010205012
IDE		: Eclipse
İşletim Sistemi : Ubuntu
*/
#define __GLUTMOUSE__
#include "mouse.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <stdio.h>

int line = 0;
static GLfloat xRot = 0.0f;
static GLfloat zRot = 0.0f;
static float Ww,Hh;
float selectionSpace = 0.5;
mouse_t mouse;
GLfloat nRange = 10.0f;

// 16 kontrol noktası belirliyoruz
   GLfloat Ax = 2.0; GLfloat Ay =  0.0; GLfloat Az =  1.0;
   GLfloat Bx = 3.0; GLfloat By =  2.0; GLfloat Bz = -1.0;
   GLfloat Cx =  6.0; GLfloat Cy =  2.0; GLfloat Cz =  1.0;
   GLfloat Dx =  9.0; GLfloat Dy =  0.0; GLfloat Dz = -1.0;

   GLfloat Ex = 0.0; GLfloat Ey =  3.0; GLfloat Ez =  1.0;
   GLfloat Fx = 3.0; GLfloat Fy =  4.0; GLfloat Fz = -1.0;
   GLfloat Gx =  6.0; GLfloat Gy =  4.0; GLfloat Gz =  1.0;
   GLfloat Hx =  10.0; GLfloat Hy =  3.0; GLfloat Hz = -1.0;

   GLfloat Ix = 0.0; GLfloat Iy = 5.0; GLfloat Iz =  1.0;
   GLfloat Jx = 3.0; GLfloat Jy = 6.0; GLfloat Jz = -1.0;
   GLfloat Kx =  6.0; GLfloat Ky = 6.0; GLfloat Kz =  1.0;
   GLfloat Lx =  10.0; GLfloat Ly = 5.0; GLfloat Lz = -1.0;

   GLfloat Mx = 1.0; GLfloat My = 9.0; GLfloat Mz =  1.0;
   GLfloat Nx = 3.0; GLfloat Ny = 8.0; GLfloat Nz = -1.0;
   GLfloat Ox =  6.0; GLfloat Oy = 8.0; GLfloat Oz =  1.0;
   GLfloat Px =  7.0; GLfloat Py = 9.0; GLfloat Pz = -1.0;


void Menu(int selection){
	if(selection == 1)
		line = !line;

	glutPostRedisplay();

}


void displayFcn (void)
{

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	//x ve z koordinatına göre döndürme işlemlerini yapıyoruz.
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	if(line){
	glPolygonMode(GL_BACK,GL_LINE);
	glPolygonMode(GL_FRONT,GL_LINE);
	}
	else
	{
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	}

	glColor3f(1,1,1);




    // detail, detaillandırmayı sağlamaktadır. Eğer 3 seçerseniz 4'e 4 bir yüzey çizer
    int    detail = 3;
    GLfloat swapDetail = 1.0 / (GLfloat)detail;

    // Köşeler (maksimum detail 20·20 )
    GLfloat Xv[21][21];
    GLfloat Yv[21][21];
    GLfloat Zv[21][21];

    // Normal vektörler
    GLfloat Xn[21][21];
    GLfloat Yn[21][21];
    GLfloat Zn[21][21];

    // detail 20'yi aşmamalı
    if(detail > 20)
    {
      detail = 20;
    }

    //  Değişkenler
    GLfloat a = 1.0;
    GLfloat b = 1.0 - a;
    GLfloat c = 1.0;
    GLfloat d = 1.0 - c;

    // Tanjant vektörleri
    GLfloat Xta;
    GLfloat Yta;
    GLfloat Zta;

    GLfloat Xtc;
    GLfloat Ytc;
    GLfloat Ztc;

    for(int i = 0; i <= detail; i++)
    {
      for(int j = 0; j <= detail; j++)
      {
        // İlk köşeler
        Xv[i][j] = Ax*a*a*a*c*c*c   + Bx*3*a*a*a*c*c*d
             + Cx*3*a*a*a*c*d*d + Dx*a*a*a*d*d*d
             + Ex*3*a*a*b*c*c*c + Fx*9*a*a*b*c*c*d
             + Gx*9*a*a*b*c*d*d + Hx*3*a*a*b*d*d*d
             + Ix*3*a*b*b*c*c*c + Jx*9*a*b*b*c*c*d
             + Kx*9*a*b*b*c*d*d + Lx*3*a*b*b*d*d*d
             + Mx*b*b*b*c*c*c   + Nx*3*b*b*b*c*c*d
             + Ox*3*b*b*b*c*d*d + Px*b*b*b*d*d*d;




        Yv[i][j] = Ay*a*a*a*c*c*c   + By*3*a*a*a*c*c*d
             + Cy*3*a*a*a*c*d*d + Dy*a*a*a*d*d*d
             + Ey*3*a*a*b*c*c*c + Fy*9*a*a*b*c*c*d
             + Gy*9*a*a*b*c*d*d + Hy*3*a*a*b*d*d*d
             + Iy*3*a*b*b*c*c*c + Jy*9*a*b*b*c*c*d
             + Ky*9*a*b*b*c*d*d + Ly*3*a*b*b*d*d*d
             + My*b*b*b*c*c*c   + Ny*3*b*b*b*c*c*d
             + Oy*3*b*b*b*c*d*d + Py*b*b*b*d*d*d;



        Zv[i][j] = Az*a*a*a*c*c*c   + Bz*3*a*a*a*c*c*d
             + Cz*3*a*a*a*c*d*d + Dz*a*a*a*d*d*d
             + Ez*3*a*a*b*c*c*c + Fz*9*a*a*b*c*c*d
             + Gz*9*a*a*b*c*d*d + Hz*3*a*a*b*d*d*d
             + Iz*3*a*b*b*c*c*c + Jz*9*a*b*b*c*c*d
             + Kz*9*a*b*b*c*d*d + Lz*3*a*b*b*d*d*d
             + Mz*b*b*b*c*c*c   + Nz*3*b*b*b*c*c*d
             + Oz*3*b*b*b*c*d*d + Pz*b*b*b*d*d*d;


        // a'ya göre türevi alınmış fonksiyon
        Xta = Ax*3*a*a*c*c*c       + Bx*9*a*a*c*c*d
            + Cx*9*a*a*c*d*d       + Dx*3*a*a*d*d*d
            + Ex*3*(2*a-3*a*a)*c*c*c   + Fx*9*(2*a-3*a*a)*c*c*d
            + Gx*9*(2*a-3*a*a)*c*d*d   + Hx*3*(2*a-3*a*a)*d*d*d
            + Ix*3*(1-4*a+3*a*a)*c*c*c + Jx*9*(1-4*a+3*a*a)*c*c*d
            + Kx*9*(1-4*a+3*a*a)*c*d*d + Lx*3*(1-4*a+3*a*a)*d*d*d
            + Mx*3*(2*a-1-a*a)*c*c*c   + Nx*9*(2*a-1-a*a)*c*c*d
            + Ox*9*(2*a-1-a*a)*c*d*d   + Px*3*(2*a-1-a*a)*d*d*d;

        Yta = Ay*3*a*a*c*c*c       + By*9*a*a*c*c*d
            + Cy*9*a*a*c*d*d       + Dy*3*a*a*d*d*d
            + Ey*3*(2*a-3*a*a)*c*c*c   + Fy*9*(2*a-3*a*a)*c*c*d
            + Gy*9*(2*a-3*a*a)*c*d*d   + Hy*3*(2*a-3*a*a)*d*d*d
            + Iy*3*(1-4*a+3*a*a)*c*c*c + Jy*9*(1-4*a+3*a*a)*c*c*d
            + Ky*9*(1-4*a+3*a*a)*c*d*d + Ly*3*(1-4*a+3*a*a)*d*d*d
            + My*3*(2*a-1-a*a)*c*c*c   + Ny*9*(2*a-1-a*a)*c*c*d
            + Oy*9*(2*a-1-a*a)*c*d*d   + Py*3*(2*a-1-a*a)*d*d*d;

        Zta = Az*3*a*a*c*c*c       + Bz*9*a*a*c*c*d
            + Cz*9*a*a*c*d*d       + Dz*3*a*a*d*d*d
            + Ez*3*(2*a-3*a*a)*c*c*c   + Fz*9*(2*a-3*a*a)*c*c*d
            + Gz*9*(2*a-3*a*a)*c*d*d   + Hz*3*(2*a-3*a*a)*d*d*d
            + Iz*3*(1-4*a+3*a*a)*c*c*c + Jz*9*(1-4*a+3*a*a)*c*c*d
            + Kz*9*(1-4*a+3*a*a)*c*d*d + Lz*3*(1-4*a+3*a*a)*d*d*d
            + Mz*3*(2*a-1-a*a)*c*c*c   + Nz*9*(2*a-1-a*a)*c*c*d
            + Oz*9*(2*a-1-a*a)*c*d*d   + Pz*3*(2*a-1-a*a)*d*d*d;

        Xtc = Ax*3*a*a*a*c*c       + Bx*3*a*a*a*(2*c-3*c*c)
            + Cx*3*a*a*a*(1-4*c+3*c*c) + Dx*3*a*a*a*(-1+2*c-c*c)
            + Ex*9*a*a*b*c*c       + Fx*9*a*a*b*(2*c-3*c*c)
            + Gx*9*a*a*b*(1-4*c+3*c*c) + Hx*9*a*a*b*(-1+2*c-c*c)
            + Ix*9*a*b*b*c*c       + Jx*9*a*b*b*(2*c-3*c*c)
            + Kx*9*a*b*b*(1-4*c+3*c*c) + Lx*9*a*b*b*(-1+2*c-c*c)
            + Mx*3*b*b*b*c*c       + Nx*3*b*b*b*(2*c-3*c*c)
            + Ox*3*b*b*b*(1-4*c+3*c*c) + Px*3*b*b*b*(-1+2*c-c*c);

        Ytc = Ay*3*a*a*a*c*c       + By*3*a*a*a*(2*c-3*c*c)
            + Cy*3*a*a*a*(1-4*c+3*c*c) + Dy*3*a*a*a*(-1+2*c-c*c)
            + Ey*9*a*a*b*c*c       + Fy*9*a*a*b*(2*c-3*c*c)
            + Gy*9*a*a*b*(1-4*c+3*c*c) + Hy*9*a*a*b*(-1+2*c-c*c)
            + Iy*9*a*b*b*c*c       + Jy*9*a*b*b*(2*c-3*c*c)
            + Ky*9*a*b*b*(1-4*c+3*c*c) + Ly*9*a*b*b*(-1+2*c-c*c)
            + My*3*b*b*b*c*c       + Ny*3*b*b*b*(2*c-3*c*c)
            + Oy*3*b*b*b*(1-4*c+3*c*c) + Py*3*b*b*b*(-1+2*c-c*c);

        Ztc = Az*3*a*a*a*c*c       + Bz*3*a*a*a*(2*c-3*c*c)
            + Cz*3*a*a*a*(1-4*c+3*c*c) + Dz*3*a*a*a*(-1+2*c-c*c)
            + Ez*9*a*a*b*c*c       + Fz*9*a*a*b*(2*c-3*c*c)
            + Gz*9*a*a*b*(1-4*c+3*c*c) + Hz*9*a*a*b*(-1+2*c-c*c)
            + Iz*9*a*b*b*c*c       + Jz*9*a*b*b*(2*c-3*c*c)
            + Kz*9*a*b*b*(1-4*c+3*c*c) + Lz*9*a*b*b*(-1+2*c-c*c)
            + Mz*3*b*b*b*c*c       + Nz*3*b*b*b*(2*c-3*c*c)
            + Oz*3*b*b*b*(1-4*c+3*c*c) + Pz*3*b*b*b*(-1+2*c-c*c);

        // Normal vektörü bul
        // Bu formülden geliyor
        Xn[i][j] = (Yta*Ztc) - (Ytc*Zta);

        Yn[i][j] = (Xtc*Zta) - (Xta*Ztc);

        Zn[i][j] = (Xta*Ytc) - (Xtc*Yta);


        // normal vektörün uzunluğunu bul
        GLfloat lengthVector = sqrt((Xn[i][j]*Xn[i][j])+(Yn[i][j]
                         *Yn[i][j])+(Zn[i][j]*Zn[i][j]));

        // Normalize et (ve sıfırla bölmeyi engelle)
        if(lengthVector > 0)
        {
          lengthVector = 1.0/lengthVector;
          Xn[i][j] *= lengthVector;

          Yn[i][j] *= lengthVector;

          Zn[i][j] *= lengthVector;

        }

        //döngü içinde c ve d değişimi
        c -= swapDetail;
        d  = 1.0 - c;
      }
      //a değişkeni döngü dışında değerleniyor
      a -= swapDetail;
      b  = 1.0 - a;

      // c değişkeni tekrar döngüye girmek için resetleniyor
      c = 1.0;
      d = 1.0 - c;
    }

    glBegin(GL_LINES);
    glVertex2d(-10,0);
    glVertex2d(10,0);
    glVertex2d(0,10);
    glVertex2d(0,-10);
    glEnd();



    glColor3f(1,1,1);


    //Çizimin yapıldığı fonksiyon
    for(int m = 0; m < detail; m++)
    {
      glBegin(GL_TRIANGLE_STRIP);
      for(int n = 0; n <= detail; n++)
      {
    	  //Normallerin çizimle alakası, sonraki noktanın doğrultusunu belirtiyor olmasıdır. Çizime katılmazlar.
        glNormal3d(Xn[m][n],Yn[m][n],Zn[m][n]);
        glVertex3d(Xv[m][n],Yv[m][n],Zv[m][n]);



        //Burada Xn[m+1][n] alma sebebi GL_TRANGLE_STRIP kullanmamdandır. Bu şekilde yapılmazsa görüntü düzgün oluşmaz.
        glNormal3d(Xn[m+1][n],Yn[m+1][n],Zn[m+1][n]);
        glVertex3d(Xv[m+1][n],Yv[m+1][n],Zv[m+1][n]);
      }
      glEnd();

    }
    glColor3f(0,0,1);
        glPointSize(7);
        glBegin(GL_POINTS);
        glVertex3f(Ax,Ay,Az);glVertex3f(Bx,By,Bz);glVertex3f(Cx,Cy,Cz);glVertex3f(Dx,Dy,Dz);
        glVertex3f(Ex,Ey,Ez);glVertex3f(Fx,Fy,Fz);glVertex3f(Gx,Gy,Gz);glVertex3f(Hx,Hy,Hz);
        glVertex3f(Ix,Iy,Iz);glVertex3f(Jx,Jy,Jz);glVertex3f(Kx,Ky,Kz);glVertex3f(Lx,Ly,Lz);
        glVertex3f(Mx,My,Mz);glVertex3f(Nx,Ny,Nz);glVertex3f(Ox,Oy,Oz);glVertex3f(Px,Py,Pz);
        glEnd();
        glPopMatrix();

	glFlush ();
}

void glutResize (int w, int h)
{

		if(h == 0)
		h = 1;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w <= h)
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
		else
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
		glMatrixMode(GL_MODELVIEW);
		Ww=w;
		Hh=h;
		glLoadIdentity();
}

void Timer(int value)
	{
	glutTimerFunc(50,Timer, 0);
	glutPostRedisplay();
	}

void SpecialKeys(int key, int x, int y)
	{
	if(key == GLUT_KEY_UP)
	xRot-= 5.0f;
	if(key == GLUT_KEY_DOWN)
	xRot += 5.0f;
	if(key == GLUT_KEY_LEFT)
	zRot -= 5.0f;
	if(key == GLUT_KEY_RIGHT)
	zRot += 5.0f;
	glutPostRedisplay();
	}
void glutMouse(int button, int state, int x, int y)
{
	x = (x-Ww/2)/nRange;
	y = (Hh/2-y)/nRange;
   mouse.SetState(button, state);
   mouse.SetNewPos(x,y);
   printf("Mouse %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
}

void glutMotion(int x, int y)
{
	x = (x-Ww/2)/nRange;
	y = (Hh/2-y)/nRange;
   mouse.SetOldPos(mouse.GetX(), mouse.GetY());
   mouse.SetNewPos(x, y);
   if(mouse.GetOldX()>Ax-selectionSpace&&mouse.GetOldX()<Ax+selectionSpace&&mouse.GetOldY()>Ay-selectionSpace&&mouse.GetOldY()<Ay+selectionSpace ){
  	   Ax = mouse.GetX();
  	   Ay = mouse.GetY();
     }
   else if(mouse.GetOldX()>Bx-selectionSpace&&mouse.GetOldX()<Bx+selectionSpace&&mouse.GetOldY()>By-selectionSpace&&mouse.GetOldY()<By+selectionSpace ){
     	   Bx = mouse.GetX();
     	   By = mouse.GetY();
        }
   else  if(mouse.GetOldX()>Cx-selectionSpace&&mouse.GetOldX()<Cx+selectionSpace&&mouse.GetOldY()>Cy-selectionSpace&&mouse.GetOldY()<Cy+selectionSpace ){
  	   Cx = mouse.GetX();
     	   Cy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Dx-selectionSpace&&mouse.GetOldX()<Dx+selectionSpace&&mouse.GetOldY()>Dy-selectionSpace&&mouse.GetOldY()<Dy+selectionSpace ){
     	   Dx = mouse.GetX();
     	   Dy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Ex-selectionSpace&&mouse.GetOldX()<Ex+selectionSpace&&mouse.GetOldY()>Ey-selectionSpace&&mouse.GetOldY()<Ey+selectionSpace ){
     	   Ex = mouse.GetX();
     	   Ey = mouse.GetY();
        }
   else if(mouse.GetOldX()>Fx-selectionSpace&&mouse.GetOldX()<Fx+selectionSpace&&mouse.GetOldY()>Fy-selectionSpace&&mouse.GetOldY()<Fy+selectionSpace ){
     	   Fx = mouse.GetX();
     	   Fy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Gx-selectionSpace&&mouse.GetOldX()<Gx+selectionSpace&&mouse.GetOldY()>Gy-selectionSpace&&mouse.GetOldY()<Gy+selectionSpace ){
     	   Gx = mouse.GetX();
     	   Gy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Hx-selectionSpace&&mouse.GetOldX()<Hx+selectionSpace&&mouse.GetOldY()>Hy-selectionSpace&&mouse.GetOldY()<Hy+selectionSpace ){
     	   Hx = mouse.GetX();
     	   Hy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Ix-selectionSpace&&mouse.GetOldX()<Ix+selectionSpace&&mouse.GetOldY()>Iy-selectionSpace&&mouse.GetOldY()<Iy+selectionSpace ){
     	   Ix = mouse.GetX();
     	   Iy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Jx-selectionSpace&&mouse.GetOldX()<Jx+selectionSpace&&mouse.GetOldY()>Jy-selectionSpace&&mouse.GetOldY()<Jy+selectionSpace ){
     	   Jx = mouse.GetX();
     	   Jy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Kx-selectionSpace&&mouse.GetOldX()<Kx+selectionSpace&&mouse.GetOldY()>Ky-selectionSpace&&mouse.GetOldY()<Ky+selectionSpace ){
     	   Kx = mouse.GetX();
     	   Ky = mouse.GetY();
        }
   else if(mouse.GetOldX()>Lx-selectionSpace&&mouse.GetOldX()<Lx+selectionSpace&&mouse.GetOldY()>Ly-selectionSpace&&mouse.GetOldY()<Ly+selectionSpace ){
     	   Lx = mouse.GetX();
     	   Ly = mouse.GetY();
        }
   else if(mouse.GetOldX()>Mx-selectionSpace&&mouse.GetOldX()<Mx+selectionSpace&&mouse.GetOldY()>My-selectionSpace&&mouse.GetOldY()<My+selectionSpace ){
     	   Mx = mouse.GetX();
     	   My = mouse.GetY();
        }
   else if(mouse.GetOldX()>Nx-selectionSpace&&mouse.GetOldX()<Nx+selectionSpace&&mouse.GetOldY()>Ny-selectionSpace&&mouse.GetOldY()<Ny+selectionSpace ){
     	   Nx = mouse.GetX();
     	   Ny = mouse.GetY();
        }
   else if(mouse.GetOldX()>Ox-selectionSpace&&mouse.GetOldX()<Ox+selectionSpace&&mouse.GetOldY()>Oy-selectionSpace&&mouse.GetOldY()<Oy+selectionSpace ){
     	   Ox = mouse.GetX();
     	   Oy = mouse.GetY();
        }
   else if(mouse.GetOldX()>Px-selectionSpace&&mouse.GetOldX()<Px+selectionSpace&&mouse.GetOldY()>Py-selectionSpace&&mouse.GetOldY()<Py+selectionSpace ){
     	   Px = mouse.GetX();
     	   Py = mouse.GetY();
        }



   printf("Mouse Drag Button ");

   if (mouse.IsLeft())
      printf("Left\n");
   else if (mouse.IsRight())
      printf("Right\n");
   else
      printf("Middle\n");

   printf("\tFrom   %d %d\n", mouse.GetOldX(), mouse.GetOldY());
   printf("\tTo     %d %d\n", mouse.GetX(), mouse.GetY());
   printf("\tChange %d %d\n", mouse.GetDiffX(), mouse.GetDiffY());
}



int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Bezier yuzeyi");
	glutCreateMenu(Menu);
	glutAddMenuEntry("Cizgili/Cizgisiz",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(glutResize);
	glutSpecialFunc(SpecialKeys);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);
	//Timer fonksiyonu çizgili veya çizgisiz görünümün anında oluşmasını sağlamak için eklendi
	glutTimerFunc(50,Timer, 0);
	glutMainLoop();
	return 0;
}
