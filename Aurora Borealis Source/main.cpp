/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	main for screenSaver project
*/
#include <vector>
#include <iostream>
#include <array>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES 
#include <math.h>
#include "meshBase.h"
#include "spline.h"
#include "ribbon.h"
#include "endlessSpline.h"
#include "endlessRibbon.h"
#include "skybox.h"
#include "sphere.h"

using namespace glm;
using namespace std;

//window class storing necessary variables for display and time
class window
{
public:
	window()
	{
		this->time_of_last_pause = this->total_time_paused = 0.0f;
		this->window_handle = -1;
		this->fov = 50.0f;
		this->nearPlane = 0.1f;
		this->farPlane = 300.0f;
		this->normals = this->wireframe = this->paused = false;
		this->size = ivec2(700, 600);
		this->period = 1000 / 60;
		this->wireframe = false;
		this->camRadius = 10.0f;
		this->camPos = vec4(normalize(vec3(1.0f, 0.0f, 1.0f)) * this->camRadius, 1.0f);
		this->camVDegree = 0.0f;
		this->rmClicked = false;
		this->aspect = float(size.x) / float(size.y);
		this->isClosed = false;
	}
	float fov, nearPlane, farPlane;
	ivec2 size;
	int period;
	bool isClosed;
	bool wireframe, paused, normals;;
	vec4 camPos;
	float camRadius;
	float camVDegree;
	float aspect;
	float time_of_last_pause, total_time_paused;
	int window_handle;
	mat4 fpvMv, fpvProj;
	vec2 currentMousePos;
	bool rmClicked;
	vector<string> instructions;

} window;

//Creates objects to be drawn in the scene
EndlessRibbon endlessRib;
EndlessRibbon endlessRibTwo;
Skybox nightSky;
Sphere skySphere;
Sphere moon;

// Utility function for conversion from degree to radians
float toRadian(float d)
{
	return (float)(d / 180.0f * M_PI);
}

//Takes down all objects in the scene and ends program
void CloseFunc()
{
	window.window_handle = -1;
	endlessRib.TakeDown();
	endlessRibTwo.TakeDown();
	nightSky.TakeDown();
	skySphere.TakeDown();
	moon.TakeDown();
	glutLeaveMainLoop();
}

//Handles the reshaping of the window
void ReshapeFunc(int w, int h)
{
	//If window is not closed, resize the window, recalculate the aspect, and redisplay the window
	if(window.window_handle != -1 && h > 0) {
		window.size = ivec2(w, h);
		window.aspect = float(window.size.x) / float(window.size.y);
		glutPostWindowRedisplay(window.window_handle);
	}
}

//Implements keyboard commands for toggling of wireframe, pausing, full screen display, display of normals, and closing of the program
void KeyboardFunc(unsigned char c, int x, int y)
{ 

	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch(c) 
	{
		case 'p':
			if (window.paused == true)
			{
				// Will be leaving paused state
				window.total_time_paused += (current_time - window.time_of_last_pause);
			}
			else
			{
				// Will be entering paused state
				window.time_of_last_pause = current_time;
			}
			window.paused = !window.paused;
			break;
		case 'w': 
			//toggles wireframe if avaliable for display
			window.wireframe = !window.wireframe;
			break;
		case 'n':
			//toggles normals if avaliable for display
			window.normals = !window.normals;
			break;
		case 'f':
			//toggles full screen mode
			glutFullScreenToggle();
			break;
		case 'x':
		case 27: // Esc
			// Closes program
			CloseFunc();
			return;
	}

	//Redisplays windows
	glutPostWindowRedisplay(window.window_handle);
}


