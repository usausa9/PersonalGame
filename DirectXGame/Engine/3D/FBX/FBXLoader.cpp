/**
 * @file	FBXLoader.cpp
 * @brief	FBXのローダー
**/

#include "FBXLoader.h"

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const string FBXLoader::sBASE_DIRECTORY_ = "Resources/Models/FBX/";
const string FBXLoader::sDEFAULT_TEXTURE_FILENAME_ = "white1x1.png";

string FBXLoader::ExtractFileName(const string& path)
{
	size_t pos1;
	// 区切り文字 '\\' が出てくる最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	// 区切り文字 '/' が出てくる最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void FBXLoader::Initialize(ID3D12Device* device)
{
	// 再初期化チェック
	assert(fbxManager_ == nullptr);

	// 引数からメンバ変数に代入
	this->device_ = device;

	// FBXマネージャの生成
	fbxManager_ = FbxManager::Create();

	// FBXマネージャの入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager_, IOSROOT);
	fbxManager_->SetIOSettings(ios);

	// FBXインポータの生成
	fbxImporter_ = FbxImporter::Create(fbxManager_, "");
}

void FBXLoader::Finalize()
{
	// 各種FBXインスタンスの破棄
	fbxImporter_->Destroy();
	fbxManager_->Destroy();
}

FBXModel* FBXLoader::LoadModelFromFile(const string& modelName)
{
	// モデルと同じ名前のフォルダから読み込む
	const string directoryPath = sBASE_DIRECTORY_ + modelName + "/";

	// 拡張子.fbxを付加
	const string fileName = modelName + ".fbx";

	// 連結してフルパスを得る
	const string fullpath = directoryPath + fileName;

	// ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter_->Initialize(fullpath.c_str(), -1, fbxManager_->GetIOSettings()))
	{
		assert(0);
	}

	// シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager_, "fbxScene");

	// ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter_->Import(fbxScene);

	// モデル生成
	FBXModel* model = new FBXModel();
	model->name_ = modelName;

	// FBXノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();
	// あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
	model->nodes_.reserve(nodeCount);

	// ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(model, fbxScene->GetRootNode());

	// FBXシーン解放
	model->fbxScene_ = fbxScene;

	// バッファ生成
	model->CreateBuffers(device_);

	return model;
}

void FBXLoader::ParseNodeRecursive(FBXModel* model, FbxNode* fbxNode, Node* parent)
{
	// ノード名を取得
	string name = fbxNode->GetName();

	// モデルにノードを追加 
	model->nodes_.emplace_back();
	Node& node = model->nodes_.back();
	// ノード名を取得
	node.name_ = fbxNode->GetName();

	// FBXノードのローカル移動情報
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	// 形式変換して代入
	node.rotation_ = { (float)rotation[0], (float)rotation[1],  (float)rotation[2], 0.0f };
	node.scaling_ = { (float)scaling[0], (float)scaling[1],  (float)scaling[2], 0.0f };
	node.translation_ = { (float)translation[0], (float)translation[1],  (float)translation[2], 1.0f };

	// 回転角を Degree(度) から rad(ラジアン) に 変換
	node.rotation_.x = UsaMath::DegreesToRadians(node.rotation_.x);
	node.rotation_.y = UsaMath::DegreesToRadians(node.rotation_.y);
	node.rotation_.z = UsaMath::DegreesToRadians(node.rotation_.z);

	// スケール、回転、平行移動行列の計算用Matの宣言
	Matrix4 matScaling, matRotation, matTranslation;
	Matrix4 rotX, rotY, rotZ;

	// スケール、回転、平行移動行列の計算
	// 行列の計算 : スケール 
	matScaling = matScaling.Scale({ node.scaling_.x, node.scaling_.y, node.scaling_.z });
	// 行列の計算 : 回転
	rotX = rotX.RotateX(node.rotation_.x);
	rotY = rotY.RotateY(node.rotation_.y);
	rotZ = rotZ.RotateZ(node.rotation_.z);
	matRotation = Matrix4::Identity();
	matRotation *= rotZ;
	matRotation *= rotX;
	matRotation *= rotY;
	// 行列の計算 : 平行移動
	matTranslation = matTranslation.Translate({ node.translation_.x, node.translation_.y, node.translation_.z });

	// ローカル変形行列の計算
	node.transform_ = Matrix4::Identity();
	node.transform_ *= matScaling;
	node.transform_ *= matRotation;
	node.transform_ *= matTranslation;

	// グローバル変形行列の計算
	node.globalTransform_ = node.transform_;
	if (parent)
	{
		node.parent_ = parent;

		// 親の変形を乗算
		node.globalTransform_ *= parent->globalTransform_;
	}

	// FBSノードのメッシュ情報を解析
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			model->meshNode_ = &node;
			ParseMesh(model, fbxNode);
		}
	}

	// 子ノードに対して再帰呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
	}
}

