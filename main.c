#include <windows.h>	//Inclusion de variables de windows
#include <glut.h>	//Inclusion de GLUT mira que la tienes ahi
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	
#include "castillo.h"
#include "cuerposGeometricos.h"


//Para la texturizacion
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int W_WIDTH = 500;	 //Ancho de la ventana
int W_HEIGHT = 500;		//Alto de la ventana

#define GL_PI 3.14f
//Milisegundos que tarda en redibujar
#define MYTIEMPO 41

// �ngulos de rotaci�n para la c�mara
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

int agua[16];
int agua_indice = 0;

float Rot = 0;
camara = 0;
sueloScale = 50;
int cuadrado = 0, cono = 0, cilindro = 0, rectangulo = 0, cubo = 0, esfera = 0, piramide;

int valorCutOff = 33;

const int SLICES = 32;
const int STACKS = 32;

void Idle();
void reshape(int width, int height);
void skyBox();

//Texturas paisaje
int hierba = 0, tejado = 0, muro = 0, cielo = 0, tejadoCasa[5], muroCasa[5];

int flag = 0;

// GL_LIGHT1
GLfloat ambient_1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat diffuse_1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specular_1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat luzPos_1[4] = { 0.0f, 0.0f, 30.0f, 1.0f };
GLfloat spotDir_1[3] = { 0.0f, 0.0f, -1.0f };
GLfloat specRef_1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


//Asigno la camara a cada caso
void onMenu(int opcion) {
	//En funcion de la opcion selecciono la camara que quiero
	switch (opcion) {
	case 1:
		camara = 1;
		break;

	case 2:
		camara = 2;
		break;
	}
	glutPostRedisplay();
}

//Creo el menu
void myMenu(void) {
	int menuFondo = 0;

	menuFondo = glutCreateMenu(onMenu);

	glutAddMenuEntry("Voyayer", 1); //La sonda que nos muestra todo el sistema
	glutAddMenuEntry("Cubo", 2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* funcion que dibuja los ejes*/
void myEjes() {

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(.75f, 0.0f, 0.0f);
	glEnd();


	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, .75f, 0.0f);
	glEnd();


	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.f, 0.0f, .75f);
	glEnd();

	//Diagonal
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(.35f, .35, .35f);
	glEnd();

}

//Lista del cono
int myCono() {
	int indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);


	GLUquadric* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);

	// Enable texture coordinates
	gluQuadricTexture(quadric, GL_TRUE);

	glPushMatrix();
	gluCylinder(quadric, 0.0f, 1.0f, 1.0f, SLICES, STACKS);
	glPopMatrix();

	gluDeleteQuadric(quadric);
	glEndList();
	return indice;
}

// Lista piramide

//Lista del Cilindro
int myCilindro() {
	int indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);
	glEnable(GL_BLEND);

	GLUquadric* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);

	// Enable texture coordinates
	gluQuadricTexture(quadric, GL_TRUE);

	glPushMatrix();
	gluCylinder(quadric, 1.0f, 1.0f, 1.0f, SLICES, STACKS);
	glPopMatrix();

	gluDeleteQuadric(quadric);
	glEndList();
	return indice;
}

//Lista del subo
int myRectangulo() {
	int indice;
	indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);

	// Aplicar la textura en el cuadrado
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, muro);

	glBegin(GL_TRIANGLES);
	// CUADRADO 1
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, -.5f, -.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, -.5f, -.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, -.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, -.5f, -.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, .5f, -.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, -.5f);

	// CUADRADO 2
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, -.5f, .5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, -.5f, .5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, .5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, .5f, .5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, -.5f, .5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, .5f);

	// CUADRADO 3
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.5f, .5f, -.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, -.5f, -.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-.5f, .5f, .5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, -.5f, .5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, .5f, .5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.5f, -.5f, -.5f);

	// CUADRADO 4
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, -.5f, -.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, -.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(.5f, .5f, .5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(.5f, .5f, .5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(.5f, -.5f, .5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, -.5f, -.5f);

	// CUADRADO 5
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, -.5f, -.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, -.5f, .5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, -.5f, .5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, -.5f, .5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, -.5f, -.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, -.5f, -.5f);

	// CUADRADO 6
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, .5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(.5f, .5f, -.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, .5f, -.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, .5f, -.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, .5f, .5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(.5f, .5f, .5f);

	glEnd();


	// Aplicar la textura en el cuadrado
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();
	return indice;
}

//Funcion para dibujar el suelo
void dibujaSuelo() {

	for (int i = -1500; i <= 1500; i += sueloScale)
		for (int j = -1500; j <= 1500; j += sueloScale) {
			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			glTranslatef(i, j, 0);

			glScalef(sueloScale, sueloScale, sueloScale);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, hierba);
			glCallList(cuadrado);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
		}
}

