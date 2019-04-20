//
//  filter.h
//  Follower
//
//  Created by Charles Holbrow on 11/24/18.
//

#ifndef filter_h
#define filter_h
template < class T, int SIZE >
class Filter {
private:
    T values[SIZE];
    int position = 0;
public:
    void push(T v) {
        values[position] = v;
        if (++position >= SIZE) {
            position = position % SIZE;
        }
    };
    void fill(T v) {
        for (int i = 0; i < SIZE; i++) {
            values[i] = v;
        }
    }
    T average(){
        T result;
        for (int i = 0; i < SIZE; i++) {
            result += values[i];
        }
        return result / SIZE;
    }
};

#endif /* filter_h */
