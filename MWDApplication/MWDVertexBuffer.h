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
		MWDArray<MWDDataBuffer*> m_pData[MWDVertexFormat::VF_MAX];    //每一个Arr保存多通道的一种顶点属性
		MWDVertexFormat* m_pVertexFormat;								//维护当前VertexBuffer的顶点解析方案(AttributePointer)
		unsigned int m_uiOneVertexSize;									//维护每一个顶点的Byte长度
		unsigned int m_uiVertexNum;										//顶点个数
		void* vbo_data;													//解析完成后的vbo数据
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
		//获取一个顶点的字节长度
		unsigned int GetOneVertexSize()const {
			return m_uiOneVertexSize;
		};
		//获取顶点数目
		unsigned int GetNum()const {
			return m_uiVertexNum;
		};
		//获取Buffer总Byte长度
		unsigned int GetByteSize(unsigned int Attribute,unsigned int level = 0)const {
			return m_uiVertexNum * m_uiOneVertexSize;
		};
		//获取存储数据类型(int ,float.........)
		unsigned int GetDataType(unsigned int Attribute)const {
			return  (*m_pData[Attribute][0]).GetDataType();
		};
		//除了textureCoord支持多通道，其他都是单通道
		void* GetData(unsigned int Attribute, unsigned int channel = 0)const {
			if (Attribute >= MWDVertexFormat::VF_MAX)
				return NULL;
			if (channel >= (unsigned int)m_pData[Attribute].GetNum())
				return NULL;
			return m_pData[Attribute][channel];
		};
		//把数据提交给Renderer
		void LoadResource(MWDRenderer* pRenderer) {
			
		};
		bool SetData(MWDDataBuffer* pData, unsigned int Attribute) {
			if (!pData || m_pVertexFormat || Attribute >= MWDVertexFormat::VF_MAX)
				return 0;

			if (!pData->GetData())
				return 0;

			//坐标（一套）
			if (Attribute == MWDVertexFormat::VF_POSITION)
			{
				if (!m_pData[Attribute].GetNum())
					(m_pData[Attribute]).AddElement(pData);
				else
					return 0;
			}
			//法线（一套）
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
			//颜色（一套）
			else if (Attribute == MWDVertexFormat::VF_COLOR)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//混合权重(一套)
			else if (Attribute == MWDVertexFormat::VF_BLENDWEIGHT)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//通用属性，用于骨骼蒙皮动画（一套）
			else if (Attribute == MWDVertexFormat::VF_BLENDINDICES)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//副法线（一套）
			else if (Attribute == MWDVertexFormat::VF_BINORMAL)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//切线（一套）
			else if (Attribute == MWDVertexFormat::VF_TANGENT)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//雾效（一套）
			else if (Attribute == MWDVertexFormat::VF_FOG)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//深度（一套）
			else if (Attribute == MWDVertexFormat::VF_DEPTH)
			{
				if (!m_pData[Attribute].GetNum())
					m_pData[Attribute].AddElement(pData);
				else
					return 0;
			}
			//纹理UV（很多套）
			else if (Attribute == MWDVertexFormat::VF_TEXCOORD)
			{
				m_pData[Attribute].AddElement(pData);
			}
			else
				return 0;
			//Mesh包含的顶点个数就是DataBuffer的长度
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

		//获取uiVF类型的顶点属性有几套
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
		//获取第uiLevel套UV坐标
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

		//设置完数据之后，自动解析出m_pVertexFormat
		MWDVertexFormat* GetVertexFormat() {
			return m_pVertexFormat;
		};
	private:
		//解析重构内存数据为显存格式
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
		//把内存数据提交到显存
		void LoadData() {
			((MWDVBO*)GetResource())->SetData(vbo_data, GetNum() * m_uiOneVertexSize);//填充vbo数据
			SwitchResource();
			int attribute_num = m_pVertexFormat->m_FormatArray.GetNum();
			unsigned int datatype, offset, semantics, semantics_index;
			for (int i = 0; i < attribute_num; ++i) {
				datatype = m_pVertexFormat->m_FormatArray[i].DataType;
				offset = m_pVertexFormat->m_FormatArray[i].OffSet;
				semantics = m_pVertexFormat->m_FormatArray[i].Semantics;
				semantics_index = m_pVertexFormat->m_FormatArray[i].SemanticsIndex;
				((MWDVBO*)GetResource())->SetAttributePointer(semantics, MWDDataBuffer::ms_DataTypeByte[datatype], GL_UNSIGNED_INT, false, m_uiOneVertexSize, offset);//设置vbo解析格式
			}

		};
		//将内存数据提交到显存，看设置调用ClearInfo            
		void LoadDataToVBO() {
			GetVertexFormat();
			GenerateVboData();
			LoadData();
		};
		//如果只存在内存，就把显存vbo删掉；如果只存在显存，就把内存数据释放掉     
		void ClearInfo() {};      
	};
	DECLARE_Ptr(MWDVertexBuffer);
	MWDTYPE_MARCO(MWDVertexBuffer);
}


