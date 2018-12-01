# Distinguishing between Red and Blue Treasures

In order to be able to distinguish between red and blue treasures, we had made our camera output to be RGB444 and saturated the down-sampling for red and thresholded the blue values coming in. When designing our downsampler and image processor for Lab 4, we had the goal of distinguishing red and blue treasures in mind. This design is what allowed us to observe this outcome in the following video in which we place red and blue treasures in front of our camera and successfully distinguish between them:

https://www.youtube.com/watch?v=X9Ipn2ejz8U

# Shape Detection

Once we were able to distinguish between a red and a blue treasure, the next step was to determine what shape a treasure is. Our strategy was to sample three equally distanced pixel rows from the image. We noticed the following:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/Screen%20Shot%202018-11-30%20at%207.21.46%20PM.png width=700>

As demonstrated by the figure, if we compare the intensity of red between the three samples, we would be able to determine which shape it is. We could even average these samples for greater accuracy. The rules would be as follows:

```
Take Color Samples at Rows 40, 120, and 160.

If Color Intensity is same across all samples:
   shape = rectanlge
   
If Color Intensity is highest at row 120 and rows 40/160 are similar:
   shape = diamond

If Color Intensity is monotonically increasing:
   shape = triangle

```

With the amount of noise that we were experiencing with the camera and after hours of changing registers and wires, we were unable to achieve a steady feed that could implement the strategy above. However, the following video demonstrates what should happen but was not what we able to get:

https://www.youtube.com/watch?v=hRiXi_E4cNg
