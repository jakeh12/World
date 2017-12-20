#include "draw.h"

void draw_triangles(GLuint buffer, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *)(sizeof(GLfloat) * 0));
    glDrawArrays(GL_TRIANGLES, 0, count);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
