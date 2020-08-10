#include <iostream>
using namespace std;

// Assigning Object
class MyCopy {
   int a;
   int b;
public:
   void setab(int i, int j) {
      a = i;
      b = j;
   }
   void showab() {
      cout << "a is " << a << '\n';
      cout << "b is " << b << '\n';
   }
};

// Pointer Object
class MyPointer {
   int num;
public:
   void setNum(int val) {
      num = val;
   }
   void getNum() {
      cout << num << "\n";
   }
};


// Array Object
class MyArray {
   int x;
   int y;
public:
   MyArray() {};
   MyArray(int i) {
      x = i;
   }
   MyArray(int i, int j) {
      x = i;
      y = j;
   }
   void setX(int i) {
      x = i;
   }
   int getX() {
      return x;
   }
   int getY() {
      return y;
   }
};


// Queue Class
const int maxQsize = 100;
class Queue {
   char q[maxQsize];
   int size;
   int putloc;
   int getloc;
public:
   Queue(int len) {
      if (len > maxQsize) len = 1;
      else if (len <= 0) len = 1;

      size = len;
      putloc = getloc = 0;
   }
   void put(char ch) {
      if (putloc == size) {
          cout << " -- Queue is full.\n";
          return;
      }
      putloc++;
      q[putloc] = ch;
   }
   char get() {
      if (getloc == putloc) {
          cout << " -- Queue is empty.\n";
          return 0;
      }
      getloc++;
      return q[getloc];
   }
}; //end


// setter dan getter
class MySetter {
   int i; // private by default
public:
   int getI();
   void setI(int j);

   int autoGetI() {
      return i;
   }
   void autoSetI(int j) {
      i = j;
   }
};
int MySetter::getI() {
   return i;
}
void MySetter::setI(int j) {
   i = j;
}


// constructor berparameter
class MyClass {
public:
   int x;
   MyClass(int i);
   ~MyClass();
};
MyClass::MyClass(int i) {
   x = i;
}
MyClass::~MyClass() {
   cout << "Destructing object whose x value is " << x << std::endl;
}


class Vehicle { // class
public:
   int passengers;
   int fuelcap;
   int mpg;

   // fungsi
   int range();

   // deklarasi constructor dan destructor
   int x;
   Vehicle();
   ~Vehicle();
};

// implementasi fungsi
int Vehicle::range()
{
   return mpg * fuelcap;
}

// implementasi Vehicle constructor
Vehicle::Vehicle() {
   x = 10;
}
// implementasi Vehicle destructor
Vehicle::~Vehicle() {
   cout << "Destructing...\n";
}

int bab1() // class dan objek
{
   Vehicle minivan;
   Vehicle sportcar;

   int range1, range2;

   minivan.passengers = 7;
   minivan.fuelcap = 16;
   minivan.mpg = 21;

   sportcar.passengers = 2;
   sportcar.fuelcap = 14;
   sportcar.mpg = 12;

   //range1 = minivan.fuelcap * minivan.mpg;
   //range2 = sportcar.fuelcap * sportcar.mpg;

   range1 = minivan.range();
   range2 = sportcar.range();

   cout << "Minivan can carry " << minivan.passengers << " With a range of " << range1 << std::endl;
   cout << "Sportcar can carry " << sportcar.passengers << " With a range of " << range2 << std::endl;

   cout << minivan.x << " " << sportcar.x << std::endl;
   return 0;
}

int bab2() { // constructor paramater
   MyClass t1(5); // atau t1 = 5
   MyClass t2(19);

   cout << t1.x << " " << t2.x << std::endl;

   return 0;
}

int bab3() { // setter dan getter
   MySetter s;
   s.setI(10);

   cout << "Luar: " << s.getI() << std::endl;

   s.autoSetI(9);

   cout << "Auto: " << s.autoGetI() << std::endl;
   return 0;
}

int bab4() { // queue class
   Queue bigQ(100);
   Queue smallQ(4);
   char ch;
   int i;

   cout << "Using bigQ to store the alphabet.\n";
   for (i=0; i<26; i++)
      bigQ.put('A' + i);

   cout << "Contents of BigQ: ";
   for (i=0; i<26; i++) {
      ch = bigQ.get();

      if (ch != 0)
         cout << ch;
   }

   cout << "\n\n";

   cout << "Using smallQ to generate errors.\n";
   for (i=0; i<5; i++) {
       cout << "Attempting to store " << (char) ('Z' -i);
       smallQ.put('Z' - i);

       cout << "\n";
   }
   cout << "\n";

   cout << "Contents of smallQ: ";
   for (i=0; i<5; i++) {
       ch = smallQ.get();

       if (ch != 0) cout << ch;
   }
   cout << "\n";
}

int bab5() { // array objek
   MyArray obs[4];
   int i;

   for (i=0; i<4; i++)
      obs[i].setX(i);

   for (i=0; i<4; i++)
      cout << "obs[" << i << "].getX(): " << obs[i].getX() << std::endl;

   cout << "\n\n";

   MyArray obj[4] = { -1, -2, -3, -4 };

   for (i=0; i<4; i++)
      cout << "obj[" << i << "].getX(): " << obj[i].getX() << "\n";

   cout << "\n\n";

   MyArray oba[4][2] = {
      MyArray(1, 2), MyArray(3, 4),
      MyArray(5, 6), MyArray(7, 8),
      MyArray(9, 10), MyArray(11, 12),
      MyArray(13, 14), MyArray(15, 16)
   };

   for (i=0; i<4; i++) {
      cout << oba[i][0].getX() << ' ';
      cout << oba[i][0].getY() << "\n";
      cout << oba[i][1].getX() << ' ';
      cout << oba[i][1].getY() << "\n";
   }
   return 0;
}

int bab6() { // pointer object
   MyPointer ob, *p;
   ob.setNum(1);
   ob.getNum();

   p = &ob;
   p->setNum(20);
   p->getNum();

   cout << "\n\n";

   MyPointer po[2], *a;
   po[0].setNum(10);
   po[1].setNum(20);

   a = &po[0];
   a->getNum();
   a++;
   a->getNum();
   a--;
   a->getNum();
   return 0;
}

int bab7() {
   MyCopy ob1, ob2;
   ob1.setab(10, 20);
   ob2.setab(0, 0);
   cout << "ob1 sebelum assign: \n";
   ob1.showab();
   cout << "ob2 sebelum assign: \n";
   ob2.showab();
   cout << '\n';

   ob2 = ob1;
   cout << "ob1 setelah assign: \n";
   ob1.showab();
   cout << "ob2 setelah assign: \n";
   ob2.showab();
   cout << '\n';

   ob1.setab(-1, -1);

   cout << "ob1 setelah reset ob1: \n";
   ob1.showab();
   cout << "ob2 setetlah reset ob2: \n";
   ob2.showab();

   return 0;
}

int main() {
   return bab7();
}
