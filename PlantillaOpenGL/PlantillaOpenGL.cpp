// PlantillaOpenGL.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

using namespace std;

float xTriangulo = 0.0f;
float yTriangulo = 0.0f;

float xCuadrado = 0.7f;
float yCuadrado = -0.8f;

float velocidad = 0.5f;

double tiempoAnterior = 0.0;
double tiempoActual = 0.0;
double tiempoDiferencial = 0.0;

float rTriangulo = 1.0f;
float gTriangulo = 1.0f;
float bTriangulo = 1.0f;

//Creamos una ventana
GLFWwindow *window;

void dibujarTriangulo() {
	glPushMatrix();


	//Transformaciones
	glTranslatef(xTriangulo, yTriangulo, 0.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(rTriangulo, gTriangulo, bTriangulo);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glEnd();
	glPopMatrix();
}

void dibujarCuadrado() {
	glPushMatrix();
	//Transformaciones
	glTranslatef(xCuadrado, yCuadrado, 0.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.4f, 0.5f);
	glVertex3f(-0.1f, 0.1, 0.0f);
	glVertex3f(-0.1f, -0.1, 0.0f);
	glVertex3f(0.1f, -0.1, 0.0f);
	glVertex3f(0.1f, 0.1, 0.0f);
	glEnd();
}
void moverTriangulo() {
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	int estadoTeclaAbajo = glfwGetKey(window, GLFW_KEY_DOWN);
	if (estadoTeclaDerecha == GLFW_PRESS) {
		xTriangulo += velocidad * tiempoDiferencial;
	}
	if (estadoTeclaIzquierda == GLFW_PRESS) {
		xTriangulo -= velocidad * tiempoDiferencial;
	}
	if (estadoTeclaArriba == GLFW_PRESS) {
		yTriangulo += velocidad * tiempoDiferencial;
	}
	if (estadoTeclaAbajo == GLFW_PRESS) {
		yTriangulo -= velocidad * tiempoDiferencial;
	}
}

void dibujar(){
	dibujarTriangulo();
	dibujarCuadrado();
}

void checarColisiones() {
	// si el triangulo colisiciona con el cuadrado
	if (xTriangulo + 0.2f < xCuadrado + 0.1 &&
		xTriangulo - 0.2f > xCuadrado - 0.1f &&
		yTriangulo - 0.2f < yCuadrado + 0.1f &&
		yTriangulo + 0.2f > yCuadrado + 0.1f) {
		//cout << "Colision en x" << endl;
		gTriangulo = 0.0f;
		bTriangulo = 0.0f;
	}
	else {
		gTriangulo = 1.0f;
		bTriangulo = 1.0f;

		xCuadrado = (rand() % 2) - 0.8;
		yCuadrado = (rand() % 2) - 0.8;
		
	}
}

void actualizar() {
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;
	moverTriangulo();
	checarColisiones();
	tiempoAnterior = tiempoActual;
}

void Teclado_callback(GLFWwindow *window, int key, int scancodde, int action, int mods) {

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		xTriangulo += velocidad;
	}
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		xTriangulo -= velocidad;
	}
	else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		yTriangulo += velocidad;
	}
	else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		yTriangulo -= velocidad;
	}
}

int main()
{

	//Si no se puede inicializar glfw
	//Iniciamos ejecucion
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	//Si se pudo iniciar GLFW
	//entonces inicializamosla ventana
	window = glfwCreateWindow(1024, 768, "Ventana", NULL, NULL);
	//Si nopodemos iniciar la ventana
	//Entonces terminamos ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido el contexto activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK) {
		cout << glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL;

	//glfwSetKeyCallback(window, Teclado_callback);

	tiempoAnterior = glfwGetTime();

	//ciclode dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		//Establecer region de dibujo
		glViewport(0, 0, 1024, 768);
		//Establece el color de borrado
		glClearColor(0.3, 1, 0.8, 1);
		//Borramos
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();

		//Cambiar los buffers
		glfwSwapBuffers(window);
		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}