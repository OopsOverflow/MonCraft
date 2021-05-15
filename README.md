[![license](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat-square "License")](https://github.com/OopsOverflow/MonCraft/blob/main/LICENSE)

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/monjang-logo.png?token=ARYPWFCI53SSJADIC4J6IBTAVFSMW" alt="3d"/>
</p>

# MonCraft 

<p align=center>
Sandbox game developed using C++ 14 &amp; OpenGL 3.3 🕹
</p>
<p align=center>
Why Buy Minecraft When You Can Code it Yourself? 💹
</p>

***

## Compiling
***Supports Windows and Linux. (Should work on mac but not tested)***
### Dependencies
 1. SDL2
 2. SFML
 3. GLEW

****

### World Interactions

 1. [x] Moving Around --- WASD
 2. [x] Look Around --- Move Mouse
 3. [x] Detach Mouse --- Esc
 4. [x] Block Placing --- RMB
 5. [x] Selective Block Removal --- LMB
 6. [x] Sprinting --- CTRL
 7. [x] Filed of View (**FOV**) changes while sprinting ✅
 8. [x] Chang View --- F5
 9. [x] God Mode --- Double Space
 10. [x] Ascend (In God Mode Only) --- Space
 11. [x] Descend (In God Mode Only) --- Shift
 12. [ ] Crouch -- Coming Soon!
 13. [ ] Place on Edge -- Coming Soon!
 
****

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

### Ambient Occlusion 🌗
Ambient occlusion is a shading and rendering method used for global background or indirect shading of objects.
This method is used to add more realism to the render because it creates soft global shadows that contribute to the visual separation of objects.
This is a global method, i.e., the brightness value of every point of the object depends on other objects in the scene.
Shadows do not remain black, they dissipate, creating halftones and obscuring those places in space that get less light.

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/aoc.png?token=ARYPWFHQPZ7RW2Q7XBTA5ETAVFQDY" alt="aoc"/>
</p>

*****

### Beautiful Terrain
#### High Definition texture pack 🎨
Unique artistic textures with vivid colors. Made-In-House by the one & only @PinoulMarcel
Mojang don't DMCA us. 

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/beautiful.png?token=ARYPWFCHH36PJURBJ4MMZLLAVFQNS" alt="world"/>
</p>

#### Procedurally Generated World 🌐
`Simplex Noise` under `src/noise/noise.cpp` is used to create the world map.
It's finely tuned to give different terrain heights and features, such as hills, mountains and oceans.

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/collage.png?token=ARYPWFEMU3IK7YJ4UOXHA3TAVFQGW" alt="noise"/>
</p>

#### Superb Terrain Blending 🌲
`src/noise/voronoi.cpp` is used for a seamless biome blending ⚡

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/voronoi.png?token=ARYPWFBCTVMAG63IGZ42AZLAVFQIU" alt="vor"/>
</p>

#### Biomes 🌍
***Seven unique*** biomes with different features and characteristics.  

****
### Dynamic Blocks 
***
### Wide Texture Atlas 🧭
***
### SkyBox 🌌
High Definition skybox using cubemaps.
***
### Transparency 🔎
Moncraft implements transparent blocks such as `Water_Block` and more are to come!
***
### 3D Chunks
It's not a big it's ***literally a feature***.

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/chunk3d.png?token=ARYPWFGFYCVNEZMXHKKIH5LAVFQTA" alt="3d"/>
</p>

***
### Eargasmic Music 🎶
Royalty-free high definition music that fits the game's atmosphere. 🥚
|  Track Name|Artist © |
|--|--|
| A New Sleep |Arden Forest  |
| Mays |Van Sandano  |
| Reconstruct |Amaranth Cove  |
| Minecraft Happy Music |	 --- |
***
### Sound Effects
Literal bovine excrement 🙂 
💘Memory leaks
***

### Normal Map
***
###  Multithreading 👷‍♂️
Allows for huge performance gains by using multiple `worker_threads` to generate the environment.
Under `src/terrain/terrain.hpp`.
***
### No OS Specific dependencies 🐧
***
### 3D AABB Collision detection 💥
***
### Player and World Animations
#### Block Breaking ⛏

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/wood.gif?token=ARYPWFFMXHXCPIFVXHD4D3DAVFTEW" alt="3d"/>
</p>

### Player Movement Animations

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/animat.png?token=ARYPWFB7IIPDB2L2MY4VEO3AVFTCY" alt="wood"/>
</p>

### Water Surface Animation
Using a custom made looping noise map.

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/water_normal.gif?token=ARYPWFEUZS4IN2LH5XUKGKDAVFRNG" alt="water"/>
</p>

****


# The Team Behid MonCraft


|    Name            |GitHub|Email|
|----------------|-------------------------------|-----------------------------|
|Brossier Mathis|@k2d222            |      |
|Mahmoud Houssem|@OopsOverflow            |contact@houssem.com            |
|Surer Pierre|@Marcel||
