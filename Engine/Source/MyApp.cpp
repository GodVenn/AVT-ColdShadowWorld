#include "../Headers/pch.h"
#include "../Headers/Engine.h"
/**/
////////////////////////////////////
/// MEMORY LEAK DETECTION
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
////////////////////////////////////
using namespace engine;
// TODO
// Note: This flag is used to run the engine with a pre-maded terrain
//		 intead of the procedurally generated one. Change to 0 in order
//		 to use the procedural terrain.
#define TEST_TERRAIN 0

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

	// MAP GENERATION (PERLIN NOISE PARAMETERS)
	const float persistence = 0.6f; // in [0, 1]
	const float lacunarity = 2.2f;	// >=1
	const int octaves = 6;			// >=1
		// Sizes determine "level of detail, and are unrelated to the actual size of the final terrain
	const int mapSizeX = 500;
	const int mapSizeY = 500;
	
	// TERRAIN
	const float terrainMaxHeight = 15;
	const float terrainSizeX = 100;
	const float terrainSizeY = 100;
	unsigned int terrainSimplicity = 5; // The higher the more simplified compared to height map

	/// CAMERA SETUP VALUES
	// Main Camera
	Camera* mainCamera = nullptr;
	CameraController* camController = nullptr;
	const Vec3 initialEye = Vec3(0.0f, terrainMaxHeight, terrainSizeY/2);
	const Vec3 initialCenter = Vec3(0.0f, 0.0f, -1.0f);
	const Vec3 initialUp = Vec3(0.0f, 1.0f, 0.0f);
	const float fovThreshold = 45.f;
	float fov = 45.f;

	const float near = 0.1f;
	const float far = 10000.f;
	float aspect = 0.f;

	/// CAMERA MOVEMENTS
	const float cameraSpeed = 2.f; // Seems to work nicely with a value of ~1/50 of terrain size
	const float cameraSensitivity = 0.05f;

	//Particles
	Particle default_Particle;
	int particlesToSpawn = 5;
	float maxTime = 0.0001f; //Particle respawn time
	float timer = maxTime;

	// HUD Camera
	Camera* hudCamera = nullptr;
	Follow2DCameraController* hudCameraController = nullptr;
	float HUDoffset = terrainMaxHeight + 50.f;

	// Shadow Map
	Camera* cameraShadow = nullptr;
	const float boxSize = 1.f;
	float nearShadowPlane = 0.01f;
	float farShadowPlane = 100.f;
	float boxShadowSize = 100.7f;
	uint32_t shadowMapResolution = 4096 * 3;

	// Quad2D;
	Quad2D* quad = nullptr;

	// Gooch and Shadow
	Vec3 LightPos = Vec3(40.f, 40.f, -40.f); 
	float silhouetteOffset = 0.03f;

	void createTextures();
	void createMeshes();
	void setupCamera();
	void createShaderPrograms();
	void createSceneGraph();
	void createMainScene();
	void createShadowMapScene();
	void createHUDScene();
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
	ShaderProgram* normalsShader = new ShaderProgram();
	normalsShader->addShader(shaderFolder + "normals_vs.glsl", GL_VERTEX_SHADER);
	normalsShader->addShader(shaderFolder + "normals_fs.glsl", GL_FRAGMENT_SHADER);
	normalsShader->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	normalsShader->addAttribute(Mesh::NORMALS, engine::NORMAL_ATTRIBUTE);
	normalsShader->addUniform(engine::MODEL_MATRIX);
	normalsShader->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	normalsShader->create();
	ShaderProgramManager::getInstance()->add("Normals", normalsShader);
	/** /
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
	/**/

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
	ShaderProgramManager::getInstance()->add("QuadTexture", quadTexture);
	/** /
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
	/**/
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

	ShaderProgram* shadowMap = new ShaderProgram();
	shadowMap->addShader(shaderFolder + "goochShading_vs.glsl", GL_VERTEX_SHADER);
	shadowMap->addShader(shaderFolder + "goochShading_fs.glsl", GL_FRAGMENT_SHADER);
	shadowMap->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	shadowMap->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	shadowMap->addUniform(engine::MODEL_MATRIX);
	shadowMap->create();
	ShaderProgramManager::getInstance()->add("ShadowMap", shadowMap);

	ShaderProgram* shadowDebug = new ShaderProgram();
	shadowDebug->addShader(shaderFolder + "ShadowMapDebug_vs.glsl", GL_VERTEX_SHADER);
	shadowDebug->addShader(shaderFolder + "ShadowMapDebug_fs.glsl", GL_FRAGMENT_SHADER);
	shadowDebug->addUniform("u_ShadowMap");
	shadowDebug->create();
	ShaderProgramManager::getInstance()->add("ShadowMapDebug", shadowDebug);

	ShaderProgram* skybox = new ShaderProgram();
	skybox->addShader(shaderFolder + "skybox_vs.glsl", GL_VERTEX_SHADER);
	skybox->addShader(shaderFolder + "skybox_fs.glsl", GL_FRAGMENT_SHADER);
	skybox->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	skybox->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	skybox->addUniform(engine::MODEL_MATRIX);
	skybox->addUniform("skybox");
	skybox->create();
	ShaderProgramManager::getInstance()->add("Skybox", skybox);



	ShaderProgram* particles = new ShaderProgram();
	particles->addShader(shaderFolder + "particle_vs.glsl", GL_VERTEX_SHADER);
	particles->addShader(shaderFolder + "particle_geom.glsl", GL_GEOMETRY_SHADER);
	particles->addShader(shaderFolder + "particle_fs.glsl", GL_FRAGMENT_SHADER);
	particles->addAttribute(Mesh::VERTICES, engine::VERTEX_ATTRIBUTE);
	particles->addAttribute(1, engine::COLOR_ATTRIBUTE);
	particles->addUniform("snowText"),
	particles->addUniformBlock(engine::VIEW_PROJECTION_MATRIX, UBO_CAMERA);
	particles->create();
	ShaderProgramManager::getInstance()->add("Particles", particles);
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

	// Shadow Map
	TextureShadowMap* shadowMap = new TextureShadowMap();
	shadowMap->create(shadowMapResolution, shadowMapResolution);
	TextureManager::getInstance()->add("ShadowMap", (TextureShadowMap*)shadowMap);

	// Skybox
	CubeMap* SkyBox = new CubeMap();
	std::vector<std::string> faces = 
	{
		"Textures\\Skybox\\right.png",
		"Textures\\Skybox\\left.png",
		"Textures\\Skybox\\up.png",
		"Textures\\Skybox\\down.png",
		"Textures\\Skybox\\front.png",
		"Textures\\Skybox\\back.png"
	};
	SkyBox->load(faces);
	TextureManager::getInstance()->add("Skybox", SkyBox);


	std::string texturePathSnow = TextureFolder + "snow.png";
	Texture2D* snow = new Texture2D();
	snow->load(texturePathSnow);
	TextureManager::getInstance()->add("Snow", snow);
}