void dibujaMuros() {
	//Muro Frontal 
	glPushMatrix();
	glTranslatef(0, 0, 100);
	glScalef(175, 50, 20);
	//Lo roto para ponerlo de pie
	glRotatef(-90.0f, 1, 0, 0);
	glCallList(rectangulo);
	glPopMatrix();

	//Muro Lateral Izq
	glPushMatrix();
	glTranslatef(-100, 0, 0);
	glRotatef(-90.0f, 0, 1, 0);
	glScalef(175, 50, 20);
	//Lo roto para ponerlo de pie
	glRotatef(-90.0f, 1, 0, 0);
	glCallList(rectangulo);
	glPopMatrix();

	//Muro Lateral Dcho
	glPushMatrix();
	glTranslatef(100, 0, 0);
	glRotatef(-90.0f, 0, 1, 0);
	glScalef(175, 50, 20);
	//Lo roto para ponerlo de pie
	glRotatef(-90.0f, 1, 0, 0);
	glCallList(rectangulo);
	glPopMatrix();

	//Muro Trasero 
	glPushMatrix();
	glTranslatef(0, 0, -100);
	glScalef(175, 50, 20);
	//Lo roto para ponerlo de pie
	glRotatef(-90.0f, 1, 0, 0);
	glCallList(rectangulo);
	glPopMatrix();
}

void dibujaTorre() {

	//Torre frontal 1: paredes
	glPushMatrix();
	glTranslatef(100, -25, 100);
	glScalef(25, 100, 25);
	//Lo roto para ponerlo de pie
	glRotatef(-90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, muro);
	glCallList(cilindro);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre frontal 1: tejado
	glPushMatrix();
	glTranslatef(100, 100, 100);
	glScalef(35, 35, 35);
	//Lo roto para ponerlo de pie
	glRotatef(90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tejado);
	glCallList(cono);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre frontal 2: paredes
	glPushMatrix();
	//traslado 200 en el eje x
	glTranslatef(-100, -25, 100);
	glScalef(25, 100, 25);
	glRotatef(-90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, muro);
	glCallList(cilindro);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre frontal 2: tejado
	glPushMatrix();
	glTranslatef(-100, 100, 100);
	glScalef(35, 35, 35);
	//Lo roto para ponerlo de pie
	glRotatef(90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tejado);
	glCallList(cono);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre trasera 1:paredes
	glPushMatrix();
	glTranslatef(100, -25, -100);
	glScalef(25, 100, 25);
	glRotatef(-90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, muro);
	glCallList(cilindro);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre trasera 1: tejado
	glPushMatrix();
	glTranslatef(100, 100, -100);
	glScalef(35, 35, 35);
	//Lo roto para ponerlo de pie
	glRotatef(90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tejado);
	glCallList(cono);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre trasera 2: paredes
	glPushMatrix();
	glTranslatef(-100, -25, -100);
	glScalef(25, 100, 25);
	glRotatef(-90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, muro);
	glCallList(cilindro);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre trasera 2: tejado
	glPushMatrix();
	glTranslatef(-100, 100, -100);
	glScalef(35, 35, 35);
	//Lo roto para ponerlo de pie
	glRotatef(90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tejado);
	glCallList(cono);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void dibujaCastillo() {

	//Muro Frontal 
	glPushMatrix();
		glTranslatef(0, 25, 0);
		glScalef(100, 100, 100);
		//Lo roto para ponerlo de pie
		glRotatef(-90.0f, 1, 0, 0);
		glCallList(rectangulo);
	glPopMatrix();

	//Torre frontal 1: paredes
	glPushMatrix();
		glTranslatef(0, 50, 0);
		glScalef(25, 100, 25);
		//Lo roto para ponerlo de pie
		glRotatef(-90.0f, 1, 0, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, muro);
		glCallList(cilindro);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre frontal 1: tejado
	glPushMatrix();
		glTranslatef(0, 180, 0);
		glScalef(35, 35, 35);
		glLightfv(GL_LIGHT1, GL_POSITION, luzPos_1);
		//Lo roto para ponerlo de pie
		glRotatef(90.0f, 1, 0, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tejado);
		glCallList(cono);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, valorCutOff);
}

