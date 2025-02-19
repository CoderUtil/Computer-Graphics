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
using namespace std;

//	顶点
struct Point{
	float x;
	float y;
	Point(float x_ = 0, float y_ = 0) {
		x = x_;
		y = y_;
	}
};


vector<Point> points;
vector<Point> bezierPoints;
vector<Point> dynamicPoints;
int dynamicCount;				//	每次画几个点
int i;


//鼠标位置
double mouseX = 0.0;
double mouseY = 0.0;

void Bezier() {

	//	统计一次要画的动态点的个数
	dynamicCount = 0;
	for (int i = 1; i < points.size(); ++i) {
		for (int j = 0; j < points.size() - i; ++j) {
			dynamicCount++;
		}
	}

	Point* Q = new Point[points.size()];
	for (double t = 0.0; t <= 1; t += 0.005) {
		vector<vector<Point>> temp1;
		for (int i = 1; i < points.size(); ++i) {
			vector<Point> temp2;
			for (int j = 0; j < points.size() - i; ++j) {
				if (i == 1) {	// i==1时,第一次迭代,由已知控制点计算  
					Q[j].x = points[j].x * (1 - t) + points[j + 1].x * t;
					Q[j].y = points[j].y * (1 - t) + points[j + 1].y * t;
				}
				else {
					// i != 1时,通过上一次迭代的结果计算  
					Q[j].x = Q[j].x * (1 - t) + Q[j + 1].x * t;
					Q[j].y = Q[j].y * (1 - t) + Q[j + 1].y * t;
				}
				dynamicPoints.push_back(Point(Q[j].x, Q[j].y));
			}
		}
		bezierPoints.push_back(Point(Q[0].x, Q[0].y));
	}
	delete Q;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//	设置渲染窗口的尺寸. 参数分别是距左下角的x, y距离, 宽, 高
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	//	检测用户是否按下了Esc, 按下则退出窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				//	获取鼠标位置
				glfwGetCursorPos(window, &mouseX, &mouseY);
				//	放入顶点数组
				points.push_back(Point(mouseX, mouseY));
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				//	弹出顶点
				points.pop_back();
				break;
			default:
				return;
		}
		bezierPoints.clear();
		dynamicPoints.clear();
		i = 0;
		Bezier();
	}
	return;
}

//	将vector<Point>转换成float[]
float* createVertices(vector<Point> v) {
	float* vertices = new float[v.size() * 6];
	for (int i = 0; i < v.size(); i++) {
		vertices[i * 6 + 0] = -1.0 + v[i].x / 800 * 2;
		vertices[i * 6 + 1] = 1.0 - v[i].y / 800 * 2;
		vertices[i * 6 + 2] = 0.0f;
		vertices[i * 6 + 3] = 0.0f;
		vertices[i * 6 + 4] = 1.0f;
		vertices[i * 6 + 5] = 1.0f;
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
	//	注册鼠标点击事件
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	


	//	使用GLAD加载OpenGL提供的函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//	创建VAO对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//	生成VBO对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


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
	
	int t = 0;

	//	循环渲染
	while (!glfwWindowShouldClose(window)) {	//	判断渲染是否结束
												//	获取输入
		processInput(window);

		//	清屏
		glClear(GL_COLOR_BUFFER_BIT);

		//	激活着色器
		glUseProgram(shaderProgram);

		vector<Point> result;
		result.insert(result.end(), points.begin(), points.end());
		result.insert(result.end(), bezierPoints.begin(), bezierPoints.end());
		result.insert(result.end(), dynamicPoints.begin(), dynamicPoints.end());
		float* vertices = createVertices(result);	


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 6 * result.size() * sizeof(float), vertices, GL_STATIC_DRAW);

		//	设置位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//	设置颜色属性
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//	取消绑定
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);
		delete vertices;

		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, points.size());


		glPointSize(4.0f);
		glDrawArrays(GL_LINE_STRIP, 0, points.size());
		glDrawArrays(GL_POINTS, points.size(), bezierPoints.size());
		
		t++;
		if (t == 50) {
			i++;
			t = 0;
		}

		if (points.size() + bezierPoints.size() + i * dynamicCount < result.size()) {
			int c = 0;
			for (int j = points.size() - 1; j >= 2; j--) {
				glDrawArrays(GL_LINE_STRIP, points.size() + bezierPoints.size() + i * dynamicCount + c, j);
				c += j;
			}
		}


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

