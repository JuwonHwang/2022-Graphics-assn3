#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

bool loadOBJ(
	std::string path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec3>& out_collider
) {
	//std::cout << "Loading OBJ file " << path << std::endl;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::ifstream fin(path);
	if (fin.fail()){
		std::cout << "Impossible to open the file" << std::endl;
		return false;
	}

	while (1) {

		std::string lineHeader;
		// read the first word of the line
		fin >> lineHeader;
		if (fin.eof())
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		//std::cout << lineHeader << " ";
		if (lineHeader == "v") {
			glm::vec3 vertex;
			fin >> vertex.x >> vertex.y >> vertex.z;
			//std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
			temp_vertices.push_back(vertex);
			out_collider.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv;
			fin >> uv.x >> uv.y;
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			//std::cout << uv.x << " " << uv.y << std::endl;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal;
			fin >> normal.x >> normal.y >> normal.z;
			//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f") {
			char buf;
			int vertexIndex[3], uvIndex[3], normalIndex[3];

			fin >> vertexIndex[0] >> buf >> uvIndex[0] >> buf >> normalIndex[0];
			//std::cout << vertexIndex[0] << " " << buf << " " << uvIndex[0] << " " << buf << " " << normalIndex[0] << " ";

			fin >> vertexIndex[1] >> buf >> uvIndex[1] >> buf >> normalIndex[1];
			//std::cout << vertexIndex[1] << " " << buf << " " << uvIndex[1] << " " << buf << " " << normalIndex[1] << " ";

			fin >> vertexIndex[2] >> buf >> uvIndex[2] >> buf >> normalIndex[2];
			//std::cout << vertexIndex[2] << " " << buf << " " << uvIndex[2] << " " << buf << " " << normalIndex[2] << std::endl;

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fin.getline(stupidBuffer,1000);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
}