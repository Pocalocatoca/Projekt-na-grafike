#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>


using namespace std;

//*
struct Vector3d
{
	double x;
	double y;
	double z;
};

struct face_index
{
	unsigned vertex;
	unsigned normal;
};

class Object
{
	std::vector<Vector3d> _vertices;
	std::vector<Vector3d> _normals;
	std::vector<face_index> _indices;


public:
	Object(string filename)
	{
		fstream file(filename);
		if (file.fail())
		{
			cerr << "File \"" << filename << "\" is not available" << endl;
			file.close();
			return;
		}

		string text;
		while (getline(file, text))
		{
			stringstream ss(text);
			string prefix;
			ss >> prefix;
			if (prefix == "v")
			{
				Vector3d temp;
				ss >> temp.x >> temp.y >> temp.z;
				_vertices.push_back(temp);
			}
			else if (prefix == "vn")
			{
				Vector3d temp;
				ss >> temp.x >> temp.y >> temp.z;
				_normals.push_back(temp);
			}
			else if (prefix == "f")
			{
				string temp;
				face_index index;
				
				ss >> temp;
				{
					stringstream fs(temp);

					getline(fs, temp, '/');
					index.vertex = stoi(temp) - 1;
					getline(fs, temp, '/');
					getline(fs, temp, '/');
					index.normal = stoi(temp) - 1;


					_indices.push_back(index);
				}
				ss >> temp;
				{
					stringstream fs(temp);

					getline(fs, temp, '/');
					index.vertex = stoi(temp) - 1;
					getline(fs, temp, '/');
					getline(fs, temp, '/');
					index.normal = stoi(temp) - 1;


					_indices.push_back(index);
				}
				ss >> temp;
				{
					stringstream fs(temp);

					getline(fs, temp, '/');
					index.vertex = stoi(temp) - 1;
					getline(fs, temp, '/');
					getline(fs, temp, '/');
					index.normal = stoi(temp) - 1;


					_indices.push_back(index);
				}
			}
		}

		file.close();
	}

	void Display()
	{
		glPushMatrix();
		glBegin(GL_TRIANGLES);

		for (auto index : _indices)
		{
			Vector3d normal = _normals[index.normal];
			Vector3d vertex = _vertices[index.vertex];
			
			glNormal3d(normal.x, normal.y, normal.z);
			glVertex3d(vertex.x, vertex.y, vertex.z);
		}

		glEnd();
		glPopMatrix();
	}
};
//*/