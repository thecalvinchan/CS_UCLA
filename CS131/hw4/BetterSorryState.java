import java.util.concurrent.atomic.AtomicIntegerArray;

class BetterSafeState implements State {
    private AtomicIntegerArray value;
    private byte maxval;

    BetterSafeState(byte[] v) { 
        int temp[] = new int[v.length]; 
        for (int i=0;i<v.length;i++) {
            temp[i] = v[i];    
        }
        value = new AtomicIntegerArray(temp); 
        maxval = 127; 
    }

    BetterSafeState(byte[] v, byte m) { 
        int temp[] = new int[v.length]; 
        for (int i=0;i<v.length;i++) {
            temp[i] = v[i];    
        }
        value = new AtomicIntegerArray(temp); 
        maxval = m; 
    }

    public int size() { return value.length(); }

    public byte[] current() { 
        byte val[] = new byte[size()];
        for (int i=0;i<size();i++) {
            val[i] = (byte) value.get(i);
        }
        return val; 
    }

    public boolean swap(int i, int j) {
        if (value.get(i) <= 0 || value.get(j) >= maxval) {
            return false;
        }
        value.decrementAndGet(i);
        value.incrementAndGet(j);
        return true;
    }
}
