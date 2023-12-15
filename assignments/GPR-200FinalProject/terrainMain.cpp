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
//void framebufferSizeCallback(GLFWwindow* window, int width, int height);
//void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);
//unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
//
//int SCREEN_WIDTH = 1080;
//int SCREEN_HEIGHT = 720;
//
//float prevTime;
//ew::Vec3 bgColor = ew::Vec3(0.1f);
//
//ew::Camera camera;
//ew::CameraController cameraController;
//
//int main() {
//	printf("Initializing...");
//	if (!glfwInit()) {
//		printf("GLFW failed to init!");
//		return 1;
//	}
//
//	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
//	if (window == NULL) {
//		printf("GLFW failed to create window");
//		return 1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
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
//	unsigned int heightMap = ew::loadTexture("assets/HeightMap.png", GL_REPEAT, GL_LINEAR);
//	unsigned int groundTexture = ew::loadTexture("assets/GrassTexture.jpg", GL_REPEAT, GL_LINEAR);
//	unsigned int rockTexture = ew::loadTexture("assets/RockTexture.jpg", GL_REPEAT, GL_LINEAR);
//	unsigned int snowTexture = ew::loadTexture("assets/SnowTexture.jpg", GL_REPEAT, GL_LINEAR);
//
//	ew::MeshData terrainMeshData = ew::createPlane(50.0f, 50.0f, 512.0);
//	ew::Mesh terrainMesh(terrainMeshData);
//	ew::Transform terrainTransform;
//
//	resetCamera(camera,cameraController);
//
//	while (!glfwWindowShouldClose(window)) {
//		glfwPollEvents();
//
//		float time = (float)glfwGetTime();
//		float deltaTime = time - prevTime;
//		prevTime = time;
//
//		//Update camera
//		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
//		cameraController.Move(window, &camera, deltaTime);
//
//		//RENDER
//		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		shader.use();
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, heightMap);
//		shader.setInt("_HeightMap", 0);
//
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, groundTexture);
//		shader.setInt("_GrassTexture", 1);
//
//		glActiveTexture(GL_TEXTURE2);
//		glBindTexture(GL_TEXTURE_2D, rockTexture);
//		shader.setInt("_RockTexture", 2);
//
//		glActiveTexture(GL_TEXTURE3);
//		glBindTexture(GL_TEXTURE_2D, snowTexture);
//		shader.setInt("_SnowTexture", 3);
//
//
//		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
//		shader.setMat4("_Model", terrainTransform.getModelMatrix());
//		terrainMesh.draw();
//
//		//Render UI
//		{
//			ImGui_ImplGlfw_NewFrame();
//			ImGui_ImplOpenGL3_NewFrame();
//			ImGui::NewFrame();
//
//			ImGui::Begin("Settings");
//			if (ImGui::CollapsingHeader("Camera")) {
//				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
//				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
//				ImGui::Checkbox("Orthographic", &camera.orthographic);
//				if (camera.orthographic) {
//					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
//				}
//				else {
//					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
//				}
//				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
//				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
//				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
//				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
//				if (ImGui::Button("Reset")) {
//					resetCamera(camera, cameraController);
//				}
//			}
//
//			ImGui::ColorEdit3("BG color", &bgColor.x);
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
//void framebufferSizeCallback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//	SCREEN_WIDTH = width;
//	SCREEN_HEIGHT = height;
//}
//
//void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
//	camera.position = ew::Vec3(0, 5, 5);
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
//unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
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