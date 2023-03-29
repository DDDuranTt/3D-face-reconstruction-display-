#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "InputSystem.h"
#include "Model.h"




#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "config.h"

void PrintMat(glm::mat4& mat) {
	for (int i = 0; i < 4; ++i)
	{
		std::cout << mat[i].x << '\t' << mat[i].y << '\t' << mat[i].z << '\t' << mat[i].w << std::endl;
	}
	std::cout << "=================" << std::endl;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(450, 800, "OpenGL Tutorial", NULL, NULL);
#ifdef Show
	window = glfwCreateWindow(1920, 1080, "OpenGL Tutorial", NULL, NULL);
#else
	window = glfwCreateWindow(960, 540, "OpenGL Tutorial", NULL, NULL);
	//window = glfwCreateWindow(1920, 1080, "OpenGL Tutorial", NULL, NULL);
#endif // Show


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() == GLEW_OK) {
		std::cout << glGetString(GL_VERSION) << std::endl;
	}
	else {
		std::cout << "GLEW do not init success" << std::endl;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	{
		//float positions[] = {
		//		-10.0f, -10.0f,  10.0f, 0.0f, 0.0f,
		//		 10.0f, -10.0f,  10.0f, 1.0f, 0.0f,
		//		 10.0f,  10.0f,  10.0f, 1.0f, 1.0f,
		//		-10.0f,  10.0f,  10.0f, 0.0f, 1.0f,

		//		 10.0f, -10.0f, -10.0f, 0.0f, 0.0f,
		//		 10.0f,  10.0f, -10.0f, 1.0f, 0.0f,
		//		-10.0f,  10.0f, -10.0f, 1.0f, 1.0f,
		//		-10.0f, -10.0f, -10.0f, 0.0f, 1.0f
		//};

		//glm::vec3 cubePositions[] = {
		//  glm::vec3(10.0f,  0.0f,  0.0f),
		//  glm::vec3(20.0f,  50.0f, -150.0f),
		//  glm::vec3(-15.0f, -22.0f, -25.0f),
		//  glm::vec3(-38.0f, -20.0f, -123.0f),
		//  glm::vec3(24.0f, -4.0f, -35.0f),
		//  glm::vec3(-17.0f,  30.0f, -75.0f),
		//  glm::vec3(13.0f, -20.0f, -25.0f),
		//  glm::vec3(15.0f,  20.0f, -25.0f),
		//  glm::vec3(15.0f,  2.0f, -15.0f),
		//  glm::vec3(-13.0f,  10.0f, -15.0f)
		//};

		//unsigned int indices[] = {
		//	0, 1, 2, 2, 3, 0,
		//	1,4,5,5,2,1,
		//	4,7,6,6,5,4,
		//	7,0,3,3,6,7,
		//	3,2,5,5,6,3,
		//	7,4,1,1,0,7
		//};

		//float positions[] = { -300.0f, -400.0f, 0.0f, 0.0f, 0.0f,
		//					   300.0f, -400.0f, 0.0f, 1.0f, 0.0f,
		//					   300.0f,  400.0f, 0.0f, 1.0f, 1.0f,
		//					  -300.0f,  400.0f, 0.0f, 0.0f, 1.0f };
		float positions[] = { -480.0f, -270.0f, 0.0f, 0.0f, 1.0f, 0.0f,
							   480.0f, -270.0f, 0.0f, 1.0f, 1.0f, 1.0f,
							   480.0f,  270.0f, 0.0f, 1.0f, 0.0f, 0.0f,
							  -480.0f,  270.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};


		Shader shader("res/shaders/Basic.shader");
		Shader faceshader("res/shaders/face.shader");
		Shader planeShader("res/shaders/plane.shader");

		Shader leftFace("res/shaders/leftFace.shader");
		Shader rightFace("res/shaders/rightFace.shader");
		Shader centerFace("res/shaders/centerFace.shader");

		VertexArray va;
		VertexBuffer vb(positions, 8 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

#ifdef Show
		Texture texure("C:/Users/Chen/Desktop/RealSence/c1.png");
		Texture texure1("C:/Users/Chen/Desktop/RealSence/c2.png");
		Texture texure2("C:/Users/Chen/Desktop/RealSence/c3.png");
		texure.Bind(0);
		texure1.Bind(1);
		texure2.Bind(2);

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));//开启深度测试
		GLCall(glDepthFunc(GL_LEQUAL));
		Model center("C:/Users/Chen/Desktop/res/center.ply", "C:/Users/Chen/Desktop/res/centerAlpha.txt");
		Model left("C:/Users/Chen/Desktop/res/left.ply", "C:/Users/Chen/Desktop/res/leftAlpha.txt");
		Model right("C:/Users/Chen/Desktop/res/right.ply", "C:/Users/Chen/Desktop/res/rightAlpha.txt");

#else
		Texture texure("C:/Users/Chen/Desktop/2022-0411_4RealSense数据/c1.png");
		texure.Bind(0);

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));//开启深度测试

		Model test("C:/Users/Chen/Desktop/res/concat_recon_v10.ply");
