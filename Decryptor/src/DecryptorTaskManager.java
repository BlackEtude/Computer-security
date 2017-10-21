import org.apache.commons.codec.binary.Base64;
import javax.crypto.Cipher;
import javax.crypto.spec.*;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.regex.*;

class DecryptorTaskManager {
    private static PrintStream stream;
    private String keySuffix;
    private String initVector;
    private String encrypted;
    private long maxValue;

    DecryptorTaskManager (String keySuffix, String iv, String encrypted) {
        long startTime = System.currentTimeMillis();
        this.keySuffix = keySuffix;
        initVector = iv;
        this.encrypted = encrypted;
        this.maxValue = 0xfffffff;

        try {
            stream = new PrintStream(System.out, true, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        System.out.println("Key: " + keySuffix);
        startExecutor();
        System.out.println("Searching is done...");
        System.out.println("Time: " + (System.currentTimeMillis() - startTime) / 1000 + " sec" + "\n");
    }

    private void startExecutor() {
//        ExecutorService executor = Executors.newWorkStealingPool();
        ExecutorService executor = Executors.newFixedThreadPool(8);
        List<Future<String>> list = new ArrayList<>();

//        FOR only 1 fixed:
        Callable<String> worker;
        Future<String> submit;
        int prefixFixedSize = 1;
        String s;
        char c;

        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            s = String.valueOf(c);
            worker = new DecryptorTask(s, maxValue, prefixFixedSize, s+"000000");
            submit = executor.submit(worker);
            list.add(submit);
        }

        for(int i = 0; i < 10; i++) {
            c = (char) ('0' + i);
            s = String.valueOf(c);
            worker = new DecryptorTask(s, maxValue, prefixFixedSize, s +"000000");
            submit = executor.submit(worker);
            list.add(submit);
        }

        for (Future<String> f : list) {
            try {
//                if (!(f.get()).equals(""))
                    System.out.println(f.get());
            } catch (ExecutionException | InterruptedException e) {
                e.printStackTrace();
            }
        }
        executor.shutdown();
    }

    private static void printToConsole(String msg, String taskNumber) {
        stream.print("#" + taskNumber + " ==> " + msg);
    }

    class DecryptorTask implements Callable<String> {
        private String name;
        private String start;
        private int searchSpace;
        private long max;
        private int prefixSize;

        DecryptorTask(String name, long max, int prefixSize, String start) {
            this.name = name;
            this.prefixSize = prefixSize;
            this.start = start;
            this.max = max;
            searchSpace = 64 - keySuffix.length();
        }

        @Override
        public String call() throws Exception {
            String prefix, prePrefix = name.substring(0, prefixSize);
            long i = Long.parseLong(start, 16);

            while (i <= max){
                prefix = String.format("%0" + searchSpace + "x", (max & i));
                if (prePrefix.equals(prefix.substring(0, prefixSize))) {
                    String result = decryptWithKey(prefix + keySuffix);
                    if (result != null && isCorrectMsg(result))
                        printToConsole("Possible solution: " + result + " from prefix: " + prefix + "\n\n", name);
                }
                else
                    return "";
                i++;
            }
            return "";
        }

        private String decryptWithKey(String key) {
            try {
                IvParameterSpec iv = new IvParameterSpec(hexStringToByteArray(initVector));
                SecretKeySpec skeySpec = new SecretKeySpec(hexStringToByteArray(key),"AES");
                Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
                cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);
                return new String(cipher.doFinal(Base64.decodeBase64(encrypted)),"UTF-8");
            } catch (Exception e) {
                return null;
            }
        }

        private byte[] hexStringToByteArray(String s) {
            int len = s.length();
            byte[] data = new byte[len / 2];
            for (int i = 0; i < len; i += 2)
                data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
            return data;
        }

        private boolean isCorrectMsg(String decryptedMsg) {
            String substring = decryptedMsg.substring(2, decryptedMsg.length()-2);
            Pattern p = Pattern.compile("[a-zA-Z\\w\\d\\s\\p{Punct}ąćęłńóśżźĄĆĘŁŃÓŚŻŹ]*");
            Matcher m = p.matcher(substring);
            return m.matches();
        }
    }
}