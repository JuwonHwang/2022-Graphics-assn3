#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct textureData {
	unsigned char* data;
	int width;
	int height;
	int nrChannels;
};

std::vector<std::pair<const char*, struct textureData*>> datas;

void textureLoader(const char* filename) {
	int width, height, nrChannels;
	struct textureData * texturedata = 0;
	unsigned char* data = 0;
	std::vector<std::pair<const char*, struct textureData*>>::iterator i=datas.begin();
	while (i != datas.end()) {
		if (strcmp(i->first, filename) == 0) {
			texturedata = i->second;
			break;
		}
		else {
			i++;
		}
	}
	if (texturedata == 0) {
		data = stbi_load(filename, &width, &height, &nrChannels, 0);
		if (data) {
			texturedata = new struct textureData;
			texturedata->data = data;
			texturedata->width = width;
			texturedata->height = height;
			texturedata->nrChannels = nrChannels;
			datas.push_back(std::pair<const char*, struct textureData*>(filename, texturedata));
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

	}
	else {
		data = texturedata->data;
		width = texturedata->width;
		height = texturedata->height;
		nrChannels = texturedata->nrChannels;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
}