#endif // Show


		//Texture texure("res/textures/test.PNG");

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();




		//Model test("D:/pointCloud/mine/res/concat_recon_7_left_uv_Total_funSelect.obj");

		Renderer renderer;

		Camera camera(60.0f, glm::vec3(0, 0, 0));

		InputSystem input(window, &camera);



		/*----------Imgui init------------*/
		const char* glsl_version = "#version 330";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);


		/*----------Imgui state------------*/
		glm::vec3 planetranslation(0, 0, 466.965f);

		glm::vec3 modeltranslation(0, 0, 0);
		float colorTransparency = 1.0f;
		float threshold = 0.0f;
		glm::mat4 modelRotate(1.0f);

		bool showCenter = true;
		bool showleft = true;
		bool showright = true;

		float leftEdge1 = -13.2f;
		float leftEdge2 = -24.4f;

		float rightEdge1 = 11.6f;
		float rightEdge2 = 22.8f;

		float centerLeftEdge1 = 22.8f;
		float centerLeftEdge2 = 13.2f;
		float centerRightEdge1 = -31.6f;
		float centerRightEdge2 = -7.6f;
		//modelRotate[0][0] = 0.999724;
		//modelRotate[1][0] = -0.00885273;
		//modelRotate[2][0] = 0.0216444;
		//modelRotate[3][0] = -3.4157;

		//modelRotate[0][1] = -0.0101512;
		//modelRotate[1][1] = 0.66947;
		//modelRotate[2][1] = 0.742769;
		//modelRotate[3][1] = -274.451;

		//modelRotate[0][2] = -0.0210654;
		//modelRotate[1][2] = -0.742782;
		//modelRotate[2][2] = 0.669199;
		//modelRotate[3][2] = 135.903;

		glm::vec3 modelscale(1, 1, 1);
		glm::vec3 modelangle(0, 0, 0);
		glm::vec3 viewTranslation(0, 0, 0);
		glm::vec3 viewRotate(0, 0, 0);
		bool changeColor = false;


		float r = 0.0f;
		float increment = 0.002f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			camera.UpdateDeltaTime();
			input.InputDetect();

			glm::mat4 mvp;

			shader.Bind();
			if (changeColor) {
				shader.SetUniform4f("u_Color", r, 0.3, 0.8, 0.7);
				if (r > 1.0f)
					increment = -increment;
				else if (r < 0.0f)
					increment = -increment;

				r += increment;
			}
			else {
				shader.SetUniform4f("u_Color", 0.3, 1.0, 0.3, 0.5);
			}
#ifdef Show
			glm::mat4 modeltrans = glm::translate(glm::mat4(1.0f), modeltranslation);
			glm::mat4 modelrotateX = glm::rotate(glm::mat4(1.0f), glm::radians(modelangle.x), glm::vec3(1, 0, 0));
			glm::mat4 modelrotateY = glm::rotate(glm::mat4(1.0f), glm::radians(modelangle.y), glm::vec3(0, 1, 0));
			glm::mat4 modelrotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(modelangle.z), glm::vec3(0, 0, 1));
			glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), modelscale);
			glm::mat4 modelTotal = modeltrans * modelrotateX * modelrotateY * modelrotateZ * modelScale;
			mvp = camera.GetMatrix() * modelTotal;

			//PrintMat(mvp);
			faceshader.Bind();
			faceshader.SetUniformMat4f("u_MVP", mvp);

			if (showCenter) {
				centerFace.Bind();
				centerFace.SetUniformMat4f("u_MVP", mvp);
				centerFace.SetUniform1f("centerLeftEdge1", centerLeftEdge1);
				centerFace.SetUniform1f("centerLeftEdge2", centerLeftEdge2);
				centerFace.SetUniform1f("centerRightEdge1", centerRightEdge1);
				centerFace.SetUniform1f("centerRightEdge2", centerRightEdge2);
				renderer.Draw(center, centerFace, 0);
			}

			if (showleft) {
				leftFace.Bind();
				leftFace.SetUniformMat4f("u_MVP", mvp);
				leftFace.SetUniform1f("leftEdge1", leftEdge1);
				leftFace.SetUniform1f("leftEdge2", leftEdge2);
				renderer.Draw(left, leftFace, 1);
			}


			if (showright) {
				rightFace.Bind();
				rightFace.SetUniformMat4f("u_MVP", mvp);
				rightFace.SetUniform1f("rightEdge1", rightEdge1);
				rightFace.SetUniform1f("rightEdge2", rightEdge2);
				renderer.Draw(right, rightFace, 2);
			}

			//if (showCenter) {
			//	renderer.Draw(center, faceshader, 0);
			//}
			//if (showleft) {
			//	renderer.Draw(left, faceshader, 1);
			//}
			//if (showright) {
			//	renderer.Draw(right, faceshader, 2);
			//}

