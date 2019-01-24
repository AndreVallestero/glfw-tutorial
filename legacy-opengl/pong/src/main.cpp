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
        int moveDownKeyState = 0;
        int moveUpKeyState = 0;

        void draw(void);
        void update(double);

        Paddle(float);

    private:
        float width = 0.025f, height = 0.2f;
        float speed = 1.f / 3; // Take 3 seconds to go from top of screen to bot;

        float posX, posY = 0 - height / 2;
};

Paddle::Paddle(float posXStart) {
    posX = posXStart + width / 2;
}

void Paddle::update(double timeDelta) {
    posY += moveUpKeyState + moveDownKeyState * speed * timeDelta;
}

void Paddle::draw(void) {
    glBegin(GL_QUADS);
    glVertex2f(posX, posY);
    glVertex2f(posX + width, posY);
    glVertex2f(posX + width, posY + height);
    glVertex2f(posX, posY + height);
    glEnd();
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

void update(double, Paddle, Paddle, Ball);
void draw(Paddle, Paddle, Ball);
void reset();

int main(void) {
    glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 360, "pong", NULL, NULL);
	glfwMakeContextCurrent(window);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	int width, height;
    int score_left = 0, score_right = 0;
    double loopTimePrev = glfwGetTime();

    Paddle left_paddle = Paddle(-0.8f);
    Paddle right_paddle = Paddle(0.8f);
    Ball ball;

	while (!glfwWindowShouldClose(window)) {
        double loopTimeNow = glfwGetTime();
        double loopTimeDelta = loopTimeNow - loopTimePrev;
        loopTimePrev = loopTimeNow;

        std::cout << "FPS: " << 1 / loopTimeDelta << std::endl;

        glClear(GL_COLOR_BUFFER_BIT);
		
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
		
        update(loopTimeDelta, left_paddle, right_paddle, ball);
        draw(left_paddle, right_paddle, ball);

        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

void update(double timeDelta, Paddle left_paddle, Paddle right_paddle, Ball ball) {
    left_paddle.update(timeDelta);
    right_paddle.update(timeDelta);
    ball.update(timeDelta);
}

void draw(Paddle left_paddle, Paddle right_paddle, Ball ball) {
    glColor3i(1, 1, 1);
    left_paddle.draw();
    right_paddle.draw();
    ball.draw();
}