#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

struct Vertex
{
	float x, y, z;
};

struct Face
{
	unsigned int a, b, c;
};

static std::vector<glm::vec3> vertices;
static std::vector<glm::vec3> faces;

static void load_obj(std::string path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << path << '\n';
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			glm::vec3 vertex;
			s >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		} else if (line.substr(0, 2) == "f ") {
			std::istringstream s(line.substr(2));
			std::string splitted;
			std::vector<unsigned int> indices;
			while (std::getline(s, splitted, ' ')) {
				unsigned int index;
				std::istringstream(splitted) >> index;
				indices.push_back(index - 1);
			}

			for (size_t i = 2; i < indices.size(); ++i) {
				glm::uvec3 face = { indices[0], indices[i - 1], indices[i] };
				faces.push_back(face);
			}
		}
	}

	file.close();
	std::cout << "Loaded " << vertices.size() << "vertices.\n";
}

static void draw_obj()
{
	glBegin(GL_TRIANGLES);
	
	for (auto const& face : faces)
	{
		auto const& a = vertices.at(face.x);
		auto const& b = vertices.at(face.y);
		auto const& c = vertices.at(face.z);

		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
	}

	glEnd();
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		int width, height;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwGetWindowSize(window, &width, &height);

		float x = (float)(2 * xpos / width - 1);
		float y = (float)(2 * (height - ypos) / height - 1);

		vertices[0] = glm::vec3{x, y, 0};
	}
}

int main()
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Example", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("GLFW failed to create window");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		glfwTerminate();
		throw std::runtime_error("GLAD failed to load GL functions");
	}

	printf("OpenGL version:", glGetString(GL_VERSION));

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	load_obj("C:\\Users\\scc\\Desktop\\opengl_-practise\\assets\\monkey.obj");
	
	while (!glfwWindowShouldClose(window))
	{	
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		draw_obj();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}