void FBXLoader::ParseMesh(FBXModel* model, FbxNode* fbxNode)
{
	// ノードのメッシュを取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	// 頂点座標読み取り
	ParseMeshVertices(model, fbxMesh);
	// 面を構成するデータの読み取り
	ParseMeshFaces(model, fbxMesh);
	// マテリアルの読み取り
	ParseMaterial(model, fbxNode);
	// スキニング情報の読み取り
	ParseSkin(model, fbxMesh);
}

void FBXLoader::ParseMeshVertices(FBXModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices_;

	// 頂点座標データの数
	const int controlPointsCount = fbxMesh->GetControlPointsCount();

	// 必要数だけ頂点データ配列を確保
	FBXModel::VertexPosNormalUvSkin vert{};
	model->vertices_.resize(controlPointsCount, vert);

	// FBXメッシュの頂点座標配列を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	// FBXメッシュの全頂点座標をモデル内の配列にコピーする
	for (int i = 0; i < controlPointsCount; i++)
	{
		FBXModel::VertexPosNormalUvSkin& vertex = vertices[i];

		// 座標のコピー
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}

void FBXLoader::ParseMeshFaces(FBXModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices_;
	auto& indices = model->indices_;

	// 1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);
	// 面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	// UVデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	// UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	// 面ごとの情報読み取り
	for (int i = 0; i < polygonCount; i++)
	{
		// 面を構成する頂点の数を取得 (3なら三角形ポリゴン)
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		// 1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++)
		{
			// FBX頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			// 頂点法線読み込み
			FBXModel::VertexPosNormalUvSkin& vertex = vertices[index];
			FbxVector4 normal;

			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			// テクスチャUV読み込み
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				// 0番決め打ちで読み込み
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}

			// インデックス配列に頂点インデックス追加
			// 3頂点目までなら
			if (j < 3)
			{
				// 1点追加し、他の2点と三角形を構築する
				indices.push_back(static_cast<unsigned short>((index)));
			}
			// 4頂点目
			else
			{
				// 3点追加し、
				// 4角形の0,1,2,3の内 2,3,0で三角形を構築する
				int index2 = (indices[indices.size() - 1]);
				int index3 = (index);
				int index0 = (indices[indices.size() - 3]);
				indices.push_back(static_cast<unsigned short>(index2));
				indices.push_back(static_cast<unsigned short>(index3));
				indices.push_back(static_cast<unsigned short>(index0));
			}
		}
	}
}

void FBXLoader::ParseMaterial(FBXModel* model, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		// 先頭のマテリアルを取得
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		// テクスチャを読み込んだかどうかを表すフラグ
		bool textureLoaded = false;

		if (material)
		{
			// FbxSurFaceLambertクラスかどうかを調べる
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				// 環境光変数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				model->ambient_.x = (float)ambient.Get()[0];
				model->ambient_.y = (float)ambient.Get()[1];
				model->ambient_.z = (float)ambient.Get()[2];

				// 拡散反射光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				model->diffuse_.x = (float)diffuse.Get()[0];
				model->diffuse_.y = (float)diffuse.Get()[1];
				model->diffuse_.z = (float)diffuse.Get()[2];

				// ディフューズテクスチャを取り出す
				const FbxProperty diffuseProperty =
					material->FindProperty(FbxSurfaceMaterial::sDiffuse);

				if (diffuseProperty.IsValid())
				{
					const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
					if (texture)
					{
						const char* filepath = texture->GetFileName();

						// ファイルパスからファイル名抽出
						string path_str(filepath);
						string name = ExtractFileName(path_str);

						// テクスチャ読み込み
						LoadTexture(model, sBASE_DIRECTORY_ + model->name_ + "/" + name);
						textureLoaded = true;
					}
				}
			}
		}

		// テクスチャがない場合は白テクスチャを貼り付ける
		if (!textureLoaded)
		{
			LoadTexture(model, sBASE_DIRECTORY_ + sDEFAULT_TEXTURE_FILENAME_);
		}
	}
}

