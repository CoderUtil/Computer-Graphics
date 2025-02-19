#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//	设置渲染窗口的尺寸. 参数分别是距左下角的x, y距离, 宽, 高
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	//	检测用户是否按下了Esc, 按下则退出窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void BresenhamLine(int x1, int y1, int x2, int y2, float(&vertices)[10000], int& length) {
	int dx = x2 - x1;
	int dy = y2 - y1;

	//	斜率不存在
	if (dx == 0) {
		if (dy < 0) {
			swap(x1, x2);
			swap(y1, y2);
		}

		int x = x1;
		int y = y1;
		for (int i = 0; i < dy; i++) {
			vertices[length] = x / 400.0f;
			vertices[length + 1] = y / 400.0f;
			vertices[length + 2] = 0.f;
			length += 3;
			y++;
		}
	}
	else {
		float m = (float)dy / dx;

		if (fabs(m) <= 1 && x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
		}

		if (fabs(m) > 1 && y1 > y2) {
			swap(x1, x2);
			swap(y1, y2);
		}

		dx = x2 - x1;
		dy = y2 - y1;
		m = (float)dy / dx;

		if (fabs(m) <= 1) {
			int steps = x2 - x1;

			if (dy >= 0) {
				int p = 2 * dy - dx;
				int x = x1;
				int y = y1;

				vertices[length] = x / 400.0f;
				vertices[length + 1] = y / 400.0f;
				vertices[length + 2] = 0.f;
				length += 3;

				for (int i = 0; i < steps; i++) {
					if (p <= 0) {
						x = x + 1;
						p = p + 2 * dy;
					}
					else {
						x = x + 1;
						y = y + 1;
						p = p + 2 * dy - 2 * dx;
					}

					vertices[length] = x / 400.0f;
					vertices[length + 1] = y / 400.0f;
					vertices[length + 2] = 0.f;
					length += 3;
				}
			}
			else {
				int p = 2 * (-dy) - dx;
				int x = x1;
				int y = -y1;

				vertices[length] = x / 400.0f;
				vertices[length + 1] = -y / 400.0f;
				vertices[length + 2] = 0.f;
				length += 3;

				for (int i = 0; i < steps; i++) {
					if (p <= 0) {
						x = x + 1;
						p = p + 2 * (-dy);
					}
					else {
						x = x + 1;
						y = y + 1;
						p = p + 2 * (-dy) - 2 * dx;
					}

					vertices[length] = x / 400.0f;
					vertices[length + 1] = -y / 400.0f;
					vertices[length + 2] = 0.f;
					length += 3;
				}
			}
		}
		else {
			int steps = y2 - y1;
			if (dx >= 0) {
				int p = 2 * dy - dx;
				int x = x1;
				int y = y1;

				vertices[length] = x / 400.0f;
				vertices[length + 1] = y / 400.0f;
				vertices[length + 2] = 0.f;
				length += 3;

				for (int i = 0; i < steps; i++) {
					if (p <= 0) {
						y = y + 1;
						p = p + 2 * dx;
					}
					else {
						x = x + 1;
						y = y + 1;
						p = p + 2 * dx - 2 * dy;
					}

					vertices[length] = x / 400.0f;
					vertices[length + 1] = y / 400.0f;
					vertices[length + 2] = 0.f;
					length += 3;
				}

			}
			else {
				int p = 2 * (-dx) - dy;
				int x = -x1;
				int y = y1;

				vertices[length] = -x / 400.0f;
				vertices[length + 1] = y / 400.0f;
				vertices[length + 2] = 0.f;
				length += 3;

				for (int i = 0; i < steps; i++) {
					if (p <= 0) {
						y = y + 1;
						p = p + 2 * (-dx);
					}
					else {
						x = x + 1;
						y = y + 1;
						p = p + 2 * (-dx) - 2 * dy;
					}

					vertices[length] = -x / 400.0f;
					vertices[length + 1] = y / 400.0f;
					vertices[length + 2] = 0.f;
					length += 3;
				}
			}
		}
	}

}

int printLine(float(&vertices)[10000], int x1, int y1, int x2, int y2, int x3, int y3) {
	int length = 0;
	BresenhamLine(x1, y1, x2, y2, vertices, length);
	BresenhamLine(x1, y1, x3, y3, vertices, length);
	BresenhamLine(x2, y2, x3, y3, vertices, length);

	return length;
}

