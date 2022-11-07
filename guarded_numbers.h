#pragma once
//operation:
//  ensure mValues stays between mMax and mMin inclusive
//limitations:
//  no protection for max <= min
//  this will result if value always being max
//

class GuardedFloat {
private:
    float mMax, mMin, mValue;

public:
    GuardedFloat() {
        setGuards(1.0, 0.1);
        set(mMin);
    }

    //overload "=" operator using the set function defined above
    void operator=(float setTo) {
        set(setTo);
    }


    float get() const {
        return mValue;
    }

    void set(float setTo) {
        if (setTo > mMax) mValue = mMax;
        else if (setTo < mMin) mValue = mMin;
        else mValue = setTo;
    }
    
    void setGuards(float max, float min) {
        if (mMax < mMin) {
            mMax = min;
            mMin = max;
        }
        else {
            mMax = max;
            mMin = min;
        }
    }

};


class GuardedInt {
private:
    int mMax, mMin, mValue;

public:
    GuardedInt() {
        setGuards(10, 1);
        set(mMin);
    }

    //overload "=" operator using the set function defined above
    void operator=(int setTo) {
        set(setTo);
    }


    int get() const {
        return mValue;
    }

    void set(int setTo) {
        if (setTo > mMax) mValue = mMax;
        else if (setTo < mMin) mValue = mMin;
        else mValue = setTo;
    }

    void setGuards(int max, int min) {
        if (mMax < mMin) {
            mMax = min;
            mMin = max;
        }
        else {
            mMax = max;
            mMin = min;
        }
    }

};