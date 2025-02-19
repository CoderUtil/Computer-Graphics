#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "shader.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
//鼠标位置
float mouseX = 0.0f;
float mouseY = 0.0f;
bool firstMouse = true;
//每一帧间隔时间
float deltaTime = 0.0f;
//上一帧的时刻
float lastFrame = 0.0f;

//	光源坐标
glm::vec3 lightPos(20.0f, 8.0f, 20.0f);

int main() {
	//	初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//	创建一个窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//	设置窗口上下文为当前线程的主上下文
	glfwMakeContextCurrent(window);
	//	当窗口大小改变时调用此函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//	通知glfw捕获鼠标与滚轮事件
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	//	使用GLAD加载OpenGL提供的函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//	顶点数组
	float vertices[] = {
		-2.0f, -2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		2.0f, -2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		2.0f, 2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		2.0f, 2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		-2.0f, 2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		-2.0f, -2.0f, -2.0f, 0.0f, 0.0f, -1.0f,

		-2.0f, -2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		2.0f, -2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		-2.0f, 2.0f, 2.0f, -1.0f, 0.0f, 0.0f,
		-2.0f, 2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, 2.0f, -1.0f, 0.0f, 0.0f,
		-2.0f, 2.0f, 2.0f, -1.0f, 0.0f, 0.0f,

		2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f,

		-2.0f, -2.0f, -2.0f, 0.0f, -1.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 0.0f, -1.0f, 0.0f,
		2.0f, -2.0f, 2.0f, 0.0f, -1.0f, 0.0f,
		2.0f, -2.0f, 2.0f, 0.0f, -1.0f, 0.0f,
		-2.0f, -2.0f, 2.0f, 0.0f, -1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f, 0.0f, -1.0f, 0.0f,

		-2.0f, 2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
	};


	//	cubeVAO
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	//	VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//	设置位置属性
	//	属性值为0的顶点属性，有3个值，相邻顶点的同一属性距离为6个float，偏移为0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//	法向量属性
	//	属性值为1的顶点属性，有3个值，相邻顶点的同一属性距离为6个float，偏移为3个float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//	lightVAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//	VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//	设置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//	开启深度测试
	glEnable(GL_DEPTH_TEST);

	//	初始化ImgUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	bool enable_camera = false;
	bool choose_phong = true;
	bool choose_gouraud = false;
	bool light_move = false;
	float ambientStrength = 0.1f;
	float diffuseStrength = 1.0f;
	float specularStrength = 1.0f;
	int ShininessStrength = 30;
	glm::vec3 viewPos;

	//	循环渲染
	while (!glfwWindowShouldClose(window)) {	//	判断渲染是否结束

		if (enable_camera) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}


		//计算当前时间和帧间隔时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//	获取键盘输入
		processInput(window);

		//	清屏
		glClear(GL_COLOR_BUFFER_BIT);
		//	清除深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		// 创建ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();	
		ImGui::Begin("Edit cube");
		ImGui::SetWindowFontScale(1.4);
		if (ImGui::Button("enable camera")) {
			if (enable_camera) {
				enable_camera = false;
			}
			else {
				enable_camera = true;
			}
		}
		if (ImGui::RadioButton("Phong Shader", choose_phong)) {
			choose_gouraud = false;
			choose_phong = true;
		}
		if (ImGui::RadioButton("Gouraud Shader", choose_gouraud)) {
			choose_gouraud = true;
			choose_phong = false;
		}
		ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.01f, 10.0f);
		ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.01f, 10.0f);
		ImGui::SliderFloat("specularStrength", &specularStrength, 0.01f, 10.0f);
		ImGui::SliderInt("ShininessStrength", &ShininessStrength, 0, 256);

		if (ImGui::Button("light move")) {
			if (light_move)
				light_move = false;
			else 
				light_move = true;
		}

		ImGui::End();


		//	灯光移动
		if (light_move) {
			lightPos.x = sin(glfwGetTime()) * 8.0f;
			lightPos.y = sin(glfwGetTime()) * 8.0f;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);


		if (enable_camera) {
			view = camera.GetViewMatrix();
			viewPos = camera.Position;

			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}
		else {
			//把场景移动(-1.0f, -1.0f, -5.0f)
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
			viewPos = glm::vec3(1.0f, 1.0f, 5.0f);

			projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}

		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));


		if (choose_phong) {

			//	创建着色器
			Shader cubeShader("cubePhongShader.vs", "cubePhongShader.fs");

			//	激活着色器
			cubeShader.use();

			//	设置着色器程序uniform
			cubeShader.setFloat("ambientStrength", ambientStrength);
			cubeShader.setFloat("diffuseStrength", diffuseStrength);
			cubeShader.setFloat("specularStrength", specularStrength);
			cubeShader.setInt("ShininessStrength", ShininessStrength);

			cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			cubeShader.setVec3("lightPos", lightPos);
			cubeShader.setVec3("viewPos", viewPos);

			cubeShader.setMat4("model", model);
			cubeShader.setMat4("view", view);
			cubeShader.setMat4("projection", projection);

			//	画物体
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else if (choose_gouraud) {
			Shader cubeShader("cubeGouraudShader.vs", "cubeGouraudShader.fs");
			//使用着色器程序
			cubeShader.use();
			//设置着色器程序uniform
			cubeShader.setFloat("ambientStrength", ambientStrength);
			cubeShader.setFloat("diffuseStrength", diffuseStrength);
			cubeShader.setFloat("specularStrength", specularStrength);
			cubeShader.setInt("ShininessStrength", ShininessStrength);

			cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			cubeShader.setVec3("lightPos", lightPos);
			cubeShader.setVec3("viewPos", viewPos);

			cubeShader.setMat4("model", model);
			cubeShader.setMat4("view", view);
			cubeShader.setMat4("projection", projection);

			//	画物体
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		Shader lightShader("lightShader.vs", "lightShader.fs");

		lightShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, lightPos);
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		

		//	画灯光
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);		//	交换颜色缓冲, 即渲染
	}

	//	释放VAO、VBO
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//	释放渲染资源
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//	键盘输入
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

//	鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		mouseX = xpos;
		mouseY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - mouseX;
	float yoffset = mouseY - ypos;

	mouseX = xpos;
	mouseY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//	鼠标放缩
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}