# Lighting4
## Description
An asynchronous, CPU-based, 2D dynamic lighting library for projects that use the Allegro graphics library. To utilize the library you add LightSources (sun, flash light, lantern) and LightBlockers (lines that block the rays of light from the LightSources).  The LightSource draw method has a time complexity of O(nk) where n is the number of LightBlocker end points and k is the precision of the angle (in bytes).
LightSoures can be processed on individual threads and can run in parrallel with the main thread.

## Example Images
![alt tag](/Imgs/farmerGameLight.PNG)
Lighting system implemented in a small platformer with no Gaussain Blur.
![alt tag](/Imgs/farmerGameLightGaus.PNG)
Now with two passes of Gaussian Blur.
