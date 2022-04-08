#include "Shape.hlsli"

VSOutput main(float4 pos : POSITION, float4 color : COLOR) {
	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);
	output.color = color;
	return output;
}