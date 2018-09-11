# Lighting4
## Description
An asynchronous, CPU-based, 2D dynamic lighting library for projects that use the Allegro graphics library. To utilize the library you add LightSources (sun, flash lights, lanterns) and LightBlockers (lines that block the rays of light).  The LightSource draw method has a time complexity of O(nk) where n is the number of LightBlocker end points and k is the precision of the angle (in bytes).
LightSoures can be processed on individual threads and can run in parrallel with the main thread.

## Example Images
![alt tag](/Imgs/farmerGameLight.PNG)
Lighting system implemented in a small platformer with no Gaussain Blur.
![alt tag](/Imgs/farmerGameLightGaus.PNG)
Now with two passes of Gaussian Blur.

# Building
***Clone repository with --recurse-submodules***  
Requirements: Allegro (5.2.4)

#### Requirement Installation
1. Installing Allegro
    * Windows:
        1. After you generate the FarmerGame solution, goto Tools -> NuGet Packet Manager -> Manage NuGet Packages for Solution... -> Browse
        2. Search for and install Allegro for AllegroExt, Lighting4, and Example1
        3. Goto the properties for the Allegro projects and set the dependencies like this ![Allegro Configuration](https://github.com/Jester565/FarmerGame/blob/master/rdme/AllegConf.png)
    * Ubuntu:
        1. Install Allegro5 Package with [this guide](https://wiki.allegro.cc/index.php?title=Install_Allegro_from_Ubuntu_PPAs)

#### Build
```
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017" -A x64 -T host=x64 -Dexamples=ON       #For Ubuntu don't include -G option
```
Run make or build the solution  
Set Example1 as Startup Project after building on Visual Studio

#### Troubleshooting
* If using Visual Studio, make sure all projects are using /MT runtime linking and Basic Runtime Checks is set to default.
