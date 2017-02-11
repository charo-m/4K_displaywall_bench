
#ifndef __Util__proto__
#define __Util__proto__

#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

#include <png/lodepng.h>


#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX 0x904B
#ifndef GL_TEXTURE_FREE_MEMORY_ATI
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#endif

namespace proto {


class Util {

public:
static bool in_test_mode;
static bool blit;
static int blit_rect[4];

public:
static bool compileShader (GLuint &shader, GLenum type,
                             const char *src)
{ shader = glCreateShader (type);
  glShaderSource (shader, 1, &src, NULL);
  glCompileShader (shader);

#if defined(DEBUG)
  GLint logLength;
  glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
      std::vector<GLchar> plog(logLength);
      glGetShaderInfoLog (shader, logLength, &logLength, &plog[0]);
      std::string strLog(&plog[0], &plog[0] + logLength);
      std::cout << strLog << std::endl;
  }
#endif

  GLint status;
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  if (status == 0) {
      glDeleteShader (shader);
      return false;
  }
  return true;
}


static bool linkProgram (GLuint &program)
{ glLinkProgram (program);

#if defined(DEBUG)
  GLint logLength;
  glGetProgramiv (program, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
      std::vector<GLchar> plog(logLength);
      glGetProgramInfoLog (program, logLength, &logLength, &plog[0]);
      std::string strLog(&plog[0], &plog[0] + logLength);
      std::cout << strLog << std::endl;
  }
#endif

  GLint status;
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == 0) {
      return false;
  }
  return true;
}

static std::string loadShaderSourceFromFile (const char *filepath)
{ std::ifstream in (filepath);
  std::stringstream ss;
  ss << in.rdbuf ();
  return ss.str ();
}


static float query_gpu_mem_usage_nvidia()
{
  GLint minfo;
  glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &minfo);
  GLint evicted;
  glGetIntegerv(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evicted);
  if (in_test_mode)
    return float(minfo/1024);
  else
    { printf("GL nvidia total available memory %d MB  (evicted memory %d) \n", minfo/1024, evicted/1024);
      return 0.0f;
    }
}

static float query_gpu_mem_usage_amd()
{
  GLint minfo[4];
  glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, minfo);
  if (in_test_mode)
    return float(minfo[0]/1024);
  else
    { printf("GL amd total free memory in pool %d MB\n", minfo[0]/1024);
      return 0.0f;
    }
}

static float query_memory(bool test_mode)
{
  in_test_mode = test_mode;
  std::string vendor_string ( reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
  if (!test_mode)
    std::cout << vendor_string << std::endl;
  if (vendor_string . find ("nvidia") != std::string::npos ||
      vendor_string . find ("NVIDIA") != std::string::npos)
    return query_gpu_mem_usage_nvidia ();
  else if (vendor_string . find ("ati") != std::string::npos ||
           vendor_string . find ("amd") != std::string::npos)
    return query_gpu_mem_usage_amd ();
  else if (!test_mode)
    { printf("unsupported graphics card vendor\n");
      return 0.0f;
    }
}


static bool decode_png_image(std::vector<GLubyte> &image_data,
                             unsigned int &img_width,
                             unsigned int &img_height,
                             const std::string &img_path)
{
  unsigned error = lodepng::decode(image_data, img_width, img_height, img_path.c_str());
  if (error && !in_test_mode)
    std::cout << "lodepng decode error " << error << ": " << lodepng_error_text(error) << std::endl;
  return error ? false : true;
}

};


}

#endif // defined(__Scene__proto__)
