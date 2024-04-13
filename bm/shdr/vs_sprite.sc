$input a_position, a_texCoord0, a_color0
$output v_texCoord0, v_color0

#include "../common/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_texCoord0 = a_texCoord0;
	v_color0 = a_color0;
}
