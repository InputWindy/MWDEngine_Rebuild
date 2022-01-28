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
	MWDRenderer::GetMainRenderer()->DrawMesh();
	int x, y, com;
	void* data = stbi_load("C:/Users/InputWindy/Desktop/testpic.png",&x,&y,&com,0);
	MWDTexture2D tex2D(0,MWDTexture::TexMode::WRAP_S,MWDTexture::TexParam::LINEAR,x,y,MWDTexture::TexFormat::RGB,data,false);
	
	#pragma region 显示图片
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	ImTextureID image_id = (GLuint*)textureID;
	#pragma endregion

	MWDModel model = MWDModel(_T("test_model"), _T("C:/Users/InputWindy/Desktop/My3DEngine-master/resource/objModel/car.obj"),false);
	int len;
	model.GetComponentByType<MWDMeshComponent>()->GetMeshData()->GetVertexBuffer()->GetPositionData()->PrintFloat();
	int size = model.GetComponentByType<MWDMeshComponent>()->GetMeshData()->GetVertexBuffer()->GetNum();
	int size2 = model.GetComponentByType<MWDMeshComponent>()->GetMeshData()->GetVertexBuffer()->GetNormalData()->GetNum();
	int size3 = model.GetComponentByType<MWDMeshComponent>()->GetMeshData()->GetIndexBuffer()->GetNum();
	/*for (int i = 0; i < size; ++i) {
		cout << *((float*)vert + i*sizeof(float))<< " ";
	}*/
	cout << size << " " << size2 << " " << size3 << endl;
	while (!glfwWindowShouldClose(Hwindow)) {
		ImGuiFrameBegin();
		//ImGui::ShowDemoWindow();
		ImGui::Begin("showPNG");
		// 第一个参数：生成的纹理的id
		// 第2个参数：Image的大小
		// 第3，4个参数：UV的起点坐标和终点坐标，UV是被规范化到（0，1）之间的坐标
		// 第5个参数：图片的色调
		// 第6个参数：图片边框的颜色
		ImGui::Image(image_id, ImGui::GetContentRegionAvail(), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0), ImVec4(0, 0, 0, 1), ImVec4(0, 0, 0, 1));
		ImGui::End();
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