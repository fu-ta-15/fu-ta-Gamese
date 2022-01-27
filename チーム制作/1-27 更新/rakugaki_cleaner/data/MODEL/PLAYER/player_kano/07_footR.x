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
 60;
 1.51916;-2.99613;0.91452;,
 1.42188;-0.19285;0.87556;,
 -1.62130;-0.19032;0.87552;,
 -1.35864;-2.99613;0.91452;,
 -1.82130;-0.19032;0.67552;,
 1.62188;-0.19285;0.67556;,
 -1.82145;-0.18944;-1.73054;,
 -1.35864;-2.99613;-1.65924;,
 -1.62145;-0.18944;-1.93054;,
 1.42191;-0.19194;-1.93030;,
 1.51916;-2.99613;-1.65924;,
 1.62191;-0.19194;-1.73030;,
 -0.17651;-7.30509;0.24466;,
 1.05326;-7.29942;0.24554;,
 1.35326;-7.29942;-0.05446;,
 -0.47651;-7.30509;-0.05534;,
 -0.47690;-7.30318;-0.92167;,
 -0.17690;-7.30318;-1.22167;,
 1.05334;-7.29740;-1.22226;,
 1.35334;-7.29740;-0.92226;,
 -2.28750;-4.48064;2.76000;,
 1.71850;-4.47469;2.76000;,
 1.71850;-2.95622;2.76000;,
 -2.28750;-2.95622;2.76000;,
 -2.28750;-4.47863;-2.90100;,
 -2.28750;-2.95622;-2.90100;,
 1.71850;-4.47256;-2.90100;,
 1.71850;-2.95622;-2.90100;,
 1.51850;-0.22644;2.76000;,
 -2.08750;-0.22672;2.76000;,
 1.71850;-0.22628;2.56000;,
 -2.08750;-0.02659;2.56000;,
 1.51850;-0.02630;2.56000;,
 -2.28750;-0.22660;2.56000;,
 -2.28750;-0.22298;-2.70100;,
 -2.08750;-0.02296;-2.70100;,
 -2.08750;-0.22282;-2.90100;,
 1.51850;-0.22254;-2.90100;,
 1.51850;-0.02267;-2.70100;,
 1.71850;-0.22266;-2.70100;,
 1.13393;-7.60972;-0.05762;,
 -0.16941;-7.60972;-0.05762;,
 -0.16941;-7.60972;-0.94773;,
 1.13393;-7.60972;-0.94773;,
 1.14686;-7.27431;-1.31920;,
 -0.23674;-7.28080;-1.31854;,
 -0.57413;-7.28080;-0.98114;,
 -0.57369;-7.28295;-0.00681;,
 -0.23630;-7.28295;0.33058;,
 1.14677;-7.27658;0.33158;,
 1.48417;-7.27658;-0.00583;,
 1.48425;-7.27431;-0.98180;,
 1.34414;-5.45364;0.61495;,
 -0.73817;-5.45364;0.61497;,
 1.55445;-5.45364;0.40465;,
 -0.94820;-5.45364;0.40492;,
 -0.94820;-5.45364;-1.29404;,
 -0.73807;-5.45364;-1.50417;,
 1.34404;-5.45364;-1.50417;,
 1.55445;-5.45364;-1.29377;;
 
 54;
 4;0,1,2,3;,
 3;3,2,4;,
 3;5,1,0;,
 4;3,4,6,7;,
 3;7,6,8;,
 4;7,8,9,10;,
 3;10,9,11;,
 4;10,11,5,0;,
 4;12,13,0,3;,
 3;0,13,14;,
 3;15,12,3;,
 4;16,15,3,7;,
 3;17,16,7;,
 4;18,17,7,10;,
 3;19,18,10;,
 4;14,19,10,0;,
 4;20,21,22,23;,
 4;24,20,23,25;,
 4;26,24,25,27;,
 4;21,26,27,22;,
 4;22,28,29,23;,
 3;30,28,22;,
 4;31,29,28,32;,
 3;23,29,33;,
 3;30,32,28;,
 3;31,33,29;,
 4;23,33,34,25;,
 4;35,34,33,31;,
 3;25,34,36;,
 3;35,36,34;,
 4;25,36,37,27;,
 4;38,37,36,35;,
 3;27,37,39;,
 3;38,39,37;,
 4;27,39,30,22;,
 4;32,30,39,38;,
 4;31,32,38,35;,
 4;40,41,42,43;,
 4;44,43,42,45;,
 4;46,42,41,47;,
 4;48,41,40,49;,
 4;50,40,43,51;,
 3;43,44,51;,
 3;42,46,45;,
 3;41,48,47;,
 3;40,50,49;,
 4;52,53,48,49;,
 4;54,52,49,50;,
 4;53,55,47,48;,
 4;55,56,46,47;,
 4;56,57,45,46;,
 4;57,58,44,45;,
 4;58,59,51,44;,
 4;59,54,50,51;;
 
 MeshMaterialList {
  6;
  54;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.012800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\face_3.jpg";
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
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  63;
  0.380968;-0.078457;0.921253;,
  -0.376152;-0.132438;0.917044;,
  -0.904895;-0.203514;-0.373829;,
  0.380375;-0.104946;-0.918859;,
  -0.382298;-0.022949;0.923754;,
  0.382458;0.026109;0.923604;,
  -0.911638;-0.159942;-0.378595;,
  -0.916430;-0.118555;0.382231;,
  0.382287;-0.075762;-0.920932;,
  -0.378903;-0.124126;-0.917074;,
  0.923873;-0.001231;0.382698;,
  0.923027;-0.043508;-0.382266;,
  0.375328;-0.181925;0.908863;,
  -0.365496;-0.240077;0.899319;,
  -0.892690;-0.264827;0.364653;,
  -0.896393;-0.246586;-0.368340;,
  -0.369975;-0.193819;-0.908599;,
  0.378139;-0.134011;-0.915998;,
  0.920012;-0.100374;-0.378817;,
  0.918456;-0.120252;0.376799;,
  -0.983361;-0.181662;0.000000;,
  -0.000001;-0.098644;-0.995123;,
  0.999294;-0.037570;0.000000;,
  0.252439;0.018495;0.967436;,
  -0.252441;0.018497;0.967436;,
  -0.967436;0.018472;-0.252439;,
  0.252455;0.018470;-0.967432;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.364741;0.379686;0.850178;,
  -0.364791;0.379680;0.850159;,
  -0.850022;0.379754;0.365035;,
  -0.850166;0.379829;-0.364620;,
  -0.364936;0.379922;-0.849989;,
  0.364885;0.379928;-0.850008;,
  0.850145;0.379854;-0.364643;,
  0.850001;0.379779;0.365055;,
  -0.366986;0.854580;0.367444;,
  0.366882;0.854616;0.367463;,
  0.367014;0.854948;-0.366558;,
  -0.367120;0.854911;-0.366537;,
  1.000000;0.000000;0.000000;,
  0.349447;-0.872059;-0.342635;,
  -0.317063;-0.889859;-0.328058;,
  -0.312673;-0.895136;0.317752;,
  0.344541;-0.878185;0.331788;,
  0.344951;-0.488503;0.801482;,
  -0.327462;-0.530087;0.782161;,
  -0.772122;-0.545192;0.326487;,
  -0.778497;-0.532391;-0.332419;,
  -0.334298;-0.499739;-0.799066;,
  0.351311;-0.455853;-0.817789;,
  0.829056;-0.434012;-0.352561;,
  0.823300;-0.449113;0.347094;,
  0.375326;-0.181924;0.908864;,
  -0.365506;-0.240079;0.899315;,
  -0.896393;-0.246592;-0.368336;,
  0.378137;-0.134014;-0.915998;,
  0.918456;-0.120254;0.376799;,
  -0.892693;-0.264831;0.364642;,
  -0.369978;-0.193822;-0.908597;,
  0.920010;-0.100379;-0.378821;;
  54;
  4;0,5,4,1;,
  3;1,4,7;,
  3;10,5,0;,
  4;20,7,6,2;,
  3;2,6,9;,
  4;21,9,8,3;,
  3;3,8,11;,
  4;22,11,10,22;,
  4;13,12,0,1;,
  3;0,12,19;,
  3;14,13,1;,
  4;15,14,20,2;,
  3;16,15,2;,
  4;17,16,21,3;,
  3;18,17,3;,
  4;19,18,22,22;,
  4;27,27,23,24;,
  4;28,28,28,25;,
  4;29,29,29,26;,
  4;42,42,42,42;,
  4;23,30,31,24;,
  3;37,30,23;,
  4;38,31,30,39;,
  3;24,31,32;,
  3;37,39,30;,
  3;38,32,31;,
  4;28,32,33,25;,
  4;41,33,32,38;,
  3;25,33,34;,
  3;41,34,33;,
  4;29,34,35,26;,
  4;40,35,34,41;,
  3;26,35,36;,
  3;40,36,35;,
  4;42,36,37,42;,
  4;39,37,36,40;,
  4;38,39,40,41;,
  4;46,45,44,43;,
  4;52,43,44,51;,
  4;50,44,45,49;,
  4;48,45,46,47;,
  4;54,46,43,53;,
  3;43,52,53;,
  3;44,50,51;,
  3;45,48,49;,
  3;46,54,47;,
  4;55,56,48,47;,
  4;59,55,47,54;,
  4;56,60,49,48;,
  4;60,57,50,49;,
  4;57,61,51,50;,
  4;61,58,52,51;,
  4;58,62,53,52;,
  4;62,59,54,53;;
 }
 MeshTextureCoords {
  60;
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
