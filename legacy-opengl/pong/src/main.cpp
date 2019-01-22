/* 
 * Sample structure:
 * https://github.com/minetest/minetest/tree/stable-0.4/src
 * https://github.com/jakesgordon/javascript-pong/blob/master/pong.js
 * https://www.glfw.org/docs/3.0/quick.html
 */

#include <GLFW/glfw3.h>

int main(void) {
    glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "pong", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	float ratio;
	int width, height;
	while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
		
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
		
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}