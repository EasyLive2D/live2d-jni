#include "common.hpp"
#include "com_arkueid_live2d_Live2D_LAppModel.h"

extern std::mutex mapMutex;
extern std::unordered_map<size_t, LAppModel *> gModelMap;

static const char *handleFieldName = "nativeObjHandle";

// 获取 Java 对象中的 nativeHandle 字段
jlong getCppHandle(JNIEnv *env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, handleFieldName, "J"); // 获取 nativeHandle 字段ID
    return env->GetLongField(obj, fid);                        // 获取字段值（即 C++ 对象的指针）
}

/*
 * Class:     Live2D_LAppModel
 * Method:    createNativeObj
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_createNativeObj(JNIEnv *, jobject)
{
    // std::lock_guard<std::mutex> lock(mapMutex);
    LAppModelObject *self = new LAppModelObject;
    self->key = (size_t)self;
    self->model = new LAppModel();
    gModelMap[self->key] = self->model;
    self->matrixManager.Initialize();
    return self->key;
}

/*
 * Class:     Live2D_LAppModel
 * Method:    destroyNativeObj
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_destroyNativeObj(JNIEnv *env, jobject obj, jlong)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    // std::lock_guard<std::mutex> lock(mapMutex);
    if (gModelMap.find(self->key) != gModelMap.end())
    {
        gModelMap.erase(self->key);
        Info("[M] release: LAppModel(at=%p)", self->model);
        delete self->model;
    }

    delete self;
    Info("[M] deallocate: LAppModelObject(at=%p)", self);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    loadModelJson
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_loadModelJson(JNIEnv *env, jobject obj, jstring jstr)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    jboolean isCopy; // TODO: if necessary
    const char *fileName = env->GetStringUTFChars(jstr, &isCopy);
    self->model->LoadAssets(fileName);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    resize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_resize(JNIEnv *env, jobject obj, jint ww, jint wh)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->matrixManager.UpdateScreenToScene(ww, wh);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    draw
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_draw(JNIEnv *env, jobject obj)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    LAppPal::UpdateTime();
    self->model->Draw(self->matrixManager.GetProjection(self->model));
}

/*
 * Class:     Live2D_LAppModel
 * Method:    startMotion
 * Signature: (Ljava/lang/String;IILLive2D/LAppModel/OnStartMotionListener;LLive2D/LAppModel/OnFinishMotionListener;)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_startMotion(JNIEnv *env, jobject obj, jstring jgroup, jint no, jint priority, jobject onStartListener, jobject onFinishListener)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    jmethodID onStartMethodID = nullptr, onFinishMethodID = nullptr;
    // 获取方法ID
    if (onStartListener != nullptr)
    {
        jclass listenerClass = env->GetObjectClass(onStartListener);
        onStartMethodID = env->GetMethodID(listenerClass, "onStart", "(Ljava/lang/String;I)V");
    }

    if (onFinishListener != nullptr)
    {
        jclass listenerClass = env->GetObjectClass(onFinishListener);
        onFinishMethodID = env->GetMethodID(listenerClass, "onFinish", "()V");
    }

    auto onStart = [=](const char *group, int no)
    {
        if (onStartMethodID == nullptr)
            return;
        jstring groupStr = env->NewStringUTF(group);
        env->CallVoidMethod(onStartListener, onStartMethodID, groupStr, no);
        env->DeleteLocalRef(groupStr);
    };

    auto onFinish = [=](Csm::ACubismMotion *)
    {
        if (onFinishMethodID == nullptr)
            return;
        env->CallVoidMethod(onFinishListener, onFinishMethodID);
    };

    jboolean isCopy;
    const char *group = env->GetStringUTFChars(jgroup, &isCopy);
    Csm::CubismMotionQueueEntryHandle handle = self->model->StartMotion(group, no, priority, onStart, onFinish);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    startRandomMotion
 * Signature: (Ljava/lang/String;ILLive2D/LAppModel/OnStartMotionListener;LLive2D/LAppModel/OnFinishMotionListener;)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_startRandomMotion(JNIEnv *env, jobject obj, jstring jgroup, jint priority, jobject onStartListener, jobject onFinishListener)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    jmethodID onStartMethodID = nullptr, onFinishMethodID = nullptr;
    // 获取方法ID
    if (onStartListener != nullptr)
    {
        jclass listenerClass = env->GetObjectClass(onStartListener);
        onStartMethodID = env->GetMethodID(listenerClass, "onStart", "(Ljava/lang/String;I)V");
    }

    if (onFinishListener != nullptr)
    {
        jclass listenerClass = env->GetObjectClass(onFinishListener);
        onFinishMethodID = env->GetMethodID(listenerClass, "onFinish", "()V");
    }

    auto onStart = [=](const char *group, int no)
    {
        if (onStartMethodID == nullptr)
            return;
        jstring groupStr = env->NewStringUTF(group);
        env->CallVoidMethod(onStartListener, onStartMethodID, groupStr, no);
        env->DeleteLocalRef(groupStr);
    };

    auto onFinish = [=](Csm::ACubismMotion *)
    {
        if (onFinishMethodID == nullptr)
            return;
        env->CallVoidMethod(onFinishListener, onFinishMethodID);
    };

    jboolean isCopy;
    const char *group = env->GetStringUTFChars(jgroup, &isCopy);
    Csm::CubismMotionQueueEntryHandle handle = self->model->StartRandomMotion(group, priority, onStart, onFinish);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setExpression
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setExpression(JNIEnv *env, jobject obj, jstring jexpId)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    jboolean isCopy;
    const char *expressionId = env->GetStringUTFChars(jexpId, &isCopy);
    self->model->SetExpression(expressionId);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setRandomExpression
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setRandomExpression(JNIEnv *env, jobject obj)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->model->SetRandomExpression();
}

/*
 * Class:     Live2D_LAppModel
 * Method:    hitTest
 * Signature: (Ljava/lang/String;II)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_hitTest(JNIEnv *env, jobject obj, jfloat x, jfloat y)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    return env->NewStringUTF(self->model->HitTest(x, y).GetRawString());
}

/*
 * Class:     Live2D_LAppModel
 * Method:    hasMocConsistencyFromFile
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_hasMocConsistencyFromFile(JNIEnv *env, jobject obj, jstring jstr)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    jboolean isCopy;
    const char *fileName = env->GetStringUTFChars(jstr, &isCopy);
    return self->model->HasMocConsistencyFromFile(fileName);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    touch
 * Signature: (IILLive2D/LAppModel/OnStartMotionListener;LLive2D/LAppModel/OnFinishMotionListener;)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_touch(JNIEnv *env, jobject obj, jfloat x, jfloat y, jobject onStartListener, jobject onFinishListener)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    float xf = x;
    float yf = y;
    self->matrixManager.ScreenToScene(&xf, &yf);

    const char *hitArea = self->model->HitTest(xf, yf).GetRawString();
    if (strlen(hitArea) != 0)
    {
        Info("hit area: [%s]", hitArea);
        if (strcmp(hitArea, HIT_AREA_HEAD) == 0)
            self->model->SetRandomExpression();

        jmethodID onStartMethodID = nullptr, onFinishMethodID = nullptr;
        // 获取方法ID
        if (onStartListener != nullptr)
        {
            jclass listenerClass = env->GetObjectClass(onStartListener);
            onStartMethodID = env->GetMethodID(listenerClass, "onStart", "(Ljava/lang/String;I)V");
        }

        if (onFinishListener != nullptr)
        {
            jclass listenerClass = env->GetObjectClass(onFinishListener);
            onFinishMethodID = env->GetMethodID(listenerClass, "onFinish", "()V");
        }

        auto onStart = [=](const char *group, int no)
        {
            if (onStartMethodID == nullptr)
                return;
            jstring groupStr = env->NewStringUTF(group);
            env->CallVoidMethod(onStartListener, onStartMethodID, groupStr, no);
            env->DeleteLocalRef(groupStr);
        };

        auto onFinish = [=](Csm::ACubismMotion *)
        {
            if (onFinishMethodID == nullptr)
                return;
            env->CallVoidMethod(onFinishListener, onFinishMethodID);
        };
        self->model->StartRandomMotion(hitArea, MOTION_PRIORITY_FORCE, onStart, onFinish);
    }
}

/*
 * Class:     Live2D_LAppModel
 * Method:    drag
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_drag(JNIEnv *env, jobject obj, jfloat mx, jfloat my)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    float xf = (float)mx;
    float yf = (float)my;
    self->matrixManager.ScreenToScene(&xf, &yf);

    self->model->SetDragging(xf, yf);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    isMotionFinished
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_isMotionFinished(JNIEnv *env, jobject obj)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    return self->model->IsMotionFinished();
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setOffset
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setOffset(JNIEnv *env, jobject obj, float dx, float dy)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->matrixManager.SetOffset(dx, dy);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setScale
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setScale(JNIEnv *env, jobject obj, jfloat scale)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->matrixManager.SetScale(scale);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setParameterValue
 * Signature: (Ljava/lang/String;FF)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setParameterValue(JNIEnv *env, jobject obj, jstring jparamId, jfloat value, jfloat weight)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    jboolean isCopy;
    const char *paramId = env->GetStringUTFChars(jparamId, &isCopy);
    self->model->SetParameterValue(paramId, value, weight);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    addParameterValue
 * Signature: (Ljava/lang/String;F)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_addParameterValue(JNIEnv *env, jobject obj, jstring jparamId, jfloat value)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    jboolean isCopy;
    const char *paramId = env->GetStringUTFChars(jparamId, &isCopy);
    self->model->AddParameterValue(paramId, value);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    update
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_update(JNIEnv *env, jobject obj)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->model->Update();
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setAutoBreathEnable
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setAutoBreathEnable(JNIEnv *env, jobject obj, jboolean enable)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->model->SetAutoBreathEnable(enable);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    setAutoBlinkEnable
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_setAutoBlinkEnable(JNIEnv *env, jobject obj, jboolean enable)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    self->model->SetAutoBlinkEnable(enable);
}

/*
 * Class:     Live2D_LAppModel
 * Method:    GetParameterCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_getParameterCount(JNIEnv *env, jobject obj)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);
    return self->model->GetParameterCount();
}

/*
 * Class:     Live2D_LAppModel
 * Method:    GetParameter
 * Signature: (I)LLive2D/Parameter;
 */
