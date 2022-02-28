//Double Pendulum.  13.02.2022
//This is simulation of double pendulum.
//Equations of motion of pendulumus were obtained by using Lagrangian dynamics.
//Differential equations of motion are solved numerically with Runge-Kutta 4 method.
//ouz81 made this.
//ouz81 was here. ouz81 is still here. He's watching por...porsche races. That's right!

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "shader.h"

GLFWwindow* window;

#define PI 3.141592   //Holy Pi!
#define h 0.005       //step length for Runge-Kutta
#define g 9.81        //gravity
#define R 1           //length of rods

//Initial angle and velocity for pendulum 1
#define THETA1_0 3.0f
#define OMEGA1_0 0

//Initial angle and velocity for pendulum 2
#define THETA2_0 0.7f
#define OMEGA2_0 0

class Pendulum{
	public:
		float circleVertices[2160];
		float rodVertices[10];
		void drawCircle(float);
		void drawRod(float);
		float angle;
};

void Pendulum::drawCircle(float distance){
    int corner_one, corner_two, corner_three;//corners of triangles. GL_TRIANGLES starts to draw counterclockwise.
    corner_one = -6;
    corner_two = -4;
    corner_three = -2;

    float radius = 0.025f;

    for(int angle = 1; angle <= 360; angle ++){
        corner_one = corner_one + 6;
        corner_two = corner_two + 6;
        corner_three = corner_three + 6;

        circleVertices[corner_one] = 0.0f;
        circleVertices[corner_one + 1] = 0.0f - distance;

        circleVertices[corner_two] = radius * cos((angle -1) * 3.1416 / 180);
        circleVertices[corner_two + 1] = radius * sin((angle -1) * 3.1416 / 180) - distance;

        circleVertices[corner_three] = radius * cos((angle) * 3.1416 / 180);
        circleVertices[corner_three + 1] = radius * sin((angle) * 3.1416 / 180) - distance;

    }    
}

void Pendulum::drawRod(float distance){
	rodVertices[0] =		  -0.01f;	rodVertices[1] = 0.0f - distance;
 	rodVertices[2] = 		   0.01f;	rodVertices[3] = 0.0f - distance;
	rodVertices[4] = 		   0.01f;	rodVertices[5] = 0.4f - distance;
    rodVertices[6] = 		  -0.01f;	rodVertices[7] = 0.4f - distance;
    rodVertices[8] =   		  -0.01f;	rodVertices[9] = 0.0f - distance;
	
}

//***VARIABLES****
//Initial values are assigned to these variables here.
float theta_1 = THETA1_0;
float omega_1 = OMEGA1_0;
float theta_2 = THETA2_0;
float omega_2 = OMEGA2_0;
float m1 = 2;
float m2 = 2; //masses of pendulums
float L1 = 1;
float L2 = 1; //rod lengths


//***PENDULUM 1****

float k1, k2, k3, k4, l1, l2, l3, l4;

float f_1(float omg){
	return omg;
}

float g_1(float theta1, float theta2, float omg1, float omg2){
    return ((m2 * g * sin(theta2) * cos(theta1 - theta2))
     - (m2 * sin(theta1 - theta2) * (L1 * (omg1 * omg1) * cos(theta1 - theta2) + (L2 * (omg2 * omg2))))
     - ((m1 + m2) * g *sin(theta1))) / (L1 * (m1 + (m2 * sin(theta1 - theta2) * sin(theta1 - theta2)))); 
}

/*float g_1(float theta1, float theta2, float omg1, float omg2){
	return (-m2 * L2 * (omg2 * omg2) * sin(theta1 - theta2) - (g * sin(theta1) * (m1 + m2)) - (m2 * L2 * omg2 * cos(theta1 - theta2))) /
	                                                           (L1 * (m1 + m2));
}*/

//***PENDULUM 2****

float p1, p2, p3, p4, q1, q2, q3, q4;

float f_2(float omg){
	return omg;
}

float g_2(float theta1, float theta2, float omg1, float omg2){
    return  ((m1 + m2) * (L1 * (omg1 * omg1) * sin(theta1 - theta2)
            - (g * sin(theta2)) + (g * sin(theta1) * cos(theta1 - theta2)))
            + (m2 * L2 * omg2 * omg2 * sin(theta1 - theta2) * cos(theta1 - theta2))) / (L2 * (m1 + (m2 * sin(theta1 - theta2) * sin(theta1 - theta2))));
}

/*float g_2(float theta1, float theta2, float omg1, float omg2){
	return (L1 * omg1 * omg1 * sin(theta1 - theta2) - (g * sin(theta2)) - (L1 * omg1 * cos(theta1 - theta2))) /
	                                                              L2;
}*/

