#include "Camera.h"
#include "Define.h"


Camera::Camera() :
	Camera(0, 0, 1.0, 1)
{

}

Camera::Camera(int x, int y, double ex, int speed) {
	m_x = x;
	m_y = y;
	m_gx = x;
	m_gy = y;
	m_ex = ex;
	m_speed = 1;
	m_maxSpeed = speed == 0 ? CAMERA_SPEED_DEFAULT : speed;
	m_centerX = GAME_WIDE / 2;
	m_centerY = GAME_HEIGHT / 2;
}

Camera::Camera(const Camera* original) {
	m_x = original->getX();
	m_y = original->getY();
	m_gx = original->getGx();
	m_gy = original->getGy();
	m_ex = original->getEx();
	m_speed = original->getSpeed();
	m_maxSpeed = original->getMaxSpeed();
	m_centerX = GAME_WIDE / 2;
	m_centerY = GAME_HEIGHT / 2;
}

// �J�����̈ړ� �ڕW�n�_���߂��قǓ݊��ɂȂ�
void Camera::move() {
	if (m_x < m_gx) {
		m_x += (m_gx - m_x) / 2 + 1;
	}
	if (m_x > m_gx) {
		m_x -= (m_x - m_gx) / 2 + 1;
	}
	if (m_y < m_gy) {
		m_y += (m_gy - m_y) / 2 + 1;
	}
	if (m_y > m_gy) {
		m_y -= (m_y - m_gy) / 2 + 1;
	}
}

// �J�����ō��W�Ɗg�嗦�𒲐�����
void Camera::setCamera(int* x, int* y, double* ex) const {
	// �J��������̂�����v�Z
	int dx = *x - m_x;
	int dy = *y - m_y;

	// �g�嗦����ŋ������ς��
	dx = (int)(dx * m_ex);
	dy = (int)(dy * m_ex);
	
	// ��ʂ̒��S����ɍ��W������
	*x = m_centerX + dx;
	*y = m_centerY + dy;

	// �g�嗦
	*ex *= m_ex;
}

void Camera::getMouse(int* x, int* y) const {
	GetMousePoint(x, y);

	// ��ʒ�������̂�����v�Z
	int dx = *x - m_centerX;
	int dy = *y - m_centerY;

	// m_x��m_y�͉�ʒ����ɑΉ�����World�̍��W
	*x = m_x + dx;
	*y = m_y + dy;
}