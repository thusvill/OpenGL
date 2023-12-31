#include "Core/GameEngine.h"
#include "Core/Editor/SceneHierarchyPanel.h"

    const unsigned int Width = 1920;
    const unsigned int Height = 1080;
    bool face_culling = false;
    bool anti_aliasing = false;
    bool debug_draw = false;
    bool VSYNC = false;
    int samples = 8;


//Anti-Aliasing = MSAA

    void Docking() {

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Game Engine", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) !=
                                                         0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) !=
                                                          0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
                                    (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) !=
                                    0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) !=
                                                                0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
                                    (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0,
                                    opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }



    Ref<Scene> m_ActiveScene;
    SceneHierarchyPanel m_Panel;
int selectedLightType = 0;
int cullType = 0;
int camType = 0;
int cullFace = 0;
char path[1024] = "model_path";
float light_intensity = 1.0f;

    int main() {
        Camera camera(Width, Height, glm::vec3(2.07479f, 1.249f, -0.381916f), 45.0f, 0.1f, 1000.0f);
        camera.Orientation = glm::vec3(-4.71458f, -1.41498f, 0.173473f);
        m_ActiveScene = CreateRef<Scene>(camera);
        m_Panel.SetContex(m_ActiveScene);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        if (anti_aliasing)
            glfwWindowHint(GLFW_SAMPLES, samples);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        GLFWwindow *window = glfwCreateWindow(Width, Height, "Loading...", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to Create a Window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        gladLoadGL();

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return -1;
        }

        glViewport(0, 0, Width, Height);

        // Initialize ImGui
        ImGui::CreateContext();
        // Setup ImGui style and platform bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        // Enable docking features
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


        Shader shaderProgram("Core/Shaders/default.vert", "Core/Shaders/default.frag", "Core/Shaders/default.geom");
        Shader normalsShaderProgram("Core/Shaders/default.vert", "Core/Shaders/normals.frag",
                                    "Core/Shaders/normals.geom");

        glEnable(GL_DEPTH_TEST);



        Entity m_Camera = m_ActiveScene->CreateEntity("Camera Entity");
        m_Camera.AddComponent<CameraComponent>(camera, true);
        auto camera_c = m_Camera.GetComponent<CameraComponent>().camera;



        /*
         * Rotation x: 3.24507y: -2.10654z: 3.19486
            Position x:-4.46584 y:4.58132 z:-4.38143

         */

        auto model = m_ActiveScene->CreateEntity("model");
        model.AddComponent<MeshRenderer>(Model("../Models/statue/scene.gltf"), shaderProgram);
        Model model_m = model.GetComponent<MeshRenderer>().mesh;

        Transform model_t = model.GetComponent<Transform>();
        model_t.Scale = glm::vec3(1.0f);

        //auto model2 = m_ActiveScene->CreateEntity("model2");
        //model2.AddComponent<MeshRenderer>(Model("../Models/sword/scene.gltf"), shaderProgram);

        auto mainLight = m_ActiveScene->CreateEntity("Directional Light");
        auto light = mainLight.AddComponent<LightComponent>(shaderProgram, 0);


        double prevTime = 0.0;
        double crntTime = 0.0;
        double timeDiff = 0.0;
        unsigned int counter = 0;

        //VSYNC off
        glfwSwapInterval(0);

        FBO frameBuffer = FBO(Width, Height);

        m_ActiveScene->OnStart();
        m_Panel.OnStart();
        while (!glfwWindowShouldClose(window)) {


            crntTime = glfwGetTime();
            timeDiff = crntTime - prevTime;
            counter++;
            if (timeDiff >= 1.0 / 30.0) {
                std::string FPS = std::to_string((1.0 / timeDiff) * counter);
                std::string ms = std::to_string((timeDiff / counter) * 1000);
                std::string newTitle = "Game - " + FPS + "FPS / " + ms + "ms";
                glfwSetWindowTitle(window, newTitle.c_str());
                prevTime = crntTime;
                counter = 0;
            }

            //Draw Code
            //glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            m_Camera.GetComponent<CameraComponent>().camera.Inputs(window);

            m_ActiveScene->OnUpdate();
            m_ActiveScene->m_currentCamera = m_Camera.GetComponent<CameraComponent>().camera;

            frameBuffer.Bind();
            glEnable(GL_DEPTH_TEST);
            {
                //Draw Code
                glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                //plane.Draw(shaderProgram, camera);
                m_ActiveScene->OnDraw();
                if (debug_draw)
                    model_m.Draw(normalsShaderProgram, camera);
            }
            frameBuffer.Unbind();

            //Imgui
            {

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                Docking();

                m_Panel.OnImguiRender();
                //Viewport
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                {
                    ImGui::Begin("Viewport");
                    if(ImGui::IsMouseDragging(1) && ImGui::IsWindowHovered()){
                        m_Camera.GetComponent<CameraComponent>().camera.enableControl = true;
                    } else{
                        m_Camera.GetComponent<CameraComponent>().camera.enableControl = false;
                    }

                    GLuint fboTextureID = frameBuffer.GetTexture();
                    ImVec2 panelSize = ImGui::GetContentRegionAvail();
                    if (panelSize.x != camera_c.width || panelSize.y != camera_c.height) {
                        camera_c.Resize(panelSize.x, panelSize.y);
                        glViewport(0, 0, panelSize.x, panelSize.y);
                        frameBuffer.Resize(panelSize.x, panelSize.y);
                    }
                    ImGui::Image(reinterpret_cast<void *>(static_cast<uintptr_t>(fboTextureID)),
                                 ImVec2(panelSize.x, panelSize.y), ImVec2(0, 1), ImVec2(1, 0));

                    ImGui::End();
                }
                ImGui::PopStyleVar();

                //Rendering
                {
                    ImGui::Begin("Rendering");

                    ImGui::SeparatorText("Informations");
                    ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));

                    ImGui::SeparatorText("Settings");
                    ImGui::Checkbox("Debug Draw", &debug_draw);
                    ImGui::Checkbox("VSYNC", &VSYNC);
                    glfwSwapInterval((int) VSYNC);
                    if (ImGui::Checkbox("Anti-Aliasing", &anti_aliasing)) {


                        if (anti_aliasing) {
                            //glfwWindowHint(GLFW_SAMPLES, samples);
                            ImGui::DragInt("Samples", &samples, 1.0f, 0, 100);
                            glfwWindowHint(GLFW_SAMPLES, samples);
                            glEnable(GL_MULTISAMPLE);
                        } else {
                            glDisable(GL_MULTISAMPLE);
                        }
                    }
                    ImGui::Checkbox("Face Culling", &face_culling);
                    if (face_culling) {
                        glEnable(GL_CULL_FACE);


                        const char *cullFaces[] = {"Front face", "Back face"};
                        if (ImGui::Combo("Culling face", &cullFace, cullFaces, IM_ARRAYSIZE(cullFaces))) {
                            switch (cullFace) {
                                case 0:
                                    glCullFace(GL_FRONT);
                                    break;
                                case 1:
                                    glCullFace(GL_BACK);
                                    break;
                            }
                        }


                        const char *cullTypes[] = {"CW", "CCW"};
                        if (ImGui::Combo("FaceCulling Type", &cullType, cullTypes, IM_ARRAYSIZE(cullTypes))) {
                            switch (cullType) {
                                case 0:
                                    glFrontFace(GL_CW);
                                    break;
                                case 1:
                                    glFrontFace(GL_CCW);
                                    break;
                            }
                        }

                    } else {
                        glDisable(GL_CULL_FACE);
                    }

                    ImGui::End();
                }
                //Logging

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }


            glfwSwapBuffers(window);

            glfwPollEvents();

        }

        shaderProgram.Delete();
        normalsShaderProgram.Delete();
        m_ActiveScene->OnDelete();


        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }

