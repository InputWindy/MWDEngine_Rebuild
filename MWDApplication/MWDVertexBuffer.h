#pragma once
#include "MWDBind.h"
#include "MWDDataBuffer.h"
#include "MWDVBO.h"
namespace MWDEngine {
	class MWDVertexBuffer:public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDVertexBuffer)
		DECLARE_RTTI(MWDVertexBuffer, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexBuffer)
	public:
		friend class MWDModel;
		enum Semantics {
			POSITION,
			NORMAL,
			BINORMAL,
			TANGENT,
			BLEND_WEIGHT,
			BLEND_INDICES,
			UV
		};
	protected:
		MWDDataBuffer* m_position;									//����
		MWDDataBuffer* m_normal;									//����
		MWDDataBuffer* m_binormal;									//������
		MWDDataBuffer* m_tangent;									//����
		MWDDataBuffer* m_blend_weight;								//�����������Ȩ��
		MWDDataBuffer* m_blendindices;								//������������

		MWDArray<MWDDataBuffer*> m_uv;								//�������꼯��
		
		void* vbo_data;													//������ɺ��vbo����
	public:
		MWDVertexBuffer(unsigned int target = GL_ARRAY_BUFFER, unsigned int usage = GL_STATIC_DRAW) {
			int num = GetSwapChainNum();
			for (int i = 0; i < num; ++i) {
				MWDVBO* vbo = new MWDVBO(target, usage);
				AddResource(vbo);
			}
			m_position = new MWDDataBuffer(MWDDataBuffer::DataType_FLOAT32_3);//����12��һ��floatΪ4�����3��float����
			m_normal = new MWDDataBuffer(MWDDataBuffer::DataType_FLOAT32_3);//����12��һ��floatΪ4�����3��float����
			m_binormal = new MWDDataBuffer(MWDDataBuffer::DataType_FLOAT32_3);//����12��һ��floatΪ4�����3��float����
			m_tangent = new MWDDataBuffer(MWDDataBuffer::DataType_FLOAT32_3);//����12��һ��floatΪ4�����3��float����
			m_blend_weight = new MWDDataBuffer(MWDDataBuffer::DataType_UINT);//����4��һ��uintΪ4
			m_blendindices = new MWDDataBuffer(MWDDataBuffer::DataType_UINT);//����4��һ��uintΪ4
			for (int i = 0; i < 8; ++i) {
				MWDDataBuffer* uv = new MWDDataBuffer(MWDDataBuffer::DataType_FLOAT32_2);
				m_uv.AddElement(uv);
			}
		}
		~MWDVertexBuffer() {
			MWDMAC_DELETEA(vbo_data)
			MWDMAC_DELETE(m_position)
			MWDMAC_DELETE(m_normal)
			MWDMAC_DELETE(m_binormal)
			MWDMAC_DELETE(m_tangent)
			MWDMAC_DELETE(m_blend_weight)
			MWDMAC_DELETE(m_blendindices)
			m_uv.Destroy();
		}
		//��ȡһ��������ֽڳ���
		unsigned int GetOneVertexSize()const {
			int cnt = 0;
			if (m_position->GetData()) {
				cnt += m_position->GetStride();
			}
			if (m_normal->GetData()) {
				cnt += m_normal->GetStride();
			}
			if (m_binormal->GetData()) {
				cnt += m_binormal->GetStride();
			}
			if (m_tangent->GetData()) {
				cnt += m_tangent->GetStride();
			}
			if (m_blend_weight->GetData()) {
				cnt += m_blend_weight->GetStride();
			}
			if (m_blendindices->GetData()) {
				cnt += m_blendindices->GetStride();
			}
			int channel = m_uv.GetNum();
			for (int i = 0; i < channel; ++i) {
				if (m_uv[i]->GetData()) {
					cnt += m_uv[i]->GetStride();
				}
			}
			return cnt;
		};
		//��ȡ������Ŀ
		unsigned int GetNum() {
			return GetPositionData()->GetNum();
		};
		//��ȡ�洢��������(int ,float.........)
		unsigned int GetDataType(Semantics Attribute,unsigned int uv_channel=0)const {
			switch (Attribute)
			{
			case MWDEngine::MWDVertexBuffer::POSITION:
				return m_position->GetDataType(); break;
			case MWDEngine::MWDVertexBuffer::NORMAL:
				return m_normal->GetDataType(); break;
			case MWDEngine::MWDVertexBuffer::BINORMAL:
				return m_binormal->GetDataType(); break;
			case MWDEngine::MWDVertexBuffer::TANGENT:
				return m_tangent->GetDataType(); break;
			case MWDEngine::MWDVertexBuffer::BLEND_WEIGHT:
				return m_blend_weight->GetDataType(); break;
			case MWDEngine::MWDVertexBuffer::BLEND_INDICES:
				return m_blendindices->GetDataType(); break;
			case MWDEngine::MWDVertexBuffer::UV:
				return m_uv[uv_channel]->GetDataType(); break;
			default:
				break;
			}
			return 0; 
		};

		//����textureCoord֧�ֶ�ͨ�����������ǵ�ͨ��
		void* GetData(Semantics Attribute, unsigned int uv_channel = 0) {
			switch (Attribute)
			{
			case MWDEngine::MWDVertexBuffer::POSITION:
				return m_position->GetData(); break;
			case MWDEngine::MWDVertexBuffer::NORMAL:
				return m_normal->GetData(); break;
			case MWDEngine::MWDVertexBuffer::BINORMAL:
				return m_binormal->GetData(); break;
			case MWDEngine::MWDVertexBuffer::TANGENT:
				return m_tangent->GetData(); break;
			case MWDEngine::MWDVertexBuffer::BLEND_WEIGHT:
				return m_blend_weight->GetData(); break;
			case MWDEngine::MWDVertexBuffer::BLEND_INDICES:
				return m_blendindices->GetData(); break;
			case MWDEngine::MWDVertexBuffer::UV:
				return m_uv[uv_channel]->GetData(); break;
			default:
				break;
			}
			return NULL;
		};
		
		FORCEINLINE MWDDataBuffer* GetPositionData() {
			return m_position;
		};
		FORCEINLINE MWDDataBuffer* GetNormalData() {
			return m_normal;
		};
		FORCEINLINE MWDDataBuffer* GetBlendWeightData() {
			return m_blend_weight;
		};
		FORCEINLINE MWDDataBuffer* GetBlendIndicesData() {
			return m_blendindices;
		};
		FORCEINLINE MWDDataBuffer* GetTangentData() {
			return m_tangent;
		};
		FORCEINLINE MWDDataBuffer* GetBinormalData() {
			return m_binormal;
		};
		//��ȡ��uiLevel��UV����
		FORCEINLINE MWDDataBuffer* GetTexCoordData(unsigned int channel)const {
			return m_uv[channel];
		};

		void LoadDataToVBO() {
			GenerateVboData();
			LoadData();
		};
		
	private:
		//�����ع��ڴ�����Ϊ�Դ��ʽ
		void* GenerateVboData() {
			if (vbo_data) {
				return vbo_data;
			}
			if (!GetNum()) {
				return NULL;
			}

			void* vbo = new char[(int)GetOneVertexSize() * (int)GetNum()];

			//�������нڵ�
			int offset = 0;
			int len = 0;
			for (int i = 0; i < GetNum(); i++) {
				if (m_position->GetData()) {
					
					void* t_data = m_position->GetOneVertexData(i,len);
					MWDMemcpy((char*)vbo+offset, t_data, len);
					offset += len;
				}
				if (m_normal->GetData()) {
					
					void* t_data = m_normal->GetOneVertexData(i, len);
					MWDMemcpy((char*)vbo + offset, t_data, len);
					offset += len;
				}
				if (m_binormal->GetData()) {
					
					void* t_data = m_binormal->GetOneVertexData(i, len);
					MWDMemcpy((char*)vbo + offset, t_data, len);
					offset += len;
				}
				if (m_tangent->GetData()) {
					
					void* t_data = m_tangent->GetOneVertexData(i, len);
					MWDMemcpy((char*)vbo + offset, t_data, len);
					offset += len;
				}
				if (m_blend_weight->GetData()) {
					
					void* t_data = m_blend_weight->GetOneVertexData(i, len);
					MWDMemcpy((char*)vbo + offset, t_data, len);
					offset += len;
				}
				if (m_blendindices->GetData()) {
					
					void* t_data = m_blendindices->GetOneVertexData(i, len);
					MWDMemcpy((char*)vbo + offset, t_data, len);
					offset += len;
				}
				for (int j = 0; j < 8; ++j) {
					if (m_uv[j]->GetData()) {
						
						void* t_data = m_uv[j]->GetOneVertexData(i, len);
						MWDMemcpy((char*)vbo + offset, t_data, len);
						offset += len;
					}
				}
			}
			vbo_data =vbo ;
			return vbo_data;

		};
		//���ڴ������ύ���Դ�(�ύ���ݣ����ý�����ʽ)
		void LoadData() {
			((MWDVBO*)GetResource())->SetData(vbo_data, GetNum() * GetOneVertexSize());//���vbo����
			MWDVBO* vbo = ((MWDVBO*)GetResource()); 
			int offset = 0;
			vbo->SetAttributePointer(POSITION, m_position->GetChannel(), GL_FLOAT, false, GetOneVertexSize(), offset);//����vbo������ʽ
			offset += m_position->GetStride();
			vbo->SetAttributePointer(NORMAL, m_normal->GetChannel(), GL_FLOAT, true, GetOneVertexSize(), offset);//����vbo������ʽ
			offset += m_normal->GetStride();
			vbo->SetAttributePointer(BINORMAL, m_binormal->GetChannel(), GL_FLOAT, true, GetOneVertexSize(), offset);//����vbo������ʽ
			offset += m_binormal->GetStride();
			vbo->SetAttributePointer(TANGENT, m_tangent->GetChannel(), GL_FLOAT, true, GetOneVertexSize(), offset);//����vbo������ʽ
			offset += m_tangent->GetStride();
			vbo->SetAttributePointer(BLEND_WEIGHT, m_blend_weight->GetChannel(), GL_FLOAT, false, GetOneVertexSize(), offset);//����vbo������ʽ
			offset += m_blend_weight->GetStride();
			vbo->SetAttributePointer(BLEND_INDICES, m_blendindices->GetChannel(), GL_FLOAT, false, GetOneVertexSize(), offset);//����vbo������ʽ
			offset += m_blendindices->GetStride();
			for (int j = 0; j < 8; ++j) {
				if (m_uv[j]->GetData()) {
					vbo->SetAttributePointer(UV+j, m_uv[j]->GetChannel(), GL_FLOAT, false, GetOneVertexSize(), offset);//����vbo������ʽ
					offset += m_uv[j]->GetStride();
				}
			}
			SwitchResource();
		};
		//���ڴ������ύ���Դ棬�����õ���ClearInfo            
		
		//���ֻ�����ڴ棬�Ͱ��Դ�vboɾ�������ֻ�����Դ棬�Ͱ��ڴ������ͷŵ�     
		void ClearInfo() {
			MWDMAC_DELETEA(vbo_data)
			MWDMAC_DELETE(m_position)
			MWDMAC_DELETE(m_normal)
			MWDMAC_DELETE(m_binormal)
			MWDMAC_DELETE(m_tangent)
			MWDMAC_DELETE(m_blend_weight)
			MWDMAC_DELETE(m_blendindices)
			m_uv.Destroy();
		};     
	};
	DECLARE_Ptr(MWDVertexBuffer);
	MWDTYPE_MARCO(MWDVertexBuffer);
}


