/* 
 * Sample structure:
 * https://github.com/minetest/minetest/tree/stable-0.4/src
 * https://github.com/jakesgordon/javascript-pong/blob/master/pong.js
 * https://www.glfw.org/docs/3.0/quick.html
 * 
 * disp 16x9
 * black play area
 * white paddles
 * qa, pl are paddle controls, r to restart, f11 for full screen
 * game state wating, playing
 */

#include <GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <iostream>

int score_left = 0, score_right = 0;

class Paddle {
    public:
        float width, height = 0.15f;
        float posX, posY = 0 - height / 2;
        float velY = 0;
        float outBoundRad; // outter bounding radius
        float inBoundRad; // inner bounding radius
        float curveCoef = 1.f / 4; // How hard to change angle of ball based on how far from center it hit;

        void draw(void);
        void update(double);
        int key_handler(int, int);

        Paddle(float, float, int, int);

    private:
        float speed = 2.f / 1; // Take 1 seconds to go from top of screen to bot;

        int moveDownKeyState = 0, moveUpKeyState = 0;
        int upKey, downKey;
};

Paddle::Paddle(float posXStart, float widthStart, int upKeyId, int downKeyId) {
    posX = posXStart;
    upKey = upKeyId, downKey = downKeyId;
    width = widthStart;

    inBoundRad = ((width < height) ? width : height) / 2.0;
    outBoundRad = sqrt(pow(width / 2 + height / 2, 2));
}

void Paddle::update(double timeDelta) {
    posY += velY * timeDelta;
    if(posY > 1 - height) posY = 1 - height;
    else if (posY < -1) posY = -1;
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
    velY = (moveUpKeyState - moveDownKeyState) * speed;
    return true;
}

class Ball {
    public:
        float velX = 0, velY = 0;

        void draw(void);
        void update(double, Paddle*, Paddle*);
        void start(void);
        void reset(void);
        int collides(Paddle*);

        Ball(float);
    private:
        const float RADIUS = 0.02;
        const float START_SPEED = 1.f;

        // Number of Segments to use for drawing, 48 is highly divisible, looks good.
        const int DRAW_SEGMENTS = 48;

        // tan and cos of the angle between the Segments
        // 6.28318530718 is 2 * PI, the degree to radian conversion
        const float TAN_FACT = tanf(6.28318530718f / DRAW_SEGMENTS);
        const float RAD_FACT = cosf(6.28318530718f / DRAW_SEGMENTS);

        float posX = 0, posY = 0;
        
        float boundary;
};

Ball::Ball(float boundaryX) {
    boundary = boundaryX;
}

