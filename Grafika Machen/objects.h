#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>


using namespace std;

enum DefaultObject
{
	Box,
	ClockFace,
	Arrow
};

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

class Transform
{
	Vector3d _position;
	Vector3d _rotation;
	Vector3d _scale;
public:
	Transform(const Vector3d& position, const Vector3d& rotation, const Vector3d& scale) : _position(position), _rotation(rotation), _scale(scale) {}
	
	void SetPosition(Vector3d position)
	{
		_position = position;
	}
	void SetRotation(Vector3d rotation)
	{
		_rotation = rotation;
	}
	void SetScale(Vector3d scale)
	{
		_scale = scale;
	}
	Vector3d& GetPosition()
	{
		return _position;
	}
	Vector3d& GetRotation()
	{
		return _rotation;
	}
	Vector3d& GetScale()
	{
		return _scale;
	}
	void Apply()
	{
		glTranslated(_position.x, _position.y, _position.z);
		glRotated(_rotation.x, 1, 0, 0);
		glRotated(_rotation.y, 0, 1, 0);
		glRotated(_rotation.z, 0, 0, 1);
	}
};

class Object
{
	Transform _transform;
	std::vector<Vector3d> _vertices;
	std::vector<Vector3d> _normals;
	std::vector<face_index> _indices;


public:
	Object(DefaultObject type, Vector3d position = { 0,0,0 }, Vector3d rotation = { 0,0,0 }, Vector3d scale = { 1,1,1 }) : _transform(position, rotation, scale)
	{
		switch (type)
		{
		
		case Box:
		{
			_vertices = {
				{-0.5,-0.5,-0.5},
				{-0.5,-0.5,0.5},
				{-0.5,0.5,0.5},
				{-0.5,0.5,-0.5},
				{0.5,-0.5,-0.5},
				{0.5,-0.5,0.5},
				{0.5,0.5,0.5},
				{0.5,0.5,-0.5}
			};
			_normals = {
				{1,0,0},
				{-1,0,0},
				{0,1,0},
				{0,-1,0},
				{0,0,1},
				{0,0,-1}
			};
			_indices = {
				// +x face
				{4,0}, {6,0}, {5,0},
				{4,0}, {7,0}, {6,0},
				// -x face
				{0,1}, {1,1}, {2,1},
				{2,1}, {3,1}, {0,1},
				// +y face
				{2,2}, {6,2}, {3,2},
				{3,2}, {6,2}, {7,2},
				// -y face
				{1,3}, {0,3}, {5,3},
				{5,3}, {0,3}, {4,3},
				// +z face
				{2,4}, {1,4}, {5,4},
				{2,4}, {5,4}, {6,4},
				// -z face
				{0,5}, {3,5}, {4,5},
				{4,5}, {3,5}, {7,5}
			};
		}
		break;
		
		case ClockFace:
		{

		}
		break;
		
		case Arrow:
		{

		}
		break;
		
		default:
		break;
		}
	}
	Object(string filename, Vector3d position = { 0,0,0 }, Vector3d rotation = { 0,0,0 }, Vector3d scale = { 1,1,1 }) : _transform(position, rotation, scale)
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

	void SetTransform(Vector3d position, Vector3d rotation, Vector3d scale)
	{
		_transform.SetPosition(position);
		_transform.SetRotation(rotation);
		_transform.SetScale(scale);
	}
	Transform& GetTransform()
	{
		return _transform;
	}

	void Display()
	{
		glPushMatrix();
		_transform.Apply();
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
