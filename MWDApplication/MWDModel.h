#pragma once
#include "MWDTransform.h"
#include "MWDMeshComponent.h"
#include "MWDMeshRenderer.h"
#include "MWDDataBuffer.h"
#include "MWDEntity.h"

#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
namespace MWDEngine {

    //Model：维护各种组件，实例化时需要填写名称和模型文件路径
    //      实例化时需要读取模型文件数据，解析成为MWDDataBuffer，填写进入MeshData。
    //      Model初始化时只有全空的MWDVertexBuffer和MWDIndexBuffer，需要调用SetData()给vbo，ibo填数据，填完会自动上传到显存。
    //      模型导入完成。
	class MWDModel:public MWDEntity
	{
		DECLARE_CLASS_FUNCTION(MWDModel)
		DECLARE_RTTI(MWDModel, MWDEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDModel)
    protected:
        string directory;
    public:
        
        //Model自带Transform和MeshComponent和MeshRenderer
        MWDModel(const TCHAR* model_name = NULL,const TCHAR* file_path = NULL,bool clear = false) {
            SetName(model_name);
            AddComponent(*new MWDTransform(this));
            AddComponent(*new MWDMeshComponent(this));
            AddComponent(*new MWDMeshRenderer(this));
            if (file_path) {
                LoadModelMesh(format_change(file_path),clear);
            }
        }
        ~MWDModel() {

        }
	private:
        string format_change(const TCHAR* STR)
        {
            unsigned int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);   //首先计算TCHAR 长度。
            char* chRtn = new char[iLen * sizeof(char)];  //定义一个 TCHAR 长度大小的 CHAR 类型。
            WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);  //将TCHAR 类型的数据转换为 CHAR 类型。
            std::string str(chRtn); //最后将CHAR 类型数据 转换为 STRING 类型数据。
            return str;
        }
        void LoadModelMesh(string const& path,bool clear)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));
            //传入根节点和scene，递归解析顶点信息，并填充进Mesh
            MWDMeshData* mesh_data = GetComponentByType<MWDMeshComponent>()->GetMeshData();
            processNode(mesh_data,scene->mRootNode, scene);
            mesh_data->GetIndexBuffer()->LoadDataToIBO();
            mesh_data->GetVertexBuffer()->LoadDataToVBO();
            if (clear) {
                mesh_data->GetIndexBuffer()->ClearInfo();
                mesh_data->GetVertexBuffer()->ClearInfo();
            }
            
        }
        void processNode(MWDMeshData* mesh_data,aiNode* node, const aiScene* scene)
        {
            //cout << node->mNumChildren << endl;
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                processMesh(mesh_data,mesh, scene);
            }
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(mesh_data,node->mChildren[i], scene);
            }
        }
        void processMesh(MWDMeshData* mesh_data, aiMesh* mesh, const aiScene* scene) {
            //读取索引信息(读取成功)
            MWDDataBuffer* indices = mesh_data->GetIndexBuffer()->GetData();
            if (!indices) {
                indices = new MWDDataBuffer(MWDDataBuffer::DataType_UINT);
            }
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices->AddData(&face.mIndices[j],1,MWDDataBuffer::DataType_UINT);
                }
            }
            if (!mesh_data->GetIndexBuffer()->SetData(indices)) {
                cout << "indices解析失败！" << endl;
            }

            //读取位置信息,法线信息，UV信息
            MWDVertexBuffer* vertex_buf = mesh_data->GetVertexBuffer();
            
            MWDDataBuffer* position = vertex_buf->GetPositionData();
            MWDDataBuffer* normal = vertex_buf->GetNormalData();
            MWDDataBuffer* binormal = vertex_buf->GetBinormalData();
            MWDDataBuffer* tangent = vertex_buf->GetTangentData();
            
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                float pos[3] ;
                float nor[3] ;
                float texcoord[2] ;
                float tan[3] ;
                float binor[3] ;
                // positions
                pos[0] = mesh->mVertices[i].x;
                pos[1] = mesh->mVertices[i].y;
                pos[2] = mesh->mVertices[i].z;
                position->AddData(pos,1,MWDDataBuffer::DataType_FLOAT32_3);
                // normals
                if (mesh->HasNormals())
                {
                    nor[0] = mesh->mNormals[i].x;
                    nor[1] = mesh->mNormals[i].y;
                    nor[2] = mesh->mNormals[i].z;
                    normal->AddData(nor, 1, MWDDataBuffer::DataType_FLOAT32_3);
                }
                
                // texture coordinates
                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    int channel = mesh->GetNumUVChannels();
                    //解析第j套uv坐标
                    for (int j = 0; j < channel; ++j) {
                        texcoord[0] = mesh->mTextureCoords[j][i].x;
                        texcoord[1] = mesh->mTextureCoords[j][i].y;
                        mesh_data->GetVertexBuffer()->GetTexCoordData(j)->AddData(texcoord,1, MWDDataBuffer::DataType_FLOAT32_2);
                    }
                    
                    // tangent
                    tan[0] = mesh->mTangents[i].x;
                    tan[1] = mesh->mTangents[i].y;
                    tan[2] = mesh->mTangents[i].z;
                    tangent->AddData(tan, 1, MWDDataBuffer::DataType_FLOAT32_3);

                    // bitangent
                    binor[0] = mesh->mBitangents[i].x;
                    binor[1] = mesh->mBitangents[i].y;
                    binor[2] = mesh->mBitangents[i].z;
                    binormal->AddData(binor, 1, MWDDataBuffer::DataType_FLOAT32_3);
                }
                else {
                    //cout << "没有纹理坐标" << endl;
                }
            }
        }

        //texture由Material负责加载
        //Mesh processMesh()
        //{
        //    // process materials
        //    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        //    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        //    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        //    // Same applies to other texture as the following list summarizes:
        //    // diffuse: texture_diffuseN
        //    // specular: texture_specularN
        //    // normal: texture_normalN
        //    // 1. diffuse maps
        //    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        //    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //    // 2. specular maps
        //    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        //    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //    // 3. normal maps
        //    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        //    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        //    // 4. height maps
        //    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        //    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        //    // return a mesh object created from the extracted mesh data
        //    return Mesh(vertices, indices, textures);
        //}
        //vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
        //{
        //    vector<Texture> textures;
        //    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        //    {
        //        aiString str;
        //        mat->GetTexture(type, i, &str);
        //        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        //        bool skip = false;
        //        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        //        {
        //            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
        //            {
        //                textures.push_back(textures_loaded[j]);
        //                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
        //                break;
        //            }
        //        }
        //        if (!skip)
        //        {   // if texture hasn't been loaded already, load it
        //            Texture texture;
        //            texture.id = TextureFromFile(str.C_Str(), this->directory);
        //            texture.type = typeName;
        //            texture.path = str.C_Str();
        //            textures.push_back(texture);
        //            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        //        }
        //    }
        //    return textures;
        //}
	};
}

