import java.util.ArrayList;
import java.util.Scanner;

/**
 * Author: Claudio M Coppola
 * Date: 12/10/2017
 * Version: 1.0.0
 *
 * primerFinder finds prime numbers.
 */

public class PrimeFinder {

    public static void main(String[] args) {


        ArrayList<Integer> primes = new ArrayList<>();
        Scanner sc = new Scanner(System.in);
        int choice = 0;

        System.out.println("PRIME FINDER\n");

        while (choice != 5) {
            System.out.println("SELECT OPTION\n (1) CHECK IF PRIME\n (2) FIND PRIMES\n (3) DISPLAY PRIMES\n (4) CLEAR PRIMES\n (5) TERMINATE");
            choice = sc.nextByte();
            switch (choice) {
                case 1:
                    System.out.println("TYPE NUMBER");
                    int isPrime = sc.nextInt();
                    checkPrime(isPrime);
                    break;
                case 2:
                    System.out.println("MINIMUM");
                    int min = sc.nextInt();
                    System.out.println("MAXIMUM");
                    int max = sc.nextInt();
                    if (min < max){
                        primes = primeFinder(min, max);
                    }
                    else {
                        System.out.println("MAXIMUM VALUE IS LESS THAN MINIMUM");
                    }
                    break;
                case 3:
                    if (primes.isEmpty()){
                        System.out.println("PRIMES IS EMPTY, FIND PRIMES FIRST");
                    }
                    else{
                        System.out.println(primes + "\n");
                    }
                    break;
                case 4:
                    primes.clear();
                    break;
                case 5:
                    System.out.println("TERMINATING...");
                    break;
                default:
                    System.out.println("INCORRECT INPUT");
            }
        }
    }

    /**
     * Checks if the value passed is a prime number.
     *
     * @param num
     */
    public static void checkPrime(int num){
        for (int j = 2; j < num; j++) {
            if (num % j == 0) {
                System.out.println(num + " IS NOT A PRIME NUMBER\n");
                break;
            }
        }
        System.out.println(num + " IS A PRIME NUMBER\n");
    }


    /**
     * Searches between the minimum and
     * maximum values to find prime numbers.
     *
     * @param min Minimum value
     * @param max Maximum value
     */
    private static ArrayList<Integer> primeFinder(int min, int max) {

        ArrayList<Integer> primes = new ArrayList<>();

        for (int i = min; i <= max; i++) {

            boolean isPrime = true;

            for (int j = 2; j < i; j++) {
                if (i % j == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) {
                primes.add(i);
            }
        }
        return primes;
    }



}
