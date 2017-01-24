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


#include "TexQuad.h"

#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util.h"

using namespace proto;

#define STORAGE


TexQuad::TexQuad (const std::string &path,
                  const float win_aspect,
                  const glm::vec3 &pos,
                  const glm::vec2 &sc,
                  bool mipmap,
                  bool manual,
                  bool arrange)
 : img_path(path),
   window_aspect(win_aspect),
   position(pos),
   scale(sc),
   do_mipmap(mipmap),
   do_manual_mipmap(manual),
   mip_data(6),
   do_arrange(arrange)
{
  if (do_mipmap && do_manual_mipmap) {
    std::cout << "Generating false color mip_data, size = " << mip_data.size() << std::endl;
    GenerateManualMips ();
  }
}

TexQuad::TexQuad (const float win_aspect,
                  const glm::vec3 &pos,
                  const glm::vec2 &sc,
                  bool mipmap,
                  bool manual,
                  bool arrange)
 : window_aspect(win_aspect),
   position(pos),
   scale(sc),
   do_mipmap(mipmap),
   do_manual_mipmap(manual),
   mip_data(6),
   do_arrange(arrange)
{
  if (do_mipmap && do_manual_mipmap) {
    std::cout << "Generating false color mip_data, size = " << mip_data.size() << std::endl;
    GenerateManualMips ();
  }
}

TexQuad::~TexQuad ()
{
  Unload ();
}

void TexQuad::GenerateManualMips ()
{ PFNGLTEXSTORAGE2DPROC glTexStorage2D = 0;
  glTexStorage2D = (PFNGLTEXSTORAGE2DPROC) glfwGetProcAddress ("glTexStorage2D");
  if (!glTexStorage2D)
    std::cout << "Problems loading glTexStorage2D extension" << std::endl;

  const GLubyte fill_colors[6][4] = {
  {255, 0, 0, 255},
  {255, 127, 0, 255},
  {255, 255, 0, 255},
  {0, 255, 0, 255},
  {0, 0, 255, 255},
  {127, 0, 255, 255}
  };
  const int dim = 4096;
  for (int i=0; i<6; i++) {
    mip_data[i].resize(dim*dim*4);
    auto iter = mip_data[i].begin();
    for(std::size_t k = 1; k < dim*dim; k++) {
      std::copy(fill_colors[i], fill_colors[i]+4, iter);
      std::advance(iter, 4);
    }
  }
}

void TexQuad::Load ()
{
  bool result = true;
  // Load texture: load image, generate texture, upload texture
  if (image_data.size() == 0)
    result = Util::decode_png_image(image_data, img_width, img_height, img_path);
  if (!result || image_data.size() == 0)
   {  std::cout << "No image data!" << std::endl;
      return;
   }


  glGenTextures (1, &texName);
  glBindTexture (GL_TEXTURE_2D, texName);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  GLint num_mips = 1;
  if (do_mipmap)
    { glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      num_mips = log2(std::min(img_width, img_height));
      //std::cout << "number of mips is " << num_mips << std::endl;
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, num_mips - 1);
    }
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef STORAGE
  glTexStorage2D (GL_TEXTURE_2D, num_mips, GL_RGBA8, img_width, img_height);
  glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, img_width, img_height, GL_BGRA, GL_UNSIGNED_BYTE, &image_data[0]);
#else
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, &image_data[0]);
#endif
  if (do_mipmap)
    { if (do_manual_mipmap)
        { int w = img_width / 2; int h = img_height / 2;  int l = 0;
          while (w > 1 && h > 1)
           { l++;
             #ifdef STORAGE
             glTexSubImage2D (GL_TEXTURE_2D, l, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, &mip_data[l%6][0]);
             #else
             glTexImage2D (GL_TEXTURE_2D, l, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, &mip_data[l%6][0]);
             #endif
             //std::cout << "uploaded level " << l << "   " << w << "x" << h << std::endl;
             w *= 0.5;
             h *= 0.5;
           }
        }
      else
        { glGenerateMipmap (GL_TEXTURE_2D); }
    }
  glBindTexture (GL_TEXTURE_2D, 0);

}

void TexQuad::Unload ()
{ glDeleteTextures (1, &texName);
}

void TexQuad::Setup ()
{ Load ();
}

void TexQuad::Update ()
{
  // update shader uniforms
  float img_aspect = static_cast<float>(img_width)/static_cast<float>(img_height);
  glm::mat4 scale_mat = glm::scale (glm::mat4 (1.0f), glm::vec3 (scale.x, scale.y, 1.0f));
  if (!do_arrange)
   { if (window_aspect > 1.0f)
       scale_mat = glm::scale (glm::mat4 (1.0f), glm::vec3 (scale.x*img_aspect, scale.y, 1.0f));
     else
       scale_mat = glm::scale (glm::mat4 (1.0f), glm::vec3 (scale.x, scale.y/img_aspect, 1.0f));
   }
  glm::mat4 model = glm::translate (scale_mat, position);
  glm::mat4 modelView = view * model;
  glUniformMatrix4fv (uniform_modelview, 1, GL_FALSE, glm::value_ptr(modelView));
}

void TexQuad::Bind ()
{
  glActiveTexture (GL_TEXTURE0);
  glBindTexture (GL_TEXTURE_2D, texName);
}

void TexQuad::Unbind ()
{
  glBindTexture (GL_TEXTURE_2D, 0);
}