void dibujaCasa(int posicion_x, int posicion_z, int muro, int tejado) {

	//Torre frontal 1: paredes
	glPushMatrix();
	glTranslatef(posicion_x, -25, posicion_z);
	glScalef(35, 50, 35);
	//Lo roto para ponerlo de pie
	glRotatef(-90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, muroCasa[muro]);
	glCallList(cilindro);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Torre frontal 1: tejado
	glPushMatrix();
	glTranslatef(posicion_x, 60, posicion_z);
	glScalef(45, 45, 45);
	//Lo roto para ponerlo de pie
	glRotatef(90.0f, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tejadoCasa[tejado]);
	glCallList(cono);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void myMovimiento() {
	Rot += 5.0f;
	if (Rot > 360) Rot -= 360;

	glutPostRedisplay(); //se vuelve a dibujar
	glutTimerFunc(MYTIEMPO, myMovimiento, 0);//se vuelve a ejecutar myMovimiento
}

// Funcion de dibujo
void myDisplay(void) {
	// Clear the window with current clearing color


	myCamara(W_WIDTH, W_HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Matriz del Modelo
	//glLoadIdentity(); // Inicializamos la matriz del modelo a la identidad
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	skyBox();
	dibujaSuelo();
	srand(33);
	
	glPushMatrix();
		glTranslatef(0, 30, 0);
		glPushMatrix();
			dibujaMuros();
			dibujaTorre();
			dibujaCastillo();
			for (int i = 0; i < 5; i++) {
				dibujaCasa(-300 + (-100 * i), 200, i, i);
			}
			
		glPopMatrix();
	glPopMatrix();
	dibujaAgua();
	myEjes();

	glFlush();
	glutSwapBuffers();


}

int myCargarTexturas(char* name) {
	int textura;

	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(name, &width, &height, &nrChannels, 0);

	if (data) {
		if (nrChannels == 3) { // hierba
			//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) { // agua, tiene 4 por canal alpha
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
	}

	stbi_image_free(data);

	return textura;
}


void skyBox() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); // Habilita la ocultacion de caras
	glDisable(GL_NORMALIZE);

	glPushMatrix();

	glScalef(1200, 1200, 1200);
	glTranslatef(0, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cielo);
	glCallList(esfera);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // Habilita la ocultacion de caras
	glEnable(GL_NORMALIZE);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Castillo");

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE); // Habilita la ocultacion de caras
	glEnable(GL_NORMALIZE);

	//Eventos comentado hasta la siguiente practica
	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	glutDisplayFunc(myDisplay);
	// Funcion de actualizacion
	glutIdleFunc(Idle);
	// Funci�n de devoluci�n de llamada para el cambio de tama�o de la ventana
	glutReshapeFunc(reshape);

	//Habilito las texturas
	// glShadeModel(GL_SMOOTH);

	hierba = myCargarTexturas("hierba.jpg");
	tejado = myCargarTexturas("roof.jpg");
	muro = myCargarTexturas("muro.jpg");
	cielo = myCargarTexturas("cielo.jpg");

	muroCasa[0] = myCargarTexturas("muroCasa0.jpg");
	tejadoCasa[0] = myCargarTexturas("tejadoCasa0.jpg");
	muroCasa[1] = myCargarTexturas("muroCasa1.jpg");
	tejadoCasa[1] = myCargarTexturas("tejadoCasa1.jpg");
	muroCasa[2] = myCargarTexturas("muroCasa2.jpg");
	tejadoCasa[2] = myCargarTexturas("tejadoCasa2.jpg");
	muroCasa[3] = myCargarTexturas("muroCasa3.jpg");
	tejadoCasa[3] = myCargarTexturas("tejadoCasa3.jpg");
	muroCasa[4] = myCargarTexturas("muroCasa4.jpg");
	tejadoCasa[4] = myCargarTexturas("tejadoCasa4.jpg");
	// lago
	agua[0] = myCargarTexturas("caust00.png");
	agua[1] = myCargarTexturas("caust01.png");
	agua[2] = myCargarTexturas("caust02.png");
	agua[3] = myCargarTexturas("caust03.png");
	agua[4] = myCargarTexturas("caust04.png");
	agua[5] = myCargarTexturas("caust05.png");
	agua[6] = myCargarTexturas("caust06.png");
	agua[7] = myCargarTexturas("caust07.png");
	agua[8] = myCargarTexturas("caust08.png");
	agua[9] = myCargarTexturas("caust09.png");
	agua[10] = myCargarTexturas("caust10.png");
	agua[11] = myCargarTexturas("caust11.png");
	agua[12] = myCargarTexturas("caust12.png");
	agua[13] = myCargarTexturas("caust12.png");
	agua[14] = myCargarTexturas("caust14.png");
	agua[15] = myCargarTexturas("caust15.png");

	//myMovimiento();
	moverAgua();
	myMenu();
	myIluminacion();

	// cubo = myCubo(cielo);
	cuadrado = myCuadrado(hierba);
	cilindro = myCilindro();
	cono = myCono();
	rectangulo = myRectangulo();
	esfera = myEsfera();

	// Empieza en bucle principal
	glutMainLoop();
	return 0;
}

void reshape(int width, int height) {
	W_WIDTH = width;
	W_HEIGHT = height;

	// Ajusta la vista a las dimensiones de la ventana
	glViewport(0, 0, width, height);

	// Reinicia el sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establece el volumen de trabajo
	gluPerspective(45.0f, (GLdouble)width / height, 1.0f, 500.0f);
}

void Idle() {
	glutPostRedisplay();
}