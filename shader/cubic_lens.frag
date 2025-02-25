uniform sampler2D texture;
uniform float iTime;

varying vec2 v_texCoord;

vec2 computeUV( vec2 uv, float k, float kcube ){
    
    vec2 t = uv - .5;
    float r2 = t.x * t.x + t.y * t.y;
	float f = 0.;
    
    if( kcube == 0.0){
        f = 1. + r2 * k;
    }else{
        f = 1. + r2 * ( k + kcube * sqrt( r2 ) );
    }
    
    vec2 nUv = f * t + .5;
    nUv.y = 1. - nUv.y;
 
    return nUv;
    
}

void main() {
    
	vec2 uv = vec2(v_texCoord.s, 1.0 - v_texCoord.t);
    float k = 1.0 * sin( iTime * .9 );
    float kcube = .5 * sin( iTime );
    
    float offset = .1 * sin( iTime * .5 );
    
    float red = texture2D( texture, computeUV( uv, k + offset, kcube ) ).r; 
    float green = texture2D( texture, computeUV( uv, k, kcube ) ).g; 
    float blue = texture2D( texture, computeUV( uv, k - offset, kcube ) ).b; 
    
    gl_FragColor = vec4( red, green,blue, 1. );

}