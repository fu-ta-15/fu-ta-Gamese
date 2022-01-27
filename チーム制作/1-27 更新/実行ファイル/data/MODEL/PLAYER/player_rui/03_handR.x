xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 48;
 -6.10473;-1.08184;-1.47881;,
 -6.10473;1.11148;-1.47881;,
 -4.40164;1.43283;-1.47881;,
 -4.40164;-1.27465;-1.47881;,
 -6.10473;1.11148;1.32866;,
 -6.10473;-1.08184;1.32866;,
 -4.40164;-1.27465;1.32866;,
 -4.40164;1.43283;1.32866;,
 -6.10473;1.11148;-1.47881;,
 -6.10473;1.11148;1.32866;,
 -4.40164;1.43283;1.32866;,
 -4.40164;1.43283;-1.47881;,
 -6.10473;-1.08184;1.32866;,
 -6.10473;-1.08184;-1.47881;,
 -4.40164;-1.27465;-1.47881;,
 -4.40164;-1.27465;1.32866;,
 -7.32326;-0.39625;-1.17881;,
 -7.32326;-0.39625;1.02866;,
 -7.32326;0.42589;1.02866;,
 -7.32326;0.42589;-1.17881;,
 -7.03725;0.81639;-1.17881;,
 -7.03725;0.81639;1.02866;,
 -7.03725;0.51639;1.32866;,
 -7.03725;-0.48675;1.32866;,
 -7.03725;-0.78675;1.02866;,
 -7.03725;-0.78675;-1.17881;,
 -7.03725;-0.48675;-1.47881;,
 -7.03725;0.51639;-1.47881;,
 -6.10473;-1.08184;-1.47881;,
 -6.10473;1.11148;-1.47881;,
 -6.10473;-1.08184;1.32866;,
 -6.10473;1.11148;1.32866;,
 -0.03835;-0.91399;1.12866;,
 -0.03835;-0.91399;-1.27881;,
 -0.03835;1.04002;-1.27881;,
 -0.03835;1.04002;1.12866;,
 -0.23835;1.24002;1.12866;,
 -0.23835;1.24002;-1.27881;,
 -0.23835;1.04002;-1.47881;,
 -0.23835;-0.91399;-1.47881;,
 -0.23835;-1.11399;-1.27881;,
 -0.23835;-1.11399;1.12866;,
 -0.23835;-0.91399;1.32866;,
 -0.23835;1.04002;1.32866;,
 -4.40164;1.43283;-1.47881;,
 -4.40164;-1.27465;-1.47881;,
 -4.40164;-1.27465;1.32866;,
 -4.40164;1.43283;1.32866;;
 
 38;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,19,18,21;,
 4;22,18,17,23;,
 4;24,17,16,25;,
 4;26,16,19,27;,
 3;19,20,27;,
 3;18,22,21;,
 3;17,24,23;,
 3;16,26,25;,
 4;28,26,27,29;,
 3;29,27,20;,
 3;25,26,28;,
 4;22,23,30,31;,
 3;30,23,24;,
 3;21,22,31;,
 4;29,20,21,31;,
 4;30,24,25,28;,
 4;32,33,34,35;,
 4;36,35,34,37;,
 4;38,34,33,39;,
 4;40,33,32,41;,
 4;42,32,35,43;,
 3;35,36,43;,
 3;34,38,37;,
 3;33,40,39;,
 3;32,42,41;,
 4;44,38,39,45;,
 3;45,39,40;,
 3;37,38,44;,
 4;46,42,43,47;,
 3;47,43,36;,
 3;41,42,46;,
 4;47,36,37,44;,
 4;45,40,41,46;;
 
 MeshMaterialList {
  6;
  38;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\hair_2.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\face_2.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\skin.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.019200;0.000000;0.144000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  36;
  -0.149413;0.234131;-0.960655;,
  -0.149413;-0.234131;-0.960655;,
  -0.149413;0.234131;0.960655;,
  -0.149413;-0.234131;0.960655;,
  0.023801;0.252265;-0.967365;,
  0.021860;-0.252337;-0.967392;,
  0.023801;0.252265;0.967365;,
  0.021860;-0.252337;0.967392;,
  -0.892855;0.304421;-0.331871;,
  -0.892855;0.304421;0.331871;,
  -0.892855;-0.304421;0.331871;,
  -0.892855;-0.304421;-0.331871;,
  -0.512166;0.322822;-0.795909;,
  -0.512166;-0.322822;-0.795909;,
  -0.512166;-0.322822;0.795909;,
  -0.512166;0.322822;0.795909;,
  -0.602443;0.733906;0.313757;,
  -0.602443;0.733906;-0.313757;,
  -0.602443;-0.733906;-0.313757;,
  -0.602443;-0.733906;0.313757;,
  0.854763;0.367001;0.367001;,
  0.854763;0.367001;-0.367001;,
  0.854763;-0.367001;-0.367001;,
  0.854763;-0.367001;0.367001;,
  0.382146;-0.364406;-0.849219;,
  0.383492;0.364152;-0.848721;,
  0.383492;0.364152;0.848721;,
  0.382146;-0.364406;0.849219;,
  0.394708;0.844320;-0.362394;,
  0.394708;0.844320;0.362394;,
  0.391509;-0.845577;0.362933;,
  0.391509;-0.845577;-0.362933;,
  -0.243994;0.969777;0.000000;,
  -0.070048;0.997544;0.000000;,
  -0.208072;-0.978113;0.000000;,
  -0.037071;-0.999313;0.000000;;
  38;
  4;1,0,4,5;,
  4;2,3,7,6;,
  4;32,32,33,33;,
  4;34,34,35,35;,
  4;11,10,9,8;,
  4;17,8,9,16;,
  4;15,9,10,14;,
  4;19,10,11,18;,
  4;13,11,8,12;,
  3;8,17,12;,
  3;9,15,16;,
  3;10,19,14;,
  3;11,13,18;,
  4;1,13,12,0;,
  3;0,12,17;,
  3;18,13,1;,
  4;15,14,3,2;,
  3;3,14,19;,
  3;16,15,2;,
  4;32,17,16,32;,
  4;34,19,18,34;,
  4;23,22,21,20;,
  4;29,20,21,28;,
  4;25,21,22,24;,
  4;31,22,23,30;,
  4;27,23,20,26;,
  3;20,29,26;,
  3;21,25,28;,
  3;22,31,24;,
  3;23,27,30;,
  4;4,25,24,5;,
  3;5,24,31;,
  3;28,25,4;,
  4;7,27,26,6;,
  3;6,26,29;,
  3;30,27,7;,
  4;33,29,28,33;,
  4;35,31,30,35;;
 }
 MeshTextureCoords {
  48;
  0.808200;1.000000;,
  0.808200;0.000000;,
  0.540120;0.000000;,
  0.540120;1.000000;,
  0.191800;0.000000;,
  0.191800;1.000000;,
  0.459880;1.000000;,
  0.459880;0.000000;,
  0.808200;1.000000;,
  0.808200;0.000000;,
  0.540120;0.000000;,
  0.540120;1.000000;,
  0.808200;1.000000;,
  0.808200;0.000000;,
  0.540120;0.000000;,
  0.540120;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
