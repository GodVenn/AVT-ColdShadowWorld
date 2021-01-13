#include "../Headers/pch.h"
#include "../Headers/Engine.h"

using namespace engine;
// TODO
// Note: This flag is used to run the engine with a pre-maded terrain
//		 intead of the procedurally generated one. Change to 0 in order
//		 to use the procedural terrain.
#define TEST_TERRAIN 1

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
	BackMode* backModeCB = nullptr;
	/// BINDING POINTs
	const int UBO_CAMERA = 0;

	/// CAMERA SETUP VALUES
	Camera* mainCamera = nullptr;
	CameraController* camController = nullptr;
	const Vec3 initialEye = Vec3(0.0f, 5.0f, 5.0f);
	const Vec3 initialCenter = Vec3(0.0f, 0.0f, 0.0f);
	const Vec3 initialUp = Vec3(0.0f, 1.0f, 0.0f);
	const float fovThreshold = 45.f;
	float fov = 45.f;

	const float near = 0.1f;
	const float far = 100.f;

	float aspect = 0.f;

	//HUD CAMERA SETUP VALUES
	Camera* hudCamera = nullptr;
	Follow2DCameraController* hudCameraController = nullptr;

	// Quad2D;
	Quad2D* quad = nullptr;

	/// CAMERA MOVEMENTS
	const float cameraSpeed = 6.0f;
	const float cameraSensitivity = 0.05f;

	// Gooch and Shadow
	Vec3 LightPos = Vec3(2.f, 7.f, -3.f); 
	float silhouetteOffset = 0.015f;

	void createTextures();
	void createMeshes();
	void setupCamera();
	void createShaderPrograms();
	void createSceneGraph();
	void createSimulation();

	void destroyManagers();
	void destroySimulation();
	void destroyCallbacks();
	void destroyCameras();

	void drawSceneGraph();
	void processMovement();
	// HUD
	void renderQuad(ShaderProgram* shader, Texture* texture, const std::string& textureUniform);
	void renderHUD(RenderTargetTexture& rttHud);
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
	destroyCameras();
	destroyCallbacks();
	destroyManagers();
	destroySimulation();
}

