# Lighting4
##Description
A 2D dynamic lighting library for projects that use the Allegro graphics library.  Consists of LightSources (which are the actual sources of light, this can be the sun, a flash light or a lantern depending on which child of LightSource you pick) and
LightBlockers (lines that block the rays of light from the LightSources).  LightSources have a time complexity of O(nk) where n is the number of LightBlocker end points.
LightSoures can be processed on individual threads and run parrallel to the main thread of the game.

##Example Images
![alt tag](/Imgs/farmerGameLight.PNG)
Lighting system implemented in a small platformer with no Gaussain Blur.
![alt tag](/Imgs/farmerGameLightGaus.PNG)
Now with two passes of Gaussian Blur.
