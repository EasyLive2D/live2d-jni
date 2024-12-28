#include "com_arkueid_live2d_Live2D_v3_LAppModel.h"
#include <jni.h>
#include <LAppModel.hpp>
#include <CubismFramework.hpp>
//
// Created by ArcueidBrunestud on 2024/12/27.
//

static LAppModel *getModel(JNIEnv *env, jobject thiz) {
    jclass jLAppClass = env->GetObjectClass(thiz);
    jfieldID jNativeModelFieldID = env->GetFieldID(jLAppClass, "nativeModel", "J");
    return reinterpret_cast<LAppModel *>(env->GetLongField(thiz, jNativeModelFieldID));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_createNativeModel(JNIEnv *, jobject) {
    auto *model = new LAppModel();
    return reinterpret_cast<long long>(model);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_loadModelJson(JNIEnv *env, jobject thiz,
                                                                jstring file_name) {
    const char *json_file = env->GetStringUTFChars(file_name, nullptr);
    getModel(env, thiz)->LoadAssets(json_file);
    env->ReleaseStringUTFChars(file_name, json_file);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_destroyNativeModel(JNIEnv *env, jclass clazz,
                                                                     jlong model) {
    delete reinterpret_cast<LAppModel *>(model);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_resize(JNIEnv *env, jobject thiz, jint ww,
                                                         jint wh) {
    getModel(env, thiz)->Resize(ww, wh);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_draw(JNIEnv *env, jobject thiz) {
    getModel(env, thiz)->Draw();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_startMotion(JNIEnv *env, jobject thiz,
                                                              jstring group, jint no, jint priority,
                                                              jobject on_start_motion_handler,
                                                              jobject on_finish_motion_handler) {
    jmethodID onStartMethodID = nullptr, onFinishMethodID = nullptr;
    // 获取方法ID
    if (on_start_motion_handler != nullptr) {
        jclass listenerClass = env->GetObjectClass(on_start_motion_handler);
        onStartMethodID = env->GetMethodID(listenerClass, "onStarted", "(Ljava/lang/String;I)V");
    }

    if (on_finish_motion_handler != nullptr) {
        jclass listenerClass = env->GetObjectClass(on_finish_motion_handler);
        onFinishMethodID = env->GetMethodID(listenerClass, "onFinished", "()V");
    }

    jobject start_ref = env->NewGlobalRef(on_start_motion_handler);
    auto s_call = [=](Csm::ACubismMotion *motion) {
        if (onStartMethodID == nullptr)
            return;
        jstring groupStr = env->NewStringUTF(motion->group.c_str());
        env->CallVoidMethod(start_ref, onStartMethodID, groupStr, motion->no);
        env->DeleteLocalRef(groupStr);
        env->DeleteGlobalRef(start_ref);
    };

    jobject finish_ref = env->NewGlobalRef(on_finish_motion_handler);
    auto f_call = [=](Csm::ACubismMotion *) {
        if (onFinishMethodID == nullptr)
            return;
        env->CallVoidMethod(finish_ref, onFinishMethodID);
        env->DeleteGlobalRef(finish_ref);
    };

    const char *group_cstr = env->GetStringUTFChars(group, nullptr);
    getModel(env, thiz)->StartMotion(group_cstr, no, priority, s_call, f_call);
    env->ReleaseStringUTFChars(group, group_cstr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_startRandomMotion(JNIEnv *env, jobject thiz,
                                                                    jstring group, jint priority,
                                                                    jobject on_start_motion_handler,
                                                                    jobject on_finish_motion_handler) {
    jmethodID onStartMethodID = nullptr, onFinishMethodID = nullptr;
    // 获取方法ID
    if (on_start_motion_handler != nullptr) {
        jclass listenerClass = env->GetObjectClass(on_start_motion_handler);
        onStartMethodID = env->GetMethodID(listenerClass, "onStarted", "(Ljava/lang/String;I)V");
    }

    if (on_finish_motion_handler != nullptr) {
        jclass listenerClass = env->GetObjectClass(on_finish_motion_handler);
        onFinishMethodID = env->GetMethodID(listenerClass, "onFinished", "()V");
    }

    jobject start_ref = env->NewGlobalRef(on_start_motion_handler);
    auto s_call = [=](Csm::ACubismMotion *motion) {
        if (onStartMethodID == nullptr)
            return;
        jstring groupStr = env->NewStringUTF(motion->group.c_str());
        env->CallVoidMethod(start_ref, onStartMethodID, groupStr, motion->no);
        env->DeleteLocalRef(groupStr);
        env->DeleteGlobalRef(start_ref);
    };

    jobject finish_ref = env->NewGlobalRef(on_finish_motion_handler);
    auto f_call = [=](Csm::ACubismMotion *) {
        if (onFinishMethodID == nullptr)
            return;
        env->CallVoidMethod(finish_ref, onFinishMethodID);
        env->DeleteGlobalRef(finish_ref);
    };

    const char *group_cstr = env->GetStringUTFChars(group, nullptr);
    getModel(env, thiz)->StartRandomMotion(group_cstr, priority, s_call, f_call);
    env->ReleaseStringUTFChars(group, group_cstr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setExpression(JNIEnv *env, jobject thiz,
                                                                jstring expression_id) {
    const char *c_exp_id = env->GetStringUTFChars(expression_id, nullptr);
    getModel(env, thiz)->SetExpression(c_exp_id);
    env->ReleaseStringUTFChars(expression_id, c_exp_id);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setRandomExpression(JNIEnv *env, jobject thiz) {
    getModel(env, thiz)->SetRandomExpression();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_hitTest(JNIEnv *env, jobject thiz, jfloat x,
                                                          jfloat y) {
    const char *hit_area = getModel(env, thiz)->HitTest(x, y).GetRawString();
    return env->NewStringUTF(hit_area);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_hasMocConsistencyFromFile(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring moc_file_name) {
    const char *c_moc_fn = env->GetStringUTFChars(moc_file_name, nullptr);
    return getModel(env, thiz)->HasMocConsistencyFromFile(c_moc_fn);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_touch(JNIEnv *env, jobject thiz, jfloat x,
                                                        jfloat y, jobject on_start_motion_handler,
                                                        jobject on_finish_motion_handler) {
    jmethodID onStartMethodID = nullptr, onFinishMethodID = nullptr;
    // 获取方法ID
    if (on_start_motion_handler != nullptr) {
        jclass listenerClass = env->GetObjectClass(on_start_motion_handler);
        onStartMethodID = env->GetMethodID(listenerClass, "onStarted", "(Ljava/lang/String;I)V");
    }

    if (on_finish_motion_handler != nullptr) {
        jclass listenerClass = env->GetObjectClass(on_finish_motion_handler);
        onFinishMethodID = env->GetMethodID(listenerClass, "onFinished", "()V");
    }

    jobject start_ref = env->NewGlobalRef(on_start_motion_handler);
    auto s_call = [=](Csm::ACubismMotion *motion) {
        if (onStartMethodID == nullptr)
            return;
        jstring groupStr = env->NewStringUTF(motion->group.c_str());
        env->CallVoidMethod(start_ref, onStartMethodID, groupStr, motion->no);
        env->DeleteLocalRef(groupStr);
        env->DeleteGlobalRef(start_ref);
    };

    jobject finish_ref = env->NewGlobalRef(on_finish_motion_handler);
    auto f_call = [=](Csm::ACubismMotion *) {
        if (onFinishMethodID == nullptr)
            return;
        env->CallVoidMethod(finish_ref, onFinishMethodID);
        env->DeleteGlobalRef(finish_ref);
    };

    getModel(env, thiz)->Touch(x, y, s_call, f_call);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_drag(JNIEnv *env, jobject thiz, jfloat x,
                                                       jfloat y) {
    getModel(env, thiz)->Drag(x, y);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_isMotionFinished(JNIEnv *env, jobject thiz) {
    return getModel(env, thiz)->IsMotionFinished();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setOffset(JNIEnv *env, jobject thiz, jfloat dx,
                                                            jfloat dy) {
    getModel(env, thiz)->SetOffset(dx, dy);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setScale(JNIEnv *env, jobject thiz,
                                                           jfloat scale) {
    getModel(env, thiz)->SetScale(scale);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setParameterValue(JNIEnv *env, jobject thiz,
                                                                    jstring param_id, jfloat value,
                                                                    jfloat weight) {
    const char *cpid = env->GetStringUTFChars(param_id, nullptr);
    getModel(env, thiz)->SetParameterValue(cpid, value, weight);
    env->ReleaseStringUTFChars(param_id, cpid);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_addParameterValue(JNIEnv *env, jobject thiz,
                                                                    jstring param_id,
                                                                    jfloat value) {
    const char *cpid = env->GetStringUTFChars(param_id, nullptr);
    getModel(env, thiz)->AddParameterValue(cpid, value);
    env->ReleaseStringUTFChars(param_id, cpid);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_update(JNIEnv *env, jobject thiz) {
    getModel(env, thiz)->Update();

}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setAutoBreathEnable(JNIEnv *env, jobject thiz,
                                                                      jboolean enable) {
    getModel(env, thiz)->SetAutoBreathEnable(enable);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setAutoBlinkEnable(JNIEnv *env, jobject thiz,
                                                                     jboolean enable) {
    getModel(env, thiz)->SetAutoBlinkEnable(enable);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getParameterCount(JNIEnv *env, jobject thiz) {
    return getModel(env, thiz)->GetParameterCount();
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getParameter(JNIEnv *env, jobject thiz,
                                                               jint index) {
    jclass parameterClass = env->FindClass("com/arkueid/live2d/Live2D_v3$Parameter");

    jmethodID constructor = env->GetMethodID(parameterClass, "<init>", "()V");
    jobject parameterObject = env->NewObject(parameterClass, constructor);
    // 获取各个字段的 ID
    jfieldID idField = env->GetFieldID(parameterClass, "id", "Ljava/lang/String;");
    jfieldID typeField = env->GetFieldID(parameterClass, "type", "I");
    jfieldID valueField = env->GetFieldID(parameterClass, "value", "F");
    jfieldID maxField = env->GetFieldID(parameterClass, "max", "F");
    jfieldID minField = env->GetFieldID(parameterClass, "min", "F");
    jfieldID defaultValueField = env->GetFieldID(parameterClass, "defaultValue", "F");

    const char *id;
    int type;
    float value, maxValue, minValue, defaultValue;
    getModel(env, thiz)->GetParameter(index, id, type, value, maxValue, minValue, defaultValue);
    // 设置字段的值
    env->SetObjectField(parameterObject, idField, env->NewStringUTF(id));
    env->SetIntField(parameterObject, typeField, type);
    env->SetFloatField(parameterObject, valueField, value);
    env->SetFloatField(parameterObject, maxField, maxValue);
    env->SetFloatField(parameterObject, minField, minValue);
    env->SetFloatField(parameterObject, defaultValueField, defaultValue);

    // 返回创建的 Live2D.Parameter 对象
    return parameterObject;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getPartCount(JNIEnv *env, jobject thiz) {
    return getModel(env, thiz)->GetPartCount();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getPartId(JNIEnv *env, jobject thiz, jint index) {
    const Csm::csmString &partId = getModel(env, thiz)->GetPartId(index);
    return env->NewStringUTF(partId.GetRawString());
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getPartIds(JNIEnv *env, jobject thiz) {
    LAppModel *model = getModel(env, thiz);
    int length = model->GetPartCount();
    jclass jstrClass = env->FindClass("java/lang/String");
    jobjectArray array = env->NewObjectArray(length, jstrClass, nullptr);
    for (int i = 0; i < length; i++) {
        env->SetObjectArrayElement(array, i, env->NewStringUTF(model->GetPartId(i).GetRawString()));
    }
    return array;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setPartOpacity(JNIEnv *env, jobject thiz,
                                                                 jint index, jfloat opacity) {
    getModel(env, thiz)->SetPartOpacity(index, opacity);
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_hitPart(JNIEnv *env, jobject thiz, jfloat x,
                                                          jfloat y, jboolean top_only) {
    std::vector<std::string> partIds;
    getModel(env, thiz)->HitPart(x, y, top_only, partIds);

    int length = (int) partIds.size();
    jclass jstrClass = env->FindClass("java/lang/String");
    jobjectArray array = env->NewObjectArray(length, jstrClass, nullptr);
    for (int i = 0; i < length; i++) {
        env->SetObjectArrayElement(array, i, env->NewStringUTF(partIds[i].c_str()));
    }
    return array;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setPartScreenColor(JNIEnv *env, jobject thiz,
                                                                     jint part_index, jfloat r,
                                                                     jfloat g, jfloat b, jfloat a) {
    getModel(env, thiz)->setPartScreenColor(part_index, r, g, b, a);
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getPartScreenColor(JNIEnv *env, jobject thiz,
                                                                     jint part_index) {
    jfloatArray array = env->NewFloatArray(4);
    float r, g, b, a;
    getModel(env, thiz)->getPartScreenColor(part_index, r, g, b, a);
    env->SetFloatArrayRegion(array, 0, 1, &r);
    env->SetFloatArrayRegion(array, 1, 1, &g);
    env->SetFloatArrayRegion(array, 2, 1, &b);
    env->SetFloatArrayRegion(array, 3, 1, &a);
    return array;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_setPartMultiplyColor(JNIEnv *env, jobject thiz,
                                                                       jint part_index, jfloat r,
                                                                       jfloat g, jfloat b,
                                                                       jfloat a) {
    getModel(env, thiz)->setPartMultiplyColor(part_index, r, g, b, a);
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_arkueid_live2d_Live2D_1v3_00024LAppModel_getPartMultiplyColor(JNIEnv *env, jobject thiz,
                                                                       jint part_index) {
    jfloatArray array = env->NewFloatArray(4);
    float r, g, b, a;
    getModel(env, thiz)->getPartMultiplyColor(part_index, r, g, b, a);
    env->SetFloatArrayRegion(array, 0, 1, &r);
    env->SetFloatArrayRegion(array, 1, 1, &g);
    env->SetFloatArrayRegion(array, 2, 1, &b);
    env->SetFloatArrayRegion(array, 3, 1, &a);
    return array;
}