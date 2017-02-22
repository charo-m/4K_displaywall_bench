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

#ifndef __TexQuad__proto__
#define __TexQuad__proto__

#include <iostream>
#include <vector>

// glad extension loader: https://github.com/Dav1dde/glad
#include <glad/glad.h>

// GLFW: glfw.org
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// OpenGL Math Library: glm.g-truc.net
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"


namespace proto {


class TexQuad {

public:
  TexQuad (const std::string &path,
           const float win_aspect = 16.0f/9.0f,
           const glm::vec3 &pos = glm::vec3(0.0f),
           const glm::vec2 &scale = glm::vec2(1.0f),
           bool mipmap = false,
           bool arrange = false
          );
  TexQuad (const float win_aspect = 16.0f/9.0f,
           const glm::vec3 &pos = glm::vec3(0.0f),
           const glm::vec2 &scale = glm::vec2(1.0f),
           bool mipmap = false,
           bool arrange = false
          );
  ~TexQuad ();

  void Setup ();
  void Update ();
  void Bind ();
  void Unbind ();

  void SetImageData (const std::vector<GLubyte> &imgdata,
                     unsigned int imgwidth,
                     unsigned int imgheight)
    { image_data = imgdata;
      img_width = imgwidth;
      img_height = imgheight;
      if (Util::compress)
        Compress ();
    }
  void SetViewMatrix (GLint uniform, const glm::mat4 &mat)
    { uniform_modelview = uniform;  view = mat; }

public:
  enum vertexAttrib {
    vPosition,
    vTexCoord
  };

protected:
  void Compress ();
  void Load ();
  void Unload ();

protected:
  std::string img_path;
  float window_aspect;
  glm::vec3 position;
  glm::vec2 scale;

  std::vector<GLubyte> image_data;
  unsigned int img_width;
  unsigned int img_height;

  GLint uniform_modelview;
  glm::mat4 view;
  GLuint texName;

  bool do_mipmap;
  bool do_arrange;

  int num_compressed_bytes;
  rgba_surface compressed_image;

};

}

#endif  // defined(__TexQuad__proto__)
