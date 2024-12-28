package com.arkueid.demo

import com.arkueid.live2d.Live2D_v3


import org.lwjgl.opengl.*;

import org.lwjgl.glfw.GLFW.*;
import org.lwjgl.system.MemoryUtil


fun main() {

    val ww = 300
    val wh = 400
    glfwInit()
    val window = glfwCreateWindow(ww, wh, "Live2D_v3 Demo", MemoryUtil.NULL, MemoryUtil.NULL)
    glfwMakeContextCurrent(window)
    GL.createCapabilities()

    Live2D_v3.init()

    val model = Live2D_v3.LAppModel()
    model.loadModelJson("./Resources/v3/Haru/Haru.model3.json")
    model.resize(ww, wh)

    glfwSwapInterval(1)

    model.drag(200f, 200f)

    model.touch(200f, 200f, null, null)

    while (!glfwWindowShouldClose(window))
    {
        Live2D_v3.clearBuffer(0f, 0f, 0f, 0f)
        model.update()
        model.draw()

        glfwSwapBuffers(window)
        glfwPollEvents()
    }

    Live2D_v3.dispose()
    glfwDestroyWindow(window)
    glfwTerminate()
}