void MyApp::window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	int width = App::getInstance()->windowWidth = winx;
	int height = App::getInstance()->windowHeight = winy;

	aspect = static_cast<float>(width) / static_cast<float>(height);
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
	/**/
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


	ShaderProgram* quadTexture = new ShaderProgram();
	quadTexture->addShader(shaderFolder + "textureFBO_vs.glsl", GL_VERTEX_SHADER);
	quadTexture->addShader(shaderFolder + "textureFBO_fs.glsl", GL_FRAGMENT_SHADER);
	quadTexture->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	quadTexture->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	quadTexture->addUniform("screenTexture");
	quadTexture->addUniform("translationVector");
	quadTexture->addUniform("scaleMultiplier");
	quadTexture->create();
	quadTexture->bind();
	quadTexture->setUniform2f("translationVector", Vec2(0.65f, -0.7f));
	quadTexture->setUniform2f("scaleMultiplier", Vec2(0.25f, 0.25f));
	quadTexture->setUniform1i("screenTexture", 0);
	quadTexture->unbind();
	ShaderProgramManager::getInstance()->add("QuadTexture", quadTexture);

	ShaderProgram* hud = new ShaderProgram();
	hud->addShader(shaderFolder + "hud_vs.glsl", GL_VERTEX_SHADER);
	hud->addShader(shaderFolder + "hud_fs.glsl", GL_FRAGMENT_SHADER);
	hud->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	hud->addAttribute(Mesh::TEXCOORDS, engine::TEXCOORDS_ATTRIBUTE);
	hud->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	hud->addUniform(engine::MODEL_MATRIX);
	hud->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	hud->create();
	ShaderProgramManager::getInstance()->add("Hud", hud);

	// SILHOUETTE
	ShaderProgram* silhouette = new ShaderProgram();
	silhouette->addShader(shaderFolder + "silhouette_vs.glsl", GL_VERTEX_SHADER);
	silhouette->addShader(shaderFolder + "silhouette_fs.glsl", GL_FRAGMENT_SHADER);
	silhouette->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	silhouette->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	silhouette->addUniform(engine::MODEL_MATRIX);
	silhouette->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	silhouette->addUniform("Offset");
	silhouette->create();
	silhouette->bind();
	silhouette->setUniform1f("Offset", silhouetteOffset);
	ShaderProgramManager::getInstance()->add("Silhouette", silhouette);

	ShaderProgram* gooch = new ShaderProgram();
	gooch->addShader(shaderFolder + "goochShading_vs.glsl", GL_VERTEX_SHADER);
	gooch->addShader(shaderFolder + "goochShading_fs.glsl", GL_FRAGMENT_SHADER);
	gooch->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	gooch->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	gooch->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	gooch->addUniform(engine::MODEL_MATRIX);
	gooch->addUniform("LightPosition");
	gooch->addUniform("SurfaceColor");
	gooch->addUniform("AmbientWarm");
	gooch->addUniform("AmbientCool");
	gooch->addUniform("DiffuseCool");
	gooch->addUniform("DiffuseWarm");
	gooch->addUniform("SpecularPower");
	gooch->create();
	gooch->bind();
	gooch->setUniform3f("SurfaceColor", Vec3(0.75f, 0.75f, 0.75f));
	gooch->setUniform3f("AmbientWarm", Vec3(0.6f, 0.f, 0));
	gooch->setUniform3f("AmbientCool", Vec3(0, 0, 0.6f));
	gooch->setUniform1f("DiffuseCool", 0.45f);
	gooch->setUniform1f("DiffuseWarm", 0.45f);
	gooch->setUniform1f("SpecularPower", 45.f);
	gooch->setUniform3f("LightPosition", LightPos);
	ShaderProgramManager::getInstance()->add("Gooch", gooch);

}

/////////////////////////////////////////////////////////////////////// TEXTUREs
void MyApp::createTextures()
{
	const std::string TextureFolder = "Textures\\";

	std::string texturePath = TextureFolder + "earthbump1k.jpg";
	Texture2D* earth = new Texture2D();
	earth->load(texturePath);
	TextureManager::getInstance()->add("EarthHeightMap", earth);

	Texture2D* circle = new Texture2D();
	circle->load(TextureFolder + "Circle.png");
	TextureManager::getInstance()->add("Circle", circle);

	Texture2D* radar = new Texture2D();
	radar->load(TextureFolder + "Radar.png");
	TextureManager::getInstance()->add("Radar", radar);

	Texture2D* arrow = new Texture2D();
	arrow->load(TextureFolder + "Arrow.png");
	TextureManager::getInstance()->add("Arrow", arrow);

	// Render Target Texture for the hud
	RenderTargetTexture* rtt = new RenderTargetTexture();
	rtt->create(App::getInstance()->windowWidth, App::getInstance()->windowWidth);
	TextureManager::getInstance()->add("RTT", (RenderTargetTexture*)rtt);
}

