/**
 * W takim razie WTF z tym obcięciem klucza?
 * Jak dla mnie treść jest dość jasna teraz -> masz podany suffix 28 bajtowy dla SHA256,
 * czyli efektywnie brakuje ci tylko 4 bajtów tego SHA żeby odzyskać cały klucz.
 * Czy plaintext ma padding PKCS7 ??
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

        //a
        //Dodane do klucza: klucz : .....'f',
//        String keypiece = "c122c4efcc5bf7496b444e424ef872b177382c13b09f3979c48bb724"; // 256 bit key
//        String initVector = "e2605c0289a4962a1c4bb90095d469d4"; // 32 bytes IV
//        String encrypted = "J2p8mphOD/ZrpKMLIm/tKBwqVaNHK0SH1NANrVYImEWB+xMRR0Oa21DDMKjQTg4T2QPL7rUnT4AxgAZPcdiz1ZeiR1HQbOINGhPULENfuzJ7m5MOVChPvqpZVwDh7zVA5fnpzN0dhJHN8UvCOAx1oRfUNTQDZtKaHqPf+bBhrQiAOkSFPKNpGwBZ6xZjfuZUJkBHVWUw2SLS5/rT+UcR3V0y68qD0n78jULOat9GjhK95ZschRYy0ExK6i6K26HzUes97BtRy2qrrXDOePyFVQqBtRGw1Bz0wPVrPk8vifoSu15vhxs1vzLCCMD+uTRTVTTNhv20OHvmZs1kYlzOdxi8oG45o3m0QnEAACXOoGZZjcbXZm1tYbwPzM+1qT9vI4nG1N3TXUdj1SHT24tX09jrLRQWVcaADVDtm/DOfIiwDK9k6FhmdQPMbzgrBqEpYH+lr0lyGMSQ5AmuSJxNqLlHQ1K7sd2sURtYqvMvpqSTa7QKUA0QzqPVP080cSt7A/GOqIP5y2IhO+fDtxGy3A==";

        //Alamakota.Ale...
//        keypiece = "7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 256 bit key
//        initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
//        encrypted = "2+AemYdrAnsFVK6/VFNJlmiQuwyQBNEyfra4yxaSBRRKlsjVPGQSwDxqSXA7DROf4ObH+lXf92b60Cx1t629pjN5ou86gCJXHku2jdNXhlm1SHPNZCOtN/W8oTnG5aMsuuKPNkqOYJPuMc18zKz5NA==";
        new DecryptorTaskManager(keypiece, initVector, encrypted);
    }
    //EASY DATA
    //"Ala ma kota"
    //PREFIX:7edbf790
    //key = "7edbf7 90f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"
    private static String keypiece = "90f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 57bits
    private static String initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
    private static String encrypted = "uu9KqHy1r4DrqjhvQFG+YQ==";
}