#pragma once
//#define DebugEngine
#include "MWDApplication.h"
using namespace MWDEngine;
using namespace std;
#ifdef DebugEngine
int  main(int argc, char* argv[])
{
    //获取主线程ID
    MWDInitSystem();
    //初始化CRC，正弦对照表，系统信息
    MWDInitMath();
    MWDApplication* app = new MWDApplication();
    MWDMain::Initialize();
    if (app->ms_pApplication)
    {
        if (!app->ms_pApplication->Main(argc, argv))
        {
            cout << "引擎关闭！" << endl;
        }
    }
    MWDMain::Terminate();
    system("pause");
    return 1;
}
#else

//class testClass :public MWDObject {
//    DECLARE_RTTI_NoCreateFun(testClass, MWDObject)
//    DECLARE_INITIAL_WITH_INIT_TERMINAL(testClass)
//    DECLARE_CLASS_FUNCTION(testClass)
//public:
//    static inline bool test() {
//    cout << "test!!!" << endl;
//    return true;
//}
//};
//INITIAL_WITH_INIT_TERMINAL_BEGIN(testClass)
//ADD_INITIAL_FUNCTION(InitialClassFactory)
//ADD_INITIAL_FUNCTION_WITH_PRIORITY(test)
//INITIAL_WITH_INIT_TERMINAL_END(testClass)
//
//BEGIN_ADD_PROPERTY(testClass)
//END_ADD_PROPERTY
//class testClassB :public testClass {
//    DECLARE_RTTI_NoCreateFun(testClassB, testClass)
//    DECLARE_INITIAL_WITH_INIT_TERMINAL(testClassB)
//    DECLARE_CLASS_FUNCTION(testClassB)
//public:
//    int tb;
//    static inline bool testB() {
//    cout << "testB!!!" << endl;
//    return true;
//}
//};
//INITIAL_WITH_INIT_TERMINAL_BEGIN(testClassB)
//ADD_INITIAL_FUNCTION(InitialClassFactory)
//ADD_INITIAL_FUNCTION_WITH_PRIORITY(testB)
//INITIAL_WITH_INIT_TERMINAL_END(testClassB)
//
//BEGIN_ADD_PROPERTY(testClassB)
//END_ADD_PROPERTY
//
//
//class testClassA :public MWDObject {
//    DECLARE_RTTI_NoCreateFun(testClassA, MWDObject)
//    DECLARE_INITIAL_WITH_INIT_TERMINAL(testClassA)
//    DECLARE_CLASS_FUNCTION(testClassA)
//public:
//    int ta;
//    double tb;
//    char tc;
//    char* td;
//    MWDColorRGBA rgba;
//    testClassA() {
//        rgba = *new MWDColorRGBA((unsigned char)(1 * 255), (unsigned char)(2 * 255), (unsigned char)(3 * 255));
//    }
//    ~testClassA() {
//        delete& rgba;
//    }
//    static inline bool testA() {
//        
//        cout << "testA!!!" << endl;
//        return true;
//    }
//    bool testM() {
//        rgba = *new MWDColorRGBA((MWDREAL)1, 2, 3);
//        cout << "testM!!!" << endl;
//        return true;
//    }
//
//    static int testK(int a, double b, char c, int d) {
//        cout << "testK!!!" << endl;
//        return 1;
//    }
//};
//INITIAL_WITH_INIT_TERMINAL_BEGIN(testClassA)
//ADD_PRIORITY(MWDObject)
//ADD_INITIAL_FUNCTION(InitialClassFactory)
//ADD_INITIAL_FUNCTION_WITH_PRIORITY(testA)
//INITIAL_WITH_INIT_TERMINAL_END(testClassA)
//
//BEGIN_ADD_PROPERTY(testClassA)
//REGISTER_PROPERTY(ta, InputWindy, MWDProperty::F_REFLECT_NAME)
//REGISTER_PROPERTY(tc, InputWindy111, MWDProperty::F_REFLECT_NAME)
//REGISTER_PROPERTY_RANGE(tb, InputWindy666, 100, 10, 3, MWDProperty::F_REFLECT_NAME)
//REGISTER_PROPERTY(rgba, color, MWDProperty::F_REFLECT_NAME)
//END_ADD_PROPERTY
//
//REGISTER_RETURN_FUNCTION_NOPARAMETER(testClassA, testA, 0, bool)
//REGISTER_RETURN_FUNCTION_NOPARAMETER(testClassA, testM, 0, bool)
//REGISTER_RETURN_FUNCTION_FOURPARAMETER(testClassA, testK, 0, int, int, a, double, b, char, c, int, d)
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
int main() {
    MWDMain::Initialize(); 
	MWDAABB3 aabb = MWDAABB3();
	MWDSphere3 s = MWDSphere3();
	#pragma region glfw初始化
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* Hwindow = glfwCreateWindow(800, 600, "MWDEngine", NULL, NULL);
	if (!Hwindow) {
		cout << "窗口创建失败" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Hwindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "glad初始化失败！" << endl;
		glfwTerminate();
		return -1;
	}
	ImGuiInit(Hwindow);

	#pragma endregion
	
	/*MWDBasicGeometryTriangleMeshData* mesh = new MWDBasicGeometryTriangleMeshData(s); 
	MWDMeshComponent* component = new MWDMeshComponent();
	component->SetMesh(mesh);
	MWDWorld* mainWorld = MWDWorld::GetMainWorld();
	mainWorld->CreateNewScene();
	MWDScene* scene = mainWorld->GetSceneByName(_T("default_scene"));
	MWDCamera* camera = new MWDCamera();
	camera->AddComponent(*component);
	scene->AddCamera(camera);
	cout<<mainWorld->GetSceneByName(_T("default_scene"))->GetAllCamera()[0]->GetComponentByType<MWDMeshComponent>()->GetMeshData()->GetVertexBuffer()->GetNum()<<endl;
	cout << mainWorld->GetSceneByName(_T("default_scene"))->GetAllCamera()[0]->GetComponentByType<MWDMeshComponent>()->GetMeshData()->GetIndexBuffer()->GetNum() << endl;*/
	while (!glfwWindowShouldClose(Hwindow)) {
		ImGuiFrameBegin();
		ImGui::ShowDemoWindow();
		ImGuiFrameEnd(Hwindow);
	};

	#pragma region glfw结束
	//MWDMAC_DELETE(mesh);
	ImGuiTerminate();
	#pragma endregion
	MWDMain::Terminate();
	system("pause");
	return 0;
}
#endif // DebugEngine