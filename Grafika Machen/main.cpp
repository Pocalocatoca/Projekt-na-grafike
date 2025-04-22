#include "objects.h"
#define ZAKRES 20
#define ZASIEG 30


const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

Vector3d cam_pos = { 0,0,0 };
Vector3d cam_rot = { 0,0,0 };

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 600, "Przykladowe Okno GLFW", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Object ent1(Entablature, { 0,PILLAR_HEIGHT,0 }, {}, { 12,1,10 });
	Object ent2(Entablature, { 0,-ENTABLATURE_HEIGHT,0 }, {}, { 12,1,10 });
	Object pil1(Pillar, { 5,0,4 }, {}, { 2,1,2 });
	Object pil2(Pillar, { 5,0,-4 }, {}, { 2,1,2 });
	Object pil3(Pillar, { -5,0,-4 }, {}, { 2,1,2 });
	Object pil4(Pillar, { -5,0,4 }, {}, { 2,1,2 });
	Object pil5(Pillar, { 5,0,0 }, {}, { 2,1,2 });
	Object pil6(Pillar, { -5,0,0 }, {}, { 2,1,2 });
	Object box1(Box, {0,PILLAR_HEIGHT/2,-0.5}, {}, { 8,PILLAR_HEIGHT,9 });
	Object face(ClockFace, { 0,5.75,4 }, {90,0,0}, {8,8,8});

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();


		if (width < height && width > 0)
			glOrtho(-ZAKRES, ZAKRES, -ZAKRES * height / width, ZAKRES * height / width, -ZASIEG, ZASIEG);
		else if (width >= height && height > 0)
			glOrtho(-ZAKRES * width / height, ZAKRES * width / height, -ZAKRES, ZAKRES, -ZASIEG, ZASIEG);


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3d(1, 0, 0);

		double t = glfwGetTime();

		glRotated(20, 1, 0, 0);

		glRotated(35 * cos(t), 1, 0, 0);
		glRotated(35 * sin(t), 0, 0, 1);
		glRotated(35 * t, 0, 1, 0);
		ent1.Display();
		ent2.Display();
		pil1.Display();
		pil2.Display();
		pil3.Display();
		pil4.Display();
		pil5.Display();
		pil6.Display();
		box1.Display();

		face.Display();

		glClearColor(1, 1, 1, 1);
		glfwSwapBuffers(window);


		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}