/////////////////////////////////////////////////////////////////////// MESHes
void MyApp::createMeshes()
{
	const std::string modelsFolder = "Models\\";
	std::string cube_file = modelsFolder + "Cube.obj";
	Mesh* cube = new Mesh(cube_file);
	MeshManager::getInstance()->add("Cube", cube);

	// Quad
	quad = new Quad2D();

#if TEST_TERRAIN
	std::string testTerrain_file = modelsFolder + "TestTerrain.obj";
	Mesh* testTerrain = new Mesh(testTerrain_file);
	MeshManager::getInstance()->add("TestTerrain", testTerrain);
#else
	// Terrain
	bool calculateNormals = true;
	bool flatShading = true;
	TerrainBuilder terrainBuilder = TerrainBuilder(terrainSizeX, terrainSizeY, terrainSimplicity, terrainMaxHeight, calculateNormals);
	terrainBuilder.flatShading = flatShading;

	const std::string heightMap = "Textures\\earthbump1k.jpg";
	//terrainBuilder.setHeightMap(heightMap);
	int seed = (int)time(NULL);
	std::cout << "[Map Generation] Seed = " << seed << std::endl;
	terrainBuilder.generateHeightMap(mapSizeX, mapSizeY, octaves, persistence, lacunarity, seed);

	Mesh* terrain = terrainBuilder.buildMesh();
	MeshManager::getInstance()->add("Terrain", terrain);
#endif // TEST_TERRAIN
}
/////////////////////////////////////////////////////////////////////// CAMERA
void MyApp::setupCamera()
{
	glfwSetInputMode(App::getInstance()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float height = static_cast<float>(App::getInstance()->windowHeight);
	float width = static_cast<float>(App::getInstance()->windowWidth);

	aspect = width / height;
	// Main Camera
	mainCamera = new Camera(initialEye, initialCenter, initialUp);
	mainCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;
	camController = new CameraController(*mainCamera, height, width, initialYaw, initialPitch);
	camController->movementSpeed = cameraSpeed;
	// HUD camera
	hudCamera = new Camera({initialEye.x, initialEye.y + 20, initialEye.z}, initialEye, initialUp);
	float zoomLevel = 10.0f;
	//hudCamera->setPerspectiveProjectionMatrix(fov, aspect, near, far);
	hudCamera->setOrthographicProjectionMatrix(aspect * -zoomLevel, aspect * zoomLevel, -zoomLevel, zoomLevel, near, far);
	hudCameraController = new Follow2DCameraController(hudCamera, camController, HUDoffset);

	// Shadow Map Camera
	cameraShadow = new Camera(LightPos, initialEye, initialUp);
	cameraShadow->setOrthographicProjectionMatrix(-boxShadowSize, boxShadowSize, -boxShadowSize, boxShadowSize, nearShadowPlane, farShadowPlane);
}
/////////////////////////////////////////////////////////////////////// SCENE

void MyApp::createSceneGraph()
{
	// Creates the scene that generate the depth map
	createShadowMapScene();
	// Creates the hud's scene
	createHUDScene();
	// Creates the main scene
	createMainScene();
}

void MyApp::createMainScene()
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
	// Add shadow's light position
	terrainShader->bind();
	terrainShader->setUniformMat4("ViewLightMatrix", cameraShadow->getViewMatrix());
	terrainShader->setUniformMat4("ProjLightMatrix", cameraShadow->getProjMatrix());
	terrainShader->setUniform3f("LightPosition", LightPos);
	// Set Shadows Texture
	TextureInfo* texInfoShadow = new TextureInfo(GL_TEXTURE0, 0, "u_ShadowMap",
		(TextureShadowMap*)(TextureManager::getInstance()->get("ShadowMap")), 0);
	terrainNode->addTextureInfo(texInfoShadow);
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
	// Add shadow's light position
	terrainShader->bind();
	terrainShader->setUniformMat4("ViewLightMatrix", cameraShadow->getViewMatrix());
	terrainShader->setUniformMat4("ProjLightMatrix", cameraShadow->getProjMatrix());
	terrainShader->setUniform3f("LightPosition", LightPos);
	// Set Shadows Texture
	TextureInfo* texInfoShadow = new TextureInfo(GL_TEXTURE0, 0, "u_ShadowMap",
		(TextureShadowMap*)(TextureManager::getInstance()->get("ShadowMap")), 0);
	terrainNode->addTextureInfo(texInfoShadow);

	// SILHOUETTE 
	backModeCB = new BackMode();
	SceneNode* n_Silhouette = terrainNode->createNode();
	n_Silhouette->setMesh(MeshManager::getInstance()->get("Terrain"));
	ShaderProgram* s_Silhouette = ShaderProgramManager::getInstance()->get("Silhouette");
	n_Silhouette->setShaderProgram(s_Silhouette);
	n_Silhouette->setCallback(backModeCB);
#endif // TEST_TERRAIN

	// Skybox
	SceneNode* skybox = scene->createNode();
	skybox->setMesh(MeshManager::getInstance()->get("Cube"));
	ShaderProgram* skyBoxShader = ShaderProgramManager::getInstance()->get("Skybox");
	skybox->setShaderProgram(skyBoxShader);
	TextureInfo* texInfoSkybox = new TextureInfo(GL_TEXTURE_CUBE_MAP, 0, "skybox",
		TextureManager::getInstance()->get("Skybox"), 0);
	skybox->addTextureInfo(texInfoSkybox);
	skybox->setCallback(backModeCB);

}

