//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import org.apache.commons.codec.binary.Base64;

class DecryptorTaskManager {
    private static PrintStream stream;
    private static String keypiece;
    private static String initVector;
    private static String encrypted;
    private char[] hexBase = new char[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    private int searchSpaceSize;
    private int alphabetLength;
    private long startTime;

    DecryptorTaskManager (String keypiece, String iv, String encrypted) {
        this.alphabetLength = this.hexBase.length;
        this.startTime = System.currentTimeMillis();
        this.keypiece = keypiece;
        initVector = iv;
        this.encrypted = encrypted;
        this.searchSpaceSize = 64 - keypiece.length();

        try {
            stream = new PrintStream(System.out, true, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        this.createExecutor();
    }

    private void printToConsole(String msg, String threadName) {
        stream.print("#" + threadName + "==> " + msg);
    }

    private void createExecutor() {
        ExecutorService executor = Executors.newWorkStealingPool();
        List<Future<String>> list = new ArrayList();

        for(int i = 0; i < 15; i++) {
            Callable<String> worker = new DecryptorTask(i, hexBase[i + 1], keypiece, initVector, encrypted);
            Future<String> submit = executor.submit(worker);
            list.add(submit);
        }

        System.out.println("Amount of tasks: " + list.size());

        for (Future f : list) {
            try {
                if (!(f.get()).equals(""))
                    System.out.println((String) f.get());
            } catch (ExecutionException | InterruptedException e) {
                e.printStackTrace();
            }
        }
        executor.shutdown();
    }

    class DecryptorTask implements Callable<String> {
        private String keypiece;
        private String initVector;
        private String encrypted;
        int minPrefix;
        char maxPrefix;

        DecryptorTask(int startTransition, char maxPrefix, String keypiece, String iv, String encrypted) {
            this.minPrefix = startTransition;
            this.keypiece = keypiece;
            this.initVector = iv;
            this.encrypted = encrypted;
            this.maxPrefix = maxPrefix;
        }

        String getName() {
            return (minPrefix + "");
        }

        private String bruteForceDecrypt(int alphabetPosition, String prefix, char maxPrefix, int k) {
            if (k == 0) {
                String res = decrypt(prefix + keypiece);
//                if (res != null && analyzeSolution(res)) {
                if (res != null && isMsg(res)) {
                    printToConsole("Possible solution: " + res + " from prefix: " + prefix + "\n\n", getName());
//                    if (isMsg(res)) {
                        return ("SOLUTION: " + res + "\nPREFIX: " + prefix + "\n:" + getName());
//                    }
                }
                return "";

            } else {
                for(int i = alphabetPosition; i < alphabetLength; ++i) {
                    String newPrefix = prefix + hexBase[i];
                    if (k == 1 && prefix.charAt(0) == maxPrefix) {
                        return "";
                    }
                    bruteForceDecrypt(0, newPrefix, maxPrefix, k - 1);
                }
                return "";
            }
        }

        private String decrypt(String key) {
            try {
                IvParameterSpec iv = new IvParameterSpec(hexStringToByteArray(initVector));
                SecretKeySpec skeySpec = new SecretKeySpec(hexStringToByteArray(key), "AES");
                Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
                cipher.init(2, skeySpec, iv);
                byte[] original = cipher.doFinal(Base64.decodeBase64(encrypted));
                return new String(original);
            } catch (Exception var6) {
                return null;
            }
        }

        private byte[] hexStringToByteArray(String s) {
            int len = s.length();
            byte[] data = new byte[len / 2];

            for(int i = 0; i < len; i += 2)
                data[i / 2] = (byte)((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));

            return data;
        }

        private boolean isMsg(String res) {
//            int max = Math.min(res.length());
            String substring = res.substring(0,res.length());
            Pattern p = Pattern.compile("[a-zA-Z\\w\\d\\s\\p{Punct}ąćęłńóśżźĄĆĘŁŃÓŚŻŹ]*");
//            Matcher m = p.matcher(res);
            Matcher m = p.matcher(substring);
            return m.matches();
        }

        private boolean analyzeSolution(String res) {
            for(int i = 0; i < res.length(); ++i) {
                int v = res.charAt(i);
                if (v > 700)
                    return false;
            }
            return true;
        }

        public String call() throws Exception {
            String result =  bruteForceDecrypt(minPrefix, "", maxPrefix, searchSpaceSize);
            return result;
        }
    }
}