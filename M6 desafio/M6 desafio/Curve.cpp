#include "Curve.h"

void Curve::setShader(Shader* shader)
{
	this->shader = shader;
	shader->Use();
}

void Curve::drawCurve(glm::vec4 color)
{
	shader->setVec4("finalColor", color.r, color.g, color.b, color.a);

	glBindVertexArray(VAO);
	
	
	glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
	
	glBindVertexArray(0);

}