void MyApp::createShadowMapScene()
{
	SceneGraph* scene = new SceneGraph();
	SceneGraphManager::getInstance()->add("ShadowMap", scene);
	scene->setCamera(cameraShadow);


#if TEST_TERRAIN
	// Test terrain
	SceneNode* terrainNode = scene->getRoot();
	terrainNode->setMesh(MeshManager::getInstance()->get("TestTerrain"));
	// Gooch Shader
	ShaderProgram* terrainShader = ShaderProgramManager::getInstance()->get("ShadowMap");
	terrainNode->setShaderProgram(terrainShader);
#else
	// Terrain
	SceneNode* terrainNode = scene->getRoot();
	terrainNode->setMesh(MeshManager::getInstance()->get("Terrain"));
	// Gooch Shader
	ShaderProgram* terrainShader = ShaderProgramManager::getInstance()->get("ShadowMap");
	terrainNode->setShaderProgram(terrainShader);
#endif // TEST_TERRAIN

	TextureInfo* texInfoS = new TextureInfo(GL_TEXTURE1, 1, "snowText", TextureManager::getInstance()->get("Snow"));
	ParticleSystem::getInstance()->text = texInfoS;
}

void MyApp::createHUDScene()
{
	SceneGraph* scene = new SceneGraph();
	SceneGraphManager::getInstance()->add("HUD", scene);
	scene->setCamera(hudCamera);
	// Terrain
	SceneNode* terrainNode = scene->getRoot();
	terrainNode->setMesh(MeshManager::getInstance()->get("Terrain"));
	// Gooch Shader
	ShaderProgram* terrainShader = ShaderProgramManager::getInstance()->get("Normals");
	terrainNode->setShaderProgram(terrainShader);
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

	glBindTexture(GL_TEXTURE_2D, 0);
}
void MyApp::renderHUD(RenderTargetTexture& rttHud)
{
	// Main RTT Shader
	ShaderProgram* quadTexture_s = ShaderProgramManager::getInstance()->get("QuadTexture");
	// 1- Render circle
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);	// All fragments should pass the stencil test
	glStencilMask(0xFF);				// Able to write in the stencil buffer
	{
		quadTexture_s->bind();
		quadTexture_s->setUniform1f("interpolationFactor", 1);
		Texture2D& circle = *(Texture2D*)TextureManager::getInstance()->get("Circle");
		renderQuad(quadTexture_s, &circle, "screenTexture");
	}
	// 2 - Draw RTT on top of the circle 
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);				// Disable writing to the stencil buffer
	{
		// Obtain Radar Texture
		Texture2D& radar = *(Texture2D*)TextureManager::getInstance()->get("Radar");
		glActiveTexture(GL_TEXTURE1);
		radar.bind();
		quadTexture_s->bind();
		quadTexture_s->setUniform1i("radarTexture", 1);
		quadTexture_s->setUniform1f("interpolationFactor", 0.6f);
		renderQuad(quadTexture_s, &rttHud, "screenTexture");
	}
	// 3 - Render Arrow
	{
		// Obtain Arrow texture
		Texture2D& arrow = *(Texture2D*)TextureManager::getInstance()->get("Arrow");
		quadTexture_s->bind();
		quadTexture_s->setUniform1f("interpolationFactor", 1);
		renderQuad(quadTexture_s, &arrow, "screenTexture");
	}
	// 4 - Reset Stencil
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);
}
void MyApp::drawSceneGraph()
{
	auto width = App::getInstance()->windowWidth;
	auto height = App::getInstance()->windowHeight;
	// 1 - Draw main scene	
	TextureShadowMap& shadowMap = *(TextureShadowMap*)TextureManager::getInstance()->get("ShadowMap");
	{
		shadowMap.bindFramebuffer();
		glCullFace(GL_FRONT);
		SceneGraphManager::getInstance()->get("ShadowMap")->draw();
		glCullFace(GL_BACK);
		shadowMap.unbindFramebuffer();
	}
	// 2 - Draw main scene
	SceneGraph* scene = SceneGraphManager::getInstance()->get("Main");
	{
		glViewport(0, 0, width, height);
		scene->draw();
	}

	// 3 - Snow
	{
		float deltaTime = static_cast<float>(App::getInstance()->deltaTime);

		timer -= deltaTime;
		if (timer <= 0.0f)
		{
			for (int i = 0; i < particlesToSpawn; i++)
				ParticleSystem::getInstance()->AddParticle(default_Particle, SceneGraphManager::getInstance()->get("Main")->getCamera());
			timer = maxTime;
		}
		ParticleSystem::getInstance()->OnUpdate(deltaTime, SceneGraphManager::getInstance()->get("Main")->getCamera());
	}

	// 4 - Draw HUD
	{
		// Switch to HUD's camera
		scene->setCamera(hudCamera);
		SceneGraph& hudScene = *SceneGraphManager::getInstance()->get("HUD");
		// Obtain Render Target Texture for drawing the HUD
		RenderTargetTexture& hud = *(RenderTargetTexture*)TextureManager::getInstance()->get("RTT");
		// Draw HUD
		hud.clearColor(0.5f, 0.5f, 0.5f, 1.f);
		hud.bindFramebuffer();
		hudScene.draw();
		hud.unbindFramebuffer();
		//Render HUD into the main scene
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		renderHUD(hud);
	}
	// 5 - Draw DEBUG Shadow Map -> TODO: REMOVED OR COMMENT INNER BLOCK
	{
		/** /
		glViewport(0, 0, (int)(width/3.f), (int)(height/3.f));
		ShaderProgram& s_shadowMap = *ShaderProgramManager::getInstance()->get("ShadowMapDebug");
		shadowMap.renderQuad(&s_shadowMap, "u_ShadowMap");
		/**/
	}
	// Reset Viewport size
	glViewport(0, 0, width, height);
	// Reset shader to the original one
	scene->setCamera(mainCamera);
}

