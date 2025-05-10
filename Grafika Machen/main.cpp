#include "objects.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage.h"
#define ZASIEG 50

#define LICZBA_OB_TEXTUR 4
GLuint obiektyTextur[LICZBA_OB_TEXTUR];
const char* plikiTextur[] = { 
	"C:\\Users\\jacob\\Desktop\\marble1.jpg",
	"C:\\Users\\jacob\\Desktop\\marble2.jpg",
	"C:\\Users\\jacob\\Desktop\\color_test.jpg",
    "C:\\Users\\jacob\\Desktop\\clockface_texture.jpg"
};

enum
{
	Scene,
	Clock,
	Light0,
	Light1,
	Light2
} Mode;

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 10.0f, 0.0f };

const GLfloat light_0_diffuse[] = { 1.0f, 0.7f, 0.4f, 1.0f };
const GLfloat light_0_specular[] = { 1.0f, 0.7f, 0.4f, 1.0f };

const GLfloat light_1_diffuse[] = { 0.6f, 0.7f, 1.0f, 1.0f };
const GLfloat light_1_specular[] = { 0.6f, 0.7f, 1.0f, 1.0f };

const GLfloat light_2_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_2_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

const GLfloat reflector_position[] = { 0.0f, 0.0f, 20.0f, 1.0f };
const GLfloat reflector_direction[] = { 0.0f, 0.0f, -1.0f, 1.0f };

const GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat high_shininess[] = { 25.0f };

Vector3d cam_rot = { 25,25,0 };
Vector3d clk_rot = { 0,0,0 };
Vector3d light_0_rot = { -45,60,0 };
Vector3d light_1_rot = { -45,-60,0 };
Vector3d light_2_rot = { -60,0,0 };
GLdouble zakres = 20;

double time_haste = 1;
double t = 0;
double oldTime = 0;

bool relatorOn = true, perspectiveOn = false;