/////////////////////////////////////////////////////////////////////// MESHes
void MyApp::createMeshes()
{
	const std::string modelsFolder = "Models\\";
	std::string cube_file = modelsFolder + "Cube.obj";
	Mesh* cube = new Mesh(cube_file);
	MeshManager::getInstance()->add("Cube", cube);

#if TEST_TERRAIN
	std::string testTerrain_file = modelsFolder + "TestTerrain.obj";
	Mesh* testTerrain = new Mesh(testTerrain_file);
	MeshManager::getInstance()->add("TestTerrain", testTerrain);
#else
	// Terrain
	float terrainWidth = 10;
	float terrainLength = 5;
	float terrainMaxHeight = 0.1f;
	unsigned int terrainSimplicity = 4;
	bool calculateNormals = true;
	TerrainBuilder terrainBuilder = TerrainBuilder(terrainWidth, terrainLength, terrainSimplicity, terrainMaxHeight, calculateNormals);

	const std::string heightMap = "Textures\\earthbump1k.jpg";
	terrainBuilder.setHeightMap(heightMap);
	Mesh* terrain = terrainBuilder.buildMesh();
	MeshManager::getInstance()->add("Terrain", terrain);
#endif // TEST_TERRAIN

	// Quad
	quad = new Quad2D();

}
/////////////////////////////////////////////////////////////////////// CAMERA
void MyApp::setupCamera()
{
	glfwSetInputMode(App::getInstance()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float height = static_cast<float>(App::getInstance()->windowHeight);
	float width = static_cast<float>(App::getInstance()->windowWidth);

	aspect = width / height;

	mainCamera = new Camera(initialEye, initialCenter, initialUp);
	mainCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);

	float initialYaw = -90.0f;
	float initialPitch = 0.0f;
	camController = new CameraController(*mainCamera, height, width, initialYaw, initialPitch);

	hudCamera = new Camera({initialEye.x, initialEye.y + 20, initialEye.z}, initialEye, initialUp); //TODO: remove magic number
	float zoomLevel = 10.0f;
	//hudCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
	hudCamera->setOrthographicProjectionMatrix(aspect * -zoomLevel, aspect * zoomLevel, -zoomLevel, zoomLevel, near, far);
	hudCameraController = new Follow2DCameraController(hudCamera, camController);
}
/////////////////////////////////////////////////////////////////////// SCENE

void MyApp::createSceneGraph()
{
	SceneGraph* scene = new SceneGraph();
	SceneGraphManager::getInstance()->add("Main", scene);
	scene->setCamera(mainCamera);
	//callback = new DisableDepthCallback();

	// Test Cube
	/** /
	SceneNode* testCube = scene->getRoot();
	testCube->setMesh(MeshManager::getInstance()->get("Cube"));
	testCube->setShaderProgram(ShaderProgramManager::getInstance()->get("PenroseCube"));
	/**/
#if TEST_TERRAIN
	// Test terrain
	SceneNode* terrainNode = scene->getRoot();
	terrainNode->setMesh(MeshManager::getInstance()->get("TestTerrain"));
	// Gooch Shader
	ShaderProgram* terrainShader = ShaderProgramManager::getInstance()->get("Gooch");
	terrainNode->setShaderProgram(terrainShader);
	// Old terrain texture
	//TextureInfo* texInfo = new TextureInfo(GL_TEXTURE0, 0, "Texture", TextureManager::getInstance()->get("EarthHeightMap"));
	//terrainNode->addTextureInfo(texInfo);
	// SILHOUETTE 
	backModeCB = new BackMode();
	SceneNode* n_Silhouette = terrainNode->createNode();
	n_Silhouette->setMesh(MeshManager::getInstance()->get("TestTerrain"));
	ShaderProgram* s_Silhouette = ShaderProgramManager::getInstance()->get("Silhouette");
	n_Silhouette->setShaderProgram(s_Silhouette);
	n_Silhouette->setCallback(backModeCB);
#else
	// Test terrain
	SceneNode* terrainNode = scene->getRoot();
	terrainNode->setMesh(MeshManager::getInstance()->get("Terrain"));
	// Gooch Shader
	ShaderProgram* terrainShader = ShaderProgramManager::getInstance()->get("Gooch");
	terrainNode->setShaderProgram(terrainShader);
	// Old terrain texture
	//TextureInfo* texInfo = new TextureInfo(GL_TEXTURE0, 0, "Texture", TextureManager::getInstance()->get("EarthHeightMap"));
	//terrainNode->addTextureInfo(texInfo);
	// SILHOUETTE 
	backModeCB = new BackMode();
	SceneNode* n_Silhouette = terrainNode->createNode();
	n_Silhouette->setMesh(MeshManager::getInstance()->get("Terrain"));
	ShaderProgram* s_Silhouette = ShaderProgramManager::getInstance()->get("Silhouette");
	n_Silhouette->setShaderProgram(s_Silhouette);
	n_Silhouette->setCallback(backModeCB);
#endif // TEST_TERRAIN

	
	/**/
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
	// TODO removed
	delete quad;
	MeshManager::freeInstance();
	SceneGraphManager::freeInstance();
}

