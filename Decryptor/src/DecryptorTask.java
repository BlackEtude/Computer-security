import org.apache.commons.codec.binary.Base64;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.util.concurrent.Callable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class DecryptorTask implements Callable<String> {
//    private char[] hexBase = new char[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    private String keypiece;
    private String initVector;
    private String encrypted;
    private int searchSpaceSize;
    private String name;
    private long max;
    private int prefixSize;
    private String start;

    DecryptorTask(String keypiece, String iv, String encrypted, String name, long max, int prefixSize, String start) {
        this.keypiece = keypiece;
        this.initVector = iv;
        this.encrypted = encrypted;
        this.name = name;
        this.prefixSize = prefixSize;
        this.start = start;
        this.max = max;
        searchSpaceSize = 64 - keypiece.length();
    }

    private byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2)
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
        return data;
    }

    private boolean isMsg(String res) {
        String substring = res.substring(2, res.length()-2);
        Pattern p = Pattern.compile("[a-zA-Z\\w\\d\\s\\p{Punct}ąćęłńóśżźĄĆĘŁŃÓŚŻŹ]*");
        Matcher m = p.matcher(substring);
        return m.matches();
    }

    private String decrypt(String key) {
        try {
            IvParameterSpec iv = new IvParameterSpec(hexStringToByteArray(initVector));
            SecretKeySpec skeySpec = new SecretKeySpec(hexStringToByteArray(key), "AES");
            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);
            return new String(cipher.doFinal(Base64.decodeBase64(encrypted)), "UTF-8");
        } catch (Exception e) {
            return null;
        }
    }

    @Override
    public String call() throws Exception {
        String prefix, prePrefix = name.substring(0, prefixSize);
        long i = Long.parseLong(start, 16);

        while (i <= max){
            prefix = String.format("%0" + searchSpaceSize + "x", (max & i));
            if (prePrefix.equals(prefix.substring(0, prefixSize))) {
                String result = decrypt(prefix + keypiece);
                if (result != null && isMsg(result))
                    DecryptorTaskManager.printToConsole("Possible solution: " + result + " from prefix: " + prefix + "\n\n", name);
            }
            else
                return "";
            i++;
        }
        return "";
    }
}