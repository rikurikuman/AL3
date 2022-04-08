#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	// ※スケーリングが一様な場合のみ正しい
	float4 worldNormal = normalize(mul(world, float4(normal, 0)));
	float4 worldPos = mul(world, pos);

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(mul(projection, view), world), pos);

	output.worldpos = worldPos;
	output.normal = worldNormal.xyz;
	output.uv = uv;
	
	return output;
}