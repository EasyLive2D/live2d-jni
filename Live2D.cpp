#include "common.hpp"
#include "com_arkueid_live2d_Live2D.h"

static LAppAllocator _cubismAllocator;
static Csm::CubismFramework::Option _cubismOption;

std::mutex mapMutex;
std::unordered_map<size_t, LAppModel *> gModelMap;

/*
 * Class:     Live2D
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_init(JNIEnv *, jclass)
{
    _cubismOption.LogFunction = LAppPal::PrintLn;
    _cubismOption.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;

    Csm::CubismFramework::StartUp(&_cubismAllocator, &_cubismOption);
    Csm::CubismFramework::Initialize();
}

/*
 * Class:     Live2D
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_dispose(JNIEnv *, jclass)
{
    for (auto &pair : gModelMap)
    {
        delete pair.second;
        Info("[G] release: LAppModel(at=%p)", pair.second);
    }

    gModelMap.clear();

    Csm::CubismFramework::Dispose();
}

/*
 * Class:     Live2D
 * Method:    glewInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_glewInit(JNIEnv *, jclass)
{
    if (glewInit() != GLEW_OK)
    {
        Info("Can't initilize glew.");
    }
}

/*
 * Class:     Live2D
 * Method:    setGLProperties
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_setGLProperties(JNIEnv *, jclass)
{
    // テクスチャサンプリング設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 透過設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
 * Class:     Live2D
 * Method:    clearBuffer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_clearBuffer(JNIEnv *, jclass)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
}

/*
 * Class:     Live2D
 * Method:    setLogEnable
 * Signature: (Z)V
 */

extern bool live2dLogEnable;
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_setLogEnable(JNIEnv *, jclass, jboolean enable)
{
    live2dLogEnable = enable;
}
