
public class Test {
    private static char[] hexBase = new char[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    private static int alphabetLength = hexBase.length;
    private static String keypiece = "bf790f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 57bits
    private static String initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
    private static String encrypted = "uu9KqHy1r4DrqjhvQFG+YQ==";
    private static int counter;
    private static int searchSpaceSize = 64 - keypiece.length();

    public static void main(String args[]) {
        counter = 0;
        System.out.println("SOLUTION: " + bruteForceDecrypt(0, "000", 'g', searchSpaceSize));
        System.out.println("Counter: " + counter);
    }

    private static int getSignValue(char c) {
        switch(c) {
            case 'f': return 15;
            case 'e': return 14;
            case 'd': return 13;
            case 'c': return 12;
            case 'b': return 11;
            case 'a': return 10;
            default: break;
        }
        return (c-'0');
    }

    private static boolean decrypt(String msg) {
        return msg.equals("7ed");
    }

    private static String bruteForceDecrypt(int alphabetPosition, String prefix, char maxPrefix, int k) {
        int current = searchSpaceSize-1;
        //start => aaa
        int value;

        while(current >= 0) {
            System.out.println("Current: " + current);
            System.out.println("Char: " + prefix.charAt(current));
            value = getSignValue(prefix.charAt(current));

            while(value < hexBase.length) {
                System.out.println("Value at position " + current + " : " + value);
                String newPrefix = prefix.substring(0, current) + hexBase[value+1] + prefix.substring(current + 1);
                System.out.println("New: " + newPrefix);
                value = hexBase[value+1];
            }
            current--;
        }

        return "";
//        if (k == 0) {
//            counter++;
////            System.out.println(prefix);
//            Boolean res = decrypt(prefix);
//            if (res) {
//                System.out.println("Possible solution: " + res + " from prefix: " + prefix + "\n\n");
//                return ("SOLUTION: " + res + "\nPREFIX: " + prefix);
//            }
//            return "";
//
//        } else {
//            for(int i = alphabetPosition; i < alphabetLength; ++i) {
//                String newPrefix = prefix + hexBase[i];
//                if (k == 1) {
//                    if(prefix.charAt(0) == maxPrefix) {
//                        System.out.println("Terminate this task");
//                        //terminate this task
//                        return "";
//                    }
//                }
//                bruteForceDecrypt(0, newPrefix, maxPrefix, k - 1);
//            }
//            return "";
//        }
    }
}
