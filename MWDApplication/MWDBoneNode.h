#pragma once
#include "../MWDApplication/Graphic/��������/���/MWDTransform.h"
#include "MWDNode.h"
namespace MWDEngine {
	class MWDBoneNode : public MWDNode
	{
		DECLARE_CLASS_FUNCTION(MWDBoneNode)
		DECLARE_RTTI(MWDBoneNode, MWDNode)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBoneNode)
	public:
		//��������
		MWDBoneNode() {
			m_bIsEffector = false;
			m_bAllowTranslation.ResetBufferNum(3);
			m_fMinTranslation.ResetBufferNum(3);
			m_fMaxTranslation.ResetBufferNum(3);
			m_bAllowRotation.ResetBufferNum(3);
			m_fMinRotation.ResetBufferNum(3);
			m_fMaxRotation.ResetBufferNum(3);
			for (int i = 0; i < 3; i++)
			{
				m_bAllowTranslation[i] = false;
				m_fMinTranslation[i] = -MWDMAX_REAL;
				m_fMaxTranslation[i] = MWDMAX_REAL;
				m_bAllowRotation[i] = true;
				m_fMinRotation[i] = -MWDPI;
				m_fMaxRotation[i] = MWDPI;
			}
			m_fIKWeight = 1.0f;
		};
		virtual ~MWDBoneNode() {};
	public:
		//Use for IK
		//ЧӦ��������װ��
		bool m_bIsEffector;

		//��������
		MWDVector3 m_TargetPosInWorld;

		//�Ƿ������ƶ�
		MWDArray<bool> m_bAllowTranslation;

		//��С�ƶ���Χ
		MWDArray<MWDREAL> m_fMinTranslation;
		//����ƶ���Χ
		MWDArray<MWDREAL> m_fMaxTranslation;

		//�Ƿ�������ת
		MWDArray<bool> m_bAllowRotation;
		//��С��ת��Χ
		MWDArray<MWDREAL> m_fMinRotation;
		//�����ת��Χ
		MWDArray<MWDREAL> m_fMaxRotation;

		//IKȨ��
		MWDREAL m_fIKWeight;

		void GetIKMoveAxis(MWDVector3 Axis[3])const {
			if (m_pParent)
			{
				m_pParent->GetWorldRotate().GetUVN(Axis);
			}
			else
			{
				Axis[0] = MWDVector3(1.0f, 0.0f, 0.0f);//x
				Axis[1] = MWDVector3(0.0f, 1.0f, 0.0f);//z
				Axis[2] = MWDVector3(0.0f, 0.0f, 1.0f);//y
			}
		};
		void ComputeIKLocalRotDelta(const MWDMatrix3X3& WorldRot, MWDMatrix3X3& LocalRotDelta)const {
			if (m_pParent)
			{
				MWDMatrix3X3 Inver;
				Inver.InverseOf(m_pParent->GetWorldRotate());
				LocalRotDelta = m_pParent->GetWorldRotate() * WorldRot * Inver;
			}
			else
			{
				LocalRotDelta = WorldRot;
			}
		};

		//�任����
		MWDMatrix3X3W m_OffSetMatrix;

		virtual void SetLocalMat(const MWDMatrix3X3W MWDMat) {
			m_bIsChanged = true;
			m_Local.SetMatrix(MWDMat);
		};

		FORCEINLINE const MWDMatrix3X3W& GetBoneOffsetMatrix()const
		{
			return m_OffSetMatrix;
		}

		MWDBoneNode* GetBoneNodeFromLevel(const MWDName& BoneName) {
			MWDBoneNode* pFind = NULL;
			if (m_NodeName == BoneName)
			{
				pFind = this;
			}
			else
			{
				for (unsigned int i = 0; i < m_pChildren.GetNum(); i++)
				{
					MWDNode* pChild = m_pChildren[i];
					pFind = ((MWDBoneNode*)pChild)->GetBoneNodeFromLevel(BoneName);
					if (!pFind)
					{
						return pFind;
					}
				}
			}
			return pFind;
		};
		unsigned int GetAllBoneNum()const {
			unsigned int uiLeafNum = 1;
			for (unsigned int i = 0; i < m_pChildren.GetNum(); i++)
			{

				MWDNode* pBone = m_pChildren[i];
				uiLeafNum += ((MWDBoneNode*)pBone)->GetAllBoneNum();


			}

			return uiLeafNum;
		};
		void GetAllBoneArray(MWDArray<MWDBoneNode*>& AllNodeArray) {
			AllNodeArray.AddElement(this);
			for (unsigned int i = 0; i < m_pChildren.GetNum(); i++)
			{
				MWDBoneNode* pBoneNode = DynamicCast<MWDBoneNode>(m_pChildren[i]);
				if (pBoneNode)
				{
					pBoneNode->GetAllBoneArray(AllNodeArray);
				}

			}
		};
	};
	DECLARE_Ptr(MWDBoneNode);
	MWDTYPE_MARCO(MWDBoneNode);
}

