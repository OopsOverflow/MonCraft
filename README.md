[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/OopsOverflow/MonCraft/blob/main/LICENSE)

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/monjang-logo.png?token=ARYPWFCI53SSJADIC4J6IBTAVFSMW" alt="3d"/>
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/Moncraft.png?token=ARYPWFGCLONOBK3FGOCJEGDAVKEJI" alt="3d"/>
</p>


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
 4. GLM

****

### World Interactions

 1. [x] Moving Around --- WASD
 2. [x] Look Around --- Move Mouse
 3. [x] Detach Mouse --- Esc
 4. [x] Block Placing --- RMB
 5. [x] Selective Block Removal --- LMB
 6. [x] Select Block --- Taget Block & Press Mouse Wheel Button
 7. [x] Sprinting --- CTRL
 8. [x] Filed of View (**FOV**) changes while sprinting ✅
 9. [x] Chang View --- F5
 10. [x] God Mode --- Double Space
 11. [x] Ascend (In God Mode Only) --- Space
 12. [x] Descend (In God Mode Only) --- Shift
 13. [ ] Crouch -- Coming Soon!
 14. [ ] Place on Edge -- Coming Soon!
 
****

## Features 🔥
### Solid Integration 
MonCraft supports optimizations on both AMD and NVIDIA cards
	

✅ Nvidia `extern "C" {  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;}` 

> [http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf](http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf "http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf")

✅ AMD `__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;`

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


How does it work❓ 
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
Moncraft implements transparent blocks such as `Water_Block` & `Leaf_Block` and more are to come!

How does it work ❓

Before each frame draw, we compute the position of the chunks in the camera space. that way, we can:
- check that they are visible in the frustum -> only render visible chunks
- compute their z-distance to the camera.

That way, we can sort the chunks back-to-front to render them in that order. The sort has a cost of nlogn : it would not be possible to sort each block ! but because blocks are grouped in chunks, and there are about 1000 chunks loaded at a time, the cost is not too high.
inside each chunk, there is only one mesh. the EBO of the mesh is constructed that way :
1. First, the solid blocks which require no sorting as long as they are rendered first
2. Then, the transparent faces in ±x direction, then n ±y direction, then in ±z direction. those faces are stored from negative to positive (more or less)
3. Finally, the same transparent faces as before, but in reverse order.


That way, we render first solid blocks, then transparent blocks in the direction we are looking at, back to front (choosing if we draw using the normal or reverse indices).

***
### 3D Chunks
It might look like a bug but it's ***definetly a feature***.

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

Uses worker threads to generate chunks and precompute the opengl data. Because opengl is single-threaded, the vbos need to be created on the main thread. The workers do as much work as possible to make the task easier for the poor main thread. 
This allows for huge performance upgrades and a better playing experience as chunks are generated in a faster and a seamless way. 
We are proud to say that our game is well optimized so far, on medium budget cards the engine can easily output over 200fps (the fps counter is accessed by pressing the F key to pay respect)

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

We made a custom normal map in blender and used the fact that blocks are aligned with the x/y/z axis to our advantage : we didn’t need to send the tangent and bitangent, we could guess them from the faces alignment.
We haven’t found an effective way to calculate reflection as the water is not a plane but a block that can be placed anywhere in the world. So we couldn’t calculate a reflection on each block without raytrace light.
We only calculate specular light on water blocks, so in order to differentiate it from a mesh, we put the alpha component to 0 for the other normals.


<p align="center">
  <img src="https://raw.githubusercontent.com/OopsOverflow/MonCraft/main/screeshots/water_normal.gif?token=ARYPWFEUZS4IN2LH5XUKGKDAVFRNG" alt="water"/>
</p>

****


# The Team Behid MonCraft


|    Name            |GitHub|Email|
|----------------|-------------------------------|-----------------------------|
|Brossier Mathis|@k2d222            | mathis.brossier@universite-paris-saclay.fr     |
|Mahmoud Houssem|@OopsOverflow            |houssemmahmoudswe@gmail.com            |
|Surer Pierre|@PignoulMarcel|pierre.surer@universite-paris-saclay.fr			|


# Refrences
* [Crytek Engine : Atmosphric Effects in Games](https://developer.amd.com/wordpress/media/2012/10/Wenzel-Real-time_Atmospheric_Effects_in_Games.pdf) 
