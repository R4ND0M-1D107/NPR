shaders/gBuffer-vs.glsl
shaders/tonemapping-fs.glsl
out 0 fragmentColor
out 1 normalColor
out 2 emissionColor
out 3 maskColor
out 4 posColor
out 5 uvColor
attribute 0 position
attribute 1 vertexUV
uniform 0 screenTexture
uniform 1 normalTexture
uniform 2 depthTexture
uniform 3 emissionTexture
uniform 4 width
uniform 5 height
uniform 6 maskTexture
uniform 7 posTexture
uniform 8 uvTexture