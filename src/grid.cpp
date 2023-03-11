#include <engine/grid.h>
#include <math/Matrices.h>
#include <vector>
#include <utility/resource_manager.h>

Grid::Grid(Shader &shader, int row, int column, Vector3 color)
{
    this->shader = shader;
    this->row = row;
    this->column = column;
    this->color = color;
    this->Init();
}

Grid::~Grid()
{
    glDeleteVertexArrays(1, &this->rowVAO);
    glDeleteVertexArrays(1, &columnVAO);
}

void Grid::Init()
{
    float length = (float)column * cellSize;
    float width = (float)row * cellSize;
    std::vector<float> vertices;

    Vector3 startPos(0.0f, 0.0f, 0.0f);
    vertices.push_back(startPos.z);
    vertices.push_back(startPos.y);
    vertices.push_back(startPos.x);
    Vector3 endPos;
    
    //row attributes
    endPos = Vector3(length, 0.0f, 0.0f);
    vertices.push_back(endPos.z);
    vertices.push_back(endPos.y);
    vertices.push_back(endPos.x);
    unsigned int rowVBO;
    glGenBuffers(1, &rowVBO);
    glGenVertexArrays(1, &this->rowVAO);
    glBindVertexArray(this->rowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rowVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    //position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    //column attributes
    endPos = Vector3(0.0f, 0.0f, width);
    vertices.erase(vertices.end() - 3, vertices.end());
    vertices.push_back(endPos.z);
    vertices.push_back(endPos.y);
    vertices.push_back(endPos.x);
    unsigned int columnVBO;
    glGenBuffers(1, &columnVBO);
    glGenVertexArrays(1, &this->columnVAO);
    glBindVertexArray(this->columnVAO);
	glBindBuffer(GL_ARRAY_BUFFER, columnVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    //position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    ResourceManager::GetShader("gridShader").Use().SetVector3f("color", color);
}

void Grid::Draw()
{
    Matrix4 model;
    shader.Use();
	shader.SetMatrix4("model", model);
    glBindVertexArray(this->rowVAO);
    for (unsigned int i = 0; i <= row; i++)
    {
        model.identity();
        model.translate(Vector3(cellSize * i, 0.0f, 0.0f));
        shader.SetMatrix4("model", model);
        glDrawArrays(GL_LINES, 0, 2);
    }

    glBindVertexArray(this->columnVAO);
    for (unsigned int i = 0; i <= column; i++)
    {   
        model.identity();
        model.translate(Vector3(0.0f, 0.0f, cellSize * i));
        shader.SetMatrix4("model", model);
        glDrawArrays(GL_LINES, 0, 2);
    }

    glBindVertexArray(0);
}