void FBXLoader::LoadTexture(FBXModel* model, const string& fullpath)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata& metadata = model->metadata_;
	ScratchImage& scratchImage = model->scratchImg_;

	// ユニコード文字列に変換
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImage);

	if (FAILED(result))
	{
		assert(0);
	}
}

void FBXLoader::ParseSkin(FBXModel* model, FbxMesh* fbxMesh)
{
	// スキニング情報
	FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
	// スキニング情報がなければ終了
	if (fbxSkin == nullptr)
	{
		return;
	}

	// ボーン配列の参照
	std::vector<FBXModel::Bone>& bones = model->bones_;

	// ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	// 全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		// ボーン自体のノードの名前を取得
		const char* boneName = fbxCluster->GetLink()->GetName();

		// 新しくボーンを追加し、追加したボーンの参照を得る
		bones.emplace_back(FBXModel::Bone(boneName));
		FBXModel::Bone& bone = bones.back();
		// 自作ボーンとFBXのボーンを紐づける
		bone.fbxCluster_ = fbxCluster;

		// FBXから初期姿勢行列を取得する
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		// Matrix4に変換する
		Matrix4 initialPose;
		ConvertMatrixFromFBX(&initialPose, fbxMat);

		// 初期姿勢行列の逆行列を得る
		bone.invInitialPose_ = Matrix4::Inverse(initialPose);

		// ボーン番号とスキンウェイトのペア
		struct WeightSet
		{
			UINT index;
			float weight;
		};

		// 二次元配列(ジャグ配列)
		// list:頂点が影響を受けるボーンの全リスト
		// vector:それを前頂点分
		std::vector<std::list<WeightSet>>
			weightLists(model->vertices_.size());

		// すべてのボーンについて
		for (int k = 0; k < clusterCount; k++)
		{
			// FBXボーン情報
			FbxCluster* fbxCluster_ = fbxSkin->GetCluster(k);
			// このボーンに影響を受ける頂点の数
			int controlPointIndicesCount = fbxCluster_->GetControlPointIndicesCount();
			// このボーンに影響を受ける頂点の配列
			int* controlPointIndices = fbxCluster_->GetControlPointIndices();
			double* controlPointWeights = fbxCluster_->GetControlPointWeights();

			// 影響を受ける全頂点について
			for (int j = 0; j < controlPointIndicesCount; j++)
			{
				// 頂点番号
				int vertIndex = controlPointIndices[j];
				// スキンウェイト
				float weight = (float)controlPointWeights[j];
				// その頂点を影響を受けるボーンリストに、ボーンとウェイとのペアを追加
				weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
			}
		}

		// 頂点配列書き換え用の参照
		auto& vertices = model->vertices_;
		// 各頂点について処理
		for (int l = 0; l < vertices.size(); l++)
		{
			// 頂点のウェイトから最も大きい4つを選択
			auto& weightList = weightLists[l];
			// 大小比較用のラムダ式を指定して降順にソート
			weightList.sort(
				[](auto const& lhs, auto const& rhs)
				{
					// 左の要素の方が大きければtrue そうでなければfalseを返す
					return lhs.weight > rhs.weight;
				});

			int weightArrayIndex = 0;
			//降順ソート済みのウェイトリストから
			for (auto& weightSet : weightList)
			{
				// 頂点データに書き込み
				vertices[l].boneIndex[weightArrayIndex] = weightSet.index;
				vertices[l].boneWeight[weightArrayIndex] = weightSet.weight;
				// 4つに達したら終了
				if (++weightArrayIndex >= FBXModel::sMAX_BONE_INDICES_)
				{
					float weight = 0.0f;
					// 2番目以降のウェイトを合計
					for (int j = 1; j < FBXModel::sMAX_BONE_INDICES_; j++)
					{
						weight += vertices[l].boneWeight[l];
					}
					// 合計で1.0f(100%)になるように調整
					vertices[l].boneWeight[0] = 1.0f - weight;
					break;
				}
			}
		}
	}
}

void FBXLoader::ConvertMatrixFromFBX(Matrix4* dst, const FbxAMatrix& src)
{
	// 行
	for (int i = 0; i < 4; i++)
	{
		// 列
		for (int j = 0; j < 4; j++)
		{
			// 1要素コピー
			dst->m[i][j] = (float)src.Get(i, j);
		}
	}
}

FBXLoader* FBXLoader::GetInstance()
{
	static FBXLoader instance;
	return &instance;
}