int printCircle(float(&vertices)[10000], int xc, int yc, int r) {
	int length = 0;

	float xi = -r, yi = 0;
	float p;
	while (abs(xi) >= abs(yi)) {
		vertices[length] = (xc + xi) / 1600.0f;
		vertices[length + 1] = (yc + yi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc - xi) / 1600.0f;
		vertices[length + 1] = (yc + yi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc + xi) / 1600.0f;
		vertices[length + 1] = (yc - yi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc - xi) / 1600.0f;
		vertices[length + 1] = (yc - yi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc + yi) / 1600.0f;
		vertices[length + 1] = (yc + xi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc - yi) / 1600.0f;
		vertices[length + 1] = (yc + xi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc + yi) / 1600.0f;
		vertices[length + 1] = (yc - xi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		vertices[length] = (xc - yi) / 1600.0f;
		vertices[length + 1] = (yc - xi) / 1600.0f;
		vertices[length + 2] = 0.f;
		length += 3;

		xi += 0.5f;
		float yi_1 = sqrt(pow(r, 2) - pow(xi, 2));
		p = 2 * yi + 1 - 2 * yi_1;
		yi = p <= 0 ? yi_1 + 1 : yi_1;
	}

	return length;
}

vector<int> lineEquation(int x1, int y1, int x2, int y2) {
	// line equation: Ax+ By+ C= 0
	vector<int> res;
	int A = y2 - y1;
	int B = x1 - x2;
	int C = x2 * y1 - x1 * y2;
	res.push_back(A);
	res.push_back(B);
	res.push_back(C);
	return res;
}

vector<int> rasterizeTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	int max_x = max(x1, max(x2, x3));
	int min_x = min(x1, min(x2, x3));
	int max_y = max(y1, max(y2, y3));
	int min_y = min(y1, min(y2, y3));
	int center_x = (max_x - min_x) / 2;
	int center_y = (max_y - min_y) / 2;
	vector< vector<int> > lines;
	lines.push_back(lineEquation(x1, y1, x2, y2));
	lines.push_back(lineEquation(x1, y1, x3, y3));
	lines.push_back(lineEquation(x3, y3, x2, y2));
	// centerlize the equations
	for (int i = 0; i< 3; i++) {
		int x_temp, y_temp;
		if (i == 0) {
			x_temp = x3; y_temp = y3;
		}
		else if (i == 1) {
			x_temp = x2; y_temp = y2;
		}
		else {
			x_temp = x1; y_temp = y1;
		}
		// A*x+ B*y+ C
		if (lines[i][0] * x_temp + lines[i][1] * y_temp + lines[i][2]< 0) {
			for (int j = 0; j< lines[i].size(); j++) {
				lines[i][j] *= -1;
			}
		}
	}
	// set pixel
	vector<int> pixels;
	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			bool isInside = true;
			for (int i = 0; i< 3; i++) {
				if (lines[i][0] * x + lines[i][1] * y + lines[i][2]< 0) {
					isInside = false;
					break;
				}
			}
			if (isInside) {
				pixels.push_back(x);
				pixels.push_back(y);
			}
		}
	}
	return pixels;
}

float *creatVertices(vector<int> v) {
	int point_num = v.size() / 2;
	int total = point_num * 6;
	float *vertices = new float[total];
	for (int i = 0; i< point_num; i++) {
		// position
		vertices[i * 6 + 0] = v[i * 2 + 0] / 400.0f;
		vertices[i * 6 + 1] = v[i * 2 + 1] / 400.0f;
		vertices[i * 6 + 2] = 0.0f;
		// colors
		vertices[i * 6 + 3] = 0.8f;
		vertices[i * 6 + 4] = 1.0f;
		vertices[i * 6 + 5] = 0.8f;
	}
	return vertices;
}

int main() {
	//	初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//	创建一个窗口
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	bool show_demo_window = true;
	bool show_another_window = false;

	int x1, x2, x3, y1, y2, y3;
	cout << "请输入: x1 y1 x2 y2 x3 y3(取值范围为-200~200)" << endl;
	cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
	float lineVertices[10000] = { 0.0f };
	int lineVerticesLength = printLine(lineVertices, x1, y1, x2, y2, x3, y3);


	int xc, yc, r;
	cout << "请输入: xc yc(取值范围为-200~200) r(取值范围为0~50)" << endl;
	cin >> xc >> yc >> r;
	float circleVertices[10000] = { 0.0f };
	int circleVerticesLength = printCircle(circleVertices, xc, yc, r);


	//	创建VAO对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//	绑定VAO
	glBindVertexArray(VAO);



	//	生成VBO对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, lineVerticesLength * sizeof(float), lineVertices, GL_STATIC_DRAW);


	//	使用GLSL语言编写顶点着色器
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main(){\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
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
		"out vec4 FragColor;\n"
		"void main() {\n"
		"   FragColor = vec4(0.6f, 0.6, 0.6f, 1.0f);\n"
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


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


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	bool if_change = false;
	bool if_ras = false;

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

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			ImGui::Begin("SHAPE CHANGER");
			ImGui::SetWindowFontScale(1.4);
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Change the shape!");
			ImGui::Checkbox("CHANGE TO CIRCLE", &if_change);
			ImGui::Checkbox("Rasterize", &if_ras);
			ImGui::SliderInt("CHOOSE THE RADIUS", &r, 0, 200);
			ImGui::End();
		}

		vector<int> test;

		if (if_change) {
			circleVerticesLength = printCircle(circleVertices, xc, yc, r);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, circleVerticesLength * sizeof(float), circleVertices, GL_STATIC_DRAW);

			//	设置位置属性
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//Unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(VAO);

			//	激活着色器并绘制图元
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);

			glDrawArrays(GL_POINTS, 0, circleVerticesLength / 3);	//	设置图形的形状, 顶点数组的起始索引, 顶点数
		}
		else {

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, lineVerticesLength * sizeof(float), lineVertices, GL_STATIC_DRAW);

			//	设置位置属性
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//Unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(VAO);

			//	激活着色器并绘制图元
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);

			glDrawArrays(GL_POINTS, 0, lineVerticesLength / 3);	//	设置图形的形状, 顶点数组的起始索引, 顶点数

			if (if_ras) {
				test = rasterizeTriangle(x1, y1, x2, y2, x3, y3);

				int point_num = test.size() / 2;
				int total = point_num * 6;
				float *vertices = creatVertices(test);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, total * sizeof(float), vertices, GL_STREAM_DRAW);

				//Location
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				//Color
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				//Unbind
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(VAO);

				glDrawArrays(GL_POINTS, 0, point_num);
			}
		}
		

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

	//	释放渲染资源
	glfwTerminate();
	return 0;
}