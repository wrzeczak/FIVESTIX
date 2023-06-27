#version 420 core

//! TODO: Use a texture sampler here instead? May be faster
uniform vec4 country_colors[8];
//! TODO: Use UBO
uniform float gen_seed;

in vec2 fragTexCoord;
layout (location = 0) out vec4 terrain_color;
layout (location = 1) out vec4 country_color;
layout (location = 2) out uvec3 board_pixel_id;

// Most of this shader code is from https://www.shadertoy.com/view/7lBfDz, with changes to support more advanced generation

//http://flafla2.github.io/2014/08/09/perlinnoise.html
//https://web.archive.org/web/20160530124230
//http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
//http://eastfarthing.com/blog/2015-04-21-noise/
//https://www.youtube.com/watch?v=Or19ilef4wE
//https://www.youtube.com/watch?v=MJ3bvCkHJtE

//hash from iq
//https://www.shadertoy.com/view/Xs23D3
vec2 hash(vec2 p, float seed) {  						
	p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    
	return -1.0 + 2.0 * fract(sin(p + 20.0) * (53758.5453123 + seed));
}

float perlin_noise_2(vec2 p, float seed) {
	vec2 i = floor(p);
	vec2 f = fract(p);
    
    //grid points
    vec2 p0 = vec2(0.0, 0.0);
    vec2 p1 = vec2(1.0, 0.0);
    vec2 p2 = vec2(0.0, 1.0);
    vec2 p3 = vec2(1.0, 1.0);
    
    //distance vectors to each grid point
    vec2 s0 = f - p0;
    vec2 s1 = f - p1;
    vec2 s2 = f - p2;
    vec2 s3 = f - p3;
    
    //random gradient vectors on each grid point
    vec2 g0 = hash(i + p0, seed);
    vec2 g1 = hash(i + p1, seed);
    vec2 g2 = hash(i + p2, seed);
    vec2 g3 = hash(i + p3, seed);
    
    //gradient values
    float q0 = dot(s0, g0);
    float q1 = dot(s1, g1);
    float q2 = dot(s2, g2);
    float q3 = dot(s3, g3);
    
    //interpolant weights
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    //bilinear interpolation
    float l0 = mix(q0, q1, u.x);
    float l1 = mix(q2, q3, u.x);
    float l2 = mix(l0, l1, u.y);
    
    return l2;
}

float perlin_fbm(vec2 tex_coord, float persistence, int octaves, float seed) {
    float total = 0.0;
    float max_value = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    
    for(int i = 0; i < octaves; ++i) {
        total += perlin_noise_2(tex_coord * frequency, seed) * amplitude;
        max_value += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
        ++seed;
    }
    
    return total/max_value;
}

void render(vec2 tex_coord, float seed) {
    // main height
    float h0 = 0.5 + perlin_fbm(0.1 * tex_coord, 0.3, 8, seed);
    // continentalness
    float c = 0.5 + perlin_fbm(0.1 * tex_coord + vec2(314.123, 1231.0), 0.6, 8, seed);
    // erossion
    float e = 0.5 + perlin_fbm(0.1 * tex_coord + vec2(100.0, 100.0), 0.6, 9, seed);
    // riverness
    float r = 0.5 + perlin_fbm(0.1 * tex_coord + vec2(-100.0, 200.0), 0.4, 3, seed);
    // river (when close to middle)
    float r2 = 0.5 + perlin_fbm(0.5 * tex_coord + vec2(-300.0, 200.0), 0.6, 4, seed);

    // temperature
    // humidity, verying very slowly
    float hum = 0.5 + perlin_fbm(0.02 * tex_coord + vec2(420.0, 200.0), 0.6, 7, seed);
    // weirdness

    // return vec4(n1 + 0.5, n1 + 0.5, 0.0, 0.0);
    // return vec4(n2 + 0.5, n2 + 0.5, 0.0, 0.0);

    float h = e * c;

    float sea_base = 0.18;

    float sea_color = 0.4;  // shade of blue

    if(h < sea_base) {
        if(h < sea_base - 0.02) {
            // deep water
            terrain_color = vec4(0.0, 0.0, sea_color, 1.0);
            country_color = vec4(0);
            board_pixel_id = uvec3(-1, -1, -1);
            return;
        }
        // sea close to shore
        terrain_color = vec4(0.0, 0.08, sea_color, 1.0);
        country_color = vec4(0);
        board_pixel_id = uvec3(-1, -1, -1);
        return;
    }

    // 0 - fully connected rivers
    // 0.45 some dead ends (more open land area with no rivers)
    // 1.0 - no rivers
    float close_to_sea = (h - sea_base)/(1.0 - sea_base);  // 0 at sea, 1 at max
    float close_to_sea2 = 1.0 - pow(1.0 - close_to_sea, 32.0);
    // When close to sea, makes river more likely, thus likely to be wider.
    if(r >= 0.48 && 0.5 * 0.99 * close_to_sea2 <= r2 && r2 <= 1.0 - 0.99 * close_to_sea2 * 0.5) {
        // close_to_sea2 can get very close to 1.0, which will make super thin rivers
        // mutliply by 0.99 to set minimum river width
        // Vary color of rivers a bit related to height / closness to sea
        terrain_color = vec4(0.0, close_to_sea * 0.8, sea_color + (1.0 - sea_color) * close_to_sea, 1.0);
        country_color = vec4(0);
        board_pixel_id = uvec3(-1, -1, -1);
        return;
    }
    
    uint largest_country_index = -1;
    float largest_country_value = 0;
    for(int i = 0; i < 8; i++) {
        float country_value = 0.5 + perlin_fbm(0.1 * tex_coord, 0.4, 3, seed + 1 + i);
        if (country_value > largest_country_value) {
            largest_country_value = country_value;
            largest_country_index = i;
        }
    }

    country_color = country_colors[largest_country_index];
    board_pixel_id = uvec3(largest_country_index, -1, -1);

    // Color land more grean in humid areas
    terrain_color = vec4(h * (1.0 - hum), h * hum, h * (1.0 - hum), 1.0);
    return;
}

void main() {
    render(fragTexCoord*10.0f, gen_seed);
}