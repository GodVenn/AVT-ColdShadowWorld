#include "../Headers/pch.h"
#include "../Headers/Engine.h"
/**/


#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
using namespace engine;

class MyApp : public IApp, IUpdatable
{
public:

	virtual void initApp() override;
	virtual void displayCallback(GLFWwindow* win, float deltaTime) override;
	virtual void window_close_callback(GLFWwindow* win) override;
	virtual void window_size_callback(GLFWwindow* win, int winx, int winy) override;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void mouse_callback(GLFWwindow* win, double xpos, double ypos) override;
	virtual void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) override;

	virtual void onUpdate(float deltaTime) override;

private:
	/// ANIMATION
	const float penseroseAnimationDuration = 2.f;
	const float frameAnimationDuration = 1.f;
	bool isPenroseAnimationActivated = false;
	bool isFrameAnimationActivated = false;

	/// CALLBACKs
	DisableDepthCallback* callback = nullptr;

	/// BINDING POINTs
	const int UBO_CAMERA = 0;

	/// CAMERA SETUP VALUES
	Camera* camera = nullptr;
	Vec3 eye = Vec3(0.f, 0.f, 5.f);
	Vec3 center = Vec3(0.f, 0.f, -1.f);
	Vec3 up = Vec3(0.f, 1.f, 0.f);
	bool perspectiveProj = true;
	const float fovThreshold = 45.f;
	float fov = 45.f;
	float camFactor = 0.0055f;
	float near = 0.1f;
	float far = 1000.f;
	float top = 0.f;
	float botton = 0.f;
	float right = 0.f;
	float left = 0.f;
	float aspect = 0.f;

	/// CAMERA MOVEMENTS
	const float cameraSpeed = 6.f;
	float pitch = 0.f, yaw = -90.f;
	float lastX = 0.f, lastY = 0.f;
	bool firstMouse = true;
	bool tracking = false;


	Particle default_Particle;

	void createMeshes();
	void createCamera();
	void createShaderPrograms();
	void createSceneGraph();
	void createSimulation();

	void destroyManagers();
	void destroySimulation();
	void destroyCallbacks();

	void drawSceneGraph();
};

/////////////////////////////////////////////////////////////////////// CALLBACKs
void MyApp::initApp()
{
	createMeshes();
	createCamera();
	createShaderPrograms();
	createSceneGraph();
	createSimulation();
}

void MyApp::displayCallback(GLFWwindow* win, float deltaTime)
{
	Simulation::getInstance()->update(deltaTime);
	drawSceneGraph();
}

void MyApp::window_close_callback(GLFWwindow* win)
{
	destroyCallbacks();
	destroyManagers();
	destroySimulation();
}

void MyApp::window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	int winX = App::getInstance()->windowWidth = winx;
	int winY = App::getInstance()->windowHeight = winy;

	aspect = static_cast<float>(winX) / static_cast<float>(winY);
	right = winX * camFactor;
	left = -right;
	top = winY * camFactor;
	botton = -top;
	if (perspectiveProj)
	{
		camera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
	}
	else
	{
		camera->setOrthograpicProjectionMatrix(left, right, botton, top, near, far);
	}
}

void MyApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	if (fov > fovThreshold)
	{
		fov = fovThreshold;
	}
	if (perspectiveProj)
	{
		camera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
	}
}

void MyApp::mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.35f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	if (yaw > 89.0f)
	{
		yaw = 89.0f;
	}


	Vec3 direction = Vec3();
	direction.x = cos(yaw * DEGREES_TO_RADIANS) * cos(pitch * DEGREES_TO_RADIANS);
	direction.y = sin(pitch * DEGREES_TO_RADIANS);
	direction.z = sin(yaw * DEGREES_TO_RADIANS) * cos(pitch * DEGREES_TO_RADIANS);
	center = normalize(direction);
	camera->setViewMatrix(eye, center + eye, up);
}

