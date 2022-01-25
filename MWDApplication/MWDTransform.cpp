#include "pch.h"
#include "MWDTransform.h"
#include "MWDEntity.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDTransform)
	ADD_PRIORITY(MWDComponent)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDTransform)

    BEGIN_ADD_PROPERTY(MWDTransform)

    END_ADD_PROPERTY

    unsigned int MWDTransform::GetChildCount() const
    {
        return m_pEntity->GetChildCount();
    }

    void MWDTransform::ComponentInfoLog() const
    {
		cout << "=============TransformInfoLog_Tittle=================" << endl;
		//��������
		cout << "�������꣺" << endl;
		m_WorldPosition.Print();
		cout << "�������ţ�" << endl;
		m_WorldScale.Print();
		cout << "����ŷ���ǣ�" << endl;
		m_WorldEulerAngles.Print();
		cout << "������Ԫ����" << endl;
		m_WorldRotation.Print();

		//��������ϵ�µ�����������(ֻ��:ͨ��update������)
		cout << "����UP��" << endl;
		m_WorldUp.Print();
		cout << "����Right��" << endl;
		m_WorldRight.Print();
		cout << "����Forward��" << endl;
		m_WorldForward.Print();

		//����ڸ��ڵ㣨����Ǹ��ڵ��������һ����
		cout << "�������꣺" << endl;
		m_LocalPosition.Print();
		cout << "�������ţ�" << endl;
		m_LocalScale.Print();
		cout << "����ŷ���ǣ�" << endl;
		m_LocalEulerAngles.Print();
		cout << "������Ԫ����" << endl;
		m_LocalRotation.Print();

		//����
		cout << "��ʵ������:" << endl;
		if ( m_Parent) {
			if(m_Parent->GetEntity())
			m_Parent->GetEntity()->GetName().Print();
		}
		cout << "��ʵ�����ƣ�" << endl;
		if ( m_Root) {
			if(m_Root->GetEntity() )
			m_Root->GetEntity()->GetName().Print();
		}
		cout << "���������" << endl;
		cout << m_ChildCount << endl;

		//(ֻ��:ͨ��update������)
		cout << "����-���� �任����" << endl;
		m_WorldToLocalMatrix.Print();
		cout << "����-���� �任����" << endl;
		m_LocalToWorldMatrix.Print();
		cout << "=============TransformInfoLog_Bottom=================" << endl;
    }

}

