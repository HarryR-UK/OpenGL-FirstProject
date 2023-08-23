//
//  Shader.h
//  MoreOPENGL
//
//  Created by Guest on 23/08/2023.
//

#ifndef Shader_h
#define Shader_h
#include <GL/glew.h>
#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

struct Shader
{
private:
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    
    struct ShaderProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };
    
    unsigned int compileShader(unsigned int type, const std::string& source);
    int createShader(const std::string& vShader, const std::string& fShader);
    
public:
    
    unsigned int ID;
    ShaderType type = ShaderType::NONE;
    ShaderProgramSource shaderProgramSource;
    
    Shader(const char* filePath);
    ~Shader();
    void use();
    
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4f(const std::string& name, float x, float y, float z, float w);
};

#endif /* Shader_h */
