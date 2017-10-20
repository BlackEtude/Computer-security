import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

class DecryptorTaskManager {
    private static PrintStream stream;
    private String keySuffix;
    private String initVector;
    private String encrypted;
    private long startTime;
    private long maxValue;
    private int prefixSize = 1;

    DecryptorTaskManager (String keySuffix, String iv, String encrypted, long maxValue) {
        this.startTime = System.currentTimeMillis();
        this.keySuffix = keySuffix;
        initVector = iv;
        this.encrypted = encrypted;
        this.maxValue = maxValue;

        System.out.println("Key: " + keySuffix);

        try {
            stream = new PrintStream(System.out, true, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        createExecutor();
        System.out.println("Searching is done...\n");
        System.out.println("Time: " + (System.currentTimeMillis() - startTime) / 1000 + " sec");
    }

    private void createExecutor() {
        ExecutorService executor = Executors.newWorkStealingPool();
//            ExecutorService executor = Executors.newFixedThreadPool(8);
        List<Future<String>> list = new ArrayList<>();

//            FOR only 1 fixed:
        String s;
        char c;
        Callable<String> worker;
        Future<String> submit;

        for(int i = 0; i < 6; i++) {
            c = (char) ('a' + i);
            s = String.valueOf(c);
            worker = new DecryptorTask(keySuffix, initVector, encrypted, s, maxValue, prefixSize, s+"000000");
            submit = executor.submit(worker);
            list.add(submit);
        }

        for(int i = 0; i < 10; i++) {
            c = (char) ('0' + i);
            s = String.valueOf(c);
            worker = new DecryptorTask(keySuffix, initVector, encrypted, s, maxValue, prefixSize, s +"000000");
            submit = executor.submit(worker);
            list.add(submit);
        }

        for (Future<String> f : list) {
            try {
                if (!(f.get()).equals(""))
                    System.out.println(f.get());
            } catch (ExecutionException | InterruptedException e) {
                e.printStackTrace();
            }
        }
        executor.shutdown();
    }

    static void printToConsole(String msg, String threadName) {
        stream.print("#" + threadName + " ==> " + msg);
    }
}