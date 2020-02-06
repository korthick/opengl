#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

using namespace std; 

static unsigned int CompileShader(unsigned int type , const string& source)
{
	
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);


	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{

		int length; 
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "shader") << "\n";
		cout << message; 

		glDeleteShader(id);

		return 0;

	}

	return id; 

}


static unsigned int create_shader(const string& vertex_shaders, const string& fragment_shaders)
{

	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER , vertex_shaders);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER , fragment_shaders);


	glAttachShader(program, vs);
	glAttachShader(program, fs); 
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)


{
	if (!glfwInit())
	{
		cout << "failed to intitialize opengl ! " << "\n";
	}
	else
	{
		/*

			GLFWwindow is used to make a windows , glfwcreatewindow returns a handle S
		
		*/

		GLFWwindow* window = glfwCreateWindow(640, 480, "window", NULL, NULL);

		if (window == NULL)
		{
			cout << "cannot open window !" << "\n";
		}

		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
		{
			cout << "failded to initialize GLEW" << "\n";
		}

		static const GLfloat vertices[] = {
			-1.0f,-1.0f,0.0f,
			1.0f,-1.0f,0.0f,
			0.0f,1.0f,0.0f
		};
		

		GLuint vertexbuffer;														// its the id of the generated buffers that sits in the gpus vram 
		glGenBuffers(1, &vertexbuffer);												// generates id for the buffers 
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);								// selects the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// puts data into the buffer 

		
		/*

			the data which sits in the vram is the buffer data for which
			shader code is written to render according the buffer data on 2d or 3d space 

		*/

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/*

			glVertexAttribPointer() : 0 , gives a index to start from for the given array of vertices
			stride gives the offset value between each vertex

			glEnableVertexAttribArray() : enables the vertex array to be used

			vertex is not the same as vertices !
	
		*/
		
		/*
		
			shaders are basically blocks of code written to the gpu for rendering 

			there are two types a vertex shadder and fragment shadder 
		
		*/

		string vertexshader = "#version 330 core \n"
							  "\n"
						      "layout(location = 0) in vec4 position"
							  "\n"
							  "void main()\n"
							  "{"
							  "		gl_posion = position;\n"
						      "}\n";

		string fragmentshader = "#version 330 core \n"
							    "\n"
								"layout(location = 0) out vec4 color ;"
								"\n"
								"void main()\n"
								"{"
								"		color = vec4(0.0,1.0,0.0,1.0);\n"
								"}\n";

		unsigned int shader = create_shader(vertexshader, fragmentshader);
		glUseProgram(shader);
		
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 3);

		
			glfwSwapBuffers(window);
			glfwPollEvents();

		}

	}


}
