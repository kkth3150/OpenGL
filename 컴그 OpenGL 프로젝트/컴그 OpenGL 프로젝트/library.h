#pragma once


// �ش� ������ ���ٰ� �߸鼭 �������� �ȵȴٸ�
// ������Ʈ - (�̸�)�Ӽ� - ��Ŀ - �Ϲ� - �߰� ���̺귯�� ���丮 - �����ʿ� ������ - Dependencies ���� ����
// Ȯ�� ���� �� ����η� �ٲ��ش� --> .\Dependencies �� �ǵ���!
#ifdef NDEBUG	// Release	�϶�
#	pragma comment (lib, "libglew32.lib")
#else			// Debug	�϶�
#   pragma comment (lib, "libglew32d.lib")
#endif


// �Ʒ� ����� �������� ��ٸ�
// ������Ʈ - (�̸�)�Ӽ� - C/C++ - �Ϲ� - �߰� ���� ���丮 - �����ʿ� ������ - Dependencies ���� ����
// Ȯ�� ���� �� ����η� �ٲ��ش� --> .\Dependencies �� �ǵ���!
#define GLEW_STATIC
#include "glew.h"

#define FREEGLUT_STATIC
#include "freeglut.h"
