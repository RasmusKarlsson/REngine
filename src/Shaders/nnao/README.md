This archive contains the shader for Neural Network Ambient Occlusion as well 
as the filters saved as .tga files.

The shader and the variables within should be mainly self explainatory with the 
exception of the `gbuffer` sampler. This sampler is meant to contain a texture 
with the _camera space normals_ in the RGB channels (scaled within the range 
0-1) and the _linear camera depth_ in the alpha channel (also scaled in the 
range 0-1, where 1 represents a point at the near clip plane and 0 a point at 
the far one).

You will need to load the four filter image into the four filter samplers `F0`, 
`F1`, `F2`, `F3` and change any of the camera parameters to match your scene 
such as the projection matrices and near/far clip planes.