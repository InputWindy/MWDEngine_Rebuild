#pragma once
#include "MWDDataBuffer.h"
#include "MWDVertexFormat.h"
#include "../OpenGLAPI/MWDVBO.h"
namespace MWDEngine {
	//维护一个Mesh的所有顶点属性，11种顶点属性（其中顶点UV可以有很多套，用于不同阶段）
	//步骤：往MWDVertexBuffer填充各种顶点属性之后，直接GetVertexFormat()自动获取解析格式表，
	//		从该格式表中找到对应属性的解析方式，填充进VBO。
	//		把MWDVertexBuffer横向组合可以得到VBO所需的顶点数据
	//		①Loop(SetData()) ----> LoadDataToVBO			
	//		②Loop(SetData()) ----> GetVertexFormat() ----> GenerateVboData() ----> LoadDataToVBO()
	class  MWDVertexBuffer : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDVertexBuffer)
		DECLARE_RTTI(MWDVertexBuffer, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexBuffer)
	public:
		friend class MWDVertexFormat;
		friend class MWDResourceManager;
		MWDVertexBuffer(bool bIsStatic,MWDVBO::Target target = MWDVBO::Target::ElementArrayBuffer,MWDVBO::Usage usage = MWDVBO::Usage::StaticDraw) {
			m_bIsStatic = bIsStatic;
			m_vbo = new MWDVBO(target, usage);
		};
		MWDVertexBuffer(MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, unsigned int uiNum, MWDVBO::Target target, MWDVBO::Usage usage) {
			MWDMAC_ASSERT(FormatArray.GetNum() && uiNum);
			m_vbo = new MWDVBO(target, usage);
			m_pVertexFormat = NULL;
			m_uiOneVertexSize = 0;
			m_uiVertexNum = uiNum;
			m_pLockData = NULL;
			for (unsigned int i = 0; i < FormatArray.GetNum(); i++)
			{
				m_uiOneVertexSize += MWDDataBuffer::ms_uiDataTypeByte[FormatArray[i].DataType];
			}
			MWDVertexFormat* pVertexFormat = new MWDVertexFormat();
			pVertexFormat->m_FormatArray = FormatArray;
			m_pVertexFormat = pVertexFormat;
		};
		virtual ~MWDVertexBuffer() {
			MWDMAC_DELETE(m_vbo)
			m_vbo = NULL;
			ReleaseResource();
			for (unsigned int i = 0; i < MWDVertexFormat::VF_MAX; i++)
			{
				m_pData[i].Clear();
			}
			m_uiVertexNum = 0;
			m_pVertexFormat = NULL;
			m_uiOneVertexSize = 0;
			m_pLockData = NULL;
		};
		//给一个Mesh填充所有属性数据，一个DataBuffer就是一种顶点属性的全部顶点值
		bool SetData(MWDDataBuffer* pData, unsigned int uiVF) {
			if (!pData || m_pVertexFormat || uiVF >= MWDVertexFormat::VF_MAX)
				return 0;

			if (!pData->GetData())
				return 0;

			//坐标（一套）
			if (uiVF == MWDVertexFormat::VF_POSITION)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//法线（一套）
			else if (uiVF == MWDVertexFormat::VF_NORMAL)
			{
				m_pData[uiVF].AddElement(pData);
			}
			else if (uiVF == MWDVertexFormat::VF_PSIZE)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//颜色（一套）
			else if (uiVF == MWDVertexFormat::VF_COLOR)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//混合权重(一套)
			else if (uiVF == MWDVertexFormat::VF_BLENDWEIGHT)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//通用属性，用于骨骼蒙皮动画（一套）
			else if (uiVF == MWDVertexFormat::VF_BLENDINDICES)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//副法线（一套）
			else if (uiVF == MWDVertexFormat::VF_BINORMAL)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//切线（一套）
			else if (uiVF == MWDVertexFormat::VF_TANGENT)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//雾效（一套）
			else if (uiVF == MWDVertexFormat::VF_FOG)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//深度（一套）
			else if (uiVF == MWDVertexFormat::VF_DEPTH)
			{
				if (!m_pData[uiVF].GetNum())
					m_pData[uiVF].AddElement(pData);
				else
					return 0;
			}
			//纹理UV（很多套）
			else if (uiVF == MWDVertexFormat::VF_TEXCOORD)
			{
				m_pData[uiVF].AddElement(pData);
			}
			else
				return 0;
			//Mesh包含的顶点个数就是DataBuffer的长度
			if (!m_uiVertexNum)
				m_uiVertexNum = pData->GetNum();
			else
			{
				if (m_uiVertexNum != pData->GetNum())
					return 0;
			}
			m_uiOneVertexSize += pData->GetStride();
			return 1;
		};
		//获取uiVF属性的第uiLevel套顶点值
		FORCEINLINE MWDDataBuffer* GetData(unsigned int uiVF, unsigned int uiLevel)const {
			if (uiVF >= MWDVertexFormat::VF_MAX)
				return NULL;
			if (uiLevel >= (unsigned int)m_pData[uiVF].GetNum())
				return NULL;
			return m_pData[uiVF][uiLevel];
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

		//给Mesh填充完数据之后，顶点属性解析格式可以自动生成
		bool GetVertexFormat(MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE>& FormatArray) {
			if (m_pVertexFormat)
			{
				FormatArray = m_pVertexFormat->m_FormatArray;
			}
			else
			{
				MWDDataBuffer* pData;
				MWDVertexFormat::VERTEXFORMAT_TYPE Element;
				unsigned int iVertexSize = 0;
				for (unsigned int i = 0; i < GetPositionLevel(); i++)
				{
					pData = GetPositionData();
					if (pData)
					{
						Element.OffSet = (WORD)iVertexSize;
						iVertexSize += pData->GetStride();
						Element.DataType = pData->GetDataType();
						Element.Semantics = MWDVertexFormat::VF_POSITION;
						Element.SemanticsIndex = i;
						FormatArray.AddElement(Element);
					}
				}

				for (unsigned int i = 0; i < GetTexCoordLevel(); i++)
				{
					pData = GetTexCoordData(i);
					if (pData)
					{
						Element.OffSet = (WORD)iVertexSize;
						iVertexSize += pData->GetStride();
						Element.DataType = pData->GetDataType();
						Element.Semantics = MWDVertexFormat::VF_TEXCOORD;
						Element.SemanticsIndex = i;
						FormatArray.AddElement(Element);
					}
				}

				for (unsigned int i = 0; i < GetNormalLevel(); i++)
				{
					pData = GetNormalData();
					if (pData)
					{
						Element.OffSet = (WORD)iVertexSize;
						iVertexSize += pData->GetStride();
						Element.DataType = pData->GetDataType();
						Element.Semantics = MWDVertexFormat::VF_NORMAL;
						Element.SemanticsIndex = i;
						FormatArray.AddElement(Element);
					}
				}

				pData = GetTangentData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_TANGENT;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}

				pData = GetBinormalData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_BINORMAL;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}

				pData = GetPSizeData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_PSIZE;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}

				for (unsigned int i = 0; i < GetColorLevel(); i++)
				{

					pData = GetColorData();
					if (pData)
					{
						Element.OffSet = (WORD)iVertexSize;
						iVertexSize += pData->GetStride();
						Element.DataType = pData->GetDataType();
						Element.Semantics = MWDVertexFormat::VF_COLOR;
						Element.SemanticsIndex = i;
						FormatArray.AddElement(Element);
					}
				}

				pData = GetFogData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_FOG;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}

				pData = GetDepthData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_DEPTH;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}

				pData = GetBlendWeightData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_BLENDWEIGHT;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}

				pData = GetBlendIndicesData();
				if (pData)
				{
					Element.OffSet = (WORD)iVertexSize;
					iVertexSize += pData->GetStride();
					Element.DataType = pData->GetDataType();
					Element.Semantics = MWDVertexFormat::VF_BLENDINDICES;
					Element.SemanticsIndex = 0;
					FormatArray.AddElement(Element);
				}
				m_uiOneVertexSize = iVertexSize;
			}
			
			return (FormatArray.GetNum() > 0);
		};
		//要在GetVertexFormat()之后使用，否则生成失败返回NULL
		char* GenerateVboData() {
			if (vbo_data) {
				return vbo_data;
			}
			if (!m_uiVertexNum) {
				return NULL;
			}
			if (!m_pVertexFormat->m_FormatArray.GetNum()) {
				return NULL;
			}
			TCHAR* vbo = new TCHAR[(int)m_uiOneVertexSize*(int) m_uiVertexNum];

			unsigned int offset, DataType, Semantics, SemanticsIndex;
			int semantic_num  = m_pVertexFormat->m_FormatArray.GetNum();
			MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> format_arr = m_pVertexFormat->m_FormatArray;

			for (int i = 0; i < m_uiVertexNum; i++) {
				TCHAR* one_vertex_data = NULL;
				for (int j = 0; j < semantic_num; j++) {
					MWDDataBuffer* temp = m_pData[Semantics][SemanticsIndex];
					MWDStrcat(one_vertex_data,temp->GetStride(),MWDString::CharToTCHAR(temp->GetOneVertexData(i)));
				}
				MWDStrcat(vbo, m_uiOneVertexSize, one_vertex_data);
			}
			vbo_data = MWDString::TCHARToChar(vbo);
			return vbo_data;
		}
		void LoadData() {
			m_vbo->SetData(vbo_data, m_uiVertexNum* m_uiOneVertexSize);//填充vbo数据
			int attribute_num = m_pVertexFormat->m_FormatArray.GetNum();
			unsigned int datatype, offset, semantics, semantics_index;
			for (int i = 0; i < attribute_num; ++i) {
				datatype = m_pVertexFormat->m_FormatArray[i].DataType;
				offset = m_pVertexFormat->m_FormatArray[i].OffSet;
				semantics = m_pVertexFormat->m_FormatArray[i].Semantics;
				semantics_index = m_pVertexFormat->m_FormatArray[i].SemanticsIndex;
				m_vbo->SetAttributePointer(semantics,MWDDataBuffer::ms_uiDataTypeByte[datatype],GL_UNSIGNED_INT,false,m_uiOneVertexSize,offset);//设置vbo解析格式
			}
		}
		void LoadDataToVBO() {
			MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> tmp = MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE>();
			GetVertexFormat(tmp);
			GenerateVboData();
			LoadData();
		}
		FORCEINLINE MWDVertexFormat* GetVertexFormat()const {
			return m_pVertexFormat;
		};
		FORCEINLINE unsigned int GetOneVertexSize()const {
			return m_uiOneVertexSize;
		};

		FORCEINLINE bool GetLockDataOffSet(unsigned int uiVF, unsigned int uiLevel, unsigned int& uiOffSet)const {
			if (!m_pVertexFormat)
			{
				MWDDataBuffer* pData = NULL;
				for (unsigned int i = 0; i < uiVF; i++)
				{
					for (unsigned int j = 0; j < m_pData[i].GetNum(); j++)
					{
						pData = GetData(i, j);
						if (pData)
							uiOffSet += pData->GetStride();
					}
				}
				for (unsigned int j = 0; j < uiLevel; j++)
				{
					pData = GetData(uiVF, j);
					if (pData)
						uiOffSet += pData->GetStride();
				}
				if (!pData)
				{
					return false;
				}
				return true;
			}
			else
			{
				for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.GetNum(); i++)
				{
					if (m_pVertexFormat->m_FormatArray[i].Semantics == uiVF)
					{
						if (m_pVertexFormat->m_FormatArray[i].SemanticsIndex == uiLevel)
						{
							uiOffSet = m_pVertexFormat->m_FormatArray[i].OffSet;
							return true;
						}
					}
				}
				return false;
			}
		};
		FORCEINLINE void* GetLockData(unsigned int uiVF, unsigned int uiLevel)const {
			if (uiVF >= MWDVertexFormat::VF_MAX || !m_pLockData)
				return NULL;
			if (GetSemanticsNum(uiVF) <= uiLevel)
				return NULL;

			unsigned int uiOffSet = 0;
			if (!GetLockDataOffSet(uiVF, uiLevel, uiOffSet))
				return	NULL;
			return (unsigned char*)m_pLockData + uiOffSet;
		};

		FORCEINLINE void* GetLockPositionData(unsigned int uiLevel)const {
			return GetLockData(MWDVertexFormat::VF_POSITION, uiLevel);
		};
		FORCEINLINE void* GetLockNormalData(unsigned int uiLevel)const {
			return GetLockData(MWDVertexFormat::VF_NORMAL, uiLevel);
		};
		FORCEINLINE void* GetLockPSizeData()const {
			return GetLockData(MWDVertexFormat::VF_PSIZE, 0);
		};
		FORCEINLINE void* GetLockColorData(unsigned int uiLevel)const {
			return GetLockData(MWDVertexFormat::VF_COLOR, uiLevel);
		};
		FORCEINLINE void* GetLockBlendWeightData()const {
			return GetLockData(MWDVertexFormat::VF_BLENDWEIGHT, 0);
		};
		FORCEINLINE void* GetLockBlendIndicesData()const {
			return GetLockData(MWDVertexFormat::VF_BLENDINDICES, 0);
		};
		FORCEINLINE void* GetLockTangentData()const {
			return GetLockData(MWDVertexFormat::VF_TANGENT, 0);
		};
		FORCEINLINE void* GetLockBinormalData()const {
			return GetLockData(MWDVertexFormat::VF_BINORMAL, 0);
		};
		FORCEINLINE void* GetLockFogData()const {
			return GetLockData(MWDVertexFormat::VF_FOG, 0);
		};
		FORCEINLINE void* GetLockDepthData()const {
			return GetLockData(MWDVertexFormat::VF_DEPTH, 0);
		};
		FORCEINLINE void* GetLockTexCoordData(unsigned int uiLevel)const {
			return GetLockData(MWDVertexFormat::VF_TEXCOORD, uiLevel);
		};
		virtual unsigned int GetByteSize()const {
			return m_uiVertexNum * m_uiOneVertexSize;
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
							return MWDDataBuffer::ms_uiDataTypeChannel[m_pVertexFormat->m_FormatArray[i].DataType];
						}
						uiNum++;
					}
				}
				return 0;
			}

		};

		FORCEINLINE unsigned int PositionChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_POSITION, uiLevel);
		};
		FORCEINLINE unsigned int NormalChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_NORMAL, uiLevel);
		};
		FORCEINLINE unsigned int PSizeChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_PSIZE, 0);
		};
		FORCEINLINE unsigned int ColorChannel(unsigned int uiLevel)const {
			return GetSemanticsChannel(MWDVertexFormat::VF_COLOR, uiLevel);
		};
		FORCEINLINE unsigned int BlendWeightChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_BLENDWEIGHT, 0);
		};
		FORCEINLINE unsigned int BlendIndicesChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_BLENDINDICES, 0);
		};
		FORCEINLINE unsigned int TangentChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_TANGENT, 0);
		};
		FORCEINLINE unsigned int BinormalChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_BINORMAL, 0);
		};
		FORCEINLINE unsigned int FogChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_FOG, 0);
		};
		FORCEINLINE unsigned int DepthChannel()const {
			return GetSemanticsChannel(MWDVertexFormat::VF_DEPTH, 0);
		};
		FORCEINLINE unsigned int TexCoordChannel(unsigned int uiLevel)const {
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

		FORCEINLINE unsigned int NormalDataType(unsigned int uiLevel)const {
			return GetSemanticsDataType(MWDVertexFormat::VF_NORMAL, uiLevel);
		};
		FORCEINLINE unsigned int TangentDataType()const {
			return GetSemanticsDataType(MWDVertexFormat::VF_TANGENT, 0);
		};
		FORCEINLINE unsigned int BinormalDataType()const {
			return GetSemanticsDataType(MWDVertexFormat::VF_BINORMAL, 0);
		};

	protected:
		friend class MWDVertexFormat;
		friend class MWDGeometry;

		MWDVertexBuffer() {
			for (unsigned int i = 0; i < MWDVertexFormat::VF_MAX; i++)
			{
				m_pData[i].Clear();
			}
			m_uiVertexNum = 0;
			m_pVertexFormat = NULL;
			m_uiOneVertexSize = 0;
			m_pLockData = NULL;
		};
		//数据缓存数列（每一个buffer对应存储一种类型的顶点数据）
		//数组下标就是数据类型
		MWDArray<MWDDataBufferPtr> m_pData[MWDVertexFormat::VF_MAX];
		//顶点数量
		unsigned int m_uiVertexNum;
		//一个顶点的内存占用大小
		unsigned int m_uiOneVertexSize;
		//指向一个顶点结构（解析buffer的方案）
		MWDVertexFormatPtr m_pVertexFormat;

		MWDVBO* m_vbo;

		char* vbo_data;

		//锁定的数据
		void* m_pLockData;

	public:

		virtual	bool LoadResource(MWDRenderer* pRender);

		void* Lock() ;
		void UnLock();
		FORCEINLINE void* GetLockDataPtr()const
		{
			return m_pLockData;
		}
		virtual void ClearInfo() {
			if (m_uiSwapChainNum == m_resourceArray.GetNum())
			{
				if (m_uiMemType == MT_VRAM)
				{
					for (unsigned int i = 0; i < MWDVertexFormat::VF_MAX; i++)
					{
						m_pData[i].Clear();
					}
				}
			}
		};
	protected:
		//MWDRenderer未实现
		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		//MWDRenderer未实现
		virtual bool OnReleaseResource(MWDOpenglResource* pID);

	};
	DECLARE_Ptr(MWDVertexBuffer);
	MWDTYPE_MARCO(MWDVertexBuffer);
}

