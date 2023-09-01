#include "Core/GameEngine.h"

const unsigned int Width = 1920;
const unsigned int Height = 1920;
bool face_culling = false;
bool anti_aliasing = true;
bool debug_draw = false;
bool VSYNC =  false;
int samples = 8;


//Anti-Aliasing = MSAA

void Docking()
{

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
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
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}


int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    if(anti_aliasing)
    glfwWindowHint(GLFW_SAMPLES, samples);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(Width, Height, "Loading...", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to Create a Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;



    Shader shaderProgram("Core/Shaders/default.vert","Core/Shaders/default.frag", "Core/Shaders/default.geom");
    Shader normalsShaderProgram("Core/Shaders/default.vert","Core/Shaders/normals.frag", "Core/Shaders/normals.geom");


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);



    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);




    glEnable(GL_DEPTH_TEST);




    Camera camera(Width, Height, glm::vec3( 2.07479f , 1.249f, -0.381916f), 45.0f, 0.1f, 1000.0f);
    camera.Orientation = glm::vec3(-4.71458f, -1.41498f, 0.173473f);

    /*
     * Rotation x: 3.24507y: -2.10654z: 3.19486
        Position x:-4.46584 y:4.58132 z:-4.38143

     */

    Model plane("Models/statue/scene.gltf");

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff = 0.0;
    unsigned int counter = 0;

    //VSYNC off
    glfwSwapInterval(0);

    FBO frameBuffer = FBO(Width,Height);



    while (!glfwWindowShouldClose(window)){


        crntTime = glfwGetTime();
        timeDiff = crntTime-prevTime;
        counter++;
        if(timeDiff >= 1.0 / 30.0){
            std::string FPS = std::to_string((1.0/timeDiff)*counter);
            std::string ms = std::to_string((timeDiff/counter)*1000);
            std::string newTitle = "Game - "+FPS+"FPS / "+ms+"ms";
            glfwSetWindowTitle(window, newTitle.c_str());
            prevTime = crntTime;
            counter = 0;
        }

        //Draw Code
        //glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        camera.Inputs(window);
        camera.updateMatrix();

        frameBuffer.Bind();
        {
            //Draw Code
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            plane.Draw(shaderProgram, camera);
            if(debug_draw)
                plane.Draw(normalsShaderProgram, camera);
        }
        frameBuffer.Unbind();

        {

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            Docking();

            //Viewport
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
            {
                ImGui::Begin("Viewport");

                GLuint fboTextureID = frameBuffer.GetTexture();
                ImVec2 panelSize = ImGui::GetContentRegionAvail();
                if (panelSize.x != camera.width || panelSize.y != camera.height) {
                    camera.Resize(panelSize.x, panelSize.y);
                    glViewport(0, 0, panelSize.x, panelSize.y);
                    frameBuffer.Resize(panelSize.x, panelSize.y);
                }
                ImGui::Image(reinterpret_cast<void *>(static_cast<uintptr_t>(fboTextureID)),
                             ImVec2(panelSize.x, panelSize.y), ImVec2(0, 1), ImVec2(1, 0));

                ImGui::End();
            }
            ImGui::PopStyleVar();

            //Model
            {
                ImGui::Begin("Model");
                if(ImGui::DragFloat3("Position", glm::value_ptr(model_position), 0.1f,1.0f, 10.0f))
                    plane.Position(shaderProgram, model_position);
                    plane.Position(normalsShaderProgram, model_position);

                if(ImGui::DragFloat3("Rotation", glm::value_ptr(model_rotation), 0.1f, 0.0f, 360.0f))
                    plane.Rotation(shaderProgram, model_rotation);
                    plane.Rotation(normalsShaderProgram, model_rotation);
                //std::cout<<glm::value_ptr(model_position)<<std::endl;

                ImGui::End();
            }
            //Lightning
            {
                ImGui::Begin("Lightning");

                const char* lightTypes[] = { "Directional", "Point", "Spot" };

                if (ImGui::Combo("Light Type", &selectedLightType, lightTypes, IM_ARRAYSIZE(lightTypes)))
                {
                    GLuint light_loc = glGetUniformLocation(shaderProgram.ID, "type");
                    glUniform1i(light_loc,selectedLightType);
                }

                ImGui::End();
            }
            //Camera
            {
                ImGui::Begin("Camera");
                ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camera.Position.x, camera.Position.y, camera.Position.z);
                ImGui::Text("Camera Rotation: (%.2f, %.2f, %.2f)", camera.Orientation.x, camera.Orientation.y, camera.Orientation.z);
                if(ImGui::Button("Reset to preset")){
                    camera.Position = glm::vec3( 2.07479f , 1.249f, -0.381916f);
                    camera.Orientation = glm::vec3(-4.71458f, -1.41498f, 0.173473f);
                }
                if(ImGui::Button("Reset to 0")){
                    camera.Position = glm::vec3(0.0f);
                    camera.Orientation = glm::vec3(0.0f);
                }

                ImGui::Separator();
                const char* cameraType[] = {"Perspective","Orthographic"};
                if(ImGui::Combo("Camera type",&camType, cameraType, IM_ARRAYSIZE(cameraType))){
                    switch (camType) {
                        case 0:
                            camera.SetCameraMode(Camera::CamMode::Perspective);
                            break;
                        case 1:
                            camera.SetCameraMode(Camera::CamMode::Orthographic);
                            break;

                    }
                }

                ImGui::SeparatorText("View");

                if(camera.mode == Camera::CamMode::Perspective){
                    ImGui::DragFloat("FOV", &camera.fov, 1.0f, 5.0f, 180.0f);
                }

                ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.1f, 100.0f);
                ImGui::DragFloat("Far Plane", &camera.farPlane, 1.0f, 1.0f, 1000.0f);

                ImGui::SeparatorText("Controls");
                ImGui::DragFloat("Speed", &camera.speed, 0.01f, 0.01f, 5.0f);
                ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.5f, 1.0f, 100.0f);

                ImGui::End();
            }
            //Rendering
            {
                ImGui::Begin("Rendering");

                ImGui::SeparatorText("Informations");
                ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));

                ImGui::SeparatorText("Settings");
                ImGui::Checkbox("Debug Draw",&debug_draw);
                ImGui::Checkbox("VSYNC",&VSYNC);
                glfwSwapInterval((int)VSYNC);
                if(ImGui::Checkbox("Anti-Aliasing",&anti_aliasing)) {
                    if (anti_aliasing) {
                        if (ImGui::DragInt("Samples", &samples, 1.0f, 0, 100)) {
                            glfwWindowHint(GLFW_SAMPLES, samples);
                        }
                    }
                    if (anti_aliasing) {
                        //glfwWindowHint(GLFW_SAMPLES, samples);
                        glEnable(GL_MULTISAMPLE);
                    } else {
                        glDisable(GL_MULTISAMPLE);
                    }
                }
                ImGui::Checkbox("Face Culling",&face_culling);
                if(face_culling) {
                    glEnable(GL_CULL_FACE);


                    const char* cullFaces[] = {"Front face", "Back face"};
                    if(ImGui::Combo("Culling face", &cullFace, cullFaces, IM_ARRAYSIZE(cullFaces))){
                        switch (cullFace) {
                            case 1:
                                glCullFace(GL_BACK);
                                break;
                            case 0:
                                glCullFace(GL_FRONT);
                                break;
                        }
                    }


                    const char* cullTypes[] = {"CW","CCW"};
                    if(ImGui::Combo("FaceCulling Type", &cullType, cullTypes, IM_ARRAYSIZE(cullTypes))){
                        switch (cullType) {
                            case 0:
                                glFrontFace(GL_CW);
                                break;
                            case 1:
                                glFrontFace(GL_CCW);
                                break;
                        }
                    }

                }else{
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
    //skyboxShader.Delete();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
