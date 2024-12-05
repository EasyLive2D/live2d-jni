package com.arkueid.demo

import com.arkueid.live2d.Live2D


import org.lwjgl.opengl.*;

import org.lwjgl.glfw.GLFW.*;


fun main() {

    val ww = 300
    val wh = 400
    glfwInit()
    val window = glfwCreateWindow(ww, wh, "Live2D Demo", 0, 0)
    glfwMakeContextCurrent(window)

    Live2D.init()
    Live2D.glewInit()
    Live2D.setGLProperties()

    val model = Live2D.LAppModel()
    model.loadModelJson("./Resources/v3/Haru/Haru.model3.json")
    model.resize(ww, wh)

    GL.createCapabilities()

    glfwSwapInterval(1)

    while (!glfwWindowShouldClose(window))
    {
        Live2D.clearBuffer()
        model.update()
        model.draw()

        glfwSwapBuffers(window)
        glfwPollEvents()
    }

    Live2D.dispose()
    glfwDestroyWindow(window)
    glfwTerminate()
}