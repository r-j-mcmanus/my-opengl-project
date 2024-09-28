#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "shader.h"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class ShaderManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;  // Map of shader names to Shader objects

public:
    // Load and store a new shader
    void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
        shaders[name] = std::make_shared<Shader>(vertexPath, fragmentPath);
    }

    template <typename T, typename... Args>
    void loadShader(const std::string& name, const std::string& fragAndVertexPath, Args&&... args) {
        static_assert(std::is_base_of<Shader, T>::value, "T must be derived from Shader");

        ShaderProgramSource source = PaseShader(fragAndVertexPath);
        // Use perfect forwarding to construct the shader with the passed arguments
        shaders[name] = std::make_shared<T>(source.VertexSource, source.FragmentSource, std::forward<Args>(args)...);
    }

    void loadShader(const std::string& name, const std::string& fragAndVertexPath) {
        ShaderProgramSource source = PaseShader(fragAndVertexPath);
        shaders[name] = std::make_shared<Shader>(source.VertexSource, source.FragmentSource);
    }

    // Retrieve a shader by name
    std::shared_ptr<Shader> getShader(const std::string& name) const {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            return it->second;
        }
        return nullptr;  // Return null if shader not found
    }

    std::shared_ptr<Shader> operator[](const std::string& name) const {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            return it->second;
        }
        return nullptr;  // Return nullptr if shader not found
    }

    const static ShaderProgramSource PaseShader(const std::string& filepath)
    {
        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        // Note file should not have a new line at the end
        std::fstream stream(filepath);
        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                ss[(int)type] << line << "\n";
            }
        }

        
        std::cout << "VERTEX" << std::endl;
        std::cout << ss[0].str() << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << ss[1].str() << std::endl;
        


        return { ss[0].str(), ss[1].str() };
    }

};

