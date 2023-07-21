#include "Matrix4.h"

#include <cmath>

// �P�ʍs����
Matrix4 Matrix4::Identity()
{
	static const Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4 Matrix4::Identity(Matrix4 matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				matrix.m[j][i] = 1;
			}
			else
			{
				matrix.m[j][i] = 0;
			}
		}
	}

	return matrix;
}

// �s��̃X�J���[
Matrix4 Matrix4::ScalarProduct(Matrix4 matrix, Vector3 scalar)
{
	matrix.m[0][0] *= scalar.x;
	matrix.m[1][0] *= scalar.x;
	matrix.m[2][0] *= scalar.x;
	matrix.m[3][0] *= scalar.x;

	matrix.m[0][1] *= scalar.y;
	matrix.m[1][1] *= scalar.y;
	matrix.m[2][1] *= scalar.y;
	matrix.m[3][1] *= scalar.y;

	matrix.m[0][2] *= scalar.z;
	matrix.m[1][2] *= scalar.z;
	matrix.m[2][2] *= scalar.z;
	matrix.m[3][2] *= scalar.z;

	return matrix;
}

// �s�񓯎m�̊|���Z
Matrix4 Matrix4::Matrix4Product(Matrix4 matrix, Matrix4 matrix2)
{
	Matrix4 matResult = {};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matResult.m[j][i] =
				matrix.m[j][0] * matrix.m[i][0] +
				matrix.m[j][1] * matrix.m[i][1] +
				matrix.m[j][2] * matrix.m[i][2] +
				matrix.m[j][3] * matrix.m[i][3];
		}
	}

	return matResult;
}

// �g��k���s��
Matrix4 Matrix4::Scale(const Vector3& s)
{
	Matrix4 result
	{
		s.x , 0.0f, 0.0f, 0.0f,
		0.0f, s.y , 0.0f, 0.0f,
		0.0f, 0.0f, s.z , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

#pragma region ��]�s��

Matrix4 Matrix4::RotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos , sin , 0.0f,
		0.0f, -sin, cos , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

Matrix4 Matrix4::RotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos , 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin , 0.0f, cos , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

Matrix4 Matrix4::RotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos , sin , 0.0f, 0.0f,
		-sin, cos , 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

#pragma endregion

Matrix4 Matrix4::Translate(const Vector3& t)
{
	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x , t.y , t.z , 1.0f
	};

	return result;
}

Vector3 Matrix4::Transform(const Vector3& v, const Matrix4& m)
{
	Vector3 result
	{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};

	return result;
}

Vector3 Matrix4::TransformDivW(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]
	};

	result /= w;

	return result;
}

Matrix4 Matrix4::Inverse(const Matrix4 m)
{
	// i = �s  j = ��  k = �Ίp���������݂����

	Matrix4 mat = {};
	Matrix4 result = {};
	Matrix4 identityMatrix = {};

	mat = m;

	// identityMatrix��P�ʍs��ŏ�����
	identityMatrix = Matrix4::Identity();

	// �|���o���p �E���ɂł�4*4�����ʂɂȂ� �E���ɒP�ʍs�����
	float sweep[4][8] = {};

	// �s��̍����Ɍ��̍s��
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			sweep[i][j] = mat.m[i][j];
			sweep[i][(j + 4)] = identityMatrix.m[i][j];
		}
	}

	// �t�s��v�Z
	for (int k = 0; k < 4; k++) 
	{
		// �[�����Z�΍�
		double max = fabs(sweep[k][k]);
		int max_i = k;

		for (int i = k + 1; i < 4; i++) 
		{
			if (fabs(sweep[i][k]) > max) 
			{
				max = fabs(sweep[i][k]);
				max_i = i;
			}
		}

		// ��Βl�̍ő�l��0�Ȃ�t�s�񂪖����̂ŒP�ʍs���Ԃ��Ƃ���B
		if (fabs(sweep[max_i][k]) <= 0.000001f) 
		{
			return identityMatrix;
		}

		// ����ւ�
		if (k != max_i)
		{
			for (int i = 0; i < 8; i++) 
			{
				// ����ւ�����temp�ɁAmax_i ����
				float temp = sweep[max_i][i];

				// �R�s�[�����̂ŁAk i ����
				sweep[max_i][i] = sweep[k][i];

				// temp�Ɋi�[���Ă��������̃f�[�^�� k i�ɑ��
				sweep[k][i] = temp;
			}
		}

		// [k][k]�Ɋ|������1�ɂȂ�l�����߂� (�[�����Z�΍�ς�)
		float identityScalar = 1 / sweep[k][k];
		for (int i = 0; i < 8; i++) 
		{
			sweep[k][i] *= identityScalar;
		}

		// [i][k]��0�ɂȂ�悤��k�s�� -i�{
		for (int i = 0; i < 4; i++) 
		{
			// �P�ʍs��ʒu�Ȃ��΂�
			if (i == k) { continue; }

			// k�s�ڂɊ|����l�����߂�
			float constantNum = -sweep[i][k];

			for (int j = 0; j < 8; j++) 
			{
				// �����i k ��0�ɂȂ�
				sweep[i][j] += sweep[k][j] * constantNum;
			}
		}
	}

	// ����
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			result.m[i][j] = sweep[i][j + 4];
		}
	}

	return result;
}

Matrix4 Matrix4::CreateViewMat(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Matrix4 result = {};

	// �J�����̃��[���h�s����쐬
	Vector3 cameraVecZ = target - eye;
	cameraVecZ.Normalize();

	Vector3 cameraVecX = up.Cross(cameraVecZ);
	cameraVecX.Normalize();

	Vector3 cameraVecY = cameraVecZ.Cross(cameraVecX);
	cameraVecY.Normalize();

	result = 
	{
		cameraVecX.x, cameraVecX.y, cameraVecX.z,	0,
		cameraVecY.x, cameraVecY.y, cameraVecY.z,	0,
		cameraVecZ.x, cameraVecZ.y, cameraVecZ.z,	0,
		eye.x		, eye.y		  , eye.z		,	1
	};

	// �쐬�������[���h�s����t�s��ɕϊ�
	result = Inverse(result);

	return result;
}

Matrix4 Matrix4::CreateProjectionMat(float fovY, float aspectRatio, float nearZ, float farZ)
{
	Matrix4 result = {};

	float h = 1 / tanf(fovY / 2.0f);
	float w = h / aspectRatio;
	float z = farZ / (farZ - nearZ);
	float cameraZ = (-nearZ * farZ) / (farZ - nearZ);

	result = 
	{
		w,0,0,0,
		0,h,0,0,
		0,0,z,1,
		0,0,cameraZ,0
	};

	return result;
}

Matrix4& Matrix4::operator*=(const Matrix4& m2)
{
	Matrix4 result = {};

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++) 
			{
				result.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = result.m[i][j];
		}
	}

	*this = result;
	return *this;
}

const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
}

const Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	Matrix4 result = {};

	return result.Transform(v, m);
}