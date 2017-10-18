/**
 * W takim razie WTF z tym obcięciem klucza?
 * Jak dla mnie treść jest dość jasna teraz -> masz podany suffix 28 bajtowy dla SHA256,
 * czyli efektywnie brakuje ci tylko 4 bajtów tego SHA żeby odzyskać cały klucz.
 * Ja bym w takim razie brutował po prostu te 4 brakujące bajty i liczył na to,
 * że plaintext ma padding PKCS7 i na tej podstawie można rozpoznać poprawne deszyfrowanie.
 */
public class Decrypt {
    /**
     * ANALIZA:
     * 3 bits :
     *      1 => 0.247s
     * 4 bits :
     *      2 => 0.52s
     * 5 bits :
     *      1 =>   (threads: 16)
     *      2 =>   (threads: 256)
     *      3 =>   (threads: 65536)
     * 6 bits :
     *      2 => 42s (threads: 256)
     *      3 => 42
     *      4 => 43
     *      5 =>
     * 7 bits:
     *      5 => 720
     * 8 bits:
     *      5 =>
     */


    public static void main(String[] args) {
        new DecryptorTaskManager(keypiece, initVector, encrypted);
        //a
//        String keypiece = "c122c4efcc5bf7496b444e424ef872b177382c13b09f3979c48bb724"; // 256 bit key
//        String initVector = "8bd996f32219d7194510973f1b0c5f18"; // 32 bytes IV
//        String encrypted = "pTys94RSq1YsaL2WrqSHSgxSFLfSRBDsvsrE1SRrYrZfDABnqexeZkPCbVpKcqzxj1YbIKh6QTne+hUy0" +
//                "B7nitAHiIgrkC/+z3apX2hsBPqigPmSV5vs/iE5Q0Dq0oq5pWqz6/4y77hWw900Fy/IbSlQl+w5ujJWwyNJ3RxOehuVxujR" +
//                "ghoHjm3N1jJTURqKliY97fDvP+QVHi2D3LcYQ1notxwBuEDR7EptVXGgObMaoFMzRAvySFotOBObK96GhVYziEI19hsURBTjwsX" +
//                "OF/GDBOhqmW9BwMfhC3o12eumFPEJgqzf1IF6ECm01HFr8fUucWo31YNgSJOF9NiWmYjqaLpEaGvxyYwn+N2y6HswcKP7STa+yA4bDr" +
//                "I77VIhCVs7FhXoZM1bK3+TWQjZKh/8eQ7Vdg/xCYAj7cEGRykk6fyHRr4nWvGKiaK1S+XOGH1P5RnnfddFYaYmT5EhkX234MFSm68U3P9qF" +
//                "9OFzAwyiZJeVriTaj+1dFKnkAeCg8ycKK/YgGKrhSCK/WcEXg==";

        //Alamakota.Ale...
//        keypiece = "f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 256 bit key
//        initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
//        encrypted = "2+AemYdrAnsFVK6/VFNJlmiQuwyQBNEyfra4yxaSBRRKlsjVPGQSwDxqSXA7DROf4ObH+lXf92b60Cx1t629pjN5ou86gCJXHku2jdNXhlm1SHPNZCOtN/W8oTnG5aMsuuKPNkqOYJPuMc18zKz5NA==";
    }
    //EASY DATA
    //"Ala ma kota"
    //PREFIX:7edbf790
    //key = "7edbf790 f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"
    private static String keypiece = "bf790f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 57bits
    private static String initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
    private static String encrypted = "uu9KqHy1r4DrqjhvQFG+YQ==";
}