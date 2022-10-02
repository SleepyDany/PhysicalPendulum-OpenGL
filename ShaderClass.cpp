#include "ShaderClass.h"

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);

    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());

        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());

        in.close();
        return(contents);
    }

    throw(errno);
}

Shader::Shader(const char* vertex_file, const char* fragment_file)
{
    std::string vertex_code = get_file_contents(vertex_file);
    std::string fragment_code = get_file_contents(fragment_file);

    const char* vertex_source = vertex_code.c_str();
    const char* fragment_source = fragment_code.c_str();

    // Create Vertex Shader Object and get reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertex_source, NULL);
    // Compile Vertex Shader into machine code
    glCompileShader(vertexShader);

    // Create Fragment Shader Object and get reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragment_source, NULL);
    // Compile Fragment Shader into machine code
    glCompileShader(fragmentShader);


    // Create Shader Program Object and get reference
    ID = glCreateProgram();
    // Attach Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Programd
    glLinkProgram(ID);

    // Delete the now useless Vertex and Fragment Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}
