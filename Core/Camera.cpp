//
// Created by bios on 8/17/23.
//

#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, float newfov, float newnp, float newfp):farPlane(newfp),nearPlane(newnp),fov(newfov){
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Resize(int n_width, int n_height) {
    width  =n_width;
    height = n_height;
    updateMatrix();
}

void Camera::SetCameraMode(Camera::CamMode newMode) {
    mode = newMode;
    updateMatrix();
}

void Camera::updateMatrix(){
    //fov = FOVdeg;
    //Camera::nearPlane = nearPlane;
    //Camera::farPlane = farPlane;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);
    if(mode == CamMode::Perspective){
        projection = glm::perspective(glm::radians(fov), (float)(width/height), nearPlane, farPlane);
    }else if(mode == CamMode::Orthographic){
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, nearPlane, farPlane);
    }

    cameraMatrix  = projection * view;

}


void Camera::Matrix(Shader &shader, const char *uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform),1 , GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::LogLocation() {
    //std::cout<<"Position x: "<<Position.x<<" y: "<<Position.y<<" z: "<<Position.z<<std::endl;
}

void Camera::Inputs(GLFWwindow* window) {
    if(enableControl) {
        // Handles key inputs
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Position += current_speed * Orientation;
            LogLocation();
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Position += current_speed * -glm::normalize(glm::cross(Orientation, Up));
            LogLocation();
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Position += current_speed * -Orientation;
            LogLocation();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Position += current_speed * glm::normalize(glm::cross(Orientation, Up));
            LogLocation();
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            Position += current_speed * Up;
            LogLocation();
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            Position += current_speed * -Up;
            LogLocation();
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            current_speed = speed * 4;
        } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            current_speed = speed;
        }



        // Handles mouse inputs
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            // Hides mouse cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            // Prevents camera from jumping on the first click
            if (firstClick) {
                glfwSetCursorPos(window, (width / 2), (height / 2));
                firstClick = false;
            }

            // Stores the coordinates of the cursor
            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
            // and then "transforms" them into degrees
            float rotX = sensitivity * (float) (mouseY - (height / 2)) / height;
            float rotY = sensitivity * (float) (mouseX - (width / 2)) / width;

            // Calculates upcoming vertical change in the Orientation
            glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX),
                                                   glm::normalize(glm::cross(Orientation, Up)));

            // Decides whether or not the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
                Orientation = newOrientation;
                //std::cout << "Rotation x: "<< newOrientation.x<< " y: "<< newOrientation.y<< " z: "<< newOrientation.z << std::endl;
            }

            // Rotates the Orientation left and right
            Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (width / 2), (height / 2));
        } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
            // Unhides cursor since camera is not looking around anymore
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // Makes sure the next time the camera looks around it doesn't jump
            firstClick = true;
        }
    }else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}