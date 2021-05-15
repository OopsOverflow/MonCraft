
# MonCraft

Sandbox game developed using C++ &amp; OpenGL 
***
Why Buy Minecraft When You Can Code it Yourself? 💹
***
## Features 🔥
### Solid Integration 
MonCraft supports optimizations on both AMD and NVIDIA cards
	

 - [x] Nvidia `extern "C" {  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;}` 

> [http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf](http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf "http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf")

 - [x] AMD `__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;`
***
### Frsutrum Culling
Talk about it here, how it works
***
### Shadow Engine 🌑
MonCraft has a fully fledged shadow engine that makes the terrain look more beautiful and crisp.
Shadows also give the terrain some realism and highlight its features. 

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/shadows.png?token=ARYPWFB65TAS54BXX3AXBTDAVFAKK" alt="shadows"/>
</p>



***
### 3D Fog

> Fog is very popular element in computer graphics, so popular that in fact we are always introduced to it early in textbooks or tutorials. However these textbooks, tutorials and even APIs only go as far as a simple distance based color blending, and stop there. Even advanced demos, interactive applications and games go no further that the simple color blending. -- Inigo Quilez.

But Not MonCraft. We implements fog in a way even OG Minecraft failed to do. 
The fog implementation **augments** the terrain features like depth and curve it also quickly helps us understand the distances and therefore scale of objects, and the world itself.

The fog density depends on the direction of the sun, and the amount of global illumination in the area.
The areas with less lighting have a more dense fog.

***Same place, different direction :***
<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/fog.png?token=ARYPWFFCNL3AWIZRAZC7A5TAVFCBE" alt="fog"/>
</p>


❓ How it works?
```glsl
       float sunAmount = max( dot(vec3(m_view), lightDirection), 0.0 );
       vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // bluish
                              vec3(1.0,0.9,0.7), // yellowish
                              pow(sunAmount,8.0) );
```
`Fractal Brownian Motion` also makes the fog depend on the day/night cycle. 

***Fog moving :*** 
<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/fogmoving.gif?token=ARYPWFBSOC6AN7BMOOJFLILAVFCEE" alt="shadows"/>
</p>

#### But Wait There's More 💣
*`Fractal Brownian Motion`* was also implemented to give fog some procedurally generated animation.

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/fbm.gif?token=ARYPWFADLFC6NW7ZYAHMLDTAVFACK" alt="fbm"/>
</p>

```math
f(p) = fbm( p + fbm( p + fbm( p ) ) ) 
```

This gives fog a **crisp realistic effect**.

## The Team Behid MonCraft



|    Name            |GitHub|Email|
|----------------|-------------------------------|-----------------------------|
|Brossier Mathis|@k2d222            |      |
|Mahmoud Houssem|@OopsOverflow            |contact@houssem.com            |
|Surer Pierre|@Marcel||

