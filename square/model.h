// draw the triangle

#pragma once

class CModel
{

// variable

private:
	    GLuint id;
	    GLsizei vertex_count, stride;
	    GLint coord_per_vertex;
	    GLint vertex_offset;
	    GLhandleARB program;

public:

// function

private:
	    void CreateVertexData(float **vertex, int *vertex_size, int *vertex_count);

public:
	    CModel();
	    ~CModel();

	    void Create();
	    void Destroy();
	    void Render(float *matrix);

	    void SetShader(GLhandleARB program);
};