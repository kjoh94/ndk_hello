#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define WIDTH 600
#define HEIGHT 900

// YUV 파일에서 데이터를 읽어오는 함수
void readYUVFile(const char* filename, unsigned char* buffer, int size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Failed to open file %s\n", filename);
        return;
    }
    fread(buffer, 1, size, file); // 파일에서 데이터 읽기
    fclose(file); // 파일 닫기
}

#if 0
void renderFrame(GLuint textureId) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 화면 클리어 색상 설정
    glClear(GL_COLOR_BUFFER_BIT); // 색상 버퍼 클리어

    glUseProgram(program); // 프로그램 사용

    // 텍스처 바인딩
    glActiveTexture(GL_TEXTURE0); // 텍스처 단위 설정
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId); // 텍스처 바인딩

    // 텍스처를 사용하여 화면에 그리는 작업
    // 예: 사각형 그리기 등

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 사각형 그리기
}
#endif

int main() {
    // EGL 디스플레이 초기화
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, NULL, NULL);
    printf("eglInitialize\n");

    // EGL 설정 생성
    EGLint configAttributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, // 윈도우 서피스 설정
        EGL_RED_SIZE, 8, // 레드 색상 버퍼 비트 설정
        EGL_GREEN_SIZE, 8, // 그린 색상 버퍼 비트 설정
        EGL_BLUE_SIZE, 8, // 블루 색상 버퍼 비트 설정
        EGL_ALPHA_SIZE, 8, // 알파 색상 버퍼 비트 설정
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // OpenGL ES 2.0 사용 설정
        EGL_NONE // 설정 끝을 나타내는 값
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(display, configAttributes, &config, 1, &numConfigs); // 설정 선택

    // EGL 컨텍스트 생성
    EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2, // OpenGL ES 2.0 컨텍스트 생성
        EGL_NONE // 설정 끝을 나타내는 값
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
    printf("eglCreateContext\n");

    // EGL 표면 생성
    EGLNativeWindowType window = NULL; // 네이티브 윈도우 타입
    EGLSurface surface = eglCreateWindowSurface(display, config, window, NULL); // 윈도우 서피스 생성
    printf("eglCreateWindowSurface\n");

    // EGL 컨텍스트와 표면 연결
    eglMakeCurrent(display, surface, surface, context); // 현재 컨텍스트 설정
    printf("eglMakeCurrent\n");

    // OpenGL ES 초기화
    glViewport(0, 0, WIDTH, HEIGHT); // 뷰포트 설정
    printf("glViewport\n");

    // YUV 파일 읽기
    const char* filename = "shin2_600_900_nv21.yuv";
    int bufferSize = WIDTH * HEIGHT * 3 / 2; // YUV420 포맷
    unsigned char* buffer = (unsigned char*)malloc(bufferSize);
    readYUVFile(filename, buffer, bufferSize); // YUV 파일 읽기

    // EGL 이미지 생성
    EGLClientBuffer clientBuffer = buffer; // YUV 데이터 버퍼
    EGLImageKHR eglImage = eglCreateImageKHR(display, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuffer, 0); // EGL 이미지 생성

    #if 0
    // 텍스처 생성
    GLuint textureId;
    glGenTextures(1, &textureId); // 텍스처 생성
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId); // 텍스처 바인딩
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, eglImage); // EGL 이미지 텍스처로 지정

    // 메인 루프
    while (true) {
        renderFrame(textureId); // 화면 렌더링

        // 버퍼 스왑
        eglSwapBuffers(display, surface); // 버퍼 스왑
    }

    // EGL 리소스 해제
    eglDestroySurface(display, surface); // EGL 서피스 해제
    eglDestroyContext(display, context); // EGL 컨텍스트 해제
    eglTerminate(display); // EGL 종료

    // 메모리 해제
    free(buffer); // 메모리 해제
    #endif

    return 0;
}