//Implements camera controls of the program by directional arrow keys
void SpecialFunc(int key, int x, int y)
{
	mat4 upRotate, downRotate, leftRotate, rightRotate;
	switch(key) {
		//Rotate camera left
		case GLUT_KEY_LEFT:
			leftRotate = mat4(1.0f);
			leftRotate = rotate(leftRotate, -1.0f, vec3(0.0f, 1.0f, 0.0f));
			window.camPos = window.camPos * leftRotate;
			break;
		//Rotate camera up, to a limit
		case GLUT_KEY_UP:
			upRotate = mat4(1.0f);
			if(window.camVDegree >= 89.1f)
				break;
			upRotate = rotate(upRotate, -1.0f, normalize(vec3(-window.camPos.z, 0.0f, window.camPos.x)));
			window.camPos = window.camPos * upRotate;
			window.camVDegree++;
			break;
		//Rotate camera down, to a limit
		case GLUT_KEY_DOWN:
			downRotate = mat4(1.0f);
			if(window.camVDegree <= -89.1f)
				break;
			downRotate = rotate(downRotate, 1.0f, normalize(vec3(-window.camPos.z, 0.0f, window.camPos.x)));
			window.camPos = window.camPos * downRotate;
			window.camVDegree--;
			break;
		//Rotate camera right
		case GLUT_KEY_RIGHT:
			rightRotate = mat4(1.0f);
			rightRotate = rotate(rightRotate, 1.0f, vec3(0.0f, 1.0f, 0.0f));
			window.camPos = window.camPos * rightRotate;
			break;
		//Decreases the field of view, zoom in
		case GLUT_KEY_PAGE_DOWN:
			if(window.fov == 10.0)
				break;
			window.fov = window.fov - 0.5f;
			break;
		//Increases the field of view, zoom out
		case GLUT_KEY_PAGE_UP:
			if(window.fov == 80.0)
				break;
			window.fov = window.fov + 0.5f;
			break;
	}
}

// Handles mouse buttons
void MouseFunc(int button, int state, int x, int y)
{
	// The boolean rmClicked prevents movement when middle or right buttons are clicked
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			window.currentMousePos = vec2(x, y);
		}
	}else {
		if(state == GLUT_DOWN) {
			window.rmClicked = true;
		} else {
			window.rmClicked = false;
		}
	}
}

// Handles mouse wheel control
void MouseWheelFunc(int button, int dir, int x, int y) 
{
	if(dir > 0) {
		if(window.fov > 10) {
			window.fov--;
		}
	} else {
		if(window.fov < 80) {
			window.fov++;
		}
	}
}

// Handles mouse movement when mouse button is down only
void MotionFunc(int x, int y)
{
	// Mousefunc only sets position at time of click
	// Need to reset that position after each movement
	// Scalar just scales down the movement speed

	if(!window.rmClicked) {
		if(x != window.currentMousePos.x) 
		{
			float angle = (window.currentMousePos.x - x) / 10.0f;
			mat4 rightRotate;
			rightRotate = rotate(rightRotate, -angle, vec3(0.0f, 1.0f, 0.0f));
			window.camPos = window.camPos * rightRotate;
		}

		if(y != window.currentMousePos.y && window.camVDegree < 89.1f && window.camVDegree > -89.1f) 
		{
			float angle = (window.currentMousePos.y - y) / 10.0f;
			if(window.camVDegree + angle >= 89.0f)
			{
				angle = 89.0f - window.camVDegree;
			}
			else if(window.camVDegree + angle <= -89.0f)
			{
				angle = -89.0f - window.camVDegree;
			}
			mat4 upRotate;
			upRotate = rotate(upRotate, angle, normalize(vec3(-window.camPos.z, 0.0f, window.camPos.x)));
			window.camPos = window.camPos * upRotate;
			window.camVDegree += angle;
		}
		window.currentMousePos = vec2(x, y);
	}
}

//Timer function of the program, automatically redisplays window 
void TimerFunc(int value)
{
	if (window.window_handle != -1)
	{
		glutTimerFunc(window.period, TimerFunc, value);
		glutPostWindowRedisplay(window.window_handle);
	}
}

