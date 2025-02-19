#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	//	设置渲染窗口的尺寸. 参数分别是距左下角的x, y距离, 宽, 高
	glViewport(0, 0, width, height);		
}

void processInput(GLFWwindow *window)	{
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	bool show_demo_window = true;
	bool show_another_window = false;

	vector<ImVec4> colors;
	ImVec4 color_whole = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	colors.push_back(ImVec4(0.0f, 0.0f, 1.0f, 1.00f));
	colors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 1.00f));
	colors.push_back(ImVec4(0.0f, 1.0f, 0.0f, 1.00f));

	float vertices[] = {
		// 位置              // 颜色
		-0.25f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // 右下
		-0.75f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		-0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,    // 顶部
		-0.25f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,    
		-0.25f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    
		0.6f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.6f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.6f,  -0.4f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = { 
		0, 1, 2, // 第一个三角形
		0, 1, 3  // 第二个三角形
	};


	//	创建VAO对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//	绑定VAO
	glBindVertexArray(VAO);



	//	生成VBO对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//	创建一个缓冲并绑定到VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	把顶点数据复制到缓冲中, 最后一个参数表示数据不会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//	生成EBO对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//	使用GLSL语言编写顶点着色器
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main(){\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
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



	//	设置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//	设置颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	bool if_change_whole = false;

	//	循环渲染
	while (!glfwWindowShouldClose(window)) {	//	判断渲染是否结束
		processInput(window);

		//	清屏
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			ImGui::Begin("COLOR CHANGER");
			ImGui::SetWindowFontScale(1.4);
			//ImGui::SetWindowSize(ImVec2(510, 170));
			//ImGui::SetWindowPos(ImVec2(0, 0));
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Change the colors!");
			// Display some text (you can use a format string too)
			ImGui::ColorEdit3("RIGHT", (float*)&colors[0]); // Edit 3 floats representing a color
			ImGui::ColorEdit3("LEFT", (float*)&colors[1]); // Edit 3 floats representing a color
			ImGui::ColorEdit3("TOP", (float*)&colors[2]); // Edit 3 floats representing a color

			ImGui::Checkbox("IF CHANGE THE WHOLE COLOR", &if_change_whole);
			ImGui::ColorEdit3("WHOLE COLOR", (float*)&color_whole, 1);

			if (if_change_whole) {
				for (int i = 0; i < 3; i++) {
					vertices[i * 6 + 3] = color_whole.x;
					vertices[i * 6 + 4] = color_whole.y;
					vertices[i * 6 + 5] = color_whole.z;
				}
			}
			else {
				for (int i = 0; i < 3; i++) {
					vertices[i * 6 + 3] = colors[i].x;
					vertices[i * 6 + 4] = colors[i].y;
					vertices[i * 6 + 5] = colors[i].z;
				}
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		//	更新VBO和EBO中的顶点数据
		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//	Link vertex contributes, tell opengl how to explain the vertex datas
		//	Location
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//	Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);

		//	激活着色器并绘制图元
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);	//	设置图形的形状, 顶点数组的起始索引, 顶点数
		glDrawArrays(GL_POINTS, 5, 3);
		glDrawArrays(GL_LINES, 3, 2);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);		//	交换颜色缓冲, 即渲染
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//	释放VAO、VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//	释放渲染资源
	glfwTerminate();
	return 0;
}
