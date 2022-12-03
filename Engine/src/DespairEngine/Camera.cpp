#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "DespairEngine/Camera.hpp"

namespace engine
{
	Camera::Camera( const glm::vec3& position,
					const glm::vec3& rotation,
					const ProjectionMode projection_mode) :
					m_position(position),
					m_rotation(rotation),
					m_projection_mode(projection_mode)
	{
		updateViewMatrix();
		updateProjectionMatrix();
	}

	const glm::mat4& Camera::getViewMatrix()
	{
		if (m_update_view_matrix)
		{
			updateViewMatrix();
		}
		return m_view_matrix;
	}

	void Camera::updateViewMatrix()
	{
		const float roll_in_radians = glm::radians(m_rotation.x);
		const float pitch_in_radians = glm::radians(m_rotation.y);
		const float yaw_in_radians = glm::radians(m_rotation.z);

		const glm::mat3 rotate_matrix_x(1, 0, 0,
			0, cos(roll_in_radians), sin(roll_in_radians),
			0, -sin(roll_in_radians), cos(roll_in_radians));

		const glm::mat3 rotate_matrix_y(cos(pitch_in_radians), 0, -sin(pitch_in_radians),
			0, 1, 0,
			sin(pitch_in_radians), 0, cos(pitch_in_radians));

		const glm::mat3 rotate_matrix_z(cos(yaw_in_radians), sin(yaw_in_radians), 0,
			-sin(yaw_in_radians), cos(yaw_in_radians), 0,
			0, 0, 1);

		const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
		m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
		m_right = glm::normalize(euler_rotate_matrix * s_world_right);
		m_up = glm::cross(m_right, m_direction);

		m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
	}
	void Camera::updateProjectionMatrix()
	{
		if (m_projection_mode == ProjectionMode::Perspective)
		{
			float r = 0.1f;
			float t = 0.1f;
			float f = 10;
			float n = 0.1f;
			m_projection_matrix = glm::mat4(n / r, 0, 0, 0,
											0, n / t, 0, 0,
											0, 0, (-f - n) / (f - n), -1,
											0, 0, -2 * f * n / (f - n), 0);
		}
		else
		{
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
											0, 1 / t, 0, 0,
											0, 0, -2 / (f - n), -1,
											0, 0, (-f - n) / (f - n), 1);
		}
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		m_position = position;
		m_update_view_matrix = true;
	}
	void Camera::setRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_update_view_matrix = true;
	}
	void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		m_update_view_matrix = true;
	}
	void Camera::setProjectionMode(const ProjectionMode projection_mode)
	{
		m_projection_mode = projection_mode;
		updateProjectionMatrix();
	}

	void Camera::moveForward(const float delta)
	{
		m_position += m_direction * delta;
		m_update_view_matrix = true;
	}

	void Camera::moveRight(const float delta)
	{
		m_position += m_right * delta;
		m_update_view_matrix = true;
	}

	void Camera::moveUp(const float delta)
	{
		m_position += m_up * delta;
		m_update_view_matrix = true;
	}

	void Camera::addMovementAndRotatition(const glm::vec3& movement_delta,
		const glm::vec3& rotation_delta)
	{
		m_position += m_direction * movement_delta.x;
		m_position += m_right * movement_delta.y;
		m_position += m_up * movement_delta.z;
		m_rotation += rotation_delta;
		m_update_view_matrix = true;
	}
}