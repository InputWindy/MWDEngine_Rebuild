#pragma once
#include "MWDMeshData.h"
namespace MWDEngine {

	//只负责实现相关方法，不维护新数据
	class MWDTriangleMeshData : public MWDMeshData
	{
		DECLARE_CLASS_FUNCTION(MWDTriangleMeshData)
		DECLARE_RTTI(MWDTriangleMeshData, MWDMeshData)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTriangleMeshData)
	public:
		MWDTriangleMeshData() {};
		virtual ~MWDTriangleMeshData() {};
		//创建扇形indexBuffer索引
		bool CreateFanIndex() {
			if (m_pVertexBuffer == NULL)
				return 0;
			unsigned int Num = m_pVertexBuffer->GetPositionData()->GetNum();
			if (Num < 3)
				return 0;
			Num = (Num - 1) * 3;
			MWDDataBuffer* pIndex = NULL;
			pIndex = new MWDDataBuffer();
			if (!pIndex)
				return 0;
			if (!pIndex->CreateEmptyBuffer(Num, MWDDataBuffer::DataType_USHORT))
				return 0;

			MWDUSHORT_INDEX* pIndexBuffer = (MWDUSHORT_INDEX*)pIndex->GetData();
			if (!pIndexBuffer)
				return 0;
			//pIndexBuffer:{0,1,2,0,4,5,0,7,8........}
			for (unsigned int i = 0; i < Num / 3; i++)
			{
				pIndexBuffer[i * 3] = 0;
				pIndexBuffer[i * 3 + 1] = i + 1;
				pIndexBuffer[i * 3 + 2] = i + 2;
			}
			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer = new MWDIndexBuffer();
				if (!m_pIndexBuffer)
				{
					return 0;
				}
			}

			m_pIndexBuffer->SetData(pIndex);

