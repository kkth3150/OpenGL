#pragma once


// 해당 파일이 없다고 뜨면서 컴파일이 안된다면
// 프로젝트 - (이름)속성 - 링커 - 일반 - 추가 라이브러리 디렉토리 - 오른쪽에 점세개 - Dependencies 폴더 선택
// 확인 누른 뒤 상대경로로 바꿔준다 --> .\Dependencies 가 되도록!
#ifdef NDEBUG	// Release	일때
#	pragma comment (lib, "libglew32.lib")
#else			// Debug	일때
#   pragma comment (lib, "libglew32d.lib")
#endif


// 아래 헤더가 빨간줄이 뜬다면
// 프로젝트 - (이름)속성 - C/C++ - 일반 - 추가 포함 디렉토리 - 오른쪽에 점세개 - Dependencies 폴더 선택
// 확인 누른 뒤 상대경로로 바꿔준다 --> .\Dependencies 가 되도록!
#define GLEW_STATIC
#include "glew.h"

#define FREEGLUT_STATIC
#include "freeglut.h"
