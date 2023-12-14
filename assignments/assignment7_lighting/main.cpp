#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <ew/external/stb_image.h>
#include <iostream>

struct Vertex {
	float x, y, z;
	float u, v;
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);
unsigned int loadCubemap(std::vector<std::string> faces);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

float skyboxVertices[] = { //Ryan: The positions of each of the skybox's face vertices       
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

struct AppSettings {
	const char* shadingModeNames[6] = { "Solid Color","Normals","UVs","Texture","Lit","Texture Lit" };
	int shadingModeIndex;

	ew::Vec3 bgColor = ew::Vec3(0.1f);
	ew::Vec3 shapeColor = ew::Vec3(1.0f);
	bool wireframe = false;
	bool drawAsPoints = false;
	bool backFaceCulling = false;

	//Euler angles (degrees)
	ew::Vec3 lightRotation = ew::Vec3(0, 0, 0);
}appSettings;


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader skyboxShader("assets/skybox.vert", "assets/skybox.frag");
	ew::Shader waterShader("assets/Watervert.vert", "assets/Waterfrag.frag");

	//Sierra: Initializing all the height maps and textures
	unsigned int heightMapMain = ew::loadTexture("assets/HeightMap1.png", GL_REPEAT, GL_LINEAR);
	unsigned int heightMap2 = ew::loadTexture("assets/HeightMap2.png", GL_REPEAT, GL_LINEAR);
	unsigned int heightMap3 = ew::loadTexture("assets/HeightMap3.png", GL_REPEAT, GL_LINEAR);
	unsigned int heightMap4 = ew::loadTexture("assets/HeightMap4.png", GL_REPEAT, GL_LINEAR);
	unsigned int groundTexture = ew::loadTexture("assets/GrassTexture.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int rockTexture = ew::loadTexture("assets/RockTexture.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int snowTexture = ew::loadTexture("assets/SnowTexture.jpg", GL_REPEAT, GL_LINEAR);

	unsigned int normalMap = ew::loadTexture("assets/SnowTexture.jpg", GL_REPEAT, GL_LINEAR);

	//Sierra: Creating the base plane for the terrain
	ew::MeshData terrainMeshData = ew::createPlane(500.0f, 500.0f, 1000.0);
	ew::Mesh terrainMesh(terrainMeshData);
	ew::Transform terrainTransform;

	//Lucas: create the mesh for the water and the transformation
	ew::MeshData waterMeshData = ew::createPlane(145.0f, 140.0f, 10);
	ew::Mesh waterMesh(waterMeshData);
	ew::Transform waterTransform;
	waterTransform.position = ew::Vec3(20.0, 53.0, 50.0);

	// Lucas: variables for water properties
	float xScroll = 100;
	float yScroll = 100;
	float mixVal = 0.15;


	//Ryan: Create the skybox's VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Ryan: Grab the images being used on each face of the skybox (must be in this order, check loadCubemap function for why)
	std::vector<std::string> faces =
	{
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces); //Ryan: Turn these faces into a texture

	resetCamera(camera,cameraController);

	//Sierra: Creating boolean values for ImGui controls later
	bool heightMapChoice1 = true;
	bool heightMapChoice2 = false;
	bool heightMapChoice3 = false;
	bool heightMapChoice4 = false;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Ryan: Draw skybox
		glDepthMask(GL_FALSE);
		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);

		/* Ryan: 
		Create the view matrix for the skybox
		- Because we want the skybox to be centered on the camera, we have to remove the
		  translation portion of the view matrix.
		- To do this, we simply take the top-left 3x3 matrix of the view matrix. This keeps
		  rotation and sacling, but removes translation so the skybox is now centered on the
		  camera and moves with it.
		*/
		ew::Mat4 view = {
			camera.ViewMatrix()[0][0], camera.ViewMatrix()[1][0], camera.ViewMatrix()[2][0], 0,
			camera.ViewMatrix()[0][1], camera.ViewMatrix()[1][1], camera.ViewMatrix()[2][1], 0,
			camera.ViewMatrix()[0][2], camera.ViewMatrix()[1][2], camera.ViewMatrix()[2][2], 0,
			                        0,                         0,                         0, 1
		};

		//Ryan: Set the view and projection matrices for the skybox's VAO
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", camera.ProjectionMatrix());
		
		//Ryan: Wrap the skybox's texture around a cube using the skybox's VAO
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		//Draw terrain
		shader.use();

		//Sierra: Using boolean values to determine which height map should be rendered based
		//on values linked to ImGui controls which update every time the scene is rendered
		if (heightMapChoice2)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMap2);
			shader.setInt("_HeightMap", 0);
		}
		else if (heightMapChoice3)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMap3);
			shader.setInt("_HeightMap", 0);
		}
		else if (heightMapChoice4)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMap4);
			shader.setInt("_HeightMap", 0);
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMapMain);
			shader.setInt("_HeightMap", 0);

			//Lucas: DRAW WATER
			waterShader.use();

			//Lucas: Set water uniforms
			waterShader.setFloat("_Time", time);
			waterShader.setFloat("xScroll", xScroll);
			waterShader.setFloat("yScroll", yScroll);
			waterShader.setFloat("_mixVal", mixVal);
			waterShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
			waterShader.setVec3("_CameraPos", camera.position);

			waterShader.setInt("_Texture", 0);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, normalMap);

			waterShader.setInt("_normalMap", 4);
			ew::Vec3 lightRot = appSettings.lightRotation * ew::DEG2RAD;
			ew::Vec3 lightF = ew::Vec3(sinf(lightRot.y) * cosf(lightRot.x), sinf(lightRot.x), -cosf(lightRot.y) * cosf(lightRot.x));
			waterShader.setVec3("_LightDir", lightF);

			waterShader.setMat4("_Model", waterTransform.getModelMatrix());

			waterMesh.draw();
			shader.use();
		}

		//Sierra: Binding all the textures so that they can be called and used in the fragment shader
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, groundTexture);
		shader.setInt("_GrassTexture", 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, rockTexture);
		shader.setInt("_RockTexture", 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, snowTexture);
		shader.setInt("_SnowTexture", 3);


		shader.setMat4("_ViewProjection", camera.ProjectionMatrix()* camera.ViewMatrix());
		shader.setMat4("_Model", terrainTransform.getModelMatrix());
		terrainMesh.draw();

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			//Sierra: ImGui controls to toggle between the height map options.
			//Also code to make sure only 1 height map is selected at a time.
			if (ImGui::CollapsingHeader("Height Map Select")) {
				if (ImGui::Checkbox("HeightMapMain", &heightMapChoice1)) {
					heightMapChoice2 = false;
					heightMapChoice3 = false;
					heightMapChoice4 = false;
				}
				if (ImGui::Checkbox("HeightMap2", &heightMapChoice2)) {
					heightMapChoice1 = false;
					heightMapChoice3 = false;
					heightMapChoice4 = false;
				}
				if (ImGui::Checkbox("HeightMap3", &heightMapChoice3)) {
					heightMapChoice1 = false;
					heightMapChoice2 = false;
					heightMapChoice4 = false;
				}
				if (ImGui::Checkbox("HeightMap4", &heightMapChoice4)) {
					heightMapChoice1 = false;
					heightMapChoice2 = false;
					heightMapChoice3 = false;
				}
			}

			ImGui::ColorEdit3("BG color", &bgColor.x);
			ImGui::DragFloat("xScroll", &xScroll, 0.1f, 0.0f);
			ImGui::DragFloat("yScroll", &yScroll, 0.1f, 0.0f);
			ImGui::DragFloat("mixVal", &mixVal, 0.01f, 0.0f, 1.0f);

			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	//Sierra: Changed the default settings of position and far plane to better suit how the terrain size
	camera.position = ew::Vec3(0, 100, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 1000.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++) //Ryan: We use faces.size() in order to call the following code for each face of the skybox
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0); //Ryan: Load the data
		if (data) //Ryan: Check if the data exists
		{
			/* Ryan:
			If so, apply the texture's target
			- For cubemaps, there are six different texture target functions we can call:
				GL_TEXTURE_CUBE_MAP_POSITIVE_X, for right orientation
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X, for left orientation
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y, for top orientation
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, for bottom orientation
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z, for back orientation
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, for front orientation
			- By calling GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, we start at right orientation for first pass through the for loop,
			  but after every pass, the + i sends it to the next orientation function call. For example, when i = 1,
			  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i will return with GL_TEXTURE_CUBE_MAP_NEGATIVE_X, which gives us left orientation.
			- For this reason, the order in which we pass through our skybox images is important, as mentioned previously in the array.
			*/
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl; //Ryan: If not, throw an error
			stbi_image_free(data);
		}
	}
	//Ryan: Specify the texture's wrapping and filtering methods
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}