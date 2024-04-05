/*
 * absVal(x): return the absolute value of x.
 * - Assume that INT_MIN (-2^31) is not provided as an input.
 * - Ex) absVal(2) = 2, absVal(-1) = 1
 */
int absVal(int x) {
	int checkflag; // 해당 수가 양수인지 음수인지 확인하는 하나의  변수..!!

	checkflag = x >> 31; // (ex) 111....1110 -->> 111....1111 = -1
	// (ex) 011...1110 -->> 000....0000 = 0
	x = x ^ checkflag; // (ex) 111....1110 ^ 111....1111 = 000....0001
	// 결국 11번째 줄에서 보수를 취하는 것임..!!
	x = x + (~checkflag + 1); // 만약 x가 양수일 경우 checkflag가 0이되고, 만약 x가 음수일 경우 checkflag가 -1이 되기 때문에 -checkflag를 더해야 보수가 정확히 완료될 것입니다..!!
	
	return x;
}
