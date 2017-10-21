public class Decryptor {
    private static String keypiece = "0f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 57bits
    private static String initVector = "33a427fd1b213f4eb031ce90d92ee663"; // 32 bytes IV
    private static String encrypted = "lj7efZ8yURobG7ubb+HGcA==";

    public static void main(String[] args) {
        //EASY DATA - "Ala ma kota"
        //key = "0000f77 0f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42" //58 bits

        //alaMaKotaKotMa...
//        initVector = "a44fba66d41fe5ae4a4ac3d0a76fee74";
//        encrypted = "UXAZItgffAt0NYncw7y2WBaSBjATAgJKfq8XTnci+2ILFfrwKaG0d6GDiWKZYl2+cOcjyG8PKXRRsYaemfw/w0nUoiYvPlTykDHk1aYfjEkO90iTd11KEdvo+AZzzOVEXFewQLHx63PJSMxm1aWjjA==";
//        keypiece = "f7fc36e8003e30d2522a66e97abc549d7870b258234efc9f4ee0ee42"; // 256 bit key
//
//        //gData a)
//        initVector = "0a3366ad9633ad8abd10f13d0ee89644";
//        encrypted = "wAdSGEONchOgMilWeRl7+qUkdnP7BdqkzW8EG05yjGZqTO2fnVVDNfx5eWP5301m67vDVitqdSG84h+qhUgtwnaPHr9AzXuqfjvhIV0s79qvBYBAIIcbFlNkK5/FhDTQdP28VKmSgbnfz2RVW/TE2QzRUQCLM9PKyrwQ4fCJLR9OCTbCYHRkPLWnK7PnXnKjWLsSOI5sNQ9StUWeS8LzpPEGcDrBGznl5pSLdmvTc9Y7zOJsrXZJrEdpzwQQbWmnm2fuMYx0Z9GLx4er686+xVrXO/NCr6ocQvdeK7wLaCc=";
//        keypiece = "37f0f3f9a343eeb3ae015b9df145bdba4c7ee9ba560982867e93fe18"; // 256 bit key
//
//        //aData a)
//        initVector = "caee6de549bfbe3b1ed28c9789c922c7";
//        encrypted = "lLafSd+fDQ4D6et12MQo69qzKQU6QYZ8k+RUlvrMaB1bwjHxx1Mz1gupLBmKurI1Jsis9aLJXxqRs/nZthiRAoBCUkR9J4KhvGDtamS9omwiO8e9RG+OPKlLVW+MnpnvGVhWefX/ufXb8qNusyIB+6gBonuDpIhVHKh/k1kEicRtTzWXnKk5iqRZ9AeOMbYhpvIumOSDPWGWD/P9f380uKaX7E64VA28qZexYwu2Y+M8Ig/0/uTTBrbmvJE3dzXCjUPkyqry7gqy0yS6xXb5Gsgollz08w3iFFovSajewg2nWlLOqoTxRUhxVX5twkkG128MjdCT1SgrTvuKebaX5ghQ/lt9vHihkztJhJyogIRpiUIvBryaTuU/pkBD16vEBO6Gr6Sp57QDV+H5ibYw1Q1vNnMw/oZKRUu/yqycXlxVrHNG56mqk23u97FxaKSqbFc4lEEoTRIbMUBMiUb+1PgyfTV3Bu1Ihf/uR7TF6LzPmXNNyIjs3YQs9SP6peBM5viWb+BeEX5n2Pr/UeZ1+A==";
//        keypiece = "c122c4efcc5bf7496b444e424ef872b177382c13b09f3979c48bb724"; // 256 bit key
//
//        //mData a)
//        initVector = "1738f4f7c65d12b2a9dad887d0c6973d";
//        encrypted = "+UKZUCcTZCPA+BZulWZhZRUpplxHOs/M85HFyqy0j42BBi0PuLMD9bucYlZ7ED5V5VqRkNjRIH5BohK6B+SpGiwAbX+FtCcUEA9ATvGmgpoJihlLJHdgzv8Hj06rlN/cMb4kgVKvQ744n8rYpFZCX9reNOq+cNz0KFQWd3qsk8dowRD/8Y+QDkjlza42CiW5hdMPW3jxs9IC+/7qMSGOEzvYmS0zKEgh7PY3h3i03ac=";
//        keypiece = "b8e024f2fdfc9ae003025b52aa149c705d1d14d50727ce1bba4d9c0d"; // 256 bit key


        testEasy();
//        eightBitData();
//        gData2();
    }

    private static void testEasy() {
        char c;
        int i = 7;
        c = (char) ('0' + i);
        new DecryptorTaskManager(c+keypiece, initVector, encrypted);

    }

    private static void eightBitData() {
        char c;
        for(int i = 0; i < 10; i++) {
            c = (char) ('0' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted);

        }
        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            new DecryptorTaskManager(c+keypiece, initVector, encrypted);
        }
    }

    private static void gData2() {
        initVector = "ce22facd04bc760914bc21209ce654a6";
        encrypted = "5L2f7EDcKde0vg7WNZsaOzHB0gI7q6w1EwfdjaxVhuWkwvJDnYpviycEd7mT6+HpGQU1zd06lIuCs4GJJakwicf4aPHH779pZFy4Zbp0+o0LEzSfDruA8JaVNHi1KdUAzlvOcGeNePimPka+eJ/MnyExt+fSrp5GI4tZcB1K4+IQpNXAehrHTp4Ycaa3Qb9dwuoJfsIm4JPMzxrqeUPhZohGsXwKBJNt51+wS1LUhCdK9rQJNQrJrCoVyMGh5WsUSlrzpBs2B1lBiAeQG3dDlF+K7sczZrxt4GxrgvcwCmoYLqv8ynifZ/ki+2ssrfc+q/NRqUiZZe6vQRUEfoTT9JXKOpXx4+MvEGvr/Aj+1vbEZVYt32jk3JSCoRY0/c1T";
        keypiece = "65b6b8c512c5ddc54abdf6e0a4f19383b6161ec84bb68fa0c108100"; // 256 bit key

        char c, d;
        String s;
        for(int i = 0; i < 10; i++) {
            c = (char) ('0' + i);
            for(int j = 0; j < 10; j++) {
                d = (char) ('0' + j);
                s = "" + c + d + keypiece;
                System.out.println(s);
                new DecryptorTaskManager(s, initVector, encrypted);
            }
            for(int j = 0; j < 6; j++) {
                d = (char) ('a' + j);
                s = "" + c + d + keypiece;
                System.out.println(s);
                new DecryptorTaskManager(s, initVector, encrypted);
            }

        }

        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            for(int j = 0; j < 10; j++) {
                d = (char) ('0' + j);
                s = "" + c + d + keypiece;
                System.out.println(s);
                new DecryptorTaskManager(s, initVector, encrypted);
            }
            for(int j = 0; j < 6; j++) {
                d = (char) ('a' + j);
                s = "" + c + d + keypiece;
                System.out.println(s);
                new DecryptorTaskManager(s, initVector, encrypted);
            }
        }
    }
}