void Ball::update(double timeDelta, Paddle* leftPaddle, Paddle* rightPaddle) {
    posX += velX * timeDelta;
    posY += velY * timeDelta;

    if (posY - RADIUS <= -1) {
        velY = -velY;
        posY = -2 - posY + 2 * RADIUS;
    } else if (posY + RADIUS >= 1) {
        velY = -velY;
        posY = 2 - posY - 2 * RADIUS;
    } else if (collides(leftPaddle)) {
        Paddle paddle = *leftPaddle;

        // Position compensation looks bad without proper collision
        //posX = 2 * (paddle.posX + paddle.width) - posX + 2 * RADIUS;
        posX -= velX * timeDelta;
        velX = -velX + START_SPEED / 10;

        float paddleCenY = paddle.posY + paddle.height / 2;
        float distY = posY - paddleCenY;
        float curveMulti = distY / paddle.height;
        velY += paddle.velY / 4 + curveMulti * paddle.curveCoef;
    }  else if (collides(rightPaddle)) {
        Paddle paddle = *rightPaddle;


        // Proper position compensation looks bad without proper collision
        //posX = 2 * paddle.posX - posX - 2 * RADIUS;
        posX -= velX * timeDelta;
        velX = -velX - START_SPEED / 10;

        float paddleCenY = paddle.posY + paddle.height / 2;
        float distY = posY - paddleCenY;
        float curveMulti = distY / paddle.height;
        velY += paddle.velY / 4 + curveMulti * paddle.curveCoef;
    } else if (posX + RADIUS <= -boundary) {
        ++score_left;
        reset();
    } else if (posX - RADIUS >= boundary) {
        ++score_right;
        reset();
    }
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

void Ball::start(void) {
    // Ball is not moving
    if(!velX && !velY)
        velX = START_SPEED * ((rand() & 2) - 1); // 50/50 left or right by masking the '2' bit which could have value of 0 or 2
}

void Ball::reset(void) {
    posX = 0.f, posY = 0.f,
    velX = 0.f, velY = 0.f;
}

int Ball::collides(Paddle* pPaddle) {
    Paddle paddle = (*pPaddle);
    float paddleCenX = paddle.posX + paddle.width / 2;
    float paddleCenY = paddle.posY + paddle.height / 2;
    float distDeltaX = paddleCenX - posX;
    float distDeltaY = paddleCenY - posY;
    float distDeltaSq = pow(distDeltaX, 2) + pow(distDeltaY, 2);

    // Distance is longer than radius and paddle outer bound
    if (distDeltaSq > pow(paddle.outBoundRad + RADIUS, 2.f))
        return false;
    // Distance is shorter than the radius and paddle inner bound
    else if (distDeltaSq < pow(paddle.inBoundRad + RADIUS, 2.f))
        return true;
    // Distance is inbetween, check if ball's closest point is in rect
    else {
        // TODO: Not perfect, closest point to center isnt always closest point to rectangle. Method only perfect for squares
        // Create a unit vector by dividing components by magnitude of vector
        // then multiply it by the radius and add position to get the point closest to the paddle
        float distDelta = sqrt(distDeltaSq);
        float ballClosestPointX = posX + distDeltaX / distDelta * RADIUS;
        float ballClosestPointY = posY + distDeltaY / distDelta * RADIUS;

        // Check if point is in paddle rectangle
        if(ballClosestPointX > paddle.posX &&
            ballClosestPointX < paddle.posX + paddle.width &&
            ballClosestPointY > paddle.posY &&
            ballClosestPointY < paddle.posY + paddle.height)
            return true;
    }
    return false;
}

void update_title(GLFWwindow*, double);
void update(double);
void draw(float);
void reset(void);
void key_callback(GLFWwindow*, int, int, int, int);

Paddle* leftPaddle = 0;
Paddle* rightPaddle = 0;
Ball* ball = 0;

int main(void) {
    srand(time(0));

    glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 360, "pong", NULL, NULL);
	glfwMakeContextCurrent(window);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glfwSetKeyCallback(window, key_callback);

    float targetDispRatio = 4/3.f;
	int width, height;
    double loopTimePrev = glfwGetTime();

    float paddleWidth = 0.02f;
    leftPaddle = new Paddle(-1.f, paddleWidth, GLFW_KEY_Q, GLFW_KEY_A);
    rightPaddle = new Paddle(1.f - paddleWidth, paddleWidth, GLFW_KEY_P, GLFW_KEY_L);
    ball = new Ball(targetDispRatio);

    double nextFpsUpdateTime = 0.;

	while (!glfwWindowShouldClose(window)) {
        double loopTimeNow = glfwGetTime();
        double loopTimeDelta = loopTimeNow - loopTimePrev;
        loopTimePrev = loopTimeNow;

        if(loopTimeNow >= nextFpsUpdateTime) {
            update_title(window, loopTimeDelta);
            nextFpsUpdateTime = loopTimeNow + 1;
        }
		
        glfwGetFramebufferSize(window, &width, &height);
        float actualDispRatio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if(actualDispRatio < targetDispRatio)
            glOrtho(-targetDispRatio, targetDispRatio, -targetDispRatio/actualDispRatio, targetDispRatio/actualDispRatio, 1.f, -1.f);
        else
            glOrtho(-actualDispRatio, actualDispRatio, -1.f, 1.f, 1.f, -1.f);
        
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);

        update(loopTimeDelta);
        draw(targetDispRatio);

        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

void update_title(GLFWwindow* window, double deltaTime) {
    char title [256];
    title [255] = '\0';

    snprintf(title, 255,
        "Pong | %d vs %d [FPS: %7.2f]",
        score_left, score_right, 1 / deltaTime);

    glfwSetWindowTitle(window, title);
    std::cout << title << std::endl;
}

void update(double timeDelta) {
    (*leftPaddle).update(timeDelta);
    (*rightPaddle).update(timeDelta);
    (*ball).update(timeDelta, leftPaddle, rightPaddle);
}

void draw(float dispRatio) {
    // Draw arena
    glColor3i(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-dispRatio, -1.0f);
    glVertex2f(dispRatio, -1.0f);
    glVertex2f(dispRatio, 1.0f);
    glVertex2f(-dispRatio, 1.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);

    (*leftPaddle).draw();
    (*rightPaddle).draw();
    (*ball).draw();
}

void reset(void) {
    (*ball).reset();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        (*ball).velX /= 2;
        (*ball).velY /= 2;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        (*ball).start();
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        reset();
    else if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        int buffW, buffH;
        GLFWmonitor* primMon = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primMon);
        glfwGetFramebufferSize(window, &buffW, &buffH);

        if(buffW == mode->width && buffH == mode->height)
            glfwSetWindowMonitor(window, NULL, 64, 64, 640, 480, GLFW_DONT_CARE);
        else
            glfwSetWindowMonitor(window, primMon, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else if (!(*leftPaddle).key_handler(key, action))
        (*rightPaddle).key_handler(key, action);
}
