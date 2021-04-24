#include "globals.h"

//calcMagnitude(Vector2f):
//Purpose: returning the length/magnitude of a vector
//Input: sf::Vector2f v - 2D endpoint of a vector bound to the origin (0,0)
//Output: float - Magnitude (length/distance from origin) of vector v
float calcMagnitude(sf::Vector2f v)
{
	return sqrtf(((v.x)*(v.x))+((v.y)*(v.y)));
}

//calcDist(Vector2f, Vector2f):
//Purpose: returning the distance between two points in 2D space
//Input: sf::Vector2f a - point of a vector in 2D space
//		sf::Vector2f b - another point of a vector in 2D space with the same origin as vector a
//Output: float - Distance between point a and point b
//----------------------------------------------------
//Note: Pythagorean Theorem: a²+b²=c²
//solved for c(hypotenuse: c = √(a²+b²)
float calcDist(sf::Vector2f pta, sf::Vector2f ptb)
{
	return sqrtf(((pta.x-ptb.x)*(pta.x-ptb.x)) + ((pta.y-ptb.y)*(pta.y-ptb.y)));
}

//isCollision(Vector2f, IntRect):
//Purpose: testing bounding-box collision with a point
//Input: sf::Vector2f pt - point in 2D space
//		sf::IntRect box -  4-sided boundary box in the same space
//Output: bool - true if pt is inside the box, false otherwise
bool isCollision(sf::Vector2f pt, sf::IntRect box)
{
//	bool ret=false;
	//if(!(pt.x<box.Left || pt.x>box.Right || pt.y<box.Top || pt.y>box.Bottom)) ret=true;
	return box.contains(int(pt.x), int(pt.y));
}

//dot(Vector2f, Vector2f)
//Purpose: compute the dot product of vectors a & b
//Input: sf::Vector2f a - point of a vector in 2D space
//		sf::Vector2f b - another point of a vector in 2D space with the same origin as vector a
//Output: float - the dot product
//---------------------------------------
//Note: for vectors A(ax, ay) and B(bx, by),
//A • B = ax*bx + ay*by (= |A|*|B|*cos θ where θ is the angle between the vectors)
float dot(sf::Vector2f a, sf::Vector2f b)
{
	float ret=(a.x*b.x) + (a.y*b.y);
	return ret;
}

//normalVector(Vector2f):
//Purpose: compute the normalized vector (magnitude 1) from the given source vector
//Input: sf::Vector2f src - a vector on the 2D plane
//Output: sf::Vector2f - the normalized reculting vector
sf::Vector2f normalVector(sf::Vector2f src)
{
	float d=calcMagnitude(src);
	sf::Vector2f normal(src.x/d, src.y/d);
	return normal;
}

//angle(Vector2f):
//Purpose: to compute the angle of a vector from 2D space to a radial space
//Input: sf::Vector2f b - the vector in 2D space to be computed
//Output: float - (in radians) the angle θ around the radial unit circle
//-----------------------------------------------------------------------
//Note: in radians, the unit circle spans 0 ≤ θ ≤ 2π
//Other Note: if the y coordinate is below the x axis, the
//arccosine of x must be negated or subtracted from π to calculate the
//correct angle
float angle(sf::Vector2f b)
{
	sf::Vector2f v=normalVector(b);
	if(v.y<0)	return acosf(v.x);
	else return -acosf(v.x);

}

//unitVector(float):
//Purpose: to fabricate a vector on the 2D plane, of magnitude 1, given θ along the unit circle
//Input: float theta - in radians, θ will determine the angle to be traversed across the unit circle
//Output: sf::Vector2f - the fabricated vector, of length 1, on the 2D plane
sf::Vector2f unitVector(float theta)
{
	return sf::Vector2f(cos(theta),-sin(theta));
}

//scalar(float, Vector2f):
//Purpose: to compute the scalar multiple of a vector (multiply its magnitude)
//Input: float c - the scalar variable to be multiplied
//		sf::Vector2f v - the vector to be multiplied
//Output: sf::Vector2f - the output is a colinear vector to v, of magnitude c*|v|
sf::Vector2f scalar(float c, sf::Vector2f v)
{
	return sf::Vector2f(c*v.x, c*v.y);
}

