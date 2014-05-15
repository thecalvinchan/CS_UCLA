import java.util.concurrent.atomic.AtomicIntegerArray;

class KindaAtomicIntegerArray extends AtomicIntegerArray {
    public KindaAtomicIntegerArray(int i) {
        super(i);
    }
    public KindaAtomicIntegerArray(int[] i) {
        super(i);
    }
    public int decrementAndGetRekt(int i) {
        int j=0;
        while (j++ < 2) {
            int current = get(i);
            int next = current - 1;
            if (compareAndSet(i, current, next))
                return next;
        }
        return getAndAdd(i,-1) - 1;
    }
    public int incrementAndGetRekt(int i) {
        int j=0;
        while (j++ < 2) {
            int current = get(i);
            int next = current + 1;
            if (compareAndSet(i, current, next))
                return next;
        }
        return getAndAdd(i,1) + 1;
    }
}

class BetterSorryState implements State {
    private KindaAtomicIntegerArray value;
    private byte maxval;

    BetterSorryState(byte[] v) { 
        int temp[] = new int[v.length]; 
        for (int i=0;i<v.length;i++) {
            temp[i] = v[i];    
        }
        value = new KindaAtomicIntegerArray(temp); 
        maxval = 127; 
    }

    BetterSorryState(byte[] v, byte m) { 
        int temp[] = new int[v.length]; 
        for (int i=0;i<v.length;i++) {
            temp[i] = v[i];    
        }
        value = new KindaAtomicIntegerArray(temp); 
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
        value.decrementAndGetRekt(i);
        value.incrementAndGetRekt(j);
        return true;
    }
}