// Handles the display within the window
void DisplayFunc()
{
	if(window.window_handle == -1)
		return;

	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, (window.wireframe == true) ? GL_LINE : GL_FILL);
	
	//Sets the viewport of the FIRST PERSON window
	glViewport(0, 0, window.size.x, window.size.y);

	//Calculates and loads the projection matrix
	mat4 proj = perspective(window.fov, window.aspect, window.nearPlane, window.farPlane);
	window.fpvProj = proj;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(proj));

	//Calculates the coordinates of the camera and sets the camera position and orientation
	mat4 mv(1.0f);
	mv = lookAt(vec3(window.camPos), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	window.fpvMv = mv;
	mv = translate(mv, vec3(0.0f, -3.0f, 0.0f));

	//Loads the ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(mv));

	//enables blending, proceeds to draw the objects in the scene
	glDisable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	skySphere.Draw(proj, mv, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused);
	mat4 m = rotate(mv, ((window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused) * 0.7f, vec3(1.0f, 0.0f, 1.0f));
	m = translate(m, vec3(100.0f, 100.0f, 0.0f));
	moon.Draw(proj, m, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused);
	//Ensures objects are drawn in the correct order
	if(window.camPos.z > 0)
	{
		mat4 m = rotate(mv, 90.0f, vec3(0.0f, 1.0f, 0.0f));
		nightSky.Draw(proj, m, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused);
		endlessRib.Draw(proj, mv, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused);

		m = translate(mv, vec3(0.0f, -2.0f, 2.0f));
		m = rotate(m, 1.0f, vec3(0.0f, 1.0f, 0.0f));
		endlessRibTwo.Draw(proj, m, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused + 10.0f);
	}
	else
	{

		mat4 m = rotate(mv, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	    nightSky.Draw(proj, m, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused);

		m = translate(mv, vec3(0.0f, -2.0f, 2.0f));
		m = rotate(m, 1.0f, vec3(0.0f, 1.0f, 0.0f));
		endlessRibTwo.Draw(proj, m, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused + 10.0f);

		endlessRib.Draw(proj, mv, window.size, (window.paused ? window.time_of_last_pause : current_time) - window.total_time_paused);
	}


	/*glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 0.0f);
		
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 10.0f);
	}
	glEnd();
	glEnable(GL_LIGHTING);*/

	glEnable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

//Displays instructions onscreen if avaliable
void DisplayInstructions()
{
	if (window.window_handle == -1)
		return;

	vector<string> * s = &window.instructions;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(10, 15 * s->size(), -5.5);
	glScaled(0.1, 0.1, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}

//Sets up initial conditions 
bool InitializeGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	return true;
}

//main loop
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	ilInit();
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutSetOption(GLUT_RENDERING_CONTEXT, GLUT_USE_CURRENT_CONTEXT);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(window.size.x, window.size.y);
	window.window_handle = glutCreateWindow("Aurora Borealis");
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.period, TimerFunc, 0);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutMouseWheelFunc(MouseWheelFunc);
	glutCloseFunc(CloseFunc);
	
	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	if(!InitializeGL())
	{
		CloseFunc(); 
		return -1;
	}

	//Initializes Skybox
	if (!nightSky.Initialize())
	{
		cerr << "Skybox failed to initialize." << endl;
		CloseFunc();
	}

	//Initializes textures
	if(!MeshBase::TextureInit())
	{
		cerr << "Textures failed to initialize." <<endl;
	}

	if(!MeshBase::ShaderInit())
	{
		cerr << "Shaders failed to initialize." <<endl;
	}

	//Initializes objects that are drawn in the scene
	vec3 startPoint = vec3(0.0f, 0.0f, 0.0f);
	vec3 endPoint = vec3(3.0f, 0.0f, 0.0f);
	vec3 ctrlOne = vec3(0.5f, 0.0f, 6.0f);
	vec3 ctrlTwo = vec3(2.5f, 0.0f, -5.5f);
	endlessRib.Initialize(startPoint, endPoint, ctrlOne, ctrlTwo, 20, 200, vec3(1.0f, 0.0f, 0.0f), 14.0f);
	endlessRibTwo.Initialize(vec3(0.0f, 0.0f, 0.0f), vec3(3.0f, 0.0f, 1.0f), vec3(0.5f, 0.0f, 0.6f), vec3(2.5f, 0.0f, 0.2f), 20, 200, vec3(1.0f, 0.0f, 0.0f), 14.0f);
	skySphere.Initialize(200.0f, vec2(36, 18), "skyShader.vert", "skyShader.frag");
	moon.Initialize(10.0f, vec2(36, 18), "moonShader.vert", "moonShader.frag");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();
	return 0;
}

 