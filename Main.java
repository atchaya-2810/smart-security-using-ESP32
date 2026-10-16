class OrderThread extends Thread {
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.println("Order Process : Order " + i + " Placed");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                System.out.println(e);
            }
        }
    }
}
class DeliveryThread implements Runnable {
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.println("Delivery Status : Order " + i + " Dispatched");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                System.out.println(e);
            }
        }
    }
}
public class Main {
    public static void main(String[] args) {

        // Thread using Thread class
        OrderThread t1 = new OrderThread();

        // Thread using Runnable interface
        Thread t2 = new Thread(new DeliveryThread());

        t1.start();
        t2.start();
    }
}