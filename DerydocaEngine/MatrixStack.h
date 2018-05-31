#pragma once
#include <vector>
#include <stack>
#include "glm\glm.hpp"

class MatrixStack
{
public:
	MatrixStack() {
		m_matrixStack.push(glm::mat4());
	};
	~MatrixStack() { }

	void push(glm::mat4 matrix) {
		glm::mat4 newMatrix = getMatrix() * matrix;
		m_matrixStack.push(newMatrix);
	}

	void pop() {
		m_matrixStack.pop();
	}

	glm::mat4 getMatrix() const {
		return m_matrixStack.top();
	}
private:
	std::stack<glm::mat4> m_matrixStack;
};