//initRandom(void)
//Purpose: to begin a time-seeded random generation procedure
//Input: none
//Output: none
//------------
//Note: this function is more utility, for having an unpredictable random stream of numbers
void initRandom()
{
	srand((unsigned)time(NULL));
}

//initRandom(void)
//Purpose: to begin a seed for random generation procedure
//Input: unsigned long _seed - the seed, which startes a stream of pseudo-random numbers
//Output: none
//------------
//Note: this function is more utility, for having a partially controllable random stream of numbers
void initRandom(unsigned long _seed)
{
	srand(_seed);
}

//distortedCosine(float, long)
//Purpose: to derive a random number slightly influenced by a cosine wave
//Input: float _theta - theta in radians, θ will determine the outcome of the cosine wave
//long seed - the seed value to offest the cosine wave
// Output: Pretty much a random number 0<output<2
float distortedCosine(float _theta, long seed)
{
	//output is 0<output<2
	srand(unsigned long(777*(seed+tan(_theta*(1+seed)))));
	float ret=((float(rand()%100)/100)+((1-cos(_theta))/2));
	srand((rand()%50)+(unsigned)time(NULL));
	return ret;
}


//anyRandom(long)
//Purpose: to derive a random number
//Input: long seed - the seed value
// Output: A random number
unsigned int anyRandom(long seed)
{
	float theta=float((unsigned)time(NULL))/PI;
	srand(unsigned long(777*(seed+tan(theta*(1+seed)))));
	unsigned int ret=unsigned int(float((rand()%100)/100.0f)*100.0f);
	srand((rand()%50)+(unsigned)time(NULL));
	return int(ret);
}

//newRandom(unsigned char, unsigned char, float, long)
//Purpose: to derive a random number slightly influenced by a cosine wave
//Input: unsigned char low: the low value
// unsigned char range: the range of values
// float _theta - theta in radians, θ will determine the outcome of the cosine wave
// long seed - the seed value to offest the cosine wave
// Output: Pretty much a random number low<output<(low+range)
unsigned char newRandom(unsigned char low, unsigned char range, float _theta, long seed)
{
	return unsigned char((distortedCosine(_theta, seed)/2)*range)+low;
}

//noiseyPixel(coord, unsigned char, unsigned char, int, long)
//Purpose: returns a specific format output for inserting into a sf::Color [either red, green, blue, or alpha]
//Input: coord pos: the position values of the map to be noisey'ed
// unsigned char low: the low value
// unsigned char range: the range of values
// int con - a "constant" value
// long seed - the seed value to offest the cosine wave
unsigned char noiseyPixel(coord pos, unsigned char low, unsigned char range, int con, long seed)
{
	unsigned char pixel=0;
	unsigned char value=0;
	float theta = float((sin(float(pos.x))+cos(float(pos.y)))*PI);
	if(con>0)
		value=newRandom(low, range, theta/(con+1), seed);
	else
		value=newRandom(low, range, theta, seed);

	if(value>=0 && value<=255)
		pixel=value;
	else
		pixel=low;
	return pixel;
}


//bennyPixel(coord, unsigned char, unsigned char, float, long)
//Purpose: returns a specific format output for inserting into a sf::Color [either red, green, blue, or alpha]
//Input: coord pos: the position values of the map to be noisey'ed
// unsigned char low: the low value
// unsigned char range: the range of values
// int con - a "constant" value
// long seed - the seed value to offest the cosine wave
unsigned char bennyPixel(coord pos, unsigned char low, unsigned char range, float offset)
{
	unsigned char pixel = 0;
	unsigned char value = 0;
	sf::Vector2f inmap = toVector(pos);
	inmap.x = 1.25f * float(pos.x) + offset;
	inmap.y = 1.25f * float(pos.y) + offset;
	sf::Vector2f adj = toVector(pos) + sf::Vector2f(offset, offset);
	float bugs = (perlin(inmap) + 1.0f) / 2.0f;
	value = unsigned char(bugs*float(range)) + low;

	if (value >= 0 && value <= 255)
		pixel = value;
	else
		pixel = low;
	return pixel;
}


