#include "ShaderImporter.h"

#include "openGL.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include <iosfwd>
#include <fstream>

std::string readShaderFile(const std::string &shader_file)
{
	std::string code;

	std::string shader_library = "VertexShaders/";
	std::string real_dir = App->file_system->GetRealDirectory((shader_library + shader_file).c_str());
	real_dir.append("/");

	std::string shader_path = (real_dir + shader_file);
	std::ifstream fileStream(shader_path, std::ios::in);

	if (fileStream.is_open())
	{
		std::string line = "";
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			code.append(line + "\n");
		}
	}

	fileStream.close();
	return code;
}


unsigned int ShaderImporter::Load(const std::string &vertex_shader_asset)
{
	std::string vertexShaderSource = readShaderFile(vertex_shader_asset);

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);	
	const char *source_code = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &source_code, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		DEBUG("%s%s", "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog);
	}

	/*glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);*/

	glDeleteShader(vertexShader);
	//glDeleteShader(FragmentShaderID);

	return 0;
}	

