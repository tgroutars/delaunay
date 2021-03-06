
#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "mesh.h"

#define MAX(a, b) a > b ? a : b

using namespace std;


// Window size
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 position;\n"
  "void main()\n"
  "{\n"
  "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
  "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
  "}\n\0";


int main(int argc, char *argv[]) {

  if (argc != 2) {
    cout << "Wrong number of arguments" << endl;
    exit(69);
  }

  const char *filename = argv[1];

  // Create Mesh
  Mesh *mesh = new Mesh;
  mesh->Triangulate(filename);

  // Initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mesh", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, KeyCallback);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);

  // Vertex Shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
  }

  // Fragment Shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
  }

  // Shader Program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Vertices to display
  int size = mesh->size();
  GLdouble *vertices = new GLdouble[size * 3];
  mesh->Vertices(vertices);
  GLdouble x_max = mesh->x_max(),
          x_min = mesh->x_min(),
          y_max = mesh->y_max(),
          y_min = mesh->y_min();

  // Resize to fit in a [-1 1; -1 1] square
  int i;
  double delta = MAX((x_max - x_min), (y_max - y_min));
  for (i=0; i<size; i++) {
    vertices[i * 3] = (2. * vertices[i * 3] - (x_max + x_min)) / delta;
    vertices[i * 3 + 1] = (2. * vertices[i * 3 + 1] - (y_max + y_min)) / delta;
  }

  int n_edges = mesh->n_edges();
  GLuint *indices = new GLuint[n_edges * 2];
  mesh->Edges(indices);

  // Vertex Buffer Object
  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * n_edges * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * size * sizeof(GLdouble), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Main Loop
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0f);
    glLineWidth(1.0f);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, n_edges * 2, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_POINTS, 0, size);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }

  // Free Memory
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  delete[] indices;
  delete[] vertices;
  delete mesh;

  // Terminate GLFW
  glfwTerminate();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
