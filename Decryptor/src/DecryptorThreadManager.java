import org.apache.commons.codec.binary.Base64;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class DecryptorThreadManager {
    private ArrayList<DecryptorThread> threads;
    private static PrintStream stream;
    private static String keypiece;
    private static String initVector;
    private static String encrypted;
    private char[] hexBase = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    private int searchSpaceSize;
    private long startTime;

    DecryptorThreadManager(String keypiece, String iv, String encrypted) {
        startTime = System.currentTimeMillis();
        this.keypiece = keypiece;
        initVector = iv;
        this.encrypted = encrypted;
        searchSpaceSize = 64 - keypiece.length();
        try {
            stream = new PrintStream(System.out, true, "UTF-8");
//            decryptor.fileStream = new PrintStream(new FileOutputStream("file.txt"), true, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        threads = new ArrayList<>();
        runThreads();
    }

    private void printToConsole(String msg, String threadName) {
        stream.print("#" + threadName + "==> " + msg);
    }

    private void runThreads() {
        String n;
        int numberOfThreads = 4;
        createThread(0, 4);
    }

    private void createThread(int aPosition, int maxPrefix) {
        DecryptorThread newThread = new DecryptorThread("Thread " + aPosition, keypiece, initVector, encrypted);
        threads.add(newThread);
        int index = threads.indexOf(newThread);
        threads.get(index).start();
        threads.get(index).brute("", searchSpaceSize, aPosition, maxPrefix);
    }

    class DecryptorThread extends Thread {
        private String keypiece, initVector, encrypted;
        //        private char[] hexBase = {'d','3','a','0','c','b','1','2','4','5','6','7','8','9','e','f'};
//        private char[] hexBase = {'f','e','d','c','b','a','9','8','7','6','5','4','3','2','1','0'};
        private int alphabetLength = hexBase.length;
        int counter = 0;

        DecryptorThread (String name, String keypiece, String iv, String encrypted) {
            super(name);
            this.keypiece = keypiece;
            this.initVector = iv;
            this.encrypted = encrypted;
        }

        public void run() {
//            System.out.println("Run: "+ getName());
        }

        private void brute(String prefix, int k, int alphabetPosition, int maxPrefix) {
            bruteForceDecrypt(alphabetPosition, prefix, k, maxPrefix);
            System.out.println("Counter: " + counter);
        }

        private void bruteForceDecrypt(int alphabetPosition, String prefix, int k, int maxPrefix) {
            System.out.println(prefix);

            // Base case: k is 0, print prefix
            if (k == 0) {
                counter++;
                String res = decrypt(prefix + keypiece);
                if(res != null) {
                    if(analyzeSolution(res)) {
                        printToConsole("Possible solution: " + res + "\n\n", getName());
                        if (isMsg(res)) {
                            printToConsole("----------------------------------------------------------------------------------------------------------------\n" +
                                    "SOLUTION: " + res + "----------------------------------------------------------------------------------------------------------------\n", getName());
                            printToConsole("PREFIX: " + prefix + "\n", getName());
                            printToConsole("KEY: " + (prefix + keypiece) + "\n", getName());
                            ending();
                        }
                    }
                }
                return;
            }

            // One by one add all characters from set and recursively
            // call for k equals to k-1
            for (int i = alphabetPosition; i < alphabetLength; ++i) {
                // Next character of input added
                String newPrefix = prefix + hexBase[i];
                if(k == 0 )

//                if(k == (patternLength )) {
//                    interrupt();
//                }
                    // k is decreased, because we have added a new character
//                    printToConsole("Thread " + getName() + " recursive : " + newPrefix + ", " + (k-1) + "\n", getName());
                bruteForceDecrypt(0, newPrefix, k - 1, maxPrefix);
            }
        }

        private void ending() {
//            fileStream.println(res);
            printToConsole("Number of threads: " + threads.size() + "\n", getName());
            printToConsole("Time elapsed: " + (double)((System.currentTimeMillis() - startTime))/1000 + "\n", getName());
//            System.exit(0);
        }

        private String decrypt(String key)  {
            try {
                IvParameterSpec iv = new IvParameterSpec(hexStringToByteArray(initVector));
                SecretKeySpec skeySpec = new SecretKeySpec(hexStringToByteArray(key), "AES");
                Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
                cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);
                byte[] original = cipher.doFinal(Base64.decodeBase64(encrypted));
                return new String(original);
            } catch (Exception ex) {
//            ex.printStackTrace();
            }
            return null;
        }

        private byte[] hexStringToByteArray(String s) {
            int len = s.length();
            byte[] data = new byte[len / 2];
            for (int i = 0; i < len; i += 2)
                data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
            return data;
        }

        private boolean isMsg(String res){
            Pattern p = Pattern.compile("[a-zA-Z\\w\\d\\s\\p{Punct}ąćęłńóśżźĄĆĘŁŃÓŚŻŹ]*");
            Matcher m = p.matcher(res);
            return m.matches();
        }

        private boolean analyzeSolution(String res) {
            for(int i = 0; i < res.length(); i++) {
                int v = (int)res.charAt(i);
                if(v > 1000)
                    return false;
            }
            return true;
        }
    }
}