int main(void){
	if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( 630, 600, "DOUBLE PENDULUM", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    //creating pendulums
    Pendulum pendulum1;
    pendulum1.drawCircle(0.0f);
    pendulum1.drawRod(0.0f);
    Pendulum pendulum2;
    pendulum2.drawCircle(0.4f);
    pendulum2.drawRod(0.4f);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 0.8f, 0.0f, 0.0f);

    Shader doublePendulum("vertex.vs", "fragment.fs");
    unsigned int shaderProgram = doublePendulum.programID();

    unsigned int VBO; // This vertex buffer object is common for both pendulum

    //******PENDULUM1*******************
    unsigned VAO, VAO2; //VAO is for circle, VAO2 is for the rod of pendulum
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pendulum1.circleVertices), pendulum1.circleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //setting rod vertices
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pendulum1.rodVertices), pendulum1.rodVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //******PENDULUM2********************
    unsigned VAO3, VAO4; //VAO3 is for circle, VAO4 is for the rod of pendulum
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pendulum2.circleVertices), pendulum2.circleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //setting rod vertices
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pendulum2.rodVertices), pendulum2.rodVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    do{

    	

    	glClear(GL_COLOR_BUFFER_BIT);
    	glUseProgram(shaderProgram);

    	

        k1 = h * f_1(omega_1);
        l1 = h * g_1(theta_1, theta_2, omega_1, omega_2);
        p1 = h * f_2(omega_2);
        q1 = h * g_2(theta_1, theta_2, omega_1, omega_2);
        
        k2 = h * f_1(omega_1 + (0.5 * l1));
        l2 = h * g_1(theta_1 + (0.5 * k1), theta_2 + (0.5 * p1), omega_1 + (0.5 * l1), omega_2 + (0.5 * q1));
        p2 = h * f_2(omega_2 + (0.5 * q1));
        q2 = h * g_2(theta_1 + (0.5 * k1), theta_2 + (0.5 * p1), omega_1  + (0.5 * l1), omega_2 + (0.5 * q1));
        
        k3 = h * f_1(omega_1 + (0.5 * l2));
        l3 = h * g_1(theta_1 + (0.5 * k2), theta_2 + (0.5 * p2), omega_1 + (0.5 * l2), omega_2 + (0.5 * q2));
        p3 = h * f_2(omega_2 + (0.5 * q2));
        q3 = h * g_2(theta_1 + (0.5 * k2), theta_2 + (0.5 * p2), omega_1 + (0.5 * l2), omega_2 + (0.5 * q2));
        
        k4 = h * f_1(omega_1 + l3);
        l4 = h * g_1(theta_1 + k3, theta_2 + p3, omega_1 + l3, omega_2 + q3);
        p4 = h * f_2(omega_2 + q3);
        q4 = h * g_2(theta_1 + k3, theta_2 + p3, omega_1 + l3, omega_2 + q3);

        theta_1 = theta_1 + (k1 + (2 * k2) + (2 * k3) + k4) / 6;
        omega_1 = omega_1 + (l1 + (2 * l2) + (2 * l3) + l4) / 6;         
        //Below two lines keep the theta in range of -2PI to 2PI.
        if(theta_1 > 2 * PI) theta_1 = theta_1 - (2 * PI);
        if(theta_1 < -2 * PI) theta_1 = theta_1 + (2 * PI);
        

        theta_2 = theta_2 + (p1 + (2 * p2) + (2 * p3) + p4) / 6;
        omega_2 = omega_2 + (q1 + (2 * q2) + (2 * q3) + q4) / 6;
        //Below two lines keep the theta in range of -2PI to 2PI.
        if(theta_2 > 2 * PI) theta_2 = theta_2 - (2 * PI);
        if(theta_2 < -2 * PI) theta_2 = theta_2 + (2 * PI);

        pendulum1.angle = theta_1 * 180 / PI;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(pendulum1.angle), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.4f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 1080);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);


        pendulum2.angle = theta_2 * 180 / PI;

        glm::mat4 model2 = glm::mat4(1.0f);
        glm::mat4 projection2 = glm::mat4(1.0f);
        glm::mat4 view2 = glm::mat4(1.0f);
        view2 = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
        view2 = glm::rotate(view2, glm::radians(pendulum2.angle - pendulum1.angle), glm::vec3(0.0f, 0.0f, 1.0f));
        
        
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection2));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));

        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLES, 0, 1080);
        glBindVertexArray(VAO4);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    		glfwWindowShouldClose(window) == 0);

    glfwTerminate();

	return 0;
}