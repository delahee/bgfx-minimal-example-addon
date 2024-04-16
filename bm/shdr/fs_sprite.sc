$input v_texcoord0, v_color0

#include "common.sh"

SAMPLER2D(tex0,0);

void main(){

	//vec4 diffuse = texture2D(tex0, gl_FragCoord.xy);
	vec4 texCol = texture2D(tex0, v_texcoord0);

	//gl_FragColor = vec4(v_texcoord0,1.0,1.0);
	//gl_FragColor = v_color0;
	//gl_FragColor = v_color0 + diffuse;
	//gl_FragColor = diffuse;
	//gl_FragColor = vec4(gl_FragCoord.xy,1.0,1.0);
	vec4 col = texCol * v_color0;
	if( col.a < 0.01f)
		discard;
	gl_FragColor = col;
}
