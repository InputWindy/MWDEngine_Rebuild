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





//ImGui��ʹ��������glfw_Window
//��ʼ��˳��glfw��opengl��imGui
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
	//MWDApplication�ࣺά��һ��App��������������״̬flag�������в�ͬƽ̨��Ҫʵ�ֵĽӿ�
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
		//һ������
		MWDCommand* m_pCommand;
		//��Ⱦ�ӿ�����
		unsigned int m_uiRenderAPIType;
		//��Ļ��С
		unsigned int m_uiScreenWidth;
		unsigned int m_uiScreenHeight;
		//��������
		unsigned int m_uiAnisotropy;
		//������
		unsigned int m_uiMultisample;
		//��ʣ�µĿ����õ��߳�����
		unsigned int m_iUpdateThreadNum;

		//����һ��window
		virtual bool CreateAppWindow() {
			return true;
		};

		//������Ⱦ��
		virtual bool CreateRenderer() {
			return true;
		};
		virtual bool ReleaseRenderer() {
			return true;
		};

		//����io
		virtual bool CreateInput() {
			return true;
		};
		virtual bool ReleaseInput() {
			return true;
		};

		//������ʱ��
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
		
		//��������������
		virtual bool CreateSceneManager() {
			return true;
		};
		virtual bool ReleaseSceneManager() {
			return true;
		};

		//��������
		virtual bool CreateWorld() {
			return true;
		};
		virtual bool ReleaseWorld() {
			return true;
		};

		//����������
		virtual bool CreateMonitor() {
			return true;
		};
		virtual bool ReleaseMonitor() {
			return true;
		};

		//�����첽����ģ��
		virtual bool CreateASYNLoader() {
			return true;
		};
		virtual bool ReleaseASYNLoader() {
			return true;
		};

		//������Ⱦ�߳�
		virtual bool CreateRenderThread() {
			return true;
		};
		virtual bool ReleaseRenderThread() {
			return true;
		};

		//���������߳�
		virtual bool CreateUpdateThread() {
			return true;
		};
		virtual bool ReleaseUpdateThread() {
			return true;
		};

		//����һ֡����Ļ��ƣ���ȡio���첽����ģ���ǰ����ű������³���ǰ�������³��������������ű������³����󣩡���Ⱦ��GC
		virtual bool Run() {
			double fTime = 0.0f;
			double fFPS = 0.0f;
			double DetTime = 0.0f;/*
			ADD_TIME_PROFILE(ApplicationUpdate)*/
			GetStackMemManager().Clear();
			if (MWDTimer::ms_pTimer)
			{
				MWDTimer::ms_pTimer->UpdateFPS();
				//��ȡ��ǰ��Ϸʱ��
				fTime = MWDTimer::ms_pTimer->GetGamePlayTime();
				//��ȡFPS
				fFPS = MWDTimer::ms_pTimer->GetFPS();
				//ADD_ONLYTIME_PROFILE(FPS, fFPS)
			}
			//������Ⱦ����
//			if (MWDRenderer::ms_pRenderer)
//			{
//
//				if (MWDRenderer::ms_pRenderer->CooperativeLevel())
//				{
//					//����Ⱦ�̺߳���Դ����������ʱ��������Ⱦ�߳�
//					/*if (MWDRenderThreadSys::ms_pRenderThreadSys && MWDResourceManager::ms_bRenderThread)
//					{
//						MWDRenderThreadSys::ms_pRenderThreadSys->Begin();
//					}*/
//					{
//						//ADD_TIME_PROFILE(Update)
//						//��������io�������io�͸���io
//							/*if (MWDEngineInput::ms_pInput)
//							{
//								MWDEngineInput::ms_pInput->Update();
//							}*/
//						
//						//��Դ���������£������첽����ģ�����
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
//						//�൱��update()
//						//PreUpdate();
//
//						//���³���
//						/*if (MWDSceneManager::ms_pSceneManager)
//						{
//							MWDSceneManager::ms_pSceneManager->Update(fTime);
//						}*/
//
//						//��������
//						/*if (MWDWorld::ms_pWorld)
//						{
//							MWDWorld::ms_pWorld->Update(fTime);
//						}*/
//						//�൱��fixedupdate()
//						//PostUpdate();
//						//��Ⱦ����ʼ��Ⱦ
//						//MWDRenderer::ms_pRenderer->BeginRendering();
//
//						//������������ʼ����
//						/*if (MWDSceneManager::ms_pSceneManager)
//						{
//							MWDSceneManager::ms_pSceneManager->Draw(fTime);
//						}
//
//						if (!OnDraw())
//						{
//							return false;
//						}*/
//						//��������ʼ����
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
//					//�����Ⱦ�߳�
//					/*if (MWDRenderThreadSys::ms_pRenderThreadSys)
//					{
//						MWDRenderThreadSys::ms_pRenderThreadSys->Clear();
//					}*/
//				}
//				//��Ⱦ����֮�����������������
//				/*MWDResourceManager::ClearDynamicBufferGeometry();
//				MWDResourceManager::DelayUpdate(fTime);
//				MWDResourceManager::GC();*/
//
//			}
			return true;
		};

		//���������������еĸ���ģ��
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
		//��������ӿ�
		virtual bool Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow) {
			return false;
		};
		virtual bool Main(int argc, char* argv[]) {
			CreateEngine();

			#pragma region glfw��ʼ��
			if (!glfwInit())
				return -1;
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			GLFWwindow* Hwindow = glfwCreateWindow(800, 600, "MWDEngine", NULL, NULL);
			if (!Hwindow) {
				cout << "���ڴ���ʧ��" << endl;
				glfwTerminate();
				return -1;
			}
			glfwMakeContextCurrent(Hwindow);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				cout << "glad��ʼ��ʧ�ܣ�" << endl;
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
		//PreInitial�����̼߳������������Ƿ������Ⱦ
		virtual bool PreInitial() {
			//��ǰ֡�趨app��Ϣ
			m_uiScreenWidth = 1024;
			m_uiScreenHeight = 768;
			m_uiAnisotropy = 1;
			//m_uiMultisample = MWDRenderer::MS_NONE;

			//��ȡ����߳���
			int iUpdateMaxThreadNum = MWDGetCpuNum();
			iUpdateMaxThreadNum--;// ����һ���̷ָ߳����̣߳����߳�ʼ����ִ��
			iUpdateMaxThreadNum--;// ����̷ָ߳��첽����ģ����߼�������Ҳ��ʼ����ִ�е�

			//����߳������㣬�ͷ�һ���̸߳���Ⱦ�̣߳�����Ļ���Run��ֱ��������Ⱦ
			if (iUpdateMaxThreadNum < 0)// Render Thread all time run
			{
				//MWDResourceManager::ms_bRenderThread = false;
			}
			else
			{
				//MWDResourceManager::ms_bRenderThread = true;
				//iUpdateMaxThreadNum--;
			}

			iUpdateMaxThreadNum--; //����̷ָ߳������̣߳���Ч���ߵ�������


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

		//�����¼�
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

		//������Ļ��С:�������Ⱦ�̣߳��ٵ�����Ⱦ����ChangeScreenSize()
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
		//��Ϣ�ص�:ʵ���Ͼ��Ǹ���ִ��Application��ĸ���On������Ȼ��ִ�������Process()
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

