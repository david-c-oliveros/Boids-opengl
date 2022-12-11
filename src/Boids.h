#ifndef BOIDS_H
#define BOIDS_H

#include <iostream>
#include <GL/glew.h>
#include<GLFW/glfw3.h>


class Boids {

    private:
        GLFWwindow *window;

    public:
        Boids();
        ~Boids();

        bool Construct();
        void Start();
        void Update(float fDeltaTime);

};

#endif
