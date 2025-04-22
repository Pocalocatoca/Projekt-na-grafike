#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#define PI 3.1415

#define CLOCKFACE_SEGMENTS 36
#define CLOCKFACE_RADIUS 0.9

#define PILLAR_HEIGHT 10
#define PILLAR_SEGMENTS 24

#define ENTABLATURE_HEIGHT 1.5

using namespace std;

enum DefaultObject
{
	Box,
	ClockFace,
	Pillar,
	Entablature
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
		glScaled(_scale.x, _scale.y, _scale.z );
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

			for (unsigned i = 0; i < CLOCKFACE_SEGMENTS; i++)
			{

				double t_cos = cos(PI * i * 2 / CLOCKFACE_SEGMENTS) / 2;
				double t_sin = sin(PI * i * 2 / CLOCKFACE_SEGMENTS) / 2;

				_vertices.push_back({ t_cos,0,t_sin });
				_vertices.push_back({ t_cos,0.1,t_sin });
				_vertices.push_back({ t_cos * CLOCKFACE_RADIUS,0.05,t_sin * CLOCKFACE_RADIUS });
				_vertices.push_back({ t_cos * CLOCKFACE_RADIUS,0.1,t_sin * CLOCKFACE_RADIUS });
				
				_normals.push_back({ t_cos,0,t_sin });
				_normals.push_back({ -t_cos,0,-t_sin });
				
				_indices.push_back({ 4 * i,2 * i });
				_indices.push_back({ 4 * i + 1,2 * i });
				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 1,2 * ((i + 1) % CLOCKFACE_SEGMENTS) });

				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 1,2 * ((i + 1) % CLOCKFACE_SEGMENTS) });
				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS),2 * ((i + 1) % CLOCKFACE_SEGMENTS) });
				_indices.push_back({ 4 * i,2 * i });

				_indices.push_back({ 4 * i + 1,2 * CLOCKFACE_SEGMENTS });
				_indices.push_back({ 4 * i + 3,2 * CLOCKFACE_SEGMENTS });
				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 3,2 * CLOCKFACE_SEGMENTS });

				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 3,2 * CLOCKFACE_SEGMENTS });
				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 1,2 * CLOCKFACE_SEGMENTS });
				_indices.push_back({ 4 * i + 1,2 * CLOCKFACE_SEGMENTS });

				_indices.push_back({ 4 * i + 2,2 * i + 1});
				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 3,2 * ((i + 1) % CLOCKFACE_SEGMENTS) + 1 });
				_indices.push_back({ 4 * i + 3,2 * i + 1 });

				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 3,2 * ((i + 1) % CLOCKFACE_SEGMENTS) + 1 });
				_indices.push_back({ 4 * i + 2,2 * i + 1 });
				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 2,2 * ((i + 1) % CLOCKFACE_SEGMENTS) + 1 });

				_indices.push_back({ 4 * ((i + 1) % CLOCKFACE_SEGMENTS) + 2,2 * CLOCKFACE_SEGMENTS });
				_indices.push_back({ 4 * i + 2,2 * CLOCKFACE_SEGMENTS });
				_indices.push_back({ 4 * CLOCKFACE_SEGMENTS,2 * CLOCKFACE_SEGMENTS });
			}
			_vertices.push_back({ 0,0.05,0 });
			_normals.push_back({ 0,1,0 });
		}
		break;
		
		case Pillar:
		{
			for (unsigned i = 0; i < PILLAR_SEGMENTS; i++)
			{
				double t_cos = cos(PI * i * 2 / PILLAR_SEGMENTS) / 2;
				double t_sin = sin(PI * i * 2 / PILLAR_SEGMENTS) / 2;

				_vertices.push_back({ t_cos, PILLAR_HEIGHT, t_sin });
				_vertices.push_back({ t_cos, PILLAR_HEIGHT - 0.2, t_sin });
				_vertices.push_back({ 0.8 * t_cos, PILLAR_HEIGHT - 0.4, 0.8 * t_sin });
				_vertices.push_back({ 0.8 * t_cos, 0.4, 0.8 * t_sin });
				_vertices.push_back({ t_cos, 0.2, t_sin });
				_vertices.push_back({ t_cos, 0, t_sin });
			
				_normals.push_back({ t_cos, 0, t_sin });
				_normals.push_back({ t_cos, -1, t_sin });
				_normals.push_back({ t_cos, 1, t_sin });
			}
			for (unsigned i = 0; i < PILLAR_SEGMENTS; i++)
			{
				_indices.push_back({ 6 * i + 1, 3 * i });
				_indices.push_back({ 6 * i,3 * i });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS), 3 * ((i + 1) % PILLAR_SEGMENTS) });

				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS), 3 * ((i + 1) % PILLAR_SEGMENTS) });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 1, 3 * ((i + 1) % PILLAR_SEGMENTS) });
				_indices.push_back({ 6 * i + 1, 3 * i });
			
				_indices.push_back({ 6 * i + 2, 3 * i + 1 });
				_indices.push_back({ 6 * i + 1,3 * i + 1 });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 1, 3 * ((i + 1) % PILLAR_SEGMENTS) + 1 });

				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 1, 3 * ((i + 1) % PILLAR_SEGMENTS) + 1 });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 2, 3 * ((i + 1) % PILLAR_SEGMENTS) + 1 });
				_indices.push_back({ 6 * i + 2, 3 * i + 1 });
				
				_indices.push_back({ 6 * i + 3, 3 * i });
				_indices.push_back({ 6 * i + 2,3 * i });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 2, 3 * ((i + 1) % PILLAR_SEGMENTS) });

				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 2, 3 * ((i + 1) % PILLAR_SEGMENTS) });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 3, 3 * ((i + 1) % PILLAR_SEGMENTS) });
				_indices.push_back({ 6 * i + 3, 3 * i });
				
				_indices.push_back({ 6 * i + 4, 3 * i + 2 });
				_indices.push_back({ 6 * i + 3,3 * i + 2 });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 3, 3 * ((i + 1) % PILLAR_SEGMENTS) + 2 });

				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 3, 3 * ((i + 1) % PILLAR_SEGMENTS) + 2 });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 4, 3 * ((i + 1) % PILLAR_SEGMENTS) + 2 });
				_indices.push_back({ 6 * i + 4, 3 * i + 2 });

				_indices.push_back({ 6 * i + 5, 3 * i });
				_indices.push_back({ 6 * i + 4,3 * i });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 4, 3 * ((i + 1) % PILLAR_SEGMENTS) });

				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 4, 3 * ((i + 1) % PILLAR_SEGMENTS) });
				_indices.push_back({ 6 * ((i + 1) % PILLAR_SEGMENTS) + 5, 3 * ((i + 1) % PILLAR_SEGMENTS) });
				_indices.push_back({ 6 * i + 5, 3 * i });
			}
		}
		break;

		case Entablature:
		{
			for (unsigned i = 0; i < 4; i++)
			{
				double t_cos = cos(PI * (i-0.5) / 2) / sqrt(2);
				double t_sin = sin(PI * (i-0.5) / 2) / sqrt(2);
				double t_cos_n = cos(PI * i / 2);
				double t_sin_n = sin(PI * i / 2);

				_vertices.push_back({ t_cos, ENTABLATURE_HEIGHT, t_sin });
				_vertices.push_back({ t_cos, ENTABLATURE_HEIGHT - 0.2, t_sin });
				_vertices.push_back({ 0.95 * t_cos, ENTABLATURE_HEIGHT - 0.25, 0.95 * t_sin });
				_vertices.push_back({ 0.95 * t_cos, 0.25, 0.95 * t_sin });
				_vertices.push_back({ t_cos, 0.2, t_sin });
				_vertices.push_back({ t_cos, 0, t_sin });

				_normals.push_back({ t_cos_n, 0, t_sin_n });
				_normals.push_back({ t_cos_n, -1, t_sin_n });
				_normals.push_back({ t_cos_n, 1, t_sin_n });
			}
			for (unsigned i = 0; i < 4; i++)
			{
				_indices.push_back({ 6 * i + 1, 3 * i });
				_indices.push_back({ 6 * i,3 * i });
				_indices.push_back({ 6 * ((i + 1) % 4), 3 * i });

				_indices.push_back({ 6 * ((i + 1) % 4), 3 * i });
				_indices.push_back({ 6 * ((i + 1) % 4) + 1, 3 * i });
				_indices.push_back({ 6 * i + 1, 3 * i });

				_indices.push_back({ 6 * i + 2, 3 * i + 1 });
				_indices.push_back({ 6 * i + 1,3 * i + 1 });
				_indices.push_back({ 6 * ((i + 1) % 4) + 1, 3 * i + 1 });

				_indices.push_back({ 6 * ((i + 1) % 4) + 1, 3 * i + 1 });
				_indices.push_back({ 6 * ((i + 1) % 4) + 2, 3 * i + 1 });
				_indices.push_back({ 6 * i + 2, 3 * i + 1 });

				_indices.push_back({ 6 * i + 3, 3 * i });
				_indices.push_back({ 6 * i + 2,3 * i });
				_indices.push_back({ 6 * ((i + 1) % 4) + 2, 3 * i });

				_indices.push_back({ 6 * ((i + 1) % 4) + 2, 3 * i });
				_indices.push_back({ 6 * ((i + 1) % 4) + 3, 3 * i });
				_indices.push_back({ 6 * i + 3, 3 * i });

				_indices.push_back({ 6 * i + 4, 3 * i + 2 });
				_indices.push_back({ 6 * i + 3,3 * i + 2 });
				_indices.push_back({ 6 * ((i + 1) % 4) + 3, 3 * i + 2 });

				_indices.push_back({ 6 * ((i + 1) % 4) + 3, 3 * i + 2 });
				_indices.push_back({ 6 * ((i + 1) % 4) + 4, 3 * i + 2 });
				_indices.push_back({ 6 * i + 4, 3 * i + 2 });

				_indices.push_back({ 6 * i + 5, 3 * i });
				_indices.push_back({ 6 * i + 4,3 * i });
				_indices.push_back({ 6 * ((i + 1) % 4) + 4, 3 * i });

				_indices.push_back({ 6 * ((i + 1) % 4) + 4, 3 * i });
				_indices.push_back({ 6 * ((i + 1) % 4) + 5, 3 * i });
				_indices.push_back({ 6 * i + 5, 3 * i });
			}
			_normals.push_back({ 0,1,0 }); //12
			_normals.push_back({ 0,-1,0 }); //13

			_indices.push_back({ 12,12 });
			_indices.push_back({ 6,12 });
			_indices.push_back({ 0,12 });

			_indices.push_back({ 0,12 });
			_indices.push_back({ 18,12 });
			_indices.push_back({ 12,12 });

			_indices.push_back({ 5,13 });
			_indices.push_back({ 11,13 });
			_indices.push_back({ 17,13 });

			_indices.push_back({ 5,13 });
			_indices.push_back({ 17,13 });
			_indices.push_back({ 23,13 });
		}
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
