#include "../Headers/pch.h"
#include "../Headers/Engine.h"
/**/
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

	/// CALLBACKs
	DisableDepthCallback* callback = nullptr;

	/// BINDING POINTs
	const int UBO_CAMERA = 0;

	/// CAMERA SETUP VALUES
	Camera* mainCamera = nullptr;
	CameraController* camController = nullptr;
	const Vec3 initialEye = Vec3(0.0f, 0.0f, 5.0f);
	const Vec3 initialCenter = Vec3(0.0f, 0.0f, 0.0f);
	const Vec3 initialUp = Vec3(0.0f, 1.0f, 0.0f);
	const float fovThreshold = 45.f;
	float fov = 45.f;
	//float camFactor = 0.0055f;
	float near = 0.1f;
	float far = 100.f;
	/*float top = 0.f;
	float botton = 0.f;
	float right = 0.f;
	float left = 0.f;*/
	float aspect = 0.f;

	/// CAMERA MOVEMENTS
	const float cameraSpeed = 6.0f;
	const float cameraSensitivity = 0.05f;

	void createTextures();
	void createMeshes();
	void setupCamera();
	void createShaderPrograms();
	void createSceneGraph();
	void createSimulation();

	void destroyManagers();
	void destroySimulation();
	void destroyCallbacks();

	void drawSceneGraph();
	void processMovement();
};

/////////////////////////////////////////////////////////////////////// CALLBACKs
void MyApp::initApp()
{
	createTextures();
	createMeshes();
	setupCamera();
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
	//right = winX * camFactor;
	//left = -right;
	//top = winY * camFactor;
	//botton = -top;
	mainCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
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
	mainCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
}

void MyApp::mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	camController->setYawPitch((float)xpos, (float)ypos);
}

void MyApp::key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{

	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs
void MyApp::createShaderPrograms()
{
	const std::string shaderFolder = "Shaders\\";

	ShaderProgram* penrose = new ShaderProgram();
	penrose->addShader(shaderFolder + "cube_vs.glsl", GL_VERTEX_SHADER);
	penrose->addShader(shaderFolder + "cube_fs.glsl", GL_FRAGMENT_SHADER);
	penrose->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	penrose->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	penrose->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	penrose->addUniform(engine::MODEL_MATRIX);
	penrose->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	penrose->create();
	ShaderProgramManager::getInstance()->add("PenroseCube", penrose);


	ShaderProgram* textureShader = new ShaderProgram();
	textureShader->addShader(shaderFolder + "heightMap_vs.glsl", GL_VERTEX_SHADER);
	textureShader->addShader(shaderFolder + "heightMap_fs.glsl", GL_FRAGMENT_SHADER);
	textureShader->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	textureShader->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	textureShader->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	textureShader->addUniform("Texture");
	textureShader->addUniform(engine::MODEL_MATRIX);
	textureShader->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	textureShader->create();
	ShaderProgramManager::getInstance()->add("SimpleTexture", textureShader);
}

/////////////////////////////////////////////////////////////////////// TEXTUREs
void MyApp::createTextures()
{
	const std::string TextureFolder = "Textures\\";
	std::string texturePath = TextureFolder + "earthbump1k.jpg";
	Texture2D* earth = new Texture2D();
	earth->load(texturePath);
	TextureManager::getInstance()->add("EarthHeightMap", earth);
}

/////////////////////////////////////////////////////////////////////// MESHes
void MyApp::createMeshes()
{
	const std::string modelsFolder = "Models\\";
	std::string cube_file = modelsFolder + "Cube.obj";
	Mesh* cube = new Mesh(cube_file);
	MeshManager::getInstance()->add("Cube", cube);

	// Terrain
	float terrainWidth = 10;
	float terrainLength = 5;
	float terrainMaxHeight = 0.1f;
	unsigned int terrainSimplicity = 4;
	TerrainBuilder terrainBuilder = TerrainBuilder(terrainWidth, terrainLength, terrainSimplicity, terrainMaxHeight);

	const std::string heightMap = "Textures\\earthbump1k.jpg";
	terrainBuilder.setHeightMap(heightMap);
	Mesh* terrain = terrainBuilder.buildMesh();
	MeshManager::getInstance()->add("Terrain", terrain);

}
/////////////////////////////////////////////////////////////////////// CAMERA
void MyApp::setupCamera()
{
	glfwSetInputMode(App::getInstance()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float winY = static_cast<float>(App::getInstance()->windowHeight);
	float winX = static_cast<float>(App::getInstance()->windowWidth);

	aspect = winX / winY;

	mainCamera = new Camera(initialEye, initialCenter, initialUp, UBO_CAMERA);
	mainCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);

	float initialYaw = 90.0f;
	float initialPitch = 0.0f;
	camController = new CameraController(*mainCamera, winY, winX, initialYaw, initialPitch);

}
/////////////////////////////////////////////////////////////////////// SCENE

void MyApp::createSceneGraph()
{
	SceneGraph* scene = new SceneGraph();
	SceneGraphManager::getInstance()->add("Main", scene);
	scene->setCamera(mainCamera);
	callback = new DisableDepthCallback();

	// Test Cube
	SceneNode* testCube = scene->getRoot();
	testCube->setMesh(MeshManager::getInstance()->get("Cube"));
	testCube->setShaderProgram(ShaderProgramManager::getInstance()->get("PenroseCube"));

	// Test terrain
	SceneNode* terrainNode = scene->getRoot()->createNode();
	terrainNode->setMesh(MeshManager::getInstance()->get("Terrain"));
	ShaderProgram* terrainShader = ShaderProgramManager::getInstance()->get("SimpleTexture");
	TextureInfo* texInfo = new TextureInfo(GL_TEXTURE0, 0, "Texture", TextureManager::getInstance()->get("EarthHeightMap"));
	terrainNode->addTextureInfo(texInfo);
	terrainNode->setShaderProgram(terrainShader);

	terrainNode->setMatrix(MatFactory::createTranslationMat4(Vec3(5, 0, 0)));
}
///////////////////////////////////////////////////////////////////// SIMULATION
void MyApp::createSimulation()
{
	Simulation::getInstance()->add(this);
}

///////////////////////////////////////////////////////////////////// DETROYs
void MyApp::destroyManagers()
{
	SamplerManager::freeInstance();
	TextureManager::freeInstance();
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
void MyApp::drawSceneGraph()
{
	SceneGraphManager::getInstance()->get("Main")->draw();
}

void MyApp::processMovement()
{
	GLFWwindow* win = App::getInstance()->getWindow();
	// Capture movement input
	int right = (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS);
	int left = (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS);
	int forward = (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS);
	int backward = (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS);

	if (right || left || forward || backward)
		camController->setMovement(right, left, forward, backward);
}

void MyApp::onUpdate(float deltaTime)
{
	processMovement();
	camController->update(deltaTime);
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
	exit(EXIT_SUCCESS);
}
/////////////////////////////////////////////////////////////////////////// END