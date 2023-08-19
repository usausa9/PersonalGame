#include "OBJModel.h"

#include "DirectXBase.h"

#include <sstream>
#include <fstream>

using namespace std;

OBJModel::OBJModel()
{

}

OBJModel OBJModel::LoadFromOBJ(const std::string& modelName)
{
	// Model�^�̃C���X�^���X�������m��
	OBJModel model = OBJModel();

	// �ǂݍ���
	model.LoadFromOBJInternal(modelName);

	// �΂��ӂ�����
	model.CreateBuffers();

	return model;
}

void OBJModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	ifstream file;
	// ��ركt�@�C�����J��
	file.open(directoryPath + filename);

	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) { assert(0); }

	string line;

	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::stringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') 
		{
			key.erase(key.begin());	// �擪�폜
		}

		// �擪�� newmtl �Ȃ�}�e���A����
		if (key == "newmtl") 
		{
			// �}�e���A���ǂݍ���
			line_stream >> material_.name;
		}

		// �擪�� Ka �Ȃ�A���r�G���g�F
		if (key == "Ka") 
		{
			// �}�e���A���ǂݍ���
			line_stream >> material_.ambient.x;
			line_stream >> material_.ambient.y;
			line_stream >> material_.ambient.z;
		}

		// �擪�� Kd �Ȃ�f�B�t���[�Y�F
		if (key == "Ka") 
		{
			// �}�e���A���ǂݍ���
			line_stream >> material_.diffuse.x;
			line_stream >> material_.diffuse.y;
			line_stream >> material_.diffuse.z;
		}

		// �擪�� Ks �Ȃ�X�y�L�����[�F
		if (key == "Ks") 
		{
			// �}�e���A���ǂݍ���
			line_stream >> material_.specular.x;
			line_stream >> material_.specular.y;
			line_stream >> material_.specular.z;
		}

		// �擪�� map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			string texFPath;

			// �e�N�X�`��
			line_stream >> texFPath;

			// �e�N�X�`���̃��[�h
			material_.index = TextureManager::Load(directoryPath + texFPath);
		}
	}
	// �t�@�C�������
	file.close();
}

void OBJModel::CreateBuffers()
{
	HRESULT result;
#pragma region ���_�o�b�t�@
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���_�f�[�^ ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (int i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff_->Unmap(0, nullptr);

	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();

	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;

	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(VertexPosNormalUv);
#pragma endregion
#pragma region �C���f�b�N�X�o�b�t�@
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	// �C���f�b�N�X�f�[�^ ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);

	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices_.size(); i++) 
	{
		indexMap[i] = indices_[i]; //�C���f�b�N�X���R�s�[
	}

	// �C���f�b�N�X �}�b�s���O����
	indexBuff_->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
#pragma endregion

#pragma region �萔�o�b�t�@
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff; // 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&cbHeapProp,	// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result));

#pragma region �}�e���A���̓]��
	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_); // �}�b�s���O
	assert(SUCCEEDED(result));

	constMapMaterial_->ambient = material_.ambient;
	constMapMaterial_->diffuse = material_.diffuse;
	constMapMaterial_->specular = material_.specular;
	constMapMaterial_->alpha = material_.alpha;

	constMapMaterial_->color = { 1,1,1,1 };
#pragma endregion
}

void OBJModel::Draw()
{
	// ���_�o�b�t�@�r���[�̐ݒ�
	DirectXBase::GetInstance()->commandList_->IASetVertexBuffers(0, 1, &vbView_);
	DirectXBase::GetInstance()->commandList_->IASetIndexBuffer(&ibView_);

	// �萔�o�b�t�@�r���[���Z�b�g�i�}�e���A���j
	DirectXBase::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	DirectXBase::GetInstance()->commandList_->
		SetGraphicsRootDescriptorTable
		(1, TextureManager::GetData(material_.index)->gpuHandle);

	// �`��R�}���h
	DirectXBase::GetInstance()->commandList_->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void OBJModel::LoadFromOBJInternal(const std::string& modelName)
{
	ifstream file;

	// OBJ����̃f�[�^�ǂݍ���
	const string fileName = modelName + ".obj";
	const string directoryPath = "Resources/Models/OBJModel/" + modelName + "/";

	file.open(directoryPath + fileName); //"Resources/Models/ICO/ICO.obj"

	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) { assert(0); }

	vector<Vector3> positions; // ���_���W
	vector<Vector3> normals; // �@���x�N�g��
	vector<Vector2> texcoords; // �e�N�X�`��UV

	string line;

	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		// �擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			// �}�e���A���̃t�@�C�����ǂݍ���
			std::string filename;
			line_stream >> filename;

			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		// �擪������v�Ȃ璸�_���W
		if (key == "v") 
		{
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			position.z *= -1.0f;

			// ���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}

		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") 
		{
			// U,V�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			// V�������]
			texcoord.y = 1.0f - texcoord.y;

			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		// �擪������VN�Ȃ�@���x�N�g��
		if (key == "vn") 
		{
			// X,Y,Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normal.z *= -1.0f;

			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f") 
		{
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			int32_t triangleNum = 0;
			VertexPosNormalUv triangle[3] = {};

			while (getline(line_stream, index_string, ' ')) 
			{
				// ���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);// �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);// �X���b�V�����΂�
				index_stream >> indexNormal;

				// ���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				// �t���œ���邽�߂ɃR�s�[
				triangle[triangleNum] = vertex;
				triangleNum++;

				if (triangleNum == 3) 
				{
					triangleNum = 0;
					vertices_.emplace_back(triangle[2]);
					vertices_.emplace_back(triangle[1]);
					vertices_.emplace_back(triangle[0]);
				}

				// �C���f�b�N�X�f�[�^�̒ǉ�
				indices_.emplace_back((unsigned short)indices_.size());
			}
		}
	}
	// �t�@�C�������
	file.close();
}