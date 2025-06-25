/*
Implemente uma câmera em primeira pessoa. Para isso, recomendamos seguir as instruções 
apresentadas no material de aprofundamento. Sugerimos que vocês implementem a câmera 
como um objeto de uma classe Câmera, de maneira a agrupar todos os seus atributos e 
encapsular as ações de Mover e Rotacionar.

* ************************************************************************************
*                                    COMANDOS DE TECLA
* ************************************************************************************
* 
* X,Y,Z: rotaciona os objetos nos eixos X, Y, e Z.
* A, S, D, W: movimenta os objetos nos eixos X e Z
* I, J: movimenta os objetos no eixo Y
* 1, 2: escala os objetos nos três eixos.
* 8, 9 e 0: desliga/liga as luzes principa, secundária e de fundo.
* Seta esquerda, direita, cima e baixo: move a câmera no eixo X e Z.
* teclado numérico 7, 9: move a câmera no eixo Y.
* teclado numérico 4, 6: rotaciona a câmera para esquerda/direita
* teclado numérico 8, 2: rotaciona a câmera para cima/baixo
* teclado numérico 1, 3: rolamento da câmera no eixo Z.
* teclado numérico 5: reset da posição da câmera.

*/
#pragma once
#include <iostream>
#include <string>
#include <assert.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SimpleOBJLoader.h"
#include "MyTextureLoader.h"
#include "My3DObject.h"
#include "MyCamera.h"

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupShader(GLint& success);
std::string LoadShaderSource(const std::string& filePath);
void EnableLight(GLuint uniLocation, glm::vec3 color);

const GLuint WIDTH = (GLuint)(720 * 1.7), HEIGHT = 720;

