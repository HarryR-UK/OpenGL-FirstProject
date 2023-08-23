//
//  Shader.cpp
//  MoreOPENGL
//
//  Created by Guest on 23/08/2023.
//

#include <stdio.h>
#include "Shader.h"

Shader::~Shader()
{
    
}

Shader::Shader(const char* filePath)
{
    std::ifstream stream (filePath);
    std::string line;
    std::stringstream ss[2];
    
    type = ShaderType::NONE;
    
    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[(int) type] << line << '\n';
        }
    }
    
    if(type == ShaderType::NONE)
    {
        std::cout << "ERROR READING FILE -- MAY NOT BE A VALID FORMAT" << '\n';
        return;
    }
   
    shaderProgramSource.vertexSource = ss[0].str();
    shaderProgramSource.fragmentSource = ss[1].str();
    
    // 2 -- compile and create shader
    this->ID = createShader(shaderProgramSource.vertexSource, shaderProgramSource.fragmentSource);
    
    
    
    
    
    
    
    
    
}
void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
    unsigned int tempID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(tempID, 1, &src, nullptr);
    glCompileShader(tempID);
    
    int result;
    glGetShaderiv(tempID, GL_COMPILE_STATUS, &result);
    
    if(!result)
    {
        char infoLog[512];
        glGetShaderInfoLog(tempID, 512, NULL, infoLog);
        std::cout << "ERROR COMPILING SHADER " << infoLog << '\n';
        glDeleteShader(tempID);
        return 0;
    }
    
    return tempID;
    
}

int Shader::createShader(const std::string &vShader, const std::string &fShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
    
}

void Shader::setVec4f(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}





