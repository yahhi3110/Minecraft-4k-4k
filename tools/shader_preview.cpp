// 検証用: シェーダーをMesaで1フレーム描画してPPM出力
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
typedef GLuint (*PC)(GLenum,GLsizei,const char**); typedef void (*PU)(GLuint); typedef void (*PF)(GLint,GLsizei,const float*);
int main(int ac,char**av){
 SDL_Init(SDL_INIT_VIDEO);
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
 SDL_Window*w=SDL_CreateWindow("t",0,0,1920,1080,SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);
 SDL_GLContext c=SDL_GL_CreateContext(w); if(!c){puts(SDL_GetError());return 1;}
 printf("%s\n",glGetString(GL_VERSION));
 FILE*fp=fopen(av[1],"rb"); static char b[8192]; b[fread(b,1,8191,fp)]=0; const char*s=b;
 GLuint p=((PC)SDL_GL_GetProcAddress("glCreateShaderProgramv"))(0x8B30,1,&s);
 GLint ok; ((void(*)(GLuint,GLenum,GLint*))SDL_GL_GetProcAddress("glGetProgramiv"))(p,0x8B82,&ok); printf("link=%d\n",ok);
 char log[2048]; ((void(*)(GLuint,GLsizei,GLsizei*,char*))SDL_GL_GetProcAddress("glGetProgramInfoLog"))(p,2048,0,log); puts(log);
 ((PU)SDL_GL_GetProcAddress("glUseProgram"))(p);
 float f[7]; for(int i=0;i<7;i++) f[i]=atof(av[2+i]);
 ((PF)SDL_GL_GetProcAddress("glUniform1fv"))(0,7,f);
 glViewport(0,0,1920,1080); glRects(-1,-1,1,1);
 std::vector<unsigned char> px(1920*1080*3); glReadPixels(0,0,1920,1080,GL_RGB,GL_UNSIGNED_BYTE,px.data());
 FILE*o=fopen(av[8],"wb"); fprintf(o,"P6\n1920 1080\n255\n");
 for(int y=1079;y>=0;y--) fwrite(&px[y*1920*3],1,1920*3,o); fclose(o); return 0; }