void MyApp::processMovement()
{
	GLFWwindow* win = App::getInstance()->getWindow();
	// Capture movement input
	int right = Input::isKeyPressed(GLFW_KEY_D);
	int left = Input::isKeyPressed(GLFW_KEY_A);
	int forward = Input::isKeyPressed(GLFW_KEY_W);
	int backward = Input::isKeyPressed(GLFW_KEY_S);

	if (right || left || forward || backward)
	{
		camController->setMovement(right, left, forward, backward);
		ParticleSystem::getInstance()->SetCameraMovement(camController->getMovement());
	}

	if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS) //FORCE EMIT PARTICLE
	{
		//std::cout << "Emit!\n";
		for (int i = 0; i < 1; i++)
			ParticleSystem::getInstance()->AddParticle(default_Particle, SceneGraphManager::getInstance()->get("Main")->getCamera());
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
	int is_fullscreen = 1;
	int is_vsync = 0;
	int width = 1920;
	int height = 1080;
	engine::App::getInstance()->setupWindow(width, height, "Cold Shadow World", is_fullscreen, is_vsync);
	engine::App::getInstance()->init();
	engine::App::getInstance()->run();

	engine::App::freeInstance();
	exit(EXIT_SUCCESS);
}
/////////////////////////////////////////////////////////////////////////// END