// draw the triangle

#include "stdafx.h"
#include "model.h"

// constructor
CModel::CModel()
{
    id               = 0;
    coord_per_vertex = 3;                                   // x, y, z
    stride           = coord_per_vertex * sizeof(float);    // in bytes

    vertex_offset = 0;
}

// destructor
CModel::~CModel()
{
}

// create vertex data
void CModel::CreateVertexData(float **vertex, int *size, int *count)
{
	*count = 4;                          // 4 vertices of a square
	*size  = *count * coord_per_vertex;  // size of an array

	// allocate a one-dimensional array
	// then fill it with vertices
	*vertex = new float[*size];          
	 
	 //            1                         2
	 //     12.73  +-------------------------+
	 //            |                         |
	 //            |                         |
	 //            |                         |
	 //    -17.39  +-------------------------+
	 //          -17.39                    12.73
	//             4                         3

    // vertex 1
    (*vertex)[0] = -17.39f;   // x
    (*vertex)[1] =  12.73f;   // y
    (*vertex)[2] =   0.0f;    // z

    // vertex 2
    (*vertex)[3] = 12.73f;   // x
    (*vertex)[4] = 12.73f;   // y
    (*vertex)[5] =  0.0f;    // z

    // vertex 3
    (*vertex)[6] =  12.73f;   // x
    (*vertex)[7] = -17.39f;   // y
    (*vertex)[8] =   0.0f;    // z

    // vertex 4
    (*vertex)[ 9] = -17.39f;   // x
    (*vertex)[10] = -17.39f;   // y
    (*vertex)[11] =   0.0f;    // z
}

// create vertex buffer object
void CModel::Create()
{
    float* vertex   = NULL;
    int vertex_size = 0;

    // create vertex data
    CreateVertexData(&vertex, &vertex_size, &vertex_count);

    glGenBuffersARB(1, &id);                                                                       // generate a buffer object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);                                                      // bind the buffer object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB); // copy vertex data to the buffer object 

    // no need, release memory
    if(vertex != NULL) delete[] vertex;
}

// delete buffer object
void CModel::Destroy()
{
    glDeleteBuffersARB(1, &id);
}

// draw the triangle
void CModel::Render(float *matrix)
{
	    GLint loc_m;

		// bind the buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);

		// send matrix to a vertex shader
	    loc_m = glGetUniformLocationARB(program, "m_matrix");
	    glUniformMatrix4fvARB(loc_m, 1, false, matrix);

		// send vertices to a vertex shader
	    glVertexAttribPointerARB(LOC_VERTEX, coord_per_vertex, GL_FLOAT, GL_FALSE, stride, (GLvoid*)vertex_offset);

		// draw triangle
	    glEnableVertexAttribArrayARB(LOC_VERTEX);
	    glDrawArrays(GL_QUADS, 0, vertex_count);
	    glDisableVertexAttribArrayARB(LOC_VERTEX);

		// unbind buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

// set shader to be used
// this is returned by the class function CShader::GetHandle()
void CModel::SetShader(GLhandleARB program)
{
	    this->program = program;
}

//