//toVector(coord)
//Purpose: to cast a coord into a sf::Vector2f
//Input: coord src - the source coordinate 2-dimensional values
//Output: float vector version of src
sf::Vector2f toVector(coord src)
{
	return sf::Vector2f(float(src.x), float(src.y));
}

//operator==(coords)
//tests equivalence of 2 coords
//returns true if the two values are equal
bool operator==(const coord& a, const coord& b)
{
	return (a.x==b.x && a.y==b.y);
}

//operator!=(coords)
//tests inequivalence of 2 coords
//returns false if the two values are equal
bool operator!=(const coord& a, const coord& b)
{
	return (!(a.x==b.x && a.y==b.y));
}

//operator+(coords)
//adds 2 coords
//returns the sum of both x and y values
coord operator+(const coord& a, const coord& b)
{
	return coord(a.x+b.x, a.y+b.y);
}

//operator-(coords)
//subtracts 2 coords
//returns the difference of both x and y values
coord operator-(const coord& a, const coord& b)
{
	return coord(a.x-b.x, a.y-b.y);
}

//operator<(coords)
//tests if one coord is less than another
//returns true if the value of either is less, testing y first
bool operator<(const coord& a, const coord& b)
{
	if(a.y<b.y) return true;
	if(a.y>b.y) return false;
	if(a.x<b.x) return true;
	return false;
}

//operator<(coords)
//tests if one coord is greater than or equal to another
//returns true if the value of either is greater than or equal, testing y first
bool operator>=(const coord& a, const coord& b)
{
	if(a.y>b.y) return true;
	if(a.y<b.y) return false;
	if(a.x>=b.x) return true;
	return false;
}

//operator<(coords)
//tests if one coord is less than or equal to another
//returns true if the value of either is less than or equal, testing y first
bool operator<=(const coord& a, const coord& b)
{
	if(a.y<b.y) return true;
	if(a.y>b.y) return false;
	if(a.x<=b.x) return true;
	return false;
}

//operator<(coords)
//tests if one coord is greater than another
//returns true if the value of either is greater, testing y first
bool operator>(const coord& a, const coord& b)
{
	if(a.y>b.y) return true;
	if(a.y<b.y) return false;
	if(a.x>b.x) return true;
	return false;
}

//max3(int, int, int)
//Purpose: return the maximum value of 3 integers
//Inputs: 3 integers for testing
//Output: the maximum value of the 3
int max3(int a, int b, int c)
{
	int ret=0;
	if(a>b) ret=a; else ret=b;
	if(ret>c) return ret; else return c;
	return ret;
}

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]*/
float interpolate(float a0, float a1, float w) {
	/* // You may want clamping by inserting:
	 * if (0.0 > w) return a0;
	 * if (1.0 < w) return a1;*/

	return (a1 - a0) * w + a0;
	/* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
	 * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
	 *
	 * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
	 * return (a1 - a0) * (x * (w * 6.0 - 15.0) * w * w *w + 10.0) + a0;*/

}

/* Create random direction vector*/
sf::Vector2f randomGradient(int ix, int iy) {
	// Random float. No precomputed gradients mean this works for any number of grid coordinates
	float random = 2920.f * sin(ix * 21942.f + iy * 171324.f + 8912.f) * cos(ix * 23157.f * iy * 217832.f + 9758.f);
	sf::Vector2f ret;
	ret.x = cos(random);
	ret.y = sin(random);
	return ret;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
	// Get gradient from integer coordinates
	sf::Vector2f gradient = randomGradient(ix, iy);

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx*gradient.x + dy * gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {
	// Determine grid cell coordinates
	int x0 = (int)x;
	int x1 = x0 + 1;
	int y0 = (int)y;
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = interpolate(n0, n1, sx);

	value = interpolate(ix0, ix1, sy);
	return value;
}

float perlin(sf::Vector2f vec)
{
	return perlin(vec.x, vec.y);
}

float perlin(coord c)
{
	return perlin(toVector(c));
}