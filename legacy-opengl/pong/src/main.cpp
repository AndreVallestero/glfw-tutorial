/* 
 * Sample structure:
 * https://github.com/minetest/minetest/tree/stable-0.4/src
 * https://github.com/jakesgordon/javascript-pong/blob/master/pong.js
 * https://www.glfw.org/docs/3.0/quick.html
 * 
 * Display 16x9
 * black play area
 * white paddles
 * qa, pl are paddle controls, r to restart, f11 for full screen
 * game state wating, playing
 */

#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

class Paddle {
    public:
        void draw(void);
        void update(double);
        int key_handler(int, int);

        Paddle(float, int, int);

    private:
        float width = 0.025f, height = 0.2f;
        float speed = 2.f / 3; // Take 3 seconds to go from top of screen to bot;
        float posX, posY = 0 - height / 2;

        int moveDownKeyState = 0, moveUpKeyState = 0;
        int upKey, downKey;
};

Paddle::Paddle(float posXStart, int upKeyId, int downKeyId) {
    posX = posXStart + width / 2;
    upKey = upKeyId, downKey = downKeyId;
}

void Paddle::update(double timeDelta) {
    posY += (moveUpKeyState - moveDownKeyState) * speed * timeDelta;
}

void Paddle::draw(void) {
    glBegin(GL_QUADS);
    glVertex2f(posX, posY);
    glVertex2f(posX + width, posY);
    glVertex2f(posX + width, posY + height);
    glVertex2f(posX, posY + height);
    glEnd();
}

// Returns true if key is handled to stop other instances from wasting time
int Paddle::key_handler(int key, int action) {
    if (key == upKey && action == GLFW_PRESS)
        moveUpKeyState = true;
    else if (key == upKey && action == GLFW_RELEASE)
        moveUpKeyState = false;
    else if (key == downKey && action == GLFW_PRESS)
        moveDownKeyState = true;
    else if (key == downKey && action == GLFW_RELEASE)
        moveDownKeyState = false;
    else 
        return false;
    return true;
}

class Ball {
    public:
        void draw(void);
        void update(double);

    private:
        const float START_X = 0, START_Y = 0;
        const float RADIUS = 0.05;

        // Number of Segments to use for drawing, 12 is highly divisible, looks good.
        const int DRAW_SEGMENTS = 12;

        // tan and cos of the angle between the Segments
        // 6.28318530718 is 2 * PI, the degree to radian conversion
        const float TAN_FACT = tanf(6.28318530718f / DRAW_SEGMENTS);
        const float RAD_FACT = cosf(6.28318530718f / DRAW_SEGMENTS);

        float posX = 0, posY = 0;
        float velX = 0, velY = 0;
};

void Ball::update(double timeDelta) {
    posX += velX * timeDelta;
    posY += velY * timeDelta;
}

// Optimized circle from http://slabode.exofire.net/circle_draw.shtml
void Ball::draw(void) {
    float x = RADIUS, y = 0;

    glBegin(GL_POLYGON);
    for (int i = 0; i < DRAW_SEGMENTS; ++i) {
        glVertex2f(posX + x, posY + y);
		float tx = -y; 
		float ty = x;
		x += tx * TAN_FACT; 
		y += ty * TAN_FACT;
		x *= RAD_FACT; 
		y *= RAD_FACT;
    }
    glEnd();
}

void update(double);
void draw();
void reset();
void key_callback(GLFWwindow*, int, int, int, int);

Paddle* leftPaddle = 0;
Paddle* rightPaddle = 0;
Ball ball;

int main(void) {
    glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 360, "pong", NULL, NULL);
	glfwMakeContextCurrent(window);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glfwSetKeyCallback(window, key_callback);
	int width, height;
    int score_left = 0, score_right = 0;
    double loopTimePrev = glfwGetTime();

    leftPaddle = new Paddle(-0.8f, GLFW_KEY_Q, GLFW_KEY_A);
    rightPaddle = new Paddle(0.8f, GLFW_KEY_P, GLFW_KEY_L);

	while (!glfwWindowShouldClose(window)) {
        double loopTimeNow = glfwGetTime();
        double loopTimeDelta = loopTimeNow - loopTimePrev;
        loopTimePrev = loopTimeNow;

        //std::cout << "FPS: " << 1 / loopTimeDelta << std::endl;
		
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
		
        update(loopTimeDelta);
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

void update(double timeDelta) {
    (*leftPaddle).update(timeDelta);
    (*rightPaddle).update(timeDelta);
    ball.update(timeDelta);
}

void draw() {
    (*leftPaddle).draw();
    (*rightPaddle).draw();
    ball.draw();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(!(*leftPaddle).key_handler(key, action))
        (*rightPaddle).key_handler(key, action);
}