import java.util.Scanner;
public class Main{
	public static void main(String [] args){
		Scanner in = new Scanner(System.in);
		while(in.hasNextInt()){
			int n = in.nextInt();
			System.out.println("Input: " + n);
		}
	}
}
