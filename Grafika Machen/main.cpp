#include "objects.h"
#define ZASIEG 50

enum
{
	Scene,
	Clock,
	Light0,
	Light1
} Mode;

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

const GLfloat light_0_diffuse[] = { 1.0f, 0.7f, 0.4f, 1.0f };
const GLfloat light_0_specular[] = { 1.0f, 0.7f, 0.4f, 1.0f };

const GLfloat light_1_diffuse[] = { 0.6f, 0.7f, 1.0f, 1.0f };
const GLfloat light_1_specular[] = { 0.6f, 0.7f, 1.0f, 1.0f };

const GLfloat light_position[] = { 0.0f, 0.0f, 10.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat high_shininess[] = { 25.0f };

Vector3d cam_rot = { 0,0,0 };
Vector3d clk_rot = { 0,0,0 };
Vector3d light_0_rot = { 45,60,0 };
Vector3d light_1_rot = { -45,-60,0 };
GLdouble ZAKRES = 20;

bool relatorOn = true;

char lightmode = 3;

#define ANGLE_SPEED 5
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Vector3d* inj = nullptr;
	switch (Mode)
	{
	case Clock:
		inj = &clk_rot;
		break;
	case Light0:
		inj = &light_0_rot;
		break;
	case Light1:
		inj = &light_1_rot;
		break;
	default:
		inj = &cam_rot;
		break;
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			inj->x -= ANGLE_SPEED;
			break;
		case GLFW_KEY_DOWN:
			inj->x += ANGLE_SPEED;
			break;
		case GLFW_KEY_RIGHT:
			inj->y += ANGLE_SPEED;
			break;
		case GLFW_KEY_LEFT:
			inj->y -= ANGLE_SPEED;
			break;
		case GLFW_KEY_1:
			Mode = Scene;
			break;
		case GLFW_KEY_2:
			Mode = Clock;
			break;
		case GLFW_KEY_3:
			Mode = Light0;
			break;
		case GLFW_KEY_4:
			Mode = Light1;
			break;
		case GLFW_KEY_9:
			relatorOn = !relatorOn;
			break;
		case GLFW_KEY_0:
			lightmode++;
			if (lightmode > 3) lightmode = 0;
			break;
		case GLFW_KEY_P:
			ZAKRES += 1;
			break;
		case GLFW_KEY_O:
			ZAKRES -= 1;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, 1);
			break;
		}
	}
	if (inj->x < -90) inj->x = -90;
	else if (inj->x > 90) inj->x = 90;
	else if (inj->y < 0) inj->y += 360;
	else if (inj->y > 360) inj->y -= 360;
}

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

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

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
	Object box1(Box, { 0,PILLAR_HEIGHT / 2,-0.5 }, {}, { 8,PILLAR_HEIGHT,9 });
	Object face(ClockFace, { 0,5.75,4 }, { 90,0,0 }, { 8,8,8 });

	Object relator(Box, { 0, 0, 16 });
	Object lightObj(Box, { 0, 0, 20 });
	Object arrow1("C:\\Users\\jacob\\Desktop\\arrow.obj", { 0, 5.75, 4.5 }, {}, { 0.35, 0.35, 0.15 });
	Object arrow2("C:\\Users\\jacob\\Desktop\\arrow.obj", { 0, 5.75, 4.5 + ARROW_THICKNESS * 0.15 }, { 0,0,0 }, { 0.3, 0.3, 0.15 });
	Object arrow3("C:\\Users\\jacob\\Desktop\\arrow.obj", { 0, 5.75, 4.5 + ARROW_THICKNESS * 0.3 }, { 0,0,0 }, { 0.25, 0.25, 0.15 });

	time_t timestamp = time(NULL);
	struct tm dtm;
	localtime_s(&dtm, &timestamp);
	glClearColor(0, 0, 0, 1);
	glfwSetTime(dtm.tm_sec + 60*dtm.tm_min + 3600*dtm.tm_hour);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (width < height && width > 0)
			glOrtho(-ZAKRES, ZAKRES, -ZAKRES * height / width, ZAKRES * height / width, -ZASIEG, ZASIEG);
		else if (height > 0)
			glOrtho(-ZAKRES * width / height, ZAKRES * width / height, -ZAKRES, ZAKRES, -ZASIEG, ZASIEG);


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3d(1, 1, 1);


		double t = glfwGetTime();

		arrow1.GetTransform().GetRotation().z = -6 * t;
		arrow2.GetTransform().GetRotation().z = -t / 10;
		arrow3.GetTransform().GetRotation().z = -t / 120;


		glRotated(cam_rot.x, 1, 0, 0);
		glRotated(cam_rot.y, 0, 1, 0);

		if(relatorOn) relator.Display();

		glPushMatrix();
		glRotated(clk_rot.y, 0, 1, 0);
		glRotated(clk_rot.x, 1, 0, 0);

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

		glColor3d(1, 0, 0);
		arrow1.Display();
		glColor3d(0, 1, 0);
		arrow2.Display();
		glColor3d(0, 0, 1);
		arrow3.Display();
		glPopMatrix();

		lightObj.GetTransform().GetPosition().z = ZAKRES;

		if (lightmode & 1)
		{
			glEnable(GL_LIGHT0);
			glPushMatrix();
			glColor3d(1.0, 0.7, 0.4);
			glRotated(light_0_rot.y, 0, 1, 0);
			glRotated(light_0_rot.x, 1, 0, 0);
			lightObj.Display();
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			glPopMatrix();
		}
		else glDisable(GL_LIGHT0);

		if (lightmode & 2)
		{
			glEnable(GL_LIGHT1);
			glPushMatrix();
			glColor3d(0.6, 0.7, 1.0);
			glRotated(light_1_rot.y, 0, 1, 0);
			glRotated(light_1_rot.x, 1, 0, 0);
			lightObj.Display();
			glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light_1_diffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light_1_specular);
			glLightfv(GL_LIGHT1, GL_POSITION, light_position);
			glPopMatrix();
		}
		else glDisable(GL_LIGHT1);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}