void MyApp::key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	float deltaTime = static_cast<float>(App::getInstance()->deltaTime);
	if (key == GLFW_KEY_P && action == 1)
	{
		if (perspectiveProj)
		{
			perspectiveProj = false;
			camera->setOrthograpicProjectionMatrix(left, right, botton, top, near, far);
		}
		else
		{
			perspectiveProj = true;
			camera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
		}
	}
	if (key == GLFW_KEY_C && action == 1)
	{
		///////////ACTIVATE ANIMATION
		if (!isPenroseAnimationActivated)
		{
			isPenroseAnimationActivated = true;
		}
		else
		{
			isPenroseAnimationActivated = false;
		}
	}
	if (key == GLFW_KEY_F && action == 1)
	{
		///////////ACTIVATE ANIMATION
		if (!isFrameAnimationActivated)
		{
			isFrameAnimationActivated = true;
		}
		else
		{
			isFrameAnimationActivated = false;
		}
	}

	if (key == GLFW_KEY_R && action == 1)
	{

		isFrameAnimationActivated = false;
		isPenroseAnimationActivated = false;
	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		eye += cameraSpeed * center * deltaTime;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		eye -= cameraSpeed * center * deltaTime;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		eye -= normalize(center.crossProduct(up)) * cameraSpeed * deltaTime;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
	{
		eye += normalize(center.crossProduct(up)) * cameraSpeed * deltaTime;
	}
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
	camera->setViewMatrix(eye, eye + center, up);

	if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS)
	{
		std::cout << "Emit!\n";
		for (int i = 0; i < 1; i++)
			ParticleSystem::getInstance()->AddParticle(default_Particle);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs
void MyApp::createShaderPrograms()
{
	const std::string shaderFolder = "Shaders\\";

	ShaderProgram* penrose = new ShaderProgram();
	penrose->addShader(shaderFolder + "cube_vs.glsl",GL_VERTEX_SHADER);
	penrose->addShader(shaderFolder + "cube_fs.glsl", GL_FRAGMENT_SHADER);
	penrose->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	penrose->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	penrose->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	penrose->addUniform(engine::MODEL_MATRIX);
	penrose->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	penrose->create();
	ShaderProgramManager::getInstance()->add("PenroseCube", penrose);

	ShaderProgram* frame = new ShaderProgram();
	frame->addShader(shaderFolder + "frame_vs.glsl", GL_VERTEX_SHADER);
	frame->addShader(shaderFolder + "frame_fs.glsl", GL_FRAGMENT_SHADER);
	frame->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	frame->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	frame->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	frame->addUniform(engine::MODEL_MATRIX);
	frame->addUniform("u_Color");
	frame->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	frame->create();
	ShaderProgramManager::getInstance()->add("Frame", frame);

	ShaderProgram* frameBackground = new ShaderProgram();
	frameBackground->addShader(shaderFolder + "frame_vs.glsl", GL_VERTEX_SHADER);
	frameBackground->addShader(shaderFolder + "frame_fs.glsl", GL_FRAGMENT_SHADER);
	frameBackground->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	frameBackground->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	frameBackground->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	frameBackground->addUniform(engine::MODEL_MATRIX);
	frameBackground->addUniform("u_Color");
	frameBackground->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	frameBackground->create();
	ShaderProgramManager::getInstance()->add("FrameBackground", frameBackground);

	ShaderProgram* particles = new ShaderProgram();
	particles->addShader(shaderFolder + "particle_vs.glsl", GL_VERTEX_SHADER);
	particles->addShader(shaderFolder + "particle_geom.glsl", GL_GEOMETRY_SHADER);
	particles->addShader(shaderFolder + "particle_fs.glsl", GL_FRAGMENT_SHADER);
	particles->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	particles->addAttribute(1, engine::COLOR_ATTRIBUTE);
	particles->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	particles->create();
	ShaderProgramManager::getInstance()->add("Particles", particles);


	//setUpParticle();
}
/////////////////////////////////////////////////////////////////////// MESHEs
void MyApp::createMeshes()
{
	const std::string modelsFolder = "Models\\";
	std::string cube_file = modelsFolder +  "Cube.obj";
	Mesh* cube = new Mesh(cube_file);
	MeshManager::getInstance()->add("Cube", cube);

	std::string frame_file = modelsFolder + "Frame.obj";
	Mesh* frame = new Mesh(frame_file);
	MeshManager::getInstance()->add("Frame", frame);

	std::string frameBackground_file = modelsFolder + "FrameBackground.obj";
	Mesh* frameBackground = new Mesh(frameBackground_file);
	MeshManager::getInstance()->add("FrameBackground", frameBackground);
}
/////////////////////////////////////////////////////////////////////// CAMERA
void MyApp::createCamera()
{

	float winY = static_cast<float>(App::getInstance()->windowHeight);
	float winX = static_cast<float>(App::getInstance()->windowWidth);
	top = winY * camFactor;
	botton = -winY * camFactor;
	right = winX * camFactor;
	left = -winX * camFactor;
	aspect = winX / winY;

	/// CAMERA MOVEMENTS
	lastX = winX / 2.f;
	lastY = winY / 2.f;

	camera = new Camera(eye, eye + center, up, UBO_CAMERA);
	if (perspectiveProj)
	{
		camera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
	}
	else
	{
		camera->setOrthograpicProjectionMatrix(left, right, botton, top, near, far);
	}
}
/////////////////////////////////////////////////////////////////////// SCENE
SceneNode* createPenroseCube(SceneNode* node, Mat4 matrix, DisableDepthCallback* callback = nullptr)
{
	SceneNode* n_cube0 = new SceneNode();
	Mesh* cube = MeshManager::getInstance()->get("Cube");
	node->addNode(n_cube0);
	n_cube0->setMesh(cube);

	n_cube0->setMatrix(matrix);

	if (callback)
	{
		n_cube0->setCallback(callback);
	}

	return n_cube0;

}
void MyApp::createSceneGraph()
{

	SceneGraph* scene = new SceneGraph();
	SceneGraphManager::getInstance()->add("Main", scene);
	scene->setCamera(camera);
	callback = new DisableDepthCallback();

	/// FRAME
	ShaderProgram* s_Frame = ShaderProgramManager::getInstance()->get("Frame");
	SceneNode* root = scene->getRoot();
	root->setShaderProgram(s_Frame);
	s_Frame->bind();
	s_Frame->setUniform3f("u_Color", Vec3(0.8f, 0.52f, 0.25f));
	Mat4 frameTranslation = MatFactory::createTranslationMat4(Vec3(0, 0, -5.f));
	root->setMatrix(frameTranslation);
	Mesh* m_frame = MeshManager::getInstance()->get("Frame");
	root->setMesh(m_frame);
	root->setCallback(callback);

	/// FRAME BACKGROUND
	ShaderProgram* s_FrameBackground = ShaderProgramManager::getInstance()->get("FrameBackground");
	s_FrameBackground->bind();
	s_FrameBackground->setUniform3f("u_Color", Vec3(0, 0, 0));
	Mesh* m_frameBackground = MeshManager::getInstance()->get("FrameBackground");
	SceneNode* node_frameBackground = scene->getRoot()->createNode();
	node_frameBackground->setMesh(m_frameBackground);
	node_frameBackground->setShaderProgram(s_FrameBackground);
	node_frameBackground->setCallback(callback);

	/// PENROSE NODE
	SceneNode* pensore = scene->getRoot()->createNode();

	ShaderProgram* s_Cube = ShaderProgramManager::getInstance()->get("PenroseCube");
	pensore->setShaderProgram(s_Cube);

	Qtrn rotX = Qtrn(-45.f, engine::AXIS4D_X);
	Qtrn rotY = Qtrn(35.f, engine::AXIS4D_Y);
	Mat4 cubesRotation = GLRotationMatrix(rotY * rotX);
	Mat4 cubesTranslation = MatFactory::createTranslationMat4(Vec3(0, 1, 3));
	Mat4 matrix = cubesTranslation * cubesRotation;
	pensore->setMatrix(matrix);

	Mat4 cubeScale = MatFactory::createScaleMat4(Vec3(0.3f, 0.3f, 0.3f));
	float cubeOffset = 0.8f;

	// CUBE 1
	Vec3 posC1 = Vec3(-1 * cubeOffset, -3 * cubeOffset, 0);
	Mat4 mC1 = MatFactory::createTranslationMat4(posC1) * cubeScale;
	SceneNode* n_cubo1 = createPenroseCube(pensore, mC1);
	// CUBE 2
	Vec3 posC2 = Vec3(-2 * cubeOffset, -3 * cubeOffset, 0);
	Mat4 mC2 = MatFactory::createTranslationMat4(posC2) * cubeScale;
	SceneNode* n_cubo2 = createPenroseCube(pensore, mC2, callback);
	// CUBE 3
	Vec3 posC3 = Vec3(-3 * cubeOffset, -3 * cubeOffset, 0);
	Mat4 mC3 = MatFactory::createTranslationMat4(posC3) * cubeScale;
	SceneNode* n_cubo3 = createPenroseCube(pensore, mC3, callback);
	// CUBE 4
	Vec3 posC4 = Vec3(0, 0, -2 * cubeOffset);
	Mat4 mC4 = MatFactory::createTranslationMat4(posC4) * cubeScale;
	SceneNode* n_cubo4 = createPenroseCube(pensore, mC4);
	// CUBE 5
	Vec3 posC5 = Vec3(0, 0, -1 * cubeOffset);
	Mat4 mC5 = MatFactory::createTranslationMat4(posC5) * cubeScale;
	SceneNode* n_cubo5 = createPenroseCube(pensore, mC5);
	// CUBE 6
	Vec3 posC6 = Vec3(0, 0 * cubeOffset, 0);
	Mat4 mC6 = MatFactory::createTranslationMat4(posC6) * cubeScale;
	SceneNode* n_cubo6 = createPenroseCube(pensore, mC6);
	// CUBE 7
	Vec3 posC7 = Vec3(0, -1 * cubeOffset, 0);
	Mat4 mC7 = MatFactory::createTranslationMat4(posC7) * cubeScale;
	SceneNode* n_cubo7 = createPenroseCube(pensore, mC7);
	// CUBE 8
	Vec3 posC8 = Vec3(0, -2 * cubeOffset, 0);
	Mat4 mC8 = MatFactory::createTranslationMat4(posC8) * cubeScale;
	SceneNode* n_cubo8 = createPenroseCube(pensore, mC8);
	// CUBE 9
	Vec3 posC9 = Vec3(0, -3 * cubeOffset, 0);
	Mat4 mC9 = MatFactory::createTranslationMat4(posC9) * cubeScale;
	SceneNode* n_cubo9 = createPenroseCube(pensore, mC9);
}
///////////////////////////////////////////////////////////////////// SIMULATION
void MyApp::createSimulation()
{
	Simulation::getInstance()->add(this);
}

///////////////////////////////////////////////////////////////////// DETROYs
void MyApp::destroyManagers()
{
	AnimatorManager::freeInstance();
	ShaderProgramManager::freeInstance();
	MeshManager::freeInstance();
	SceneGraphManager::freeInstance();

}

void MyApp::destroySimulation()
{
	Simulation::freeInstance();
}

void MyApp::destroyCallbacks()
{
	delete callback;
}

///////////////////////////////////////////////////////////////////// DRAW AND UPDATEs
float maxTime = 0.1f;
float time = maxTime;
void MyApp::drawSceneGraph()
{
	float deltaTime = static_cast<float>(App::getInstance()->deltaTime);

	time -= deltaTime;
	if (time <= 0.0f)
	{
		for (int i = 0; i < 1; i++)
			ParticleSystem::getInstance()->AddParticle(default_Particle);
		time = maxTime;
	}
	ParticleSystem::getInstance()->OnUpdate(deltaTime, SceneGraphManager::getInstance()->get("Main")->getCamera());

}

void MyApp::onUpdate(float deltaTime)
{
}
///////////////////////////////////////////////////////////////////////// MAIN
int main(int argc, char* argv[])
{
	engine::App::getInstance()->setGLApp(new MyApp());
	engine::App::getInstance()->setupOpenGL(4, 3);
	int is_fullscreen = 0;
	int is_vsync = 1;
	int width = 1080;
	int height = 720;
	engine::App::getInstance()->setupWindow(width, height, "AVT Engine", is_fullscreen, is_vsync);
	engine::App::getInstance()->init();
	engine::App::getInstance()->run();

	engine::App::freeInstance();
	_CrtDumpMemoryLeaks();
	exit(EXIT_SUCCESS);
}
/////////////////////////////////////////////////////////////////////////// END