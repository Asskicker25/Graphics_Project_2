#pragma once

#include "Debugger.h"
#include "Model.h"

class DebugModels
{
private:
	int sizeMultiple = 512;

	int currentIndex = 0;

	std::vector<Model*> listOfModels;

	Model* placeHolder;
	Renderer* renderer;

public:
	DebugModels(const std::string& path);

	void Initialize();
	void GrowList();
	void Clear();

	Model* DrawDebugModel();

};