JNIEXPORT jobject JNICALL Java_com_arkueid_live2d_Live2D_00024LAppModel_getParameter(JNIEnv *env, jobject obj, jint index)
{
    LAppModelObject *self = (LAppModelObject *)getCppHandle(env, obj);

    // 获取 Live2D.Parameter 类
    jclass parameterClass = env->FindClass("com/arkueid/live2d/Live2D$Parameter");
    if (parameterClass == nullptr)
    {
        return nullptr; // 找不到类，返回 null
    }

    // 获取默认构造函数的 MethodID
    jmethodID constructor = env->GetMethodID(parameterClass, "<init>", "()V");
    if (constructor == nullptr)
    {
        return nullptr; // 找不到构造函数，返回 null
    }

    // 创建 Live2D.Parameter 类的新实例
    jobject parameterObject = env->NewObject(parameterClass, constructor);
    if (parameterObject == nullptr)
    {
        return nullptr; // 无法创建对象，返回 null
    }

    // 获取各个字段的 ID
    jfieldID idField = env->GetFieldID(parameterClass, "id", "Ljava/lang/String;");
    jfieldID typeField = env->GetFieldID(parameterClass, "type", "I");
    jfieldID valueField = env->GetFieldID(parameterClass, "value", "F");
    jfieldID maxField = env->GetFieldID(parameterClass, "max", "F");
    jfieldID minField = env->GetFieldID(parameterClass, "min", "F");
    jfieldID defaultValueField = env->GetFieldID(parameterClass, "defaultValue", "F");

    Parameter param = self->model->GetParameter(index);
    // 设置字段的值
    env->SetObjectField(parameterObject, idField, env->NewStringUTF(param.id.c_str()));
    env->SetIntField(parameterObject, typeField, param.type);
    env->SetFloatField(parameterObject, valueField, param.value);
    env->SetFloatField(parameterObject, maxField, param.maxValue);
    env->SetFloatField(parameterObject, minField, param.minValue);
    env->SetFloatField(parameterObject, defaultValueField, param.defaultValue);

    // 返回创建的 Live2D.Parameter 对象
    return parameterObject;
}