char lightmode = 3;
bool rendermode = false;

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
	case Light2:
		inj = &light_2_rot;
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
		case GLFW_KEY_5:
			Mode = Light2;
			break;
		case GLFW_KEY_8:
			perspectiveOn = !perspectiveOn;
			break;
		case GLFW_KEY_9:
			relatorOn = !relatorOn;
			break;
		case GLFW_KEY_0:
			lightmode++;
			if (lightmode > 7) lightmode = 0;
			break;
		case GLFW_KEY_M:
			rendermode = !rendermode;
			break;
		case GLFW_KEY_P:
			zakres += 1;
			break;
		case GLFW_KEY_O:
			zakres -= 1;
			if (zakres < 1) zakres = 1;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, 1);
			break;
		case GLFW_KEY_Q:
			time_haste -= 1;
			if (time_haste < -10) time_haste = -10;
			break;
		case GLFW_KEY_W:
			time_haste += 1;
			if (time_haste > 10) time_haste = 10;
			break;
		case GLFW_KEY_R:
			Mode = Scene;
			cam_rot = { 25,25,0 };
			clk_rot = { 0,0,0 };
			light_0_rot = { -45,60,0 };
			light_1_rot = { -45,-60,0 };
			light_2_rot = { -60,0,0 };
			zakres = 20;
			time_haste = 1;
			lightmode = 3;
			rendermode = false;
			perspectiveOn = false;
			t = 0;
			oldTime = 0;
			{
				time_t timestamp = time(NULL);
				struct tm dtm;
				localtime_s(&dtm, &timestamp);
				glfwSetTime(dtm.tm_sec + 60 * dtm.tm_min + 3600 * dtm.tm_hour);
			}
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

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(LICZBA_OB_TEXTUR, obiektyTextur);
	for (int i = 0; i < LICZBA_OB_TEXTUR; i++) {
		GLint iWidth, iHeight, nrChannels;
		glBindTexture(GL_TEXTURE_2D, obiektyTextur[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* data = stbi_load(plikiTextur[i], &iWidth, &iHeight, &nrChannels, 0);
		cout << iWidth << "\t" << obiektyTextur[i] << endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	Object lightObj(Box, { 0, 0, 20 });
	Object relativeObj(Box, { 0, 0, 10 }, {}, {2,2,2});

	Object Ent1(Entablature, {}, {}, { 12, 2, 6 });
	Object Ent2(Entablature, { 0,10,0 }, {}, { 12, 2, 6 });
	Object Box1(Box, { 0,5,-0.5 }, {}, { 8, 8, 5 });
	Object Pil1(Pillar, { -5,5,-2 }, {}, { 2, 8, 2 });
	Object Pil2(Pillar, { 5,5,-2 }, {}, { 2, 8, 2 });
	Object Pil3(Pillar, { -5,5,0 }, {}, { 2, 8, 2 });
	Object Pil4(Pillar, { 5,5,0 }, {}, { 2, 8, 2 });
	Object Pil5(Pillar, { -5,5,2 }, {}, { 2, 8, 2 });
	Object Pil6(Pillar, { 5,5,2 }, {}, { 2, 8, 2 });
	Object ClkFace(ClockFace, { 0,5.5,2.001 }, {0,0,180}, { 7,7,1 });
	Object Pend("C:\\Users\\jacob\\Desktop\\pendulum.obj", { 0,0,0 }, { 0,0,0 }, { 0.5,0.5,0.5 });
	Object Roof("C:\\Users\\jacob\\Desktop\\roof.obj", { 0,12,0 }, { 0,90,0 }, {3,1,0.75});
	Object Arr1("C:\\Users\\jacob\\Desktop\\arrow.obj", { 0,5.5,2 }, { 0,0,0 }, { 0.35,0.35,0.1 });
	Object Arr2("C:\\Users\\jacob\\Desktop\\arrow.obj", { 0,5.5,2 + 0.1 * ARROW_THICKNESS }, { 0,0,0 }, { 0.30,0.30,0.1 });
	Object Arr3("C:\\Users\\jacob\\Desktop\\arrow.obj", { 0,5.5,2 + 0.2 * ARROW_THICKNESS }, { 0,0,0 }, { 0.25,0.25,0.1 });

	{
	time_t timestamp = time(NULL);
	struct tm dtm;
	localtime_s(&dtm, &timestamp);
	glClearColor(0, 0, 0.3, 1);
	glfwSetTime(dtm.tm_sec + 60*dtm.tm_min + 3600*dtm.tm_hour);
	}

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

		if (perspectiveOn)
		{
			if (width < height && width > 0) glFrustum(-zakres, zakres, -zakres * height / width, zakres * height / width, 1, ZASIEG);
			else if (height > 0) glFrustum(-zakres * width / height, zakres * width / height, -zakres, zakres, 1, ZASIEG);
		}
		else
		{
			if (width < height && width > 0) glOrtho(-zakres, zakres, -zakres * height / width, zakres * height / width, -ZASIEG, ZASIEG);
			else if (height > 0) glOrtho(-zakres * width / height, zakres * width / height, -zakres, zakres, -ZASIEG, ZASIEG);
		}
		if (rendermode)
		{
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (perspectiveOn) glTranslated(0, 0, -20);

		glColor3d(1, 1, 1);
		glRotated(cam_rot.x, 1, 0, 0);
		glRotated(cam_rot.y, 0, 1, 0);
		

		t += (glfwGetTime() - oldTime) * time_haste;
		oldTime = glfwGetTime();

		Pend.GetTransform().GetRotation().z = 5 * sin(2*PI*t);
		Arr3.GetTransform().GetRotation().z = -t / 120;
		Arr2.GetTransform().GetRotation().z = -t / 10;
		Arr1.GetTransform().GetRotation().z = -t * 6;

		if(relatorOn) relativeObj.Display(obiektyTextur[2]);

		glPushMatrix();
		glRotated(clk_rot.y, 0, 1, 0);
		glRotated(clk_rot.x, 1, 0, 0);

		//*
		Ent1.Display(obiektyTextur[1]);
		Ent2.Display(obiektyTextur[1]);
		Box1.Display(obiektyTextur[1]);
		Pil1.Display(obiektyTextur[0]);
		Pil2.Display(obiektyTextur[0]);
		Pil3.Display(obiektyTextur[0]);
		Pil4.Display(obiektyTextur[0]);
		Pil5.Display(obiektyTextur[0]);
		Pil6.Display(obiektyTextur[0]);
		ClkFace.Display(obiektyTextur[3]);
		Pend.Display(obiektyTextur[0]);
		Roof.Display(obiektyTextur[0]);
		glColor3d(1, 0, 0);
		Arr1.Display();
		glColor3d(0, 1, 0);
		Arr2.Display();
		glColor3d(0, 0, 1);
		Arr3.Display();
		//*/
		glPopMatrix();

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

		if (lightmode & 4)
		{
			glEnable(GL_LIGHT2);
			glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glRotated(light_2_rot.y, 0, 1, 0);
			glRotated(light_2_rot.x, 1, 0, 0);
			lightObj.Display();
			glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, light_2_diffuse);
			glLightfv(GL_LIGHT2, GL_SPECULAR, light_2_specular);
			glLightfv(GL_LIGHT2, GL_POSITION, reflector_position);
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, reflector_direction);
			glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30);
			glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15);
			glPopMatrix();
		}
		else glDisable(GL_LIGHT2);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glDeleteTextures(LICZBA_OB_TEXTUR, obiektyTextur);
	glfwTerminate();

	return 0;
}