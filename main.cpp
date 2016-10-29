/* 4K Displaywall Bench
 * Copyright (c) 2016, Oblong Industries, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Scene.h>

#include <stdio.h>
#include <stdlib.h>

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB 0x8242

namespace proto {
  int width = 11520;
  int height = 2160;
  int swap_interval = 0;
  int gl_version = 4;
  bool do_arrange = false;
  bool do_mipmap = false;
  std::vector <std::string> image_paths;
  Scene *scene;
}

using namespace proto;

void help_msg (std::string prog)
{
  std::cout << prog << " [options]" << std::endl;
  std::cout << "    -width [w]" << std::endl;
  std::cout << "    -height [h]" << std::endl;
  std::cout << "    -swap_interval [0 (disable vsync) or 1 (enable vsync)]" << std::endl;
  std::cout << "    -gl [3 (OpenGL version 3.3) or 4 (OpenGL version 4.4, default)]" << std::endl;
  std::cout << "    -i [image path] (for each image you want to display)" << std::endl;
  std::cout << "    -layout [true | false] (true = grid, false = overlap)" << std::endl;
  std::cout << "    -mipmap [true | false]" << std::endl;
  exit (EXIT_SUCCESS);
}

void validate_args ()
{
  if (width < 0 || width > 32767)
    width = 640;
  if (height < 0 || height > 32767)
    height = 480;
  if (!(swap_interval == 0 || swap_interval == 1))
    swap_interval = 0;
  if (!(gl_version == 3 || gl_version == 4))
    gl_version = 4;
}

void parse_args (int argc, char* argv[])
{
  if (argc == 1)
    return;
  for (int i = 1; i < argc; ++i)
   {
     if (argv[i][0] == '-')
       {
         if (!strcmp(argv[i], "-width"))
           width = atoi(argv[i+1]);
         else if (!strcmp(argv[i], "-height"))
           height = atoi(argv[i+1]);
         else if (!strcmp(argv[i], "-swap_interval"))
           swap_interval = atoi(argv[i+1]);
         else if (!strcmp(argv[i], "-gl"))
           gl_version = atoi(argv[i+1]);
         else if (!strcmp(argv[i], "-i"))
           image_paths.push_back(argv[i+1]);
         else if (!strcmp(argv[i], "-layout"))
           do_arrange = (!strcmp(argv[i+1], "true")) ? true : false;
         else if (!strcmp(argv[i], "-mipmap"))
           do_mipmap = (!strcmp(argv[i+1], "true")) ? true : false;
         else if (!strcmp(argv[i], "-help") ||
                  !strcmp(argv[i], "--help"))
           help_msg (argv[0]);


         ++i;
       }
   }
}

// Callbacks
static void gl_debug_spewer (GLenum source, GLenum type, GLuint id, GLenum severity,
                          GLsizei length, const GLchar *msg, const void *data)
{
  printf ("gl debug spewer: %s\n", msg);
}
typedef void (APIENTRY *glDebugMessageCallbackARBPROC) (GLDEBUGPROCARB callback, void* userParam);
typedef void (APIENTRY *glDebugMessageControlARBPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);

static void error_callback (int error, const char* description)
{
  printf("Error: %s\n", description);
}

static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose (window, true);

  if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
      scene -> ResetZoom ();

  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
      scene -> IncrementZoom ();

  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
      scene -> DecrementZoom ();

}

static void cursor_pos_callback (GLFWwindow *window, double xpos, double ypos)
{
  //printf ("mouse pos %f  x  %f\n", xpos/width, (height - ypos)/height);
}

static void output_fps (GLFWwindow* window)
{
  static double t_start = glfwGetTime ();
  static int64_t frame_cnt = 0;
  const double reporting_interval = 1.0;  // in seconds

  double t_curr = glfwGetTime ();
  if (t_curr - t_start > reporting_interval)
  { std::cout << "frame count is " << frame_cnt << std::endl;
    double fps = static_cast<double>(frame_cnt) / (t_curr - t_start);
    std::ostringstream oss;
    oss << fps;
    std::string fps_string = "FPS: ";
    fps_string += oss.str();
    std::cout << fps_string << std::endl;
    Util::query_memory();
    glfwSetWindowTitle(window, fps_string.c_str());
    frame_cnt = 0;
    t_start = glfwGetTime();
  }
  else
  {
    frame_cnt++;
  }
}



int main (int argc, char* argv[])
{
  parse_args (argc, argv);
  validate_args ();

  glfwSetErrorCallback (error_callback);

  if (!glfwInit ())
      exit (EXIT_FAILURE);

  // Context configuration
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, gl_version);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, gl_version);
  glfwWindowHint (GLFW_CLIENT_API, GLFW_OPENGL_API);
  // Framebuffer
  glfwWindowHint (GLFW_DEPTH_BITS, 24);   // default
  glfwWindowHint (GLFW_DOUBLEBUFFER, true);   // default
  // Monitor
  glfwWindowHint (GLFW_REFRESH_RATE, GLFW_DONT_CARE);  // highest avail, default
#ifdef DEBUG
  glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

  GLFWwindow* window = glfwCreateWindow (width, height, "4K_displaywall_bench", NULL, NULL);
  if (!window)
    { glfwTerminate ();
      exit (EXIT_FAILURE);
    }

  glfwMakeContextCurrent (window);
  // glad replaces glew
  if (!gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress))
    printf ("Failed to set GLAD extension loader to glfwGetProcAddress");
  // enable vsync (disable for benchmarking, set to 0)
   glfwSwapInterval (swap_interval);

  glfwGetFramebufferSize (window, &width, &height);
  glViewport (0, 0, width, height);

  int mon_count;
  GLFWmonitor** monitors = glfwGetMonitors (&mon_count);
  for (int i = 0; i < mon_count; ++i)
  {
    const GLFWvidmode* vmode = glfwGetVideoMode(*monitors);
    std::cout << "video mode for monitor " << i << " reports " <<
	         vmode->refreshRate << " refresh rate" << std::endl;
    monitors++;
  }

  glfwSetKeyCallback (window, key_callback);
  glfwSetCursorPosCallback (window, cursor_pos_callback);

#ifdef DEBUG
  printf ("%s\n", glGetString (GL_VERSION));

  if (glfwExtensionSupported ("GL_ARB_debug_output")) {
    printf ("GL_ARB_debug_output is supported\n");
    glDebugMessageCallbackARBPROC setDebugMsgCallback = 0;
   setDebugMsgCallback =
     (glDebugMessageCallbackARBPROC) glfwGetProcAddress ("glDebugMessageCallbackARB");
   glDebugMessageControlARBPROC setDebugMsgControl = 0;
   setDebugMsgControl =
     (glDebugMessageControlARBPROC) glfwGetProcAddress ("glDebugMessageControlARB");
   if (setDebugMsgCallback && setDebugMsgControl)
     { glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
       setDebugMsgCallback (gl_debug_spewer, NULL);
       GLuint unusedIds = 0;
       setDebugMsgControl (GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
     }
  }
#endif

  scene = new Scene ();
  scene -> SetWindow (width, height);
  scene -> SetImagePaths (image_paths, do_arrange, do_mipmap);
  scene -> Setup ();

  static double t_start = glfwGetTime ();
  while (!glfwWindowShouldClose (window))
    { if (glfwGetTime () - t_start >= 1.0)
        t_start = glfwGetTime ();
      double t = glfwGetTime () - t_start;
      scene -> SetTime (t);
      scene -> Draw ();
      //output_fps (window);
      glfwSwapBuffers (window);
      glfwPollEvents ();
    }


  glfwDestroyWindow (window);

  delete scene;
  glfwTerminate ();
  exit (EXIT_SUCCESS);
}
