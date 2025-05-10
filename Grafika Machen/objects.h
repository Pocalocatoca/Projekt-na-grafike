#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define PI 3.1415

#define CLOCKFACE_SEGMENTS 36
#define CLOCKFACE_RADIUS 0.9

#define PILLAR_BASE_H 0.1
#define PILLAR_BODY_R 0.8
#define PILLAR_SEGMENTS 36

#define ENTABLATURE_INDENT_H 0.8
#define ENTABLATURE_INDENT_D 0.95

#define ARROW_THICKNESS 0.5

using namespace std;

enum DefaultObject
{
	Box,
	ClockFace,
	Pillar,
	Entablature
};

struct Vector2d
{
	double x;
	double y;
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
	unsigned texture;
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
	std::vector<Vector2d> _textures;
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
				{0.5,0.5,-0.5},
				{0.5,-0.5,-0.5},
				{-0.5,-0.5,-0.5},
				{-0.5,0.5,-0.5},

				{-0.5,0.5,0.5},
				{-0.5,-0.5,0.5},
				{0.5,-0.5,0.5},
				{0.5,0.5,0.5}
			};
			_textures = {
				{0,0}, {0,1}, {1,1}, {1,0}
			};
			_normals = {
				{0,0,-1}, {0,0,1}, {0,-1,0}, {0,1,0}, {-1,0,0}, {1,0,0}
			};
			_indices = {
				{2,1,2}, {1,2,2}, {6,3,2},
				{6,3,2}, {5,0,2}, {2,1,2},

				{3,0,3}, {4,1,3}, {7,2,3},
				{7,2,3}, {0,3,3}, {3,0,3},

				{7,0,5}, {6,1,5}, {1,2,5},
				{1,2,5}, {0,3,5}, {7,0,5},

				{3,0,4}, {2,1,4}, {5,2,4},
				{5,2,4}, {4,3,4}, {3,0,4},

				{4,0,1}, {5,1,1}, {6,2,1},
				{6,2,1}, {7,3,1}, {4,0,1},

				{0,0,0}, {1,1,0}, {2,2,0},
				{2,2,0}, {3,3,0}, {0,0,0}
			};
		}
		break;
		
		case Pillar:
		{
			for (unsigned i = 0; i < PILLAR_SEGMENTS; i++)
			{
				double cos_a = cos(2 * PI * i / PILLAR_SEGMENTS)/2;
				double sin_a = sin(2 * PI * i / PILLAR_SEGMENTS)/2;
				unsigned ig = (i + 1) % PILLAR_SEGMENTS;

				_vertices.push_back({ cos_a, 0.5, sin_a });
				_vertices.push_back({ cos_a, 0.5 - PILLAR_BASE_H, sin_a });
				_vertices.push_back({ cos_a * PILLAR_BODY_R, 0.5 - PILLAR_BASE_H, sin_a * PILLAR_BODY_R });
				_vertices.push_back({ cos_a * PILLAR_BODY_R, PILLAR_BASE_H - 0.5, sin_a * PILLAR_BODY_R });
				_vertices.push_back({ cos_a, PILLAR_BASE_H - 0.5, sin_a });
				_vertices.push_back({ cos_a, -0.5, sin_a });

				_textures.push_back({ 1-double(i) / PILLAR_SEGMENTS, 0 });
				_textures.push_back({ 1-double(i) / PILLAR_SEGMENTS, PILLAR_BASE_H });
				_textures.push_back({ 1-double(i) / PILLAR_SEGMENTS, 1 - PILLAR_BASE_H });
				_textures.push_back({ 1-double(i) / PILLAR_SEGMENTS, 1 });

				_normals.push_back({ cos_a,0,sin_a });

				_indices.push_back({ 6 * i + 1, 4 * i + 1, i });
				_indices.push_back({ 6 * i, 4 * i, i });
				_indices.push_back({ 6 * ig + 1, 4 * (i + 1) + 1, ig });

				_indices.push_back({ 6 * ig, 4 * (i + 1), ig });
				_indices.push_back({ 6 * ig + 1, 4 * (i + 1) + 1, ig });
				_indices.push_back({ 6 * i, 4 * i, i });

				_indices.push_back({ 6 * i + 2, 4 * i + 1, PILLAR_SEGMENTS });
				_indices.push_back({ 6 * i + 1, 4 * i, PILLAR_SEGMENTS });
				_indices.push_back({ 6 * ig + 2, 4 * (i + 1) + 1, PILLAR_SEGMENTS });

				_indices.push_back({ 6 * ig + 1, 4 * (i + 1), PILLAR_SEGMENTS });
				_indices.push_back({ 6 * ig + 2, 4 * (i + 1) + 1, PILLAR_SEGMENTS });
				_indices.push_back({ 6 * i + 1, 4 * i, PILLAR_SEGMENTS });

				_indices.push_back({ 6 * i + 3, 4 * i + 2, i });
				_indices.push_back({ 6 * i + 2, 4 * i + 1, i });
				_indices.push_back({ 6 * ig + 3, 4 * (i + 1) + 2, ig });

				_indices.push_back({ 6 * ig + 2, 4 * (i + 1) + 1, ig });
				_indices.push_back({ 6 * ig + 3, 4 * (i + 1) + 2, ig });
				_indices.push_back({ 6 * i + 2, 4 * i + 1, i });

				_indices.push_back({ 6 * i + 4, 4 * i + 3, PILLAR_SEGMENTS + 1 });
				_indices.push_back({ 6 * i + 3, 4 * i + 2, PILLAR_SEGMENTS + 1 });
				_indices.push_back({ 6 * ig + 4, 4 * (i + 1) + 3, PILLAR_SEGMENTS + 1 });

				_indices.push_back({ 6 * ig + 3, 4 * (i + 1) + 2, PILLAR_SEGMENTS + 1 });
				_indices.push_back({ 6 * ig + 4, 4 * (i + 1) + 3, PILLAR_SEGMENTS + 1 });
				_indices.push_back({ 6 * i + 3, 4 * i + 2, PILLAR_SEGMENTS + 1 });

				_indices.push_back({ 6 * i + 5, 4 * i + 3, i });
				_indices.push_back({ 6 * i + 4, 4 * i + 2, i });
				_indices.push_back({ 6 * ig + 5, 4 * (i + 1) + 3, ig });

				_indices.push_back({ 6 * ig + 4, 4 * (i + 1) + 2, ig });
				_indices.push_back({ 6 * ig + 5, 4 * (i + 1) + 3, ig });
				_indices.push_back({ 6 * i + 4, 4 * i + 2, i });
			}
			_textures.push_back({ 0, 0});
			_textures.push_back({ 0, PILLAR_BASE_H});
			_textures.push_back({ 0, 1 - PILLAR_BASE_H });
			_textures.push_back({ 0, 1 });

			_normals.push_back({ 0,-1,0 });
			_normals.push_back({ 0,1,0 });
		}
		break;

		case Entablature:
		{
			_vertices = {
				{ -0.5,  -0.5,  -0.5 },
				{ -0.5,  -0.5,   0.5 },
				{ -0.5,   0.5,   0.5 },
				{ -0.5,   0.5,  -0.5 },
				{  0.5,   0.5,  -0.5 },
				{  0.5,   0.5,   0.5 },
				{  0.5,  -0.5,   0.5 },
				{  0.5,  -0.5,  -0.5 },

				{ -0.5,  -0.5 * ENTABLATURE_INDENT_H,  -0.5 },
				{ -0.5,  -0.5 * ENTABLATURE_INDENT_H,   0.5 },
				{ -0.5,   0.5 * ENTABLATURE_INDENT_H,   0.5 },
				{ -0.5,   0.5 * ENTABLATURE_INDENT_H,  -0.5 },
				{  0.5,   0.5 * ENTABLATURE_INDENT_H,  -0.5 },
				{  0.5,   0.5 * ENTABLATURE_INDENT_H,   0.5 },
				{  0.5,  -0.5 * ENTABLATURE_INDENT_H,   0.5 },
				{  0.5,  -0.5 * ENTABLATURE_INDENT_H,  -0.5 },

				{ -0.5 * ENTABLATURE_INDENT_D,  -0.5 * ENTABLATURE_INDENT_H,  -0.5 * ENTABLATURE_INDENT_D },
				{ -0.5 * ENTABLATURE_INDENT_D,  -0.5 * ENTABLATURE_INDENT_H,   0.5 * ENTABLATURE_INDENT_D },
				{ -0.5 * ENTABLATURE_INDENT_D,   0.5 * ENTABLATURE_INDENT_H,   0.5 * ENTABLATURE_INDENT_D },
				{ -0.5 * ENTABLATURE_INDENT_D,   0.5 * ENTABLATURE_INDENT_H,  -0.5 * ENTABLATURE_INDENT_D },
				{  0.5 * ENTABLATURE_INDENT_D,   0.5 * ENTABLATURE_INDENT_H,  -0.5 * ENTABLATURE_INDENT_D },
				{  0.5 * ENTABLATURE_INDENT_D,   0.5 * ENTABLATURE_INDENT_H,   0.5 * ENTABLATURE_INDENT_D },
				{  0.5 * ENTABLATURE_INDENT_D,  -0.5 * ENTABLATURE_INDENT_H,   0.5 * ENTABLATURE_INDENT_D },
				{  0.5 * ENTABLATURE_INDENT_D,  -0.5 * ENTABLATURE_INDENT_H,  -0.5 * ENTABLATURE_INDENT_D }
			};
			const double tmp = (1-ENTABLATURE_INDENT_H)/2;
			_textures = { {0, 0}, {0, tmp}, {0, 1-tmp}, {0, 1}, {1, 1}, {1, 1-tmp}, {1, tmp}, {1, 0} };
			_normals = { {0,0,-1}, {0,0,1}, {0,-1,0}, {0,1,0}, {-1,0,0}, {1,0,0} };
			_indices = {
				//Begin Z+
				{2,0,1}, {10,1,1}, {13,6,1},
				{13,6,1}, {5,7,1}, {2,0,1},

				{10,1,2}, {18,1,2}, {21,6,2},
				{21,6,2}, {13,6,2}, {10,1,2},

				{18,1,1}, {17,2,1}, {22,5,1},
				{22,5,1}, {21,6,1}, {18,1,1},

				{17,2,3}, {9,2,3}, {14,5,3},
				{14,5,3}, {22,5,3}, {17,2,3},

				{9,2,1}, {1,3,1}, {6,4,1},
				{6,4,1}, {14,5,1}, {9,2,1},
				//End Z+
				
				//Begin Z-
				{4,0,0}, {12,1,0}, {11,6,0},
				{11,6,0}, {3,7,0}, {4,0,0},

				{12,1,2}, {20,1,2}, {19,6,2},
				{19,6,2}, {11,6,2}, {12,1,2},

				{20,1,0}, {23,2,0}, {16,5,0},
				{16,5,0}, {19,6,0}, {20,1,0},

				{23,2,3}, {15,2,3}, {8,5,3},
				{8,5,3}, {16,5,3}, {23,2,3},

				{15,2,0}, {7,3,0}, {0,4,0},
				{0,4,0}, {8,5,0}, {15,2,0},
				//End Z-

				//Begin X-
				{3,0,4}, {11,1,4}, {10,6,4},
				{10,6,4}, {2,7,4}, {3,0,4},

				{11,1,2}, {19,1,2}, {18,6,2},
				{18,6,2}, {10,6,2}, {11,1,2},

				{19,1,4}, {16,2,4}, {17,5,4},
				{17,5,4}, {18,6,4}, {19,1,4},

				{16,2,3}, {8,2,3}, {9,5,3},
				{9,5,3}, {17,5,3}, {16,2,3},

				{8,2,4}, {0,3,4}, {1,4,4},
				{1,4,4}, {9,5,4}, {8,2,4},
				//End X-

				//Begin X+
				{5,0,5}, {13,1,5}, {12,6,5},
				{12,6,5}, {4,7,5}, {5,0,5},
				
				{13,1,2}, {21,1,2}, {20,6,2},
				{20,6,2}, {12,6,2}, {13,1,2},

				{21,1,5}, {22,2,5}, {23,5,5},
				{23,5,5}, {20,6,5}, {21,1,5},

				{22,2,3}, {14,2,3}, {15,5,3},
				{15,5,3}, {23,5,3}, {22,2,3},

				{14,2,5}, {6,3,5}, {7,4,5},
				{7,4,5}, {15,5,5}, {14,2,5},
				//End X+

				//Begin Y+
				{3,0,3},{2,3,3},{5,4,3},
				{5,4,3},{4,7,3},{3,0,3},
				//End Y+

				//Begin Y-
				{1,0,3},{0,3,3},{7,4,3},
				{7,4,3},{6,7,3},{1,0,3},
				//End Y-
			};
		}
		break;

		case ClockFace:
		{
			for (unsigned i = 0; i < CLOCKFACE_SEGMENTS; i++)
			{
				double cos_a = cos(2 * PI * i / PILLAR_SEGMENTS) / 2;
				double sin_a = sin(2 * PI * i / PILLAR_SEGMENTS) / 2;
				unsigned ig = (i + 1) % PILLAR_SEGMENTS;

				_vertices.push_back({ cos_a, sin_a, 0 });
				_textures.push_back({ 0.5 - cos_a,sin_a + 0.5 });
				_indices.push_back({ i, i, 0 });
				_indices.push_back({ ig, ig, 0 });
				_indices.push_back({ PILLAR_SEGMENTS, PILLAR_SEGMENTS, 0 });
			}
			_vertices.push_back({ 0,0,0 });
			_textures.push_back({ 0.5,0.5 });
			_normals.push_back({ 0,0,1 });
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
			else if (prefix == "vt")
			{
				Vector2d temp;
				ss >> temp.x >> temp.y;
				_textures.push_back(temp);
			}
			else if (prefix == "f")
			{
				auto parseIndex = [](const std::string& token) 
				{
					face_index ret;
					std::stringstream s(token);
					s >> ret.vertex;
					s.ignore(50, '/');
					s >> ret.texture;
					s.ignore(50, '/');
					s >> ret.normal;
					ret.vertex--;
					ret.texture--;
					ret.normal--;
					//cout << ret.vertex << '\t' << ret.texture << '\t' << ret.normal << endl;
					return ret;
				};

				string temp;
				ss >> temp;
				_indices.push_back(parseIndex(temp));
				ss >> temp;
				_indices.push_back(parseIndex(temp));
				ss >> temp;
				_indices.push_back(parseIndex(temp));
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

	void Display(GLuint textureObj = 0)
	{
		glPushMatrix();
		_transform.Apply();
		glBindTexture(GL_TEXTURE_2D, textureObj);
		glBegin(GL_TRIANGLES);
		for (auto index : _indices)
		{
			Vector3d vertex = _vertices[index.vertex];
			Vector2d texture = _textures[index.texture];
			Vector3d normal = _normals[index.normal];

			glNormal3d(normal.x, normal.y, normal.z);
			glTexCoord2d(texture.x, texture.y);
			glVertex3d(vertex.x, vertex.y, vertex.z);
		}

		glEnd();
		glPopMatrix();
	}
};