#include "library.h"
#include <iostream>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char key, int x, int y);

void TimerFunction(int value);
float RGB[3] = { 0.f,0.f,0.f };
bool timerON = false;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600);// 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정

	glutKeyboardFunc(KeyBoard);

	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{

	glClearColor(RGB[0], RGB[1], RGB[2], 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid KeyBoard(unsigned char key, int x, int y) {



	switch (key) {
	case 'c':
		RGB[0] = 0.f;
		RGB[1] = 1.f;
		RGB[2] = 1.f;
		break;
	case 'm':
		RGB[0] = 1.f;
		RGB[1] = 0.f;
		RGB[2] = 1.f;
		break;
	case 'y':
		RGB[0] = 1.f;
		RGB[1] = 1.f;
		RGB[2] = 0.f;
		break;
	case 'a':
		RGB[0] = (float)(rand() % 100) / 100;
		RGB[1] = (float)(rand() % 100) / 100;
		RGB[2] = (float)(rand() % 100) / 100;
		break;
	case 'w':
		RGB[0] = 1.f;
		RGB[1] = 1.f;
		RGB[2] = 1.f;
		break;
	case 'k':
		RGB[0] = 0.f;
		RGB[1] = 0.f;
		RGB[2] = 0.f;
		break;
	case 't':
		timerON = true;
		glutTimerFunc(1000, TimerFunction, 1);
		break;
	case 's':
		timerON = false;
		break;
	case 'q':
		return;
	}
	glutPostRedisplay();

}

void TimerFunction(int value)
{
	if (timerON) {
		RGB[0] = (float)(rand() % 100) / 100;
		RGB[1] = (float)(rand() % 100) / 100;
		RGB[2] = (float)(rand() % 100) / 100;
		glutPostRedisplay(); // 화면 재 출력
		glutTimerFunc(1000, TimerFunction, 1); // 타이머함수 재 설정
	}
}