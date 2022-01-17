#include "pch.h"
#include "MWDScene.h"
#include "MWDWorld.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDScene)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDScene)

    BEGIN_ADD_PROPERTY(MWDScene)

    END_ADD_PROPERTY

    MWDWorld* MWDScene::GetWorld() {
        return m_World;
    }
    void MWDScene::AddWorldEntityArray(MWDEntity* entity)
    {
        //cout << m_World << endl;
        int idx = m_World->m_SceneAllEntity.FindElement(entity);
        if (idx == m_World->m_SceneAllEntity.GetNum()) {
            m_World->m_SceneAllEntity.AddElement(entity);
            return;
        }
        
    }
    void MWDScene::DeleteWorldEntityArray(MWDEntity* entity)
    {
        int idx = m_World->m_SceneAllEntity.FindElement(entity);
        /*cout << "idx=" << idx << endl;
        cout << "num=" << m_World->m_SceneAllEntity.GetNum() << endl;*/
        if (idx != m_World->m_SceneAllEntity.GetNum()) {
            //m_World->m_SceneAllEntity[idx]->GetName().Print();
            m_World->m_SceneAllEntity.Erase(idx, idx);
            return;
        }
        
    }
}