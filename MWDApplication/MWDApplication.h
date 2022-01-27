#pragma once
#pragma region include_imgui
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#pragma endregion

#include <iostream>
#include <MWDSystem.h>
#include <MWDFile.h>
#include <MWDMemManager.h>
#include <MWDSynchronize.h>
#include <MWDSingleton.h>
#include <MWDLog.h>
#include <MWDThread.h>
#include <MWDTimer.h>

#include <MWDArray.h>
#include <MWDMap.h>
#include <MWDString.h>
#include <MWDCompute.h>
#include <MWDBitArray.h>
#include <MWDContainer.h>
#include <MWDList.h>
#include <MWDQueue.h>
#include <MWDSafeQueue.h>
#include <MWDStack.h>
#include <../MWDApplication/Graphic/Core/MWDMain.h>
#include "MWDCommand.h"

#include "MWDVertexBuffer.h"
#include "MWDIndexBuffer.h"
#include "MWDBasicGeometryTriangleMeshData.h"
#include "MWDMeshComponent.h"
#include "MWDRenderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "MWDModel.h"





//ImGui的使用依赖于glfw_Window
//初始化顺序：glfw→opengl→imGui
void ImGuiInit(GLFWwindow* Hwindow) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext(NULL);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiViewportFlags_NoDecoration;

	ImGui_ImplGlfw_InitForOpenGL(Hwindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}
void ImGuiTerminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void ImGuiFrameBegin() {
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void ImGuiFrameEnd(GLFWwindow* Hwindow) {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* Hwindow = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(Hwindow);

	}
	int display_w, display_h;
	glfwGetFramebufferSize(Hwindow, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	/*glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);*/
	glfwSwapBuffers(Hwindow);
	glfwPollEvents();
}

namespace MWDEngine {
	//MWDApplication类：维护一个App单例，保存两个状态flag，是所有不同平台都要实现的接口
	class MWDApplication : public MWDMemObject
	{
	public:
		MWDApplication() {
			ms_pApplication = this;
			m_iUpdateThreadNum = 0;
			m_bIsActive = true;
			m_bIsRunning = true;
		};
		virtual ~MWDApplication() {
			delete m_pCommand;
			m_pCommand = NULL;
			ms_pApplication = NULL;
			m_bIsActive = false;
			m_bIsRunning = false;
		};

		static MWDApplication* ms_pApplication;

		bool m_bIsActive;
		bool m_bIsRunning;

	protected:
		//一条命令
		MWDCommand* m_pCommand;
		//渲染接口类型
		unsigned int m_uiRenderAPIType;
		//屏幕大小
		unsigned int m_uiScreenWidth;
		unsigned int m_uiScreenHeight;
		//各向异性
		unsigned int m_uiAnisotropy;
		//过采样
		unsigned int m_uiMultisample;
		//还剩下的可以用的线程数量
		unsigned int m_iUpdateThreadNum;

		//创建一个window
		virtual bool CreateAppWindow() {
			return true;
		};

		//创建渲染器
		virtual bool CreateRenderer() {
			return true;
		};
		virtual bool ReleaseRenderer() {
			return true;
		};

		//创建io
		virtual bool CreateInput() {
			return true;
		};
		virtual bool ReleaseInput() {
			return true;
		};

		//创建计时器
		virtual bool CreateTimer() {
			if (!MWDTimer::ms_pTimer)
			{
				MWDTimer* pTimer = new MWDTimer();
				if (!pTimer)
				{
					return false;
				}
			}
			MWDRandInit(MWDTimer::ms_pTimer->GetRandSeed());
			return true;
		};
		virtual bool ReleaseTimer() {
			MWDMAC_DELETE(MWDTimer::ms_pTimer);
			return true;
		};
		
		//创建场景管理器
		virtual bool CreateSceneManager() {
			return true;
		};
		virtual bool ReleaseSceneManager() {
			return true;
		};

		//创建世界
		virtual bool CreateWorld() {
			return true;
		};
		virtual bool ReleaseWorld() {
			return true;
		};

		//创建监听器
		virtual bool CreateMonitor() {
			return true;
		};
		virtual bool ReleaseMonitor() {
			return true;
		};

		//创建异步加载模块
		virtual bool CreateASYNLoader() {
			return true;
		};
		virtual bool ReleaseASYNLoader() {
			return true;
		};

		//创建渲染线程
		virtual bool CreateRenderThread() {
			return true;
		};
		virtual bool ReleaseRenderThread() {
			return true;
		};

		//创建更新线程
		virtual bool CreateUpdateThread() {
			return true;
		};
		virtual bool ReleaseUpdateThread() {
			return true;
		};

		//负责一帧画面的绘制：读取io→异步加载模块→前处理脚本（更新场景前）→更新场景和世界→后处理脚本（更新场景后）→渲染→GC
		virtual bool Run() {
			double fTime = 0.0f;
			double fFPS = 0.0f;
			double DetTime = 0.0f;/*
			ADD_TIME_PROFILE(ApplicationUpdate)*/
			GetStackMemManager().Clear();
			if (MWDTimer::ms_pTimer)
			{
				MWDTimer::ms_pTimer->UpdateFPS();
				//获取当前游戏时间
				fTime = MWDTimer::ms_pTimer->GetGamePlayTime();
				//获取FPS
				fFPS = MWDTimer::ms_pTimer->GetFPS();
				//ADD_ONLYTIME_PROFILE(FPS, fFPS)
			}
			//进入渲染流程
//			if (MWDRenderer::ms_pRenderer)
//			{
//
//				if (MWDRenderer::ms_pRenderer->CooperativeLevel())
//				{
//					//当渲染线程和资源管理器都打开时，开启渲染线程
//					/*if (MWDRenderThreadSys::ms_pRenderThreadSys && MWDResourceManager::ms_bRenderThread)
//					{
//						MWDRenderThreadSys::ms_pRenderThreadSys->Begin();
//					}*/
//					{
//						//ADD_TIME_PROFILE(Update)
//						//监听引擎io，如果有io就更新io
//							/*if (MWDEngineInput::ms_pInput)
//							{
//								MWDEngineInput::ms_pInput->Update();
//							}*/
//						
//						//资源监听器更新，或者异步加载模块更新
////#ifdef WINDOWS
////						if (MWDResourceMonitor::ms_pResourceMonitor)
////						{
////							MWDResourceMonitor::ms_pResourceMonitor->Update(fTime);
////						}
////#endif
////						if (MWDASYNLoadManager::ms_pASYNLoadManager)
////						{
////							MWDASYNLoadManager::ms_pASYNLoadManager->Update(fTime);
////						}
//
//						//相当于update()
//						//PreUpdate();
//
//						//更新场景
//						/*if (MWDSceneManager::ms_pSceneManager)
//						{
//							MWDSceneManager::ms_pSceneManager->Update(fTime);
//						}*/
//
//						//更新世界
//						/*if (MWDWorld::ms_pWorld)
//						{
//							MWDWorld::ms_pWorld->Update(fTime);
//						}*/
//						//相当于fixedupdate()
//						//PostUpdate();
//						//渲染器开始渲染
//						//MWDRenderer::ms_pRenderer->BeginRendering();
//
//						//场景管理器开始绘制
//						/*if (MWDSceneManager::ms_pSceneManager)
//						{
//							MWDSceneManager::ms_pSceneManager->Draw(fTime);
//						}
//
//						if (!OnDraw())
//						{
//							return false;
//						}*/
//						//分析器开始绘制
//						/*MWDProfiler::Draw();
//						MWDRenderer::ms_pRenderer->EndRendering();*/
//
//					}
//					/*if (MWDRenderThreadSys::ms_pRenderThreadSys && MWDResourceManager::ms_bRenderThread)
//					{
//						MWDRenderThreadSys::ms_pRenderThreadSys->ExChange();
//					}*/
//				}
//				else
//				{
//					//清空渲染线程
//					/*if (MWDRenderThreadSys::ms_pRenderThreadSys)
//					{
//						MWDRenderThreadSys::ms_pRenderThreadSys->Clear();
//					}*/
//				}
//				//渲染结束之后进入垃圾回收流程
//				/*MWDResourceManager::ClearDynamicBufferGeometry();
//				MWDResourceManager::DelayUpdate(fTime);
//				MWDResourceManager::GC();*/
//
//			}
			return true;
		};

		//创建和销毁引擎中的各个模块
		virtual  bool CreateEngine() {
			if (!CreateMonitor())
			{
				return false;
			}
			if (!CreateASYNLoader())
			{
				return false;
			}
			if (!CreateRenderThread())
			{
				return false;
			}
			if (!CreateUpdateThread())
			{
				return false;
			}
			if (!CreateRenderer())
			{
				return false;
			}
			if (!CreateInput())
			{
				return false;
			}
			if (!CreateSceneManager())
			{
				return false;
			}
			if (!CreateWorld())
			{
				return false;
			}
			return true;
		};
		virtual  bool ReleaseEngine() {
			if (!ReleaseWorld())
			{
				return false;
			}
			if (!ReleaseSceneManager())
			{
				return false;
			}
			if (!ReleaseInput())
				return false;
			if (!ReleaseRenderer())
			{
				return false;
			}
			if (!ReleaseUpdateThread())
			{
				return false;
			}
			if (!ReleaseRenderThread())
			{
				return false;
			}
			if (!ReleaseASYNLoader())
			{
				return false;
			}
			if (!ReleaseMonitor())
			{
				return false;
			}
			return true;
		};
	public:
		//引擎操作接口
		virtual bool Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow) {
			return false;
		};
		virtual bool Main(int argc, char* argv[]) {
			CreateEngine();

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
			
			while (!glfwWindowShouldClose(Hwindow)&& m_bIsRunning) {
				
				ImGuiFrameBegin();
				ImGui::ShowDemoWindow();
				/*bool flag = true;
				ImGui::Begin("ok", &flag);
					
				ImGui::End();*/

				ImGuiFrameEnd(Hwindow);
			};
			ImGuiTerminate();
			ReleaseEngine();
			return false;
		};
		//PreInitial负责线程计数，来控制是否进行渲染
		virtual bool PreInitial() {
			//当前帧设定app信息
			m_uiScreenWidth = 1024;
			m_uiScreenHeight = 768;
			m_uiAnisotropy = 1;
			//m_uiMultisample = MWDRenderer::MS_NONE;

			//获取最大线程数
			int iUpdateMaxThreadNum = MWDGetCpuNum();
			iUpdateMaxThreadNum--;// 其中一个线程分给主线程，主线程始终在执行
			iUpdateMaxThreadNum--;// 这个线程分给异步加载模块或者监听器，也是始终在执行的

			//如果线程数充足，就分一个线程给渲染线程，不足的话，Run会直接跳过渲染
			if (iUpdateMaxThreadNum < 0)// Render Thread all time run
			{
				//MWDResourceManager::ms_bRenderThread = false;
			}
			else
			{
				//MWDResourceManager::ms_bRenderThread = true;
				//iUpdateMaxThreadNum--;
			}

			iUpdateMaxThreadNum--; //这个线程分给物理线程，音效或者第三方库


			if (iUpdateMaxThreadNum <= 0)
			{

				//MWDResourceManager::ms_bUpdateThread = false;
			}
			else
			{
				//m_iUpdateThreadNum = iUpdateMaxThreadNum;
				//MWDResourceManager::ms_bUpdateThread = true;

			}
			return true;
		};
		virtual bool OnInitial() {
			//MWDProfiler::ClearAll();
			return true;
		};
		virtual bool OnTerminal() {
			return true;
		};
		virtual bool PostUpdate() {
			return true;
		};
		virtual bool PreUpdate() {
			return true;
		};
		virtual bool OnDraw() {
			return true;
		};

		//交互事件
		virtual void OnMove(int xPos, int yPos) {

		};
		virtual void OnReSize(int iWidth, int iHeight) {

		};
		virtual void OnKeyDown(unsigned int uiKey) {

		};
		virtual void OnKeyUp(unsigned int uiKey) {

		};
		virtual void OnLButtonDown(int xPos, int yPos) {

		};
		virtual void OnLButtonUp(int xPos, int yPos) {

		};
		virtual void OnRButtonDown(int xPos, int yPos) {

		};
		virtual void OnRButtonUp(int xPos, int yPos) {

		};
		virtual void OnMButtonDown(int xPos, int yPos) {

		};
		virtual void OnMButtonUp(int xPos, int yPos) {

		};
		virtual void OnMouseMove(int xPos, int yPos) {

		};
		virtual void OnMouseWheel(int xPos, int yPos, int zDet) {

		};

		//更改屏幕大小:先清空渲染线程，再调用渲染器的ChangeScreenSize()
		virtual void ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen = false) {
			/*if (MWDRenderThreadSys::ms_pRenderThreadSys)
			{
				MWDRenderThreadSys::ms_pRenderThreadSys->Clear();
			}

			if (MWDRenderer::ms_pRenderer->ChangeScreenSize(uiWidth, uiHeight, bWindow))
			{
				m_uiScreenWidth = uiWidth;
				m_uiScreenHeight = uiHeight;
			}*/
		};
		//消息回调:实际上就是负责执行Application里的各种On函数，然后执行世界的Process()
		static void InputMsgProc(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z) {
			/*if (uiInputType == MWDEngineInput::IT_KEYBOARD)
			{
				if (uiEvent == MWDEngineInput::IE_DOWN)
				{
					MWDApplication::ms_pApplication->OnKeyDown(uiKey);
				}
				else if (uiEvent == MWDEngineInput::IE_UP)
				{
					MWDApplication::ms_pApplication->OnKeyUp(uiKey);
				}

			}
			else if (uiInputType == MWDEngineInput::IT_MOUSE)
			{
				if (uiEvent == MWDEngineInput::IE_DOWN)
				{
					if (uiKey == MWDEngineInput::MK_RIGHT)
					{
						MWDApplication::ms_pApplication->OnRButtonDown(x, y);
					}
					else if (uiKey == MWDEngineInput::MK_LEFT)
					{
						MWDApplication::ms_pApplication->OnLButtonDown(x, y);
					}
					else if (uiKey == MWDEngineInput::MK_MIDDLE)
					{
						MWDApplication::ms_pApplication->OnMButtonDown(x, y);
					}
				}
				else if (uiEvent == MWDEngineInput::IE_UP)
				{
					if (uiKey == MWDEngineInput::MK_RIGHT)
					{
						MWDApplication::ms_pApplication->OnRButtonUp(x, y);
					}
					else if (uiKey == MWDEngineInput::MK_LEFT)
					{
						MWDApplication::ms_pApplication->OnLButtonUp(x, y);
					}
					else if (uiKey == MWDEngineInput::MK_MIDDLE)
					{
						MWDApplication::ms_pApplication->OnMButtonUp(x, y);
					}
				}
				else if (uiEvent == MWDEngineInput::IE_MOUSE_MOVE)
				{
					MWDApplication::ms_pApplication->OnMouseMove(x, y);
				}
				else if (uiEvent == MWDEngineInput::IE_WHEEL_MOVE)
				{
					MWDApplication::ms_pApplication->OnMouseWheel(x, y, z);
				}
			}
			if (MWDWorld::ms_pWorld)
			{
				MWDWorld::ms_pWorld->ProcessInput(uiInputType, uiEvent, uiKey, x, y, z);
			}*/
		
		};
		
	};
	MWDApplication* MWDApplication::ms_pApplication = NULL;
}

