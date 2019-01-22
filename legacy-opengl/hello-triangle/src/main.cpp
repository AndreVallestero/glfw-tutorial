#include <GLFW/glfw3.h>

int main(void) {
    glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "pong", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
		
        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}