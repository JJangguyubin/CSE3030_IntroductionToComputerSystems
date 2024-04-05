/*
 * conditional(x, y, z): Return z if x is 0, return y otherwise.
 * - Ex) conditional(2, 4, 5) = 4, conditional(0, 1, 2) = 2
 */
int conditional(int x, int y, int z) {
	int num1;
	int num2;

	num1 = (~(!x) + 1) & z; 
	num2 = ~(~(!x) + 1) & y;
	
	return num1 + num2;
}
