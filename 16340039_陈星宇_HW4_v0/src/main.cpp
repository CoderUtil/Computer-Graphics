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
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//	设置渲染窗口的尺寸. 参数分别是距左下角的x, y距离, 宽, 高
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	//	检测用户是否按下了Esc, 按下则退出窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

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


	//	使用GLAD加载OpenGL提供的函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//	顶点数组
	float vertices[] = {
		-2.0f, -2.0f, -2.0f, 0.2f, 0.3f, 0.4f,
		2.0f, -2.0f, -2.0f, 0.2f, 0.3f, 0.4f,
		2.0f, 2.0f, -2.0f, 0.2f, 0.3f, 0.4f,
		2.0f, 2.0f, -2.0f, 0.2f, 0.3f, 0.4f,
		-2.0f, 2.0f, -2.0f, 0.2f, 0.3f, 0.4f,
		-2.0f, -2.0f, -2.0f, 0.2f, 0.3f, 0.4f,

		-2.0f, -2.0f, 2.0f, 0.3f, 0.4f, 0.5f,
		2.0f, -2.0f, 2.0f, 0.3f, 0.4f, 0.5f,
		2.0f, 2.0f, 2.0f, 0.3f, 0.4f, 0.5f,
		2.0f, 2.0f, 2.0f, 0.3f, 0.4f, 0.5f,
		-2.0f, 2.0f, 2.0f, 0.3f, 0.4f, 0.5f,
		-2.0f, -2.0f, 2.0f, 0.3f, 0.4f, 0.5f,

		-2.0f, 2.0f, 2.0f, 0.4f, 0.5f, 0.6f,
		-2.0f, 2.0f, -2.0f, 0.4f, 0.5f, 0.6f,
		-2.0f, -2.0f, -2.0f, 0.4f, 0.5f, 0.6f,
		-2.0f, -2.0f, -2.0f, 0.4f, 0.5f, 0.6f,
		-2.0f, -2.0f, 2.0f, 0.4f, 0.5f, 0.6f,
		-2.0f, 2.0f, 2.0f, 0.4f, 0.5f, 0.6f,

		2.0f, 2.0f, 2.0f, 0.5f, 0.6f, 0.7f,
		2.0f, 2.0f, -2.0f, 0.5f, 0.6f, 0.7f,
		2.0f, -2.0f, -2.0f, 0.5f, 0.6f, 0.7f,
		2.0f, -2.0f, -2.0f, 0.5f, 0.6f, 0.7f,
		2.0f, -2.0f, 2.0f, 0.5f, 0.6f, 0.7f,
		2.0f, 2.0f, 2.0f, 0.5f, 0.6f, 0.7f,

		-2.0f, -2.0f, -2.0f, 0.6f, 0.7f, 0.8f,
		2.0f, -2.0f, -2.0f, 0.6f, 0.7f, 0.8f,
		2.0f, -2.0f, 2.0f, 0.6f, 0.7f, 0.8f,
		2.0f, -2.0f, 2.0f, 0.6f, 0.7f, 0.8f,
		-2.0f, -2.0f, 2.0f, 0.6f, 0.7f, 0.8f,
		-2.0f, -2.0f, -2.0f, 0.6f, 0.7f, 0.8f,

		-2.0f, 2.0f, -2.0f, 0.7f, 0.8f, 0.9f,
		2.0f, 2.0f, -2.0f, 0.7f, 0.8f, 0.9f,
		2.0f, 2.0f, 2.0f, 0.7f, 0.8f, 0.9f,
		2.0f, 2.0f, 2.0f, 0.7f, 0.8f, 0.9f,
		-2.0f, 2.0f, 2.0f, 0.7f, 0.8f, 0.9f,
		-2.0f, 2.0f, -2.0f, 0.7f, 0.8f, 0.9f
	};


	//	创建VAO对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//	绑定VAO
	glBindVertexArray(VAO);



	//	生成VBO对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//	绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//	使用GLSL语言编写顶点着色器
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 transform;\n"
		"out vec3 ourColor;\n"
		"void main(){\n"
		"   gl_Position = projection * view * model * transform * vec4(aPos, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\0";

	//	创建着色器对象
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//	把着色器源码附加到着色器对象上
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//	编译着色器
	glCompileShader(vertexShader);

	//	检查编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//	编写并编译片段着色器
	const char *fragmentShaderSource = "#version 330 core\n"
		"in vec3 ourColor;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"   FragColor = vec4(ourColor, 1.0f);\n"
		"}\n\0";
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//	设置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//	设置颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//	创建程序对象
	int shaderProgram = glCreateProgram();

	//	将着色器附加到程序对象上, 然后链接
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 检查链接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//	删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//	开启深度测试
	glEnable(GL_DEPTH_TEST);

	//	初始化ImgUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	bool show_demo_window = true;
	bool choose_translation = false;
	bool choose_rotation = false;
	bool choose_scale = false;

	glm::mat4 transform = glm::mat4(1.0f);

	//	循环渲染
	while (!glfwWindowShouldClose(window)) {	//	判断渲染是否结束
		processInput(window);

		//	清屏
		glClear(GL_COLOR_BUFFER_BIT);
		//	清除深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		{
			ImGui::Begin(" ");
			ImGui::SetWindowFontScale(1.4);
			ImGui::Text("Choose a kind of Transform");
			ImGui::Checkbox("Translation", &choose_translation);
			ImGui::Checkbox("Rotation", &choose_rotation);
			ImGui::Checkbox("Scale", &choose_scale);
			ImGui::End();
		}

		if (choose_translation) {
			transform = glm::translate(transform, glm::vec3(0.001f, 0.0f, 0.0f));
		}
		else if (choose_rotation) {
			transform = glm::rotate(transform, (float)glfwGetTime() / 1000, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (choose_scale) {
			transform = glm::scale(transform, glm::vec3(1.0001f, 1.0001f, 1.0001f));
		}
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//	激活着色器
		glUseProgram(shaderProgram);


		//	创建坐标转换矩阵, 将局部坐标变换为标准设备坐标 
		glm::mat4 model = glm::mat4(1.0f); 
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// 获取Uniform变量的位置
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		// 将矩阵传入着色器
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		
		//	绘制图元
		glBindVertexArray(VAO);
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//	释放渲染资源
	glfwTerminate();
	return 0;
}