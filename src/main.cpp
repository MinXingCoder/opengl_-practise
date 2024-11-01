#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>

struct Vertex
{
	float x, y, z;
};

struct Face
{
	unsigned int a, b, c;
};

static std::vector<Vertex> vertices = {
	{-0.5f, 0.5f, 0.0f},
	{-0.5f, -0.5f, 0.0f},
	{0.5f, -0.5f, 0.0f},
	{0.5f, 0.5f, 0.0f}
};

static std::vector<Face> faces = {
	{0, 1, 2},
	{0, 2, 3}
};

static void render()
{
	glBegin(GL_TRIANGLES);

	for (auto const& face : faces)
	{
		Vertex a = vertices[face.a];
		Vertex b = vertices[face.b];
		Vertex c = vertices[face.c];

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

		vertices[0] = Vertex{x, y, 0};
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
	
	while (!glfwWindowShouldClose(window))
	{	
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}