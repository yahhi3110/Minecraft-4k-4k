#include<windows.h>
#include<GL/gl.h>
#include<math.h>
#define G(r,n,...)((r(__stdcall*)(__VA_ARGS__))wglGetProcAddress(n))
const char*s=R"(#version 430
layout(location=0)uniform float u[7];
out vec4 o;
float H(vec2 p){p=floor(p);return floor(14.+7.*sin(p.x*.09+2.*cos(p.y*.07))+5.*sin(p.y*.11+p.x*.05)+2.*fract(sin(dot(floor(p/3.),vec2(12.9,78.2)))*4e4));}
void main(){
vec3 d=normalize(vec3((gl_FragCoord.xy-vec2(960,540))/540.,1)),P=vec3(u[0],u[1],u[2]);
float Y=sin(u[3]),X=cos(u[3]),y=sin(u[4]),x=cos(u[4]),a=u[5]*.05+1.;
d=vec3(d.x,d.y*x+d.z*y,d.z*x-d.y*y);
d=vec3(d.x*X+d.z*Y,d.y,d.z*X-d.x*Y);
vec3 l=normalize(vec3(.6*cos(a),sin(a),.8*cos(a))),g=sign(d),e=1./max(abs(d),1e-5),t=(step(0.,d)-g*fract(P))*e,m=vec3(0,1,0);
float D=clamp(l.y*3.+.2,0.,1.),L=0.;
vec3 C=mix(vec3(.01,.01,.03),vec3(.45,.7,1),D)*(1.-.4*max(d.y,0.))+vec3(1,.7,.4)*pow(max(dot(d,l),0.),40.)*D,S=C;
ivec3 c=ivec3(floor(P));
bool h=false;
for(int i=0;i<200&&L<110.;i++){
if(c.y<H(vec2(c.xz))){h=true;break;}
if(c.y>30&&d.y>=0.)break;
L=min(t.x,min(t.y,t.z));
m=step(t,t.yzx)*step(t,t.zxy);
t+=m*e;
c+=ivec3(m*g);
}
if(h){
vec3 n=-m*g,p=P+d*L;
float k=H(vec2(c.xz)),r=k-1.-float(c.y),w=1.;
vec3 b=r>3.?vec3(.5):vec3(.59,.42,.29);
if(r<1.&&(n.y>0.||fract(p.y)>.8))b=k<11.?vec3(.9,.8,.5):vec3(.42,.67,.25);
if(l.y>0.)for(int i=1;i<30;i++){vec3 q=p+n*.02+l*float(i)*.7;if(q.y<H(q.xz)){w=0.;break;}}
C=b*(.75+.25*fract(sin(dot(floor(p*16.)*(1.-abs(n)),vec3(.129,.782,.377)))*4e4))*(.17+.3*D+.8*D*w*max(dot(n,l),0.)*vec3(1,.9,.8));
C=mix(C,S,smoothstep(70.,110.,L));
}
o=vec4(C,1);
})";
float f[7]={256,30,256};
int main(){
HWND w=CreateWindowA("static",0,0x90000000,0,0,1920,1080,0,0,0,0);
HDC d=GetDC(w);
PIXELFORMATDESCRIPTOR p={sizeof p,1,37,0,32};
SetPixelFormat(d,ChoosePixelFormat(d,&p),&p);
wglMakeCurrent(d,wglCreateContext(d));
ShowCursor(0);
G(void,"glUseProgram",int)(G(int,"glCreateShaderProgramv",int,int,const char**)(0x8B30,1,&s));
for(DWORD z=GetTickCount(),a=z;!GetAsyncKeyState(27);){
MSG m;
while(PeekMessageA(&m,0,0,0,1));
DWORD n=GetTickCount();
float q=(n-a)*.012f;
a=n;
f[5]=(n-z)*.001f;
POINT c;
GetCursorPos(&c);
SetCursorPos(960,540);
f[3]+=(c.x-960)*.003f;
f[4]=fmaxf(-1.5f,fminf(1.5f,f[4]-(c.y-540)*.003f));
#define K(k)(GetAsyncKeyState(k)<0)
float F=(K('W')-K('S'))*q,R=(K('D')-K('A'))*q;
f[0]+=sinf(f[3])*F+cosf(f[3])*R;
f[2]+=cosf(f[3])*F-sinf(f[3])*R;
f[1]+=(K(32)-K(17))*q;
G(void,"glUniform1fv",int,int,float*)(0,7,f);
glRects(-1,-1,1,1);
SwapBuffers(d);
}
}
