#include <iostream>
#include <imgui/imgui.h>

#include <DespairEngine/Input.hpp>

#include <DespairEngine/Core.hpp>

class DespairEngineEditor : public engine::Core
{
    double m_initial_mouse_pos_x = 0.0;
    double m_initial_mouse_pos_y = 0.0;

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
        }

        camera.addMovementAndRotatition(movement_delta, rotation_delta);
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
        camera_position[0] = camera.getCameraPosition().x;
        camera_position[1] = camera.getCameraPosition().y;
        camera_position[2] = camera.getCameraPosition().z;
        camera_rotation[0] = camera.getCameraRotation().x;
        camera_rotation[1] = camera.getCameraRotation().y;
        camera_rotation[2] = camera.getCameraRotation().z;

		ImGui::Begin("Editor");
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
		ImGui::Checkbox("Perspective camera", &perspective_camera);
		ImGui::End();
	}
};

int main()
{
	auto ptr_despairEngineEditor = std::make_unique<DespairEngineEditor>();
	int returnCode = ptr_despairEngineEditor->start("Despair Engine", 1080, 720);
	return returnCode;
}