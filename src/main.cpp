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
	Vertex a, b, c;
};

static std::vector<Face> faces = {
	{
		{-0.5f, 0.5f, 0.0f},
		{-0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
	},
	{
		{-0.5f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},
	},
};

static void render()
{
	glBegin(GL_TRIANGLES);

	for (auto const& face : faces)
	{
		glVertex3f(face.a.x, face.a.y, face.a.z);
		glVertex3f(face.b.x, face.b.y, face.b.z);
		glVertex3f(face.c.x, face.c.y, face.c.z);
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

		faces[0].a = Vertex{ x, y, 0 };
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