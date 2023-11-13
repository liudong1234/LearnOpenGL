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
const float YAW = -90.0f;  //ƫ����
const float PITCH = 0.0f;   //������
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// �������
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp; // ��������ϵ���Ϸ�����  
	// ŷ����
	float Yaw;
	float Pitch;
	// ���ѡ��
	float MovementSpeed;   //����ٶ�
	float MouseSensitivity;   //���������
	float Zoom;						//���ż���

	// ʹ��������Ϊ�����Ĺ��캯��  
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// ʹ�ñ���ֵ��Ϊ�����Ĺ��캯��  
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	// ����ʹ��ŷ���Ǻ�LookAt�������ó�����ͼ����
	inline glm::mat4 GetViewMatrix()	{return glm::lookAt(Position, Position + Front, Up);}
	// ���������κ����Ƽ��̵�����ϵͳ���յ������롣���������������ʽΪ��������ö�٣�����Ӵ���ϵͳ�г��������  
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	// ���������������ϵͳ���յ������롣������x��y�����ϵ�ƫ��ֵ��  
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	//���������������¼����յ������롣ֻ��Ҫ��ֱ�������ϵ�����  
	void ProcessMouseScroll(double yoffset);


private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};
#endif