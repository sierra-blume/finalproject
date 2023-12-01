//#include <stdio.h>
//#include <math.h>
//
//#include <ew/external/glad.h>
//#include <ew/ewMath/ewMath.h>
//#include <GLFW/glfw3.h>
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//
//#include <ew/shader.h>
//#include <ew/texture.h>
//#include <ew/procGen.h>
//#include <ew/transform.h>
//#include <ew/camera.h>
//#include <ew/cameraController.h>
//#include <ew/external/stb_image.h>
//#include <iostream>
//
//struct Vertex {
//	float x, y, z;
//	float u, v;
//};
//
//void tFramebufferSizeCallback(GLFWwindow* window, int width, int height);
//void tResetCamera(ew::Camera& camera, ew::CameraController& cameraController);
//unsigned int tCreateVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
//
//int WIDTH = 1080;
//int HEIGHT = 720;
//
//float prevT;
//ew::Vec3 backgroundColor = ew::Vec3(0.1f);
//
//ew::Camera tCamera;
//ew::CameraController controller;
//
//int main() {
//	printf("Initializing...");
//	if (!glfwInit()) {
//		printf("GLFW failed to init!");
//		return 1;
//	}
//
//	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Camera", NULL, NULL);
//	if (window == NULL) {
//		printf("GLFW failed to create window");
//		return 1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, tFramebufferSizeCallback);
//
//	if (!gladLoadGL(glfwGetProcAddress)) {
//		printf("GLAD Failed to load GL headers");
//		return 1;
//	}
//
//	//Initialize ImGUI
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init();
//
//	//Global settings
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glEnable(GL_DEPTH_TEST);
//
//	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
//	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);
//
//	ew::MeshData skybox = ew::createCube(5);
//
//	std::string facesCubemap[6] =
//	{
//		"assets/skybox/right.jpg",
//		"assets/skybox/left.jpg",
//		"assets/skybox/top.jpg",
//		"assets/skybox/bottom.jpg",
//		"assets/skybox/back.jpg",
//		"assets/skybox/front.jpg"
//	};
//
//	unsigned int cubemapTexture;
//	glGenTextures(1, &cubemapTexture);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//	for (unsigned int i = 0; i < 6; i++) {
//		int width, height, nrChannels;
//		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
//		if (data) {
//			stbi_set_flip_vertically_on_load(false);
//			glTexImage2D
//			(
//				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//				0,
//				GL_RGB,
//				width,
//				height,
//				0,
//				GL_RGB,
//				GL_UNSIGNED_BYTE,
//				data
//			);
//			stbi_image_free(data);
//		}
//		else {
//			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
//			stbi_image_free(data);
//		}
//	}
//
//	ew::MeshData terrainMeshData = ew::createPlane(10.0f, 10.0f, 100.0);
//	ew::Mesh terrainMesh(terrainMeshData);
//
//	////Create cube
//	//ew::Mesh cubeMesh(ew::createCube(1.0f));
//	//ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
//	//ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
//	//ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));
//
//	////Initialize transforms
//	//ew::Transform cubeTransform;
//	//ew::Transform planeTransform;
//	//ew::Transform sphereTransform;
//	//ew::Transform cylinderTransform;
//	//planeTransform.position = ew::Vec3(0, -1.0, 0);
//	//sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
//	//cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);
//
//	tResetCamera(tCamera,controller);
//
//	while (!glfwWindowShouldClose(window)) {
//		glfwPollEvents();
//
//		float time = (float)glfwGetTime();
//		float deltaTime = time - prevT;
//		prevT = time;
//
//		//Update camera
//		tCamera.aspectRatio = (float)WIDTH / HEIGHT;
//		controller.Move(window, &tCamera, deltaTime);
//
//		//RENDER
//		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z,1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		shader.use();
//		glBindTexture(GL_TEXTURE_2D, brickTexture);
//		shader.setInt("_Texture", 0);
//		shader.setMat4("_ViewProjection", tCamera.ProjectionMatrix() * tCamera.ViewMatrix());
//
//		////Draw shapes
//		//shader.setMat4("_Model", cubeTransform.getModelMatrix());
//		//cubeMesh.draw();
//
//		//shader.setMat4("_Model", planeTransform.getModelMatrix());
//		//planeMesh.draw();
//
//		//shader.setMat4("_Model", sphereTransform.getModelMatrix());
//		//sphereMesh.draw();
//
//		//shader.setMat4("_Model", cylinderTransform.getModelMatrix());
//		//cylinderMesh.draw();
//
//		//TODO: Render point lights
//
//		//Render UI
//		{
//			ImGui_ImplGlfw_NewFrame();
//			ImGui_ImplOpenGL3_NewFrame();
//			ImGui::NewFrame();
//
//			ImGui::Begin("Settings");
//			if (ImGui::CollapsingHeader("Camera")) {
//				ImGui::DragFloat3("Position", &tCamera.position.x, 0.1f);
//				ImGui::DragFloat3("Target", &tCamera.target.x, 0.1f);
//				ImGui::Checkbox("Orthographic", &tCamera.orthographic);
//				if (tCamera.orthographic) {
//					ImGui::DragFloat("Ortho Height", &tCamera.orthoHeight, 0.1f);
//				}
//				else {
//					ImGui::SliderFloat("FOV", &tCamera.fov, 0.0f, 180.0f);
//				}
//				ImGui::DragFloat("Near Plane", &tCamera.nearPlane, 0.1f, 0.0f);
//				ImGui::DragFloat("Far Plane", &tCamera.farPlane, 0.1f, 0.0f);
//				ImGui::DragFloat("Move Speed", &controller.moveSpeed, 0.1f);
//				ImGui::DragFloat("Sprint Speed", &controller.sprintMoveSpeed, 0.1f);
//				if (ImGui::Button("Reset")) {
//					tResetCamera(tCamera, controller);
//				}
//			}
//
//			ImGui::ColorEdit3("BG color", &backgroundColor.x);
//			ImGui::End();
//			
//			ImGui::Render();
//			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//		}
//
//		glfwSwapBuffers(window);
//	}
//	printf("Shutting down...");
//}
//
//void tFramebufferSizeCallback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//	WIDTH = width;
//	HEIGHT = height;
//}
//
//void tResetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
//	camera.position = ew::Vec3(0, 0, 5);
//	camera.target = ew::Vec3(0);
//	camera.fov = 60.0f;
//	camera.orthoHeight = 6.0f;
//	camera.nearPlane = 0.1f;
//	camera.farPlane = 100.0f;
//	camera.orthographic = false;
//
//	cameraController.yaw = 0.0f;
//	cameraController.pitch = 0.0f;
//}
//
//unsigned int tCreateVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
//	unsigned int vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	//Vertex Buffer Object 
//	unsigned int vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);
//
//	//Element Buffer Object
//	unsigned int ebo;
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);
//
//	//Position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
//	glEnableVertexAttribArray(0);
//
//	//UV attribute
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
//	glEnableVertexAttribArray(1);
//
//	return vao;
//}