#ifndef VECTORUTILS2
#define VECTORUTILS2

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <Windows.h>
	#include <gl\glew.h>
	#include <GL/freeglut.h>
#endif
#include <math.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define Vector3f Point3D
#define Matrix3f Matrix3D
#define Matrix4f Matrix4D

	typedef struct Point3D
	{
		GLfloat x, y, z;
	}
	Point3D, *Point3DPtr;
	typedef GLfloat Matrix4D[16], Matrix3D[9];

	void CopyVector(Point3D *v, Point3D *dest);
	void SetVector(GLfloat x, GLfloat y, GLfloat z, Point3D *v);
// Grundläggande 3D-operationer
	void VectorSub(Point3D *a, Point3D *b, Point3D *result);
	void VectorAdd(Point3D *a, Point3D *b, Point3D *result);
	void CrossProduct(Point3D *a, Point3D *b, Point3D *result);
	GLfloat DotProduct(Point3D *a, Point3D *b);
	void ScalarMult(Point3D *a, GLfloat s, Point3D *result);
	GLfloat Norm(Point3D *a);
	void Normalize(Point3D *a);
	void CalcNormalVector(Point3D *a, Point3D *b, Point3D *c, Point3D *result);
	void SplitVector(Point3D *v, Point3D *n, Point3D *vn, Point3D *vp);

// Matrisoperationer på 4x4-matriser!
// Alla matriser är arrays av GLfloat!
// OBS att matriserna är definierade kolumnvis, som OpenGL vill ha dem!
// Dvs andra elementet är UNDER det första, inte till höger!

	void IdentityMatrix(GLfloat *m);
	void Rx(GLfloat a, GLfloat *m);
	void Ry(GLfloat a, GLfloat *m);
	void Rz(GLfloat a, GLfloat *m);
	void T(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat *m);
	void S(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat *m);
	void Mult(GLfloat *a, GLfloat *b, GLfloat *dest); // dest = a * b
	void MatrixMultPoint3D(GLfloat *a, Point3D *b, Point3D *result); // result = a * b
	void CopyMatrix(GLfloat *src, GLfloat *dest);


	void OrthoNormalizeMatrix(GLfloat *R);
	void Transpose(GLfloat *m, GLfloat *dest);
	void TransposeRotation(GLfloat *m, GLfloat *dest);
	void ArbRotate(Point3D *axis, GLfloat fi, GLfloat *m);
	void CrossMatrix(Point3D *a, GLfloat *m);
	void MatrixAdd(GLfloat *a, GLfloat *b, GLfloat *dest);

	void SetTransposed(char t);

	void lookAt(Point3D *p, Point3D *l,
			GLfloat vx, GLfloat vy, GLfloat vz,
			GLfloat *m );
	void perspective(float fovyInDegrees, float aspectRatio,
                      float znear, float zfar, float *matrix);
	void frustum(float left, float right, float bottom, float top,
                  float znear, float zfar, float *matrix);

#endif
