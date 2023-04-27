#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <DespairEngine/Input.hpp>
#include <DespairEngine/Core.hpp>

class DespairEngineEditor : public engine::Core
{
    double m_initial_mouse_pos_x = 0.0;
    double m_initial_mouse_pos_y = 0.0;
    float camera_position[3] = { 0.f, 0.f, 1.f };
    float camera_rotation[3] = { 0.f, 0.f, 0.f };
    float camera_fov = 60.f;
    float camera_near_plane = 0.1f;
    float camera_far_plane = 100.f;
    bool perspective_camera = true;

	virtual void update() override
	{
        bool move_camera = false;
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };

        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_W))
        {
            movement_delta.x += 0.05f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_S))
        {
            movement_delta.x -= 0.05f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_A))
        {
            movement_delta.y -= 0.05f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_D))
        {
            movement_delta.y += 0.05f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_E))
        {
            movement_delta.z += 0.05f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_Q))
        {
            movement_delta.z -= 0.05f;
        }

        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_UP))
        {
            rotation_delta.y -= 0.5f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_DOWN))
        {
            rotation_delta.y += 0.5f;
            move_camera = true;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_RIGHT))
        {
            rotation_delta.z -= 0.5f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_LEFT))
        {
            rotation_delta.z += 0.5f;
        }

        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_P))
        {
            rotation_delta.x += 0.5f;
        }
        if (engine::Input::IsKeyPressed(engine::KeyCode::KEY_O))
        {
            rotation_delta.x -= 0.5f;
        }

        if (engine::Input::IsMouseButtonPressed(engine::MouseButton::MOUSE_BUTTON_RIGHT))
        {
            glm::vec2 current_cursor_position = getCurrentCursorPosition();
            if (engine::Input::IsMouseButtonPressed(engine::MouseButton::MOUSE_BUTTON_LEFT))
            {
                camera.moveRight(static_cast<float>(current_cursor_position.x - m_initial_mouse_pos_x) / 100.f);
                camera.moveUp(static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 100.f);
            }
            else
            {
                rotation_delta.z += static_cast<float>(m_initial_mouse_pos_x - current_cursor_position.x) / 5.f;
                rotation_delta.y -= static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 5.f;
            }
            m_initial_mouse_pos_x = current_cursor_position.x;
            m_initial_mouse_pos_y = current_cursor_position.y;
            hideMouseCursor();
        }
        camera.addMovementAndRotatition(movement_delta, rotation_delta);
	}

    void setupDockspaceMenu()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene...", NULL))
                {

                }
                if (ImGui::MenuItem("Open Scene...", NULL))
                {

                }
                if (ImGui::MenuItem("Save Scene...", NULL))
                {

                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", NULL))
                {
                    close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }


    virtual void onMouseButtonEvent(const engine::MouseButton button_code,
        const double x_pos,
        const double y_pos,
        const bool pressed) override
    {
        m_initial_mouse_pos_x = x_pos;
        m_initial_mouse_pos_y = y_pos;
    }

	virtual void onUIDraw() override
	{
        setupDockspaceMenu();

        camera_position[0] = camera.getPosition().x;
        camera_position[1] = camera.getPosition().y;
        camera_position[2] = camera.getPosition().z;
        camera_rotation[0] = camera.getPosition().x;
        camera_rotation[1] = camera.getPosition().y;
        camera_rotation[2] = camera.getPosition().z;

        camera_fov = camera.getFieldOfView();
        camera_near_plane = camera.getNearClipPlane();
        camera_far_plane = camera.getFarClipPlane();

		ImGui::Begin("Editor");
        ImGui::SliderFloat3("light source position", light_source_position, -10.f, 10.f);
        ImGui::ColorEdit3("light source color", light_source_color);

        ImGui::SliderFloat("ambient factor", &ambient_factor, 0.f, 1.f);
        ImGui::SliderFloat("diffuse factor", &diffuse_factor, 0.f, 1.f);
        ImGui::SliderFloat("specular factor", &specular_factor, 0.f, 1.f);
        ImGui::SliderFloat("shininess", &shininess, 1.f, 128.f);
        if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f))
        {
            camera.setPosition(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
        }
        if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f))
        {
            camera.setRotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        }
		ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
		ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
        if (ImGui::SliderFloat("camera FOV", &camera_fov, 1.f, 120.f))
        {
            camera.setFieldOfView(camera_fov);
        }
        if (ImGui::SliderFloat("camera near clip plane", &camera_near_plane, 0.1f, 10.f))
        {
            camera.setNearClipPlane(camera_near_plane);
        }
        if (ImGui::SliderFloat("camera far clip plane", &camera_far_plane, 1.f, 100.f))
        {
            camera.setFarClipPlane(camera_far_plane);
        }
        if (ImGui::Checkbox("Perspective camera", &perspective_camera))
        {
            camera.setProjectionMode(perspective_camera ? engine::Camera::ProjectionMode::Perspective : engine::Camera::ProjectionMode::Orthographic);
        }
		ImGui::End();
	}
};

#include <soil/src/SOIL.h>
#include <glad/include/glad/glad.h>

int main()
{
    GLuint textureID = SOIL_load_OGL_texture(
        "your_image.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	/*auto ptr_despairEngineEditor = std::make_unique<DespairEngineEditor>();
    int returnCode = ptr_despairEngineEditor->start("SimpleEngine Editor", 1024, 1024);
	return returnCode;*/
}