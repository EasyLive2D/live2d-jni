package com.arkueid.live2d;


import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

/**
 * @author: Arkueid
 * @date: 2024/12/27
 * @desc:
 */
public class Live2D_v3 {

    public static void loadLibraryFromResources(String libraryName) throws IOException {
        ClassLoader classLoader = Live2D_v3.class.getClassLoader();

        try (InputStream inputStream = classLoader.getResourceAsStream(libraryName)) {
            if (inputStream == null) {
                throw new IllegalArgumentException("Library not found: " + libraryName);
            }


            Path tempFile = Files.createTempFile("lib", ".dll");
            tempFile.toFile().deleteOnExit(); 


            Files.copy(inputStream, tempFile, StandardCopyOption.REPLACE_EXISTING);


            System.load(tempFile.toAbsolutePath().toString());
        }
    }

    static {
        try {
            loadLibraryFromResources("live2d.dll");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static final class MotionPriority {
        public static final int NONE = 0;
        public static final int IDLE = 1;
        public static final int NORMAL = 2;
        public static final int FORCE = 3;
    }

    public static final class MotionGroup {
        public static final String IDLE = "Idle";
        public static final String TAP_HEAD = "TapHead";
    }

    public static final class HitArea {
        public static final String HEAD = MotionGroup.TAP_HEAD;
    }

    public static final class StandardParams {
        public static final String ParamAngleX = "ParamAngleX";
        public static final String ParamAngleY = "ParamAngleY";
        public static final String ParamAngleZ = "ParamAngleZ";
        public static final String ParamEyeLOpen = "ParamEyeLOpen";
        public static final String ParamEyeLSmile = "ParamEyeLSmile";
        public static final String ParamEyeROpen = "ParamEyeROpen";
        public static final String ParamEyeRSmile = "ParamEyeRSmile";
        public static final String ParamEyeBallX = "ParamEyeBallX";
        public static final String ParamEyeBallY = "ParamEyeBallY";
        public static final String ParamEyeBallForm = "ParamEyeBallForm";
        public static final String ParamBrowLY = "ParamBrowLY";
        public static final String ParamBrowRY = "ParamBrowRY";
        public static final String ParamBrowLX = "ParamBrowLX";
        public static final String ParamBrowRX = "ParamBrowRX";
        public static final String ParamBrowLAngle = "ParamBrowLAngle";
        public static final String ParamBrowRAngle = "ParamBrowRAngle";
        public static final String ParamBrowLForm = "ParamBrowLForm";
        public static final String ParamBrowRForm = "ParamBrowRForm";
        public static final String ParamMouthForm = "ParamMouthForm";
        public static final String ParamMouthOpenY = "ParamMouthOpenY";
        public static final String ParamCheek = "ParamCheek";
        public static final String ParamBodyAngleX = "ParamBodyAngleX";
        public static final String ParamBodyAngleY = "ParamBodyAngleY";
        public static final String ParamBodyAngleZ = "ParamBodyAngleZ";
        public static final String ParamBreath = "ParamBreath";
        public static final String ParamArmLA = "ParamArmLA";
        public static final String ParamArmRA = "ParamArmRA";
        public static final String ParamArmLB = "ParamArmLB";
        public static final String ParamArmRB = "ParamArmRB";
        public static final String ParamHandL = "ParamHandL";
        public static final String ParamHandR = "ParamHandR";
        public static final String ParamHairFront = "ParamHairFront";
        public static final String ParamHairSide = "ParamHairSide";
        public static final String ParamHairBack = "ParamHairBack";
        public static final String ParamHairFluffy = "ParamHairFluffy";
        public static final String ParamShoulderY = "ParamShoulderY";
        public static final String ParamBustX = "ParamBustX";
        public static final String ParamBustY = "ParamBustY";
        public static final String ParamBaseX = "ParamBaseX";
        public static final String ParamBaseY = "ParamBaseY";
    }

    public static final class Parameter {
        public String id;
        public int type;
        public float value;
        public float max;
        public float min;
        public float defaultValue;
    }

    public static native void init();

    public static native void dispose();

    public static native void clearBuffer(float r, float g, float b, float a);

    private static final String TAG = "Live2D_v3";

    public static interface OnBeganMotionCallback {
        void onStarted(String group, int no);
    }

    public static interface OnFinishedMotionCallback {
        void onFinished();
    }

    public static class LAppModel {
        private long nativeModel;

        private native long createNativeModel();

        private static native void destroyNativeModel(long model);

        public native void loadModelJson(String fileName);

        public native void resize(int ww, int wh);

        public LAppModel() {
            nativeModel = createNativeModel();
        }

        @Override
        protected void finalize() throws Throwable {
            destroyNativeModel(nativeModel);
            nativeModel = 0;
        }

        public native void draw();

        public native void startMotion(String group, int no, int priority, OnBeganMotionCallback onStartMotionHandler, OnFinishedMotionCallback onFinishMotionHandler);

        public native void startRandomMotion(String group, int priority, OnBeganMotionCallback onStartMotionHandler, OnFinishedMotionCallback onFinishMotionHandler);

        public native void setExpression(String expressionID);

        public native void setRandomExpression();

        public native String hitTest(float x, float y);

        public native boolean hasMocConsistencyFromFile(String mocFileName);

        public native void touch(float x, float y, OnBeganMotionCallback onStartMotionHandler, OnFinishedMotionCallback onFinishMotionHandler);

        public native void drag(float x, float y);

        public native boolean isMotionFinished();

        public native void setOffset(float dx, float dy);

        public native void setScale(float scale);

        public native void setParameterValue(String paramId, float value, float weight);

        public native void addParameterValue(String paramId, float value);

        public native void update();

        public native void setAutoBreathEnable(boolean enable);

        public native void setAutoBlinkEnable(boolean enable);

        public native int getParameterCount();

        public native Parameter getParameter(int index);

        public native int getPartCount();

        public native String getPartId(int index);

        public native String[] getPartIds();

        public native void setPartOpacity(int index, float opacity);

        public native String[] hitPart(float x, float y, boolean topOnly);

        public native void setPartScreenColor(int partIndex, float r, float g, float b, float a);

        public native float[] getPartScreenColor(int partIndex);

        public native void setPartMultiplyColor(int partIndex, float r, float g, float b, float a);

        public native float[] getPartMultiplyColor(int partIndex);
    }
}
