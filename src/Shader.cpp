#include "Shader.h"

Shader::Shader(const char* vertex_file_path, const char* fragment_file_path)
{
    // Create the shaders
    GLuint vertexShID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertexShCode;
    std::ifstream vertexShStream(vertex_file_path, std::ios::in);
    if (vertexShStream.is_open())
    {
        std::stringstream sstr;
        sstr << vertexShStream.rdbuf();
        vertexShCode = sstr.str();
        vertexShStream.close();
    }
    else
    {
        printf("Vertex Shader issue: impossible to open %s.\n", vertex_file_path);
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShCode;
    std::ifstream fragmentShStream(fragment_file_path, std::ios::in);
    if (fragmentShStream.is_open())
    {
        std::stringstream sstr;
        sstr << fragmentShStream.rdbuf();
        fragmentShCode = sstr.str();
        fragmentShStream.close();
    }
    else
    {
        printf("Vertex Shader issue: impossible to open %s.\n", fragment_file_path);
    }

    GLint result = GL_FALSE;
    int infoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const* vertexSourcePtr = vertexShCode.c_str();
    glShaderSource(vertexShID, 1, &vertexSourcePtr, NULL);
    glCompileShader(vertexShID);

    // Check Vertex Shader
    glGetShaderiv(vertexShID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char buffer[1024];
        glGetShaderInfoLog(vertexShID, infoLogLength, NULL, buffer);
        printf("%s\n", buffer);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const* fragmentSourcePtr = fragmentShCode.c_str();
    glShaderSource(fragmentShID, 1, &fragmentSourcePtr, NULL);
    glCompileShader(fragmentShID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char buffer[1024];
        glGetShaderInfoLog(fragmentShID, infoLogLength, NULL, buffer);
        printf("%s\n", buffer);
    }



    // Link the program
    printf("Linking program\n");
    ID = glCreateProgram();
    glAttachShader(ID, vertexShID);
    glAttachShader(ID, fragmentShID);
    glLinkProgram(ID);


    // Check the program
    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        char buffer[1024];
        glGetProgramInfoLog(ID, infoLogLength, NULL, buffer);
        printf("%s\n", buffer);
    }


    glDetachShader(ID, vertexShID);
    glDetachShader(ID, fragmentShID);

    glDeleteShader(vertexShID);
    glDeleteShader(fragmentShID);
}


void Shader::use()
{
    glUseProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set4Float(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

int Shader::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(ID, name.c_str());
}

int Shader::getAtrributeLocation(const std::string& name)
{
    return glGetAttribLocation(ID, name.c_str());
}

//void Shader::set4Matrix(const std::string &name, glm::mat4 objToWorld) const
//{
//  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(objToWorld));
//}
