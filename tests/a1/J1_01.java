// PARSER_WEEDER,CODE_GENERATION
public class J1_01 {
    public J1_01() {}
    public static int test() {
	int r1 = J1_01.m0(0);
	int r2 = J1_01.m0(1);
	int r3 = J1_01.m0(100);
	int r4 = J1_01.m1(0);
	int r5 = J1_01.m1(1);
	int r6 = J1_01.m1(100);

	int r = 0;
	if (r1==9) {
		for(int x = 2; x < r1; x = x+1) {
			r=r+1;
			x = x+2;
		}
		int z = 368/4;
	}
	{
		int x = 5;
		int j = 6;
		int y = x+j;
		for(new J1_01(); y < x; x = x+y);
	}

	if(true)
		for(;;) 
			if (true) 
				r = 2;
			else
				r6 = 3;
	else {}
			

	if (r2==6) while(true) r=r+1;
	if (r3==6) r=r+1;

	if (r4==6) r=r+1;
	if (r5==9) r=r+1;
	if (r6==6) r=r+1;
	
	return 117+r;
    }

    public static int m0(int a) {
	int r = 0;
	if (a==0) r=r+1;
	if (a!=0) r=r+2;
	if (!(a==0)) r=r+4;
	if (!(a!=0)) r=r+8;
	return r;
    }

    public int[] z;

    public static int m1(int a) {
	int r = 0;
	if (a==1) r=r+1;
	if (a!=1) r=r+2;
	if (!(a==1)) r=r+4;
	if (!(a!=1)) r=r+8;
	return r;
    }

    public char x;
    protected int zyyy;
}
