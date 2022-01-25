#pragma once
#include "MWDBind.h"
#include "MWDDataBuffer.h"
#include "MWDVBO.h"
#include "MWDVertexFormat.h"
namespace MWDEngine {
	class MWDVertexBuffer:public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDVertexBuffer)
		DECLARE_RTTI(MWDVertexBuffer, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexBuffer)
	protected:
		MWDArray<MWDDataBuffer*> m_pData[MWDVertexFormat::VF_MAX];    //ÿһ��Arr�����ͨ����һ�ֶ�������
		MWDVertexFormat* m_pVertexFormat;								//ά����ǰVertexBuffer�Ķ����������(AttributePointer)
		unsigned int m_uiOneVertexSize;									//ά��ÿһ�������Byte����
		unsigned int m_uiVertexNum;										//�������
		void* vbo_data;													//������ɺ��vbo����
	public:
		MWDVertexBuffer(unsigned int target = GL_ARRAY_BUFFER, unsigned int usage = GL_STATIC_DRAW) {
			int num = GetSwapChainNum();
			for (int i = 0; i < num; ++i) {
				MWDVBO* vbo = new MWDVBO(target, usage);
				AddResource(vbo);
			}
		}
		~MWDVertexBuffer() {
			MWDMAC_DELETE(vbo_data)
			m_pVertexFormat = NULL;
			for (int i = 0; i < MWDVertexFormat::VF_MAX; ++i) {
				m_pData[i].Destroy();
			}
		}
		//��ȡһ��������ֽڳ���
		unsigned int GetOneVertexSize()const {
			return m_uiOneVertexSize;
		};
		//��ȡ������Ŀ
		unsigned int GetNum()const {
			return m_uiVertexNum;
		};
		//��ȡBuffer��Byte����
		unsigned int GetByteSize(unsigned int Attribute,unsigned int level = 0)const {
			return m_uiVertexNum * m_uiOneVertexSize;
		};
		//��ȡ�洢��������(int ,float.........)
		unsigned int GetDataType(unsigned int Attribute)const {
			return  (*m_pData[Attribute][0]).GetDataType();
		};
		//����textureCoord֧�ֶ�ͨ�����������ǵ�ͨ��
		void* GetData(unsigned int Attribute, unsigned int channel = 0)const {
			if (Attribute >= MWDVertexFormat::VF_MAX)
				return NULL;
			if (channel >= (unsigned int)m_pData[Attribute].GetNum())
				return NULL;
			return m_pData[Attribute][channel];
		};
		//�������ύ��Renderer
		void LoadResource(MWDRenderer* pRenderer) {
			
		};
		bool SetData(MWDDataBuffer* pData, unsigned int Attribute) {
			if (!pData || m_pVertexFormat || Attribute >= MWDVertexFormat::VF_MAX)
				return 0;

			if (!pData->GetData())
				return 0;

			//���꣨һ�ף�
			if (Attribute == MWDVertexFormat::VF_POSITION)
			{
				if (!m_pData[Attribute].GetNum())
					(m_pData[Attribute]).AddElement(pData);
				else
					return 0;
			}
			//���ߣ�һ�ף�
			else if (Attribute == MWDVertexFormat::VF_NORMAL)
			{
				m_pData[Attribute].AddElement(pData);
			}
			else if (Attribute == MWDVertexFormat::VF_PSIZE)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//��ɫ��һ�ף�
			else if (Attribute == MWDVertexFormat::VF_COLOR)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//���Ȩ��(һ��)
			else if (Attribute == MWDVertexFormat::VF_BLENDWEIGHT)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//ͨ�����ԣ����ڹ�����Ƥ������һ�ף�
			else if (Attribute == MWDVertexFormat::VF_BLENDINDICES)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//�����ߣ�һ�ף�
			else if (Attribute == MWDVertexFormat::VF_BINORMAL)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//���ߣ�һ�ף�
			else if (Attribute == MWDVertexFormat::VF_TANGENT)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//��Ч��һ�ף�
			else if (Attribute == MWDVertexFormat::VF_FOG)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//��ȣ�һ�ף�
			else if (Attribute == MWDVertexFormat::VF_DEPTH)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//����UV���ܶ��ף�
			else if (Attribute == MWDVertexFormat::VF_TEXCOORD)
			{
				m_pData[Attribute].AddElement(pData);
			}
			else
				return 0;
			//Mesh�����Ķ����������DataBuffer�ĳ���
			if (!GetNum())
				m_uiVertexNum = pData->GetNum();
			else
			{
				if (m_uiVertexNum != pData->GetNum())
					return 0;
			}
			m_uiOneVertexSize += pData->GetStride();
			return 1;

		};
	
		unsigned int GetSemanticsNum(unsigned int uiSemantics)const {
			if (!m_pVertexFormat)
			{
				return m_pData[uiSemantics].GetNum();
			}
			else
			{
				unsigned int uiNum = 0;
				for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.GetNum(); i++)
				{
					if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
					{
						uiNum++;
					}
				}
				return uiNum;
			}
		};

		//��ȡuiVF���͵Ķ��������м���
		FORCEINLINE unsigned int GetLevel(unsigned int uiVF)const {
			if (uiVF >= MWDVertexFormat::VF_MAX)
				return 0;
			else
				return GetSemanticsNum(uiVF);
		};
		FORCEINLINE MWDDataBuffer* GetPositionData()const {
			if (m_pData[MWDVertexFormat::VF_PSIZE].GetNum())
				return m_pData[MWDVertexFormat::VF_PSIZE][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetNormalData()const {
			if ((unsigned int)m_pData[MWDVertexFormat::VF_NORMAL].GetNum())
				return m_pData[MWDVertexFormat::VF_NORMAL][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetPSizeData()const {
			if (m_pData[MWDVertexFormat::VF_PSIZE].GetNum())
				return m_pData[MWDVertexFormat::VF_PSIZE][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetColorData()const {
			if ((unsigned int)m_pData[MWDVertexFormat::VF_COLOR].GetNum())
				return m_pData[MWDVertexFormat::VF_COLOR][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetBlendWeightData()const {
			if (m_pData[MWDVertexFormat::VF_BLENDWEIGHT].GetNum())
				return m_pData[MWDVertexFormat::VF_BLENDWEIGHT][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetBlendIndicesData()const {
			if (m_pData[MWDVertexFormat::VF_BLENDINDICES].GetNum())
				return m_pData[MWDVertexFormat::VF_BLENDINDICES][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetTangentData()const {
			if (m_pData[MWDVertexFormat::VF_TANGENT].GetNum())
				return m_pData[MWDVertexFormat::VF_TANGENT][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetBinormalData()const {
			if (m_pData[MWDVertexFormat::VF_BINORMAL].GetNum())
				return m_pData[MWDVertexFormat::VF_BINORMAL][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetFogData()const {
			if (m_pData[MWDVertexFormat::VF_FOG].GetNum())
				return m_pData[MWDVertexFormat::VF_FOG][0];
			else
				return NULL;
		};
		FORCEINLINE MWDDataBuffer* GetDepthData()const {
			if (m_pData[MWDVertexFormat::VF_DEPTH].GetNum())
				return m_pData[MWDVertexFormat::VF_DEPTH][0];
			else
				return NULL;
		};
		//��ȡ��uiLevel��UV����
		FORCEINLINE MWDDataBuffer* GetTexCoordData(unsigned int uiLevel)const {
			if (uiLevel < (unsigned int)m_pData[MWDVertexFormat::VF_TEXCOORD].GetNum())
				return m_pData[MWDVertexFormat::VF_TEXCOORD][uiLevel];
			else
				return NULL;
		};

		FORCEINLINE unsigned int GetPositionLevel()const {
			return GetSemanticsNum(MWDVertexFormat::VF_POSITION);
		};
		FORCEINLINE unsigned int GetColorLevel()const {
			return GetSemanticsNum(MWDVertexFormat::VF_COLOR);
		};
		FORCEINLINE unsigned int GetNormalLevel()const {
			return GetSemanticsNum(MWDVertexFormat::VF_NORMAL);
		};
		FORCEINLINE unsigned int GetTexCoordLevel()const {
			return GetSemanticsNum(MWDVertexFormat::VF_TEXCOORD);
		};
		FORCEINLINE unsigned int GetVertexNum()const {
			return m_uiVertexNum;
		};

		FORCEINLINE bool HavePositionInfo(unsigned int uiLevel)const {
			return GetSemanticsNum(MWDVertexFormat::VF_POSITION) > uiLevel;
		};
		FORCEINLINE bool HaveNormalInfo(unsigned int uiLevel)const {
			return GetSemanticsNum(MWDVertexFormat::VF_NORMAL) > uiLevel;
		};
		FORCEINLINE bool HavePSizeInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_PSIZE) > 0;
		};
		FORCEINLINE bool HaveColorInfo(unsigned int uiLevel)const {
			return GetSemanticsNum(MWDVertexFormat::VF_COLOR) > uiLevel;
		};
		FORCEINLINE bool HaveBlendWeightInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_BLENDWEIGHT) > 0;
		};
		FORCEINLINE bool HaveBlendIndicesInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_BLENDINDICES) > 0;
		};
		FORCEINLINE bool HaveTangentInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_TANGENT) > 0;
		};
		FORCEINLINE bool HaveBinormalInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_BINORMAL) > 0;
		};
		FORCEINLINE bool HaveFogInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_FOG) > 0;
		};
		FORCEINLINE bool HaveDepthInfo()const {
			return GetSemanticsNum(MWDVertexFormat::VF_DEPTH) > 0;
		};
		FORCEINLINE bool HaveTexCoordInfo(unsigned int uiLevel)const {
			return GetSemanticsNum(MWDVertexFormat::VF_TEXCOORD) > uiLevel;
		};

		unsigned int GetSemanticsChannel(unsigned int uiSemantics, unsigned int uiLevel)const {
			if (!m_pVertexFormat)
			{
				if (uiLevel >= m_pData[uiSemantics].GetNum())
				{
					return 0;
				}
				return m_pData[uiSemantics][uiLevel]->GetChannel();
			}
			else
			{
				unsigned int uiNum = 0;
				for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.GetNum(); i++)
				{
					if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
					{
						if (uiLevel == uiNum)
						{
							return MWDDataBuffer::ms_DataTypeChannel[m_pVertexFormat->m_FormatArray[i].DataType];
						}
						uiNum++;
					}
				}
				return 0;
			}

		};
		unsigned int PositionChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_POSITION, uiLevel);
		};
		unsigned int NormalChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_NORMAL, uiLevel);
		};
		unsigned int PSizeChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_PSIZE, 0);
		};
		unsigned int ColorChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_COLOR, uiLevel);
		};
		unsigned int BlendWeightChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_BLENDWEIGHT, 0);
		};
		unsigned int BlendIndicesChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_BLENDINDICES, 0);
		};
		unsigned int TangentChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_TANGENT, 0);
		};
		unsigned int BinormalChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_BINORMAL, 0);
		};
		unsigned int FogChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_FOG, 0);
		};
		unsigned int DepthChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_DEPTH, 0);
		};
		unsigned int TexCoordChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_TEXCOORD, uiLevel);
		};

		unsigned int GetSemanticsDataType(unsigned int uiSemantics, unsigned int uiLevel)const {
			if (!m_pVertexFormat)
			{
				if (uiLevel >= m_pData[uiSemantics].GetNum())
				{
					return 0;
				}
				return m_pData[uiSemantics][uiLevel]->GetDataType();
			}
			else
			{
				unsigned int uiNum = 0;
				for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.GetNum(); i++)
				{
					if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
					{
						if (uiLevel == uiNum)
						{
							return m_pVertexFormat->m_FormatArray[i].DataType;
						}
						uiNum++;
					}
				}
				return 0;
			}

		};

		unsigned int NormalDataType(unsigned int uiLevel)const {
			return GetSemanticsDataType(MWDVertexFormat::VF_NORMAL, uiLevel);
		};
		unsigned int TangentDataType()const {
			return GetSemanticsDataType(MWDVertexFormat::VF_TANGENT, 0);
		};
		unsigned int BinormalDataType()const {
			return GetSemanticsDataType(MWDVertexFormat::VF_BINORMAL, 0);
		};

		//����������֮���Զ�������m_pVertexFormat
		MWDVertexFormat* GetVertexFormat() {
			return m_pVertexFormat;
		};
	private:
		//�����ع��ڴ�����Ϊ�Դ��ʽ
		char* GenerateVboData() {
			if (vbo_data) {
				return (char*)vbo_data;
			}
			if (!GetNum()) {
				return NULL;
			}
			if (!m_pVertexFormat->m_FormatArray.GetNum()) {
				return NULL;
			}
			TCHAR* vbo = new TCHAR[(int)m_uiOneVertexSize * (int)GetNum()];

			unsigned int offset, DataType, Semantics, SemanticsIndex;
			int semantic_num = m_pVertexFormat->m_FormatArray.GetNum();
			MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> format_arr = m_pVertexFormat->m_FormatArray;

			for (int i = 0; i < GetNum(); i++) {
				TCHAR* one_vertex_data = NULL;
				for (int j = 0; j < semantic_num; j++) {
					MWDDataBuffer* temp = m_pData[Semantics][SemanticsIndex];
					MWDStrcat(one_vertex_data, temp->GetStride(), MWDString::CharToTCHAR((char*)(temp->GetOneVertexData(i))));
				}
				MWDStrcat(vbo, m_uiOneVertexSize, one_vertex_data);
			}
			vbo_data = MWDString::TCHARToChar(vbo);
			return (char*)vbo_data;

		};
		//���ڴ������ύ���Դ�
		void LoadData() {
			((MWDVBO*)GetResource())->SetData(vbo_data, GetNum() * m_uiOneVertexSize);//���vbo����
			SwitchResource();
			int attribute_num = m_pVertexFormat->m_FormatArray.GetNum();
			unsigned int datatype, offset, semantics, semantics_index;
			for (int i = 0; i < attribute_num; ++i) {
				datatype = m_pVertexFormat->m_FormatArray[i].DataType;
				offset = m_pVertexFormat->m_FormatArray[i].OffSet;
				semantics = m_pVertexFormat->m_FormatArray[i].Semantics;
				semantics_index = m_pVertexFormat->m_FormatArray[i].SemanticsIndex;
				((MWDVBO*)GetResource())->SetAttributePointer(semantics, MWDDataBuffer::ms_DataTypeByte[datatype], GL_UNSIGNED_INT, false, m_uiOneVertexSize, offset);//����vbo������ʽ
			}

		};
		//���ڴ������ύ���Դ棬�����õ���ClearInfo            
		void LoadDataToVBO() {
			GetVertexFormat();
			GenerateVboData();
			LoadData();
		};
		//���ֻ�����ڴ棬�Ͱ��Դ�vboɾ�������ֻ�����Դ棬�Ͱ��ڴ������ͷŵ�     
		void ClearInfo() {};      
	};
	DECLARE_Ptr(MWDVertexBuffer);
	MWDTYPE_MARCO(MWDVertexBuffer);
}


