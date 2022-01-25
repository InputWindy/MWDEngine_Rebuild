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
		//世界坐标
		cout << "世界坐标：" << endl;
		m_WorldPosition.Print();
		cout << "世界缩放：" << endl;
		m_WorldScale.Print();
		cout << "世界欧拉角：" << endl;
		m_WorldEulerAngles.Print();
		cout << "世界四元数：" << endl;
		m_WorldRotation.Print();

		//世界坐标系下的三个方向轴(只读:通过update来更新)
		cout << "世界UP：" << endl;
		m_WorldUp.Print();
		cout << "世界Right：" << endl;
		m_WorldRight.Print();
		cout << "世界Forward：" << endl;
		m_WorldForward.Print();

		//相对于父节点（如果是根节点则跟上面一样）
		cout << "本地坐标：" << endl;
		m_LocalPosition.Print();
		cout << "本地缩放：" << endl;
		m_LocalScale.Print();
		cout << "本地欧拉角：" << endl;
		m_LocalEulerAngles.Print();
		cout << "本地四元数：" << endl;
		m_LocalRotation.Print();

		//拓扑
		cout << "父实体名称:" << endl;
		if ( m_Parent) {
			if(m_Parent->GetEntity())
			m_Parent->GetEntity()->GetName().Print();
		}
		cout << "根实体名称：" << endl;
		if ( m_Root) {
			if(m_Root->GetEntity() )
			m_Root->GetEntity()->GetName().Print();
		}
		cout << "子组件数量" << endl;
		cout << m_ChildCount << endl;

		//(只读:通过update来更新)
		cout << "世界-本地 变换矩阵：" << endl;
		m_WorldToLocalMatrix.Print();
		cout << "本地-世界 变换矩阵：" << endl;
		m_LocalToWorldMatrix.Print();
		cout << "=============TransformInfoLog_Bottom=================" << endl;
    }

}

