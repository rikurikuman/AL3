#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD) {
  VSOutput output; // ピクセルシェーダーに渡す値
  output.svpos = mul(mul(mul(projection, view), world), pos);
  output.normal = normal;
  output.uv = uv;
  return output;
}