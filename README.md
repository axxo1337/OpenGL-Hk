# OpenGL-Hk

## What is this?

A proof of concept (POC) OpenGL hook I made because many people came to me for question regarding a video I had released <br />
on YouTube about it.

![Screenshot](/repo/screenshot.png "Screenshot")

## How does it work?

It works by hooking the `wglSwapBuffers` function from OpenGL's library exports. <br />

DISCLAMER: Trying to compile this project for 32-bit using the included libraries will not work. Though the process of hooking OpenGL remains the same.
