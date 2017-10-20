public class Decryptor {
    private static String keypiece = "f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 57bits
    private static String initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
    private static String encrypted = "uu9KqHy1r4DrqjhvQFG+YQ==";

    public static void main(String[] args) {
        //EASY DATA
        //"Ala ma kota"
        //PREFIX:7edbf 90
        //key = "7edbf790 f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"

        aData();
    }

    private static void gData() {
        initVector = "0a3366ad9633ad8abd10f13d0ee89644";
        encrypted = "wAdSGEONchOgMilWeRl7+qUkdnP7BdqkzW8EG05yjGZqTO2fnVVDNfx5eWP5301m67vDVitqdSG84h+qhUgtwnaPHr9AzXuqfjvhIV0s79qvBYBAIIcbFlNkK5/FhDTQdP28VKmSgbnfz2RVW/TE2QzRUQCLM9PKyrwQ4fCJLR9OCTbCYHRkPLWnK7PnXnKjWLsSOI5sNQ9StUWeS8LzpPEGcDrBGznl5pSLdmvTc9Y7zOJsrXZJrEdpzwQQbWmnm2fuMYx0Z9GLx4er686+xVrXO/NCr6ocQvdeK7wLaCc=";
        keypiece = "37f0f3f9a343eeb3ae015b9df145bdba4c7ee9ba560982867e93fe18"; // 256 bit key

        char c;
        for(int i = 0; i < 10; i++) {
            c = (char) ('0' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted,0xfffffff);

        }

        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted,0xfffffff);
        }
    }

    private static void aData() {
        initVector = "caee6de549bfbe3b1ed28c9789c922c7";
        encrypted = "lLafSd+fDQ4D6et12MQo69qzKQU6QYZ8k+RUlvrMaB1bwjHxx1Mz1gupLBmKurI1Jsis9aLJXxqRs/nZthiRAoBCUkR9J4KhvGDtamS9omwiO8e9RG+OPKlLVW+MnpnvGVhWefX/ufXb8qNusyIB+6gBonuDpIhVHKh/k1kEicRtTzWXnKk5iqRZ9AeOMbYhpvIumOSDPWGWD/P9f380uKaX7E64VA28qZexYwu2Y+M8Ig/0/uTTBrbmvJE3dzXCjUPkyqry7gqy0yS6xXb5Gsgollz08w3iFFovSajewg2nWlLOqoTxRUhxVX5twkkG128MjdCT1SgrTvuKebaX5ghQ/lt9vHihkztJhJyogIRpiUIvBryaTuU/pkBD16vEBO6Gr6Sp57QDV+H5ibYw1Q1vNnMw/oZKRUu/yqycXlxVrHNG56mqk23u97FxaKSqbFc4lEEoTRIbMUBMiUb+1PgyfTV3Bu1Ihf/uR7TF6LzPmXNNyIjs3YQs9SP6peBM5viWb+BeEX5n2Pr/UeZ1+A==";
        keypiece = "c122c4efcc5bf7496b444e424ef872b177382c13b09f3979c48bb724"; // 256 bit key

        char c;
        for(int i = 0; i < 10; i++) {
            c = (char) ('0' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted,0xfffffff);

        }

        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted,0xfffffff);
        }
    }

    private static void alaMaKotaData() {
        initVector = "a44fba66d41fe5ae4a4ac3d0a76fee74";
        encrypted = "UXAZItgffAt0NYncw7y2WBaSBjATAgJKfq8XTnci+2ILFfrwKaG0d6GDiWKZYl2+cOcjyG8PKXRRsYaemfw/w0nUoiYvPlTykDHk1aYfjEkO90iTd11KEdvo+AZzzOVEXFewQLHx63PJSMxm1aWjjA==";
        keypiece = "f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 256 bit key

        char c;
        for(int i = 1; i < 10; i++) {
            c = (char) ('0' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted,0xfffffff);

        }

        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted,0xfffffff);
        }
    }
}