			return 1;
		};

		//通过顶点Position，自动解析生成顶点法线
		bool CreateNormal() {
			if (m_pVertexBuffer == NULL)
				return 0;
			if (m_pVertexBuffer->GetNormalData())
				return 1;
			//顶点数目
			unsigned int Num = m_pVertexBuffer->GetVertexNum();
			if (Num < 3)
				return 0;

			if (m_pIndexBuffer == NULL)
				return 0;

			//Noraml
			MWDDataBuffer* pNormal = NULL;
			pNormal = new MWDDataBuffer();
			if (!pNormal)
				return 0;
			if (!pNormal->CreateEmptyBuffer(Num, MWDDataBuffer::DataType_FLOAT32_3))
				return 0;

			MWDVector3* pNormalBuffer = (MWDVector3*)pNormal->GetData();

			//计数(每个顶点包含的法线条数)
			unsigned int* pVertexTemp = new unsigned int[Num];
			MWDMemset((void*)pVertexTemp, 0, sizeof(unsigned int) * Num);

			unsigned int TriangleNum = GetTotleNum();

			MWDDataBuffer* pIndexData = m_pIndexBuffer->GetIndexData();
			if (!pIndexData)
			{
				return 0;
			}
			// 获取索引指针
			MWDUSHORT_INDEX* pIndexBuffer = (MWDUSHORT_INDEX*)pIndexData->GetData();
			if (!pIndexBuffer)
				return 0;

			MWDDataBuffer* pData = m_pVertexBuffer->GetPositionData();
			if (!pData)
				return 0;
			if (pData->GetChannel() != 3)
				return 0;

			//获取顶点指针
			MWDVector3* pVertexBuffer = (MWDVector3*)pData->GetData();
			if (!pVertexBuffer)
				return 0;

			//遍历所有三角形
			for (unsigned int poly = 0; poly < TriangleNum; poly++)
			{

				MWDUSHORT_INDEX i0 = pIndexBuffer[poly * 3 + 0];
				MWDUSHORT_INDEX i1 = pIndexBuffer[poly * 3 + 1];
				MWDUSHORT_INDEX i2 = pIndexBuffer[poly * 3 + 2];

				MWDVector3 u = pVertexBuffer[i1] - pVertexBuffer[i0];
				MWDVector3 v = pVertexBuffer[i2] - pVertexBuffer[i0];

				MWDVector3 n;
				n.Cross(u, v);

				pVertexTemp[i0]++;
				pVertexTemp[i1]++;
				pVertexTemp[i2]++;

				pNormalBuffer[i0] += n;
				pNormalBuffer[i1] += n;
				pNormalBuffer[i2] += n;

			} // end for poly


			for (unsigned int vertex = 0; vertex < Num; vertex++)
			{
				if (pVertexTemp[vertex] > 1)
				{
					pNormalBuffer[vertex] /= (MWDREAL)1.0 * pVertexTemp[vertex];
				} // end if
				pNormalBuffer[vertex].Normalize();
			} // end for
			MWDMAC_DELETEA(pVertexTemp);

			m_pVertexBuffer->SetData(pNormal, MWDVertexFormat::VF_NORMAL);
			return 1;
		};
		//通过顶点Position，自动解析生成顶点副法线和切线
		bool CreateTangent(unsigned int uiTexCoordLevel = 0) {
			if (!m_pVertexBuffer || !m_pVertexBuffer->GetTexCoordData(uiTexCoordLevel) || !m_pIndexBuffer)
				return 0;
			if (!m_pVertexBuffer->GetPositionData())
				return 0;
			if (m_pVertexBuffer->GetTangentData() && m_pVertexBuffer->GetBinormalData())
				return 1;
			else if (m_pVertexBuffer->GetTangentData() || m_pVertexBuffer->GetBinormalData())
				return 0;
			unsigned int Num = m_pVertexBuffer->GetVertexNum();
			if (Num < 3)
				return 0;

			if (!m_pVertexBuffer->GetNormalData())
			{
				if (!CreateNormal())
					return 0;
			}

			//TangentNoraml
			MWDDataBuffer* pTangent = NULL;
			pTangent = new MWDDataBuffer();
			if (!pTangent)
				return 0;
			if (!pTangent->CreateEmptyBuffer(Num, MWDDataBuffer::DataType_FLOAT32_3))
				return 0;

			MWDVector3* pTangentBuffer = (MWDVector3*)pTangent->GetData();

			//BiNoraml
			MWDDataBuffer* pBiNormal = NULL;
			pBiNormal = new MWDDataBuffer();
			if (!pBiNormal)
				return 0;
			if (!pBiNormal->CreateEmptyBuffer(Num, MWDDataBuffer::DataType_FLOAT32_3))
				return 0;

			MWDVector3* pBiNormalBuffer = (MWDVector3*)pBiNormal->GetData();


			unsigned int TriangleNum = GetTotleNum();


			MWDDataBuffer* pIndexData = m_pIndexBuffer->GetIndexData();
			if (!pIndexData)
			{
				return 0;
			}

			// 获取索引指针
			MWDUSHORT_INDEX* pIndexBuffer = (MWDUSHORT_INDEX*)pIndexData->GetData();
			if (!pIndexBuffer)
				return 0;

			//获取顶点
			MWDDataBuffer* pData = m_pVertexBuffer->GetPositionData();
			if (!pData)
				return 0;
			if (pData->GetChannel() != 3)
				return 0;
			//获取顶点指针
			MWDVector3* pVertexBuffer = (MWDVector3*)pData->GetData();
			if (!pVertexBuffer)
				return 0;

			// get normal 
			pData = m_pVertexBuffer->GetNormalData();
			if (!pData)
				return 0;
			MWDVector3* pNormalBuffer = (MWDVector3*)pData->GetData();
			if (!pNormalBuffer)
				return 0;

			// get texcoord
			pData = m_pVertexBuffer->GetTexCoordData(uiTexCoordLevel);
			if (!pData)
				return 0;
			MWDVector2* pTexCoordBuffer = (MWDVector2*)pData->GetData();
			if (!pTexCoordBuffer)
				return 0;

			for (unsigned int poly = 0; poly < TriangleNum; poly++)
			{
				MWDUSHORT_INDEX i0 = pIndexBuffer[poly * 3 + 0];
				MWDUSHORT_INDEX i1 = pIndexBuffer[poly * 3 + 1];
				MWDUSHORT_INDEX i2 = pIndexBuffer[poly * 3 + 2];

				//U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
				MWDVector3 TangentNoraml;
				TangentNoraml =
					((pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) -
						(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y)) /
					((pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) -
						(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y));
				TangentNoraml.Normalize();
				pTangentBuffer[i0] += TangentNoraml;
				pTangentBuffer[i1] += TangentNoraml;
				pTangentBuffer[i2] += TangentNoraml;

				//V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
				MWDVector3 BiNoraml;
				BiNoraml =
					((pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) -
						(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x)) /
					((pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y) -
						(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y));
				BiNoraml.Normalize();
				pBiNormalBuffer[i0] += BiNoraml;
				pBiNormalBuffer[i1] += BiNoraml;
				pBiNormalBuffer[i2] += BiNoraml;

			}
			for (unsigned int i = 0; i < m_pVertexBuffer->GetVertexNum(); i++)
			{
				MWDVector3 N1 = pTangentBuffer[i];
				MWDVector3 N2 = pBiNormalBuffer[i];
				MWDVector3 Normal = pNormalBuffer[i];

				MWDVector3 Tangent = N1 - Normal * ((N1 * Normal) / (Normal * Normal));

				MWDVector3 Binromal = N2 - Normal * ((N2 * Normal) / (Normal * Normal)) - Tangent * ((N2 * Tangent) / (Tangent * Tangent));

				Tangent.Normalize();
				Binromal.Normalize();
				pTangentBuffer[i] = Tangent;
				pBiNormalBuffer[i] = Binromal;
			}
			m_pVertexBuffer->SetData(pBiNormal, MWDVertexFormat::VF_BINORMAL);
			m_pVertexBuffer->SetData(pTangent, MWDVertexFormat::VF_TANGENT);
			return 1;
		};

		//获取三角形数量（如果没有创建索引，就是顶点个数/3。如果创建了索引，就是索引长度/3）
		virtual unsigned int GetTotleNum()const {
			if (!m_pIndexBuffer && !m_pVertexBuffer)
			{
				return 0;
			}
			else if (!m_pIndexBuffer)
			{
				return m_pVertexBuffer->GetVertexNum() / 3;
			}
			else
			{
				return m_pIndexBuffer->GetNum() / 3;
			}
		};
		virtual unsigned int GetMeshDataType() { return MDT_TRIANGLE; }


	};
	DECLARE_Ptr(MWDTriangleMeshData);
	MWDTYPE_MARCO(MWDTriangleMeshData);
}

