#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


class Camera {
private:
	// �J�����̈ړ����x�̏����l
	const int CAMERA_SPEED_DEFAULT = 5;

	// �J�����̍��W(��ʂ̒��S)
	int m_x, m_y;

	// ���J�������������Ă�����W
	int m_gx, m_gy;

	// �J�����������X�s�[�h
	int m_speed;
	int m_maxSpeed;

	// �J�����̔{��
	double m_ex;

	// ��ʒ��S�̍��W
	int m_centerX, m_centerY;

public:
	Camera();
	Camera(int x, int y, double ex, int speed=0);
	Camera(const Camera* original);

	// �Q�b�^
	inline void getPoint(int* x, int* y) const { *x = m_x; *y = m_y; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getGx() const { return m_gx; }
	inline int getGy() const { return m_gy; }
	inline double getEx() const { return m_ex; }
	inline int getSpeed() const { return m_speed; }
	inline int getMaxSpeed() const { return m_maxSpeed; }

	// �Z�b�^
	inline void setPoint(int x, int y) { m_x = x; m_y = y; }
	inline void setGPoint(int x, int y) { m_gx = x; m_gy = y; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setGx(int x) { m_gx = x; }
	inline void setGy(int y) { m_gy = y; }
	inline void setSpeed(int speed) { m_speed = speed; }
	inline void setEx(double ex) { m_ex = ex; }

	// �J�����̓���
	void move();

	// �J�����̍��W���l�������`��ʒu�̕␳
	void setCamera(int* x, int* y, double* ex) const;

	// �}�E�X�̈ʒu���擾
	void getMouse(int* x, int* y) const;
};


#endif