bool rotateX = false, rotateY = false, rotateZ = false,
moveLeft = false, moveRight = false,
moveUp = false, moveDown = false,
moveFront = false, moveBack = false,
scaleUp = false, scaleDown = false,
enableLight1 = true, enableLight2 = true, enableLight3 = true,
moveCamLeft = false, moveCamRight = false, moveCamUp = false, moveCamDown = false,
moveCamForward = false, moveCamBackward = false,
rotateCamLeft = false, rotateCamRight = false, rotateCamUp = false, rotateCamDown = false,
rollCamLeft = false, rollCamRight = false, resetCamera = false;


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, u8"Computação Gráfica - Módulo 05", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLint setupShaderSuccess;
	GLuint texturedAOShaderID = setupShader(setupShaderSuccess);

	if (!setupShaderSuccess)
	{
		return 0;
	}	

	glUseProgram(texturedAOShaderID);

	My3DObject cubeLeft("Assets/cube.obj", "Assets/cube.mtl");
	cubeLeft.colorTextureId = MyTextureLoader::LoadTexture("Assets/Bricks059_1K-JPG_Color.jpg");
	cubeLeft.aoTextureId = MyTextureLoader::LoadTexture("Assets/Bricks059_1K-JPG_AmbientOcclusion.jpg");
	
	My3DObject cubeRight(cubeLeft);
	cubeRight.colorTextureId = MyTextureLoader::LoadTexture("Assets/PavingStones142_1K-PNG_Color.png");
	cubeRight.aoTextureId = MyTextureLoader::LoadTexture("Assets/PavingStones142_1K-PNG_AmbientOcclusion.png");

	My3DObject sphere ("Assets/sphere.obj", "Assets/sphere.mtl");
	sphere.colorTextureId = MyTextureLoader::LoadTexture("Assets/Tiles110_1K-PNG_Color.png");
	sphere.aoTextureId = MyTextureLoader::LoadTexture("Assets/Tiles110_1K-PNG_AmbientOcclusion.png");		

	//índice dos parâmetros no vertex shader
	GLint modelUniLocation = glGetUniformLocation(texturedAOShaderID, "model");
	GLint viewUniLocation = glGetUniformLocation(texturedAOShaderID, "viewMatrix");
	GLint projectionUniLocation = glGetUniformLocation(texturedAOShaderID, "projectionMatrix");

	//índice dos parâmetros no fragment shader
	GLint textureUniLocation = glGetUniformLocation(texturedAOShaderID, "colorTexture");
	GLint aoTextureUniLocation = glGetUniformLocation(texturedAOShaderID, "aoMap");
	GLint kaUniLocation = glGetUniformLocation(texturedAOShaderID, "ka");
	GLint kdUniLocation = glGetUniformLocation(texturedAOShaderID, "kd");
	GLint ksUniLocation = glGetUniformLocation(texturedAOShaderID, "ks");
	GLint mainLightUniLocation = glGetUniformLocation(texturedAOShaderID, "mainLightPosition");
	GLint fillLightUniLocation = glGetUniformLocation(texturedAOShaderID, "fillLightPosition");
	GLint backLightUniLocation = glGetUniformLocation(texturedAOShaderID, "backLightPosition");
	GLint shininessUniLocation = glGetUniformLocation(texturedAOShaderID, "shininess");
	GLint mainLightColorUniLocation = glGetUniformLocation(texturedAOShaderID, "mainLightColor");
	GLint fillLightColorUniLocation = glGetUniformLocation(texturedAOShaderID, "fillLightColor");
	GLint backLightColorUniLocation = glGetUniformLocation(texturedAOShaderID, "backLightColor");	
		
	cubeLeft.SetStartPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
	cubeRight.SetStartPosition(glm::vec3(5.0f, 0.0f, 0.0f));
	sphere.SetStartPosition(glm::vec3(0.0f, 0.0f, 0.0f));	

	MyCamera camera(30.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 15.0f), -90.0f, 0.0f, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		
	//posição das luzes
	glm::vec3 mainLightTransform = glm::vec3(0.6f, 1.2f, 2.5f);
	glm::vec3 fillLightTransform = glm::vec3(-1.0f, 0.8f, 2.0f); // Luz secundária (fill light)
	glm::vec3 backLightTransform = glm::vec3(0.0f, 1.5f, -5.5f); // Luz de recorte (back light)

	glm::vec3 mainLightColor = glm::vec3(1.0f, 0.957f, 0.898f); // main light color (warm)
	glm::vec3 fillLightColor = glm::vec3(0.7f, 0.7f, 0.8f); // fill light (cooler, bluish)
	glm::vec3 backLightColor = glm::vec3(1.0f, 1.0f, 1.0f); // back light (neutral white)
	glm::vec3 blackColor = glm::vec3(0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Descarta apenas faces traseiras
	glFrontFace(GL_CCW); // Define sentido anti-horário como frente	

	float angleIncrement = 180.0f;
	float modelSpeedFactor = 2.5f;
	float cameraSpeedFactor = 10.0f;
	float cameraRotationSpeedFactor = 25.0f;
	float scaleFactor = 1.0f;
	float currentFrameTime = 0.0f;
	float lastFrameTime = (float)glfwGetTime();
	float deltaTime = 0.0f;

	glUniformMatrix4fv(projectionUniLocation, 1, GL_FALSE, glm::value_ptr(camera.m_projectionMatrix));

	while (!glfwWindowShouldClose(window))
	{
		currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		
		glUniform3fv(mainLightUniLocation, 1, glm::value_ptr(mainLightTransform));
		glUniform3fv(fillLightUniLocation, 1, glm::value_ptr( fillLightTransform));
		glUniform3fv(backLightUniLocation, 1, glm::value_ptr(backLightTransform));	

		EnableLight(mainLightColorUniLocation, enableLight1 ? mainLightColor : blackColor);
		EnableLight(fillLightColorUniLocation, enableLight2 ? fillLightColor : blackColor);
		EnableLight(backLightColorUniLocation, enableLight3 ? backLightColor : blackColor);

		glm::vec3 modelTranslation = glm::vec3(0.0f);	
		glm::vec3 cameraTranslation = glm::vec3(0.0f);
		glm::vec3 cameraRotation = glm::vec3(0.0f);
		glm::vec3 modelRotation = glm::vec3(0.0f);
		
		float modelScaleFactor = 1.0f;
		
		if (moveLeft)
		{
			modelTranslation.x -= modelSpeedFactor * deltaTime;
		}
		else if(moveRight)
		{
			modelTranslation.x += modelSpeedFactor * deltaTime;
		}

		if (moveUp)
		{
			modelTranslation.y += modelSpeedFactor * deltaTime;
		}else if (moveDown)
		{
			modelTranslation.y -= modelSpeedFactor * deltaTime;
		}

		if (moveFront)
		{
			modelTranslation.z -= modelSpeedFactor * deltaTime;
		}else if (moveBack)
		{
			modelTranslation.z += modelSpeedFactor * deltaTime;
		}		
				
		if (scaleDown)
		{			
			modelScaleFactor = 1.0f - deltaTime * scaleFactor;
		}
		else if (scaleUp)
		{			
			modelScaleFactor = 1.0f + deltaTime * scaleFactor;
		}		
				
		if (rotateX)
		{
			modelRotation.x += angleIncrement * deltaTime;
		}

		if (rotateY)
		{
			modelRotation.y += angleIncrement * deltaTime;
		}

		if (rotateZ)
		{
			modelRotation.z += angleIncrement * deltaTime;
		}

		//movimentos de camera
		if (moveCamLeft)
		{
			cameraTranslation.x -= cameraSpeedFactor * deltaTime;
		}else if(moveCamRight)
		{
			cameraTranslation.x += cameraSpeedFactor * deltaTime;
		}

		if (moveCamUp)
		{
			cameraTranslation.y -= cameraSpeedFactor * deltaTime;
		}
		else if (moveCamDown)
		{
			cameraTranslation.y += cameraSpeedFactor * deltaTime;
		}

		if (moveCamForward)
		{
			cameraTranslation.z -= cameraSpeedFactor * deltaTime;
		}
		else if (moveCamBackward)
		{
			cameraTranslation.z += cameraSpeedFactor * deltaTime;
		}

		if (rotateCamLeft)
		{
			camera.AddYaw(-cameraRotationSpeedFactor * deltaTime);
		}
		else if (rotateCamRight)
		{
			camera.AddYaw(cameraRotationSpeedFactor * deltaTime);
		}

		if (rotateCamUp)
		{
			camera.AddPitch( -cameraRotationSpeedFactor * deltaTime);
		}
		else if (rotateCamDown)
		{
			camera.AddPitch(cameraRotationSpeedFactor * deltaTime);
		}

		if (rollCamLeft)
		{
			camera.AddRoll(-cameraRotationSpeedFactor * deltaTime);
		}
		else if (rollCamRight)
		{
			camera.AddRoll(cameraRotationSpeedFactor * deltaTime);
		}

		camera.AddPosition(cameraTranslation);

		if (resetCamera)
		{
			camera.ResetCamera();
		}

		
		camera.ApplyTransform();
		glUniformMatrix4fv(viewUniLocation, 1, GL_FALSE, glm::value_ptr(camera.m_viewMatrix));
				
		cubeLeft.SetScale(glm::vec3(modelScaleFactor));
		cubeRight.SetScale(glm::vec3(modelScaleFactor));
		sphere.SetScale(glm::vec3(modelScaleFactor));

		cubeLeft.SetTranslation(modelTranslation);
		cubeRight.SetTranslation(modelTranslation);
		sphere.SetTranslation(modelTranslation);

		cubeLeft.SetRotation(modelRotation);
		cubeRight.SetRotation(modelRotation);
		sphere.SetRotation(modelRotation);

		cubeLeft.Transform();
		cubeRight.Transform();
		sphere.Transform();

		// desenhar na tela		
		cubeLeft.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);
		cubeRight.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);
		sphere.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);		

		glfwSwapBuffers(window);
	}
	
	// Pede pra OpenGL desalocar os buffers
	cubeLeft.DeallocateOpenGL();
	cubeRight.DeallocateOpenGL();
	sphere.DeallocateOpenGL();

	glDeleteProgram(texturedAOShaderID);

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	rotateX = (key == GLFW_KEY_X) ? (action != GLFW_RELEASE) : rotateX;
	rotateY = (key == GLFW_KEY_Y) ? (action != GLFW_RELEASE) : rotateY;
	rotateZ = (key == GLFW_KEY_Z) ? (action != GLFW_RELEASE) : rotateZ;

	moveLeft = (key == GLFW_KEY_A) ? (action != GLFW_RELEASE) : moveLeft;
	moveRight = (key == GLFW_KEY_D) ? (action != GLFW_RELEASE) : moveRight;
	moveUp = (key == GLFW_KEY_I) ? (action != GLFW_RELEASE) : moveUp;
	moveDown = (key == GLFW_KEY_J) ? (action != GLFW_RELEASE) : moveDown;
	moveFront = (key == GLFW_KEY_W) ? (action != GLFW_RELEASE) : moveFront;
	moveBack = (key == GLFW_KEY_S) ? (action != GLFW_RELEASE) : moveBack;

	scaleDown = (key == GLFW_KEY_1) ? (action != GLFW_RELEASE) : scaleDown;
	scaleUp = (key == GLFW_KEY_2) ? (action != GLFW_RELEASE) : scaleUp;

	enableLight1 = (key == GLFW_KEY_8) && (action != GLFW_RELEASE) ? !enableLight1 : enableLight1;
	enableLight2 = (key == GLFW_KEY_9) && (action != GLFW_RELEASE) ? !enableLight2 : enableLight2;
	enableLight3 = (key == GLFW_KEY_0) && (action != GLFW_RELEASE) ? !enableLight3 : enableLight3;

	//camera
	moveCamLeft = (key == GLFW_KEY_LEFT) ? (action != GLFW_RELEASE) : moveCamLeft;
	moveCamRight = (key == GLFW_KEY_RIGHT) ? (action != GLFW_RELEASE) : moveCamRight;	
	moveCamForward = (key == GLFW_KEY_UP) ? (action != GLFW_RELEASE) : moveCamForward;
	moveCamBackward = (key == GLFW_KEY_DOWN) ? (action != GLFW_RELEASE) : moveCamBackward;

	moveCamUp = (key == GLFW_KEY_KP_7) ? (action != GLFW_RELEASE) : moveCamUp;
	moveCamDown = (key == GLFW_KEY_KP_9) ? (action != GLFW_RELEASE) : moveCamDown;

	rotateCamLeft = (key == GLFW_KEY_KP_4) ? (action != GLFW_RELEASE) : rotateCamLeft;
	rotateCamRight = (key == GLFW_KEY_KP_6) ? (action != GLFW_RELEASE) : rotateCamLeft;
	rotateCamUp = (key == GLFW_KEY_KP_8) ? (action != GLFW_RELEASE) : rotateCamUp;
	rotateCamDown = (key == GLFW_KEY_KP_2) ? (action != GLFW_RELEASE) : rotateCamDown;

	rollCamLeft = (key == GLFW_KEY_KP_1) ? (action != GLFW_RELEASE) : rollCamLeft;
	rollCamRight = (key == GLFW_KEY_KP_3) ? (action != GLFW_RELEASE) : rollCamRight;

	resetCamera = (key == GLFW_KEY_KP_5) ? (action != GLFW_RELEASE) : resetCamera;
	
	
}

//Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader(GLint& success)
{
	std::string vertexCode = LoadShaderSource("Shaders/cube.vert");
	std::string fragmentCode = LoadShaderSource("Shaders/cube.frag");

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Checando erros de compilação (exibição via log no terminal)
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

std::string LoadShaderSource(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Erro ao abrir o arquivo de shader: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void EnableLight(GLuint uniLocation, glm::vec3 color)
{
	glUniform3fv(uniLocation, 1, glm::value_ptr(color));
}