#else

			mvp = camera.GetMatrix() * glm::translate(glm::mat4(1.0f), planetranslation);
			planeShader.Bind();
			planeShader.SetUniformMat4f("u_MVP", mvp);
			planeShader.SetUniform1i("u_Texture", 0);
			renderer.Draw(va, ib, planeShader);


			glm::mat4 modeltrans = glm::translate(glm::mat4(1.0f), modeltranslation);
			//glm::mat4 modelrotateX = glm::rotate(glm::mat4(1.0f), glm::radians(modelangle.x), glm::vec3(1, 0, 0));
			//glm::mat4 modelrotateY = glm::rotate(glm::mat4(1.0f), glm::radians(modelangle.y), glm::vec3(0, 1, 0));
			//glm::mat4 modelrotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(modelangle.z), glm::vec3(0, 0, 1));
			glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), modelscale);
			glm::mat4 modelTotal = modeltrans * modelRotate * modelScale;
			mvp = camera.GetMatrix() * modelTotal;
			//PrintMat(mvp);
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);
			shader.SetUniform1f("u_colorTransparency", colorTransparency);
			shader.SetUniform1f("u_threshold", threshold);
			shader.SetUniform3f("u_centerView", 0, 0, -1);
			shader.SetUniform3f("u_leftView", 0, 0.706662f, -0.707552);
			shader.SetUniform3f("u_rightView", 0, -0.742942, -0.669355);
			renderer.Draw(test, shader);
#endif // Show

			{//ImGui
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				{
					ImGui::Begin("ImGui");
#ifdef Show
					ImGui::Checkbox("show Center", &showCenter);
					ImGui::Checkbox("show left", &showleft);
					ImGui::Checkbox("show right", &showright);
					ImGui::SliderFloat3("Rotation A", &modelangle.x, 0.0f, 90.0f);
					ImGui::SliderFloat("left Edge 1", &leftEdge1, -90.0f, 90.0f);
					ImGui::SliderFloat("left Edge 2", &leftEdge2, -90.0f, 90.0f);
					ImGui::SliderFloat("right Edge 1", &rightEdge1, -90.0f, 90.0f);
					ImGui::SliderFloat("right Edge 2", &rightEdge2, -90.0f, 90.0f);
					ImGui::SliderFloat("center left Edge 1", &centerLeftEdge1, -90.0f, 90.0f);
					ImGui::SliderFloat("center left Edge 2", &centerLeftEdge2, -90.0f, 90.0f);
					ImGui::SliderFloat("center right Edge 1", &centerRightEdge1, -90.0f, 90.0f);
					ImGui::SliderFloat("center right Edge 2", &centerRightEdge2, -90.0f, 90.0f);
#else
					ImGui::SliderFloat("Translation x", &modeltranslation.x, 1.0f, 21.0f);
					ImGui::SliderFloat("Translation y", &modeltranslation.y, -12.0f, 20.0f);
					ImGui::SliderFloat("Translation z", &modeltranslation.z, -200.0f, -50.0f);
					ImGui::SliderFloat("alpha", &colorTransparency, 0.0f, 1.0f);
					ImGui::SliderFloat("阈值", &threshold, 0.0f, 1.0f);
					ImGui::SliderFloat("Plane Translation z", &planetranslation.z, 0.0f, 1000.640f);
					ImGui::SliderFloat3("Scale A", &modelscale.x, 0.1f, 1.0f);
#endif
					ImGui::Checkbox("enable Camera Rotation", &camera.enableCameraRotation);
					ImGui::Checkbox("enable Change Color", &changeColor);
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::End();
				}

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}