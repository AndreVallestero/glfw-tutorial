#include <GLFW/glfw3.h>

int main(void) {
    glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
		
        glBegin(GL_TRIANGLES);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
		
        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}