void MyApp::destroySimulation()
{
	Simulation::freeInstance();
}

void MyApp::destroyCallbacks()
{
	//delete callback;
	delete backModeCB;
}

void MyApp::destroyCameras()
{
	/// Main Camera
	delete mainCamera;
	delete camController;
	/// Hud Camera
	delete hudCamera;
	delete hudCameraController;
}

///////////////////////////////////////////////////////////////////// DRAW AND UPDATEs
void MyApp::renderQuad(ShaderProgram* shader, Texture* texture, const std::string& textureUniform)
{
	shader->bind();
	shader->setUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	texture->bind();

	glDisable(GL_DEPTH_TEST);
	quad->draw();
	glEnable(GL_DEPTH_TEST);

	//glBindTexture(GL_TEXTURE_2D, 0);
}
void MyApp::renderHUD(RenderTargetTexture& rttHud)
{
	// Main RTT Shader
	ShaderProgram* quadTexture_s = ShaderProgramManager::getInstance()->get("QuadTexture");
	// 1- Render circle
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);	// All fragments should pass the stencil test
	glStencilMask(0xFF);				// Able to write in the stencil buffer
	quadTexture_s->bind();
	quadTexture_s->setUniform1f("interpolationFactor", 1);
	Texture2D& circle = *(Texture2D*)TextureManager::getInstance()->get("Circle");
	renderQuad(quadTexture_s, &circle, "screenTexture");	
	// 2 - Draw RTT on top of the circle 
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);				// Disable writing to the stencil buffer
	// Obtain Radar Texture
	Texture2D& radar = *(Texture2D*)TextureManager::getInstance()->get("Radar"); 
	glActiveTexture(GL_TEXTURE1);
	radar.bind();
	quadTexture_s->bind();
	quadTexture_s->setUniform1i("radarTexture", 1);
	quadTexture_s->setUniform1f("interpolationFactor", 0.6f);
	renderQuad(quadTexture_s, &rttHud, "screenTexture");
	// 3 - Render Arrow
	// Obtain Arrow texture
	Texture2D& arrow = *(Texture2D*)TextureManager::getInstance()->get("Arrow");
	quadTexture_s->bind();
	quadTexture_s->setUniform1f("interpolationFactor", 1);
	renderQuad(quadTexture_s, &arrow, "screenTexture");
	// Reset Stencil
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);
}
void MyApp::drawSceneGraph()
{
	// Draw main scene
	SceneGraph* scene = SceneGraphManager::getInstance()->get("Main");
	scene->draw();
	// Switch to HUD's camera
	scene->setCamera(hudCamera);
	// Obtain Render Target Texture for drawing the HUD
	RenderTargetTexture& hud = *(RenderTargetTexture*)TextureManager::getInstance()->get("RTT");

	// Draw HUD
	hud.clearColor(0.5f, 0.5f, 0.5f, 1.f);
	hud.bindFramebuffer();
	scene->draw();
	hud.unbindFramebuffer();
	//Render HUD into the main scene
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	renderHUD(hud);
	// Reset Viewport size
	glViewport(0, 0, App::getInstance()->windowWidth, App::getInstance()->windowHeight);
	// Reset shader to the original one
	scene->setCamera(mainCamera);

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
	{
		camController->setMovement(right, left, forward, backward);
	}

}

void MyApp::onUpdate(float deltaTime)
{
	processMovement();
	camController->update(deltaTime);
	hudCameraController->update(deltaTime);
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