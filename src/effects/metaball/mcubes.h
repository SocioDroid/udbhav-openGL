#include "MetaballHelper.h"

class mcubes
{
public:
	mcubes(double start_x, double start_y, double start_z, double end_x, double end_y, double end_z,
		   double step_x, double step_y, double step_z);
	virtual ~mcubes();

	void resize();
	void draw();
	void computeMetaBalls();

	void setMetaBalls(mcubes_metaballs *mb, double iso_value)
	{
		this->mb = mb;
		this->mb->set_iso_value(iso_value);
		this->metaballs_iso_value = iso_value;
	}

	void setWireframe(bool s)
	{
		this->wireframe = s;
	}

private:
	double start_x;
	double start_y;
	double start_z;

	double end_x;
	double end_y;
	double end_z;

	double step_x;
	double step_y;
	double step_z;

	int size_x;
	int size_y;
	int size_z;

	double metaballs_iso_value;

	bool wireframe;

	mcubes_metaballs *mb;

	const static int edgeTable[256];
	const static int triTable[256][16];

	vertex *vertices;

	vertex verts[12];

	GLuint vao;			 // Vertex Array Object -
	GLuint vbo_Position; // Vertex Buffer Object - Pyramid - Position
	GLuint vbo_Texcoord; // Vertex Buffer Object - Pyramid
	GLuint vbo_Normal;	 // Vertex Buffer Object - Pyramid
	vector<float> vectorVertices;
	vector<float> vectorTexCoords;
	vector<float> vectorNormals;
};
