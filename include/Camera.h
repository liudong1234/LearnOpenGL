#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;  //偏航角
const float PITCH = 0.0f;   //俯仰角
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// 相机属性
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp; // 世界坐标系的上方向量  
	// 欧拉角
	float Yaw;
	float Pitch;
	// 相机选项
	float MovementSpeed;   //鼠标速度
	float MouseSensitivity;   //鼠标灵敏度
	float Zoom;						//缩放级别

	// 使用向量作为参数的构造函数  
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// 使用标量值作为参数的构造函数  
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	// 返回使用欧拉角和LookAt矩阵计算得出的视图矩阵
	inline glm::mat4 GetViewMatrix()	{return glm::lookAt(Position, Position + Front, Up);}
	// 处理来自任何类似键盘的输入系统接收到的输入。接受输入参数的形式为相机定义的枚举（将其从窗口系统中抽象出来）  
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	// 处理来自鼠标输入系统接收到的输入。期望在x和y方向上的偏移值。  
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	//处理来自鼠标滚轮事件接收到的输入。只需要垂直滚轮轴上的输入  
	void ProcessMouseScroll(double yoffset);


private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};
#endif