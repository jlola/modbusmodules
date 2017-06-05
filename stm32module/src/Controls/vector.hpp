/*
 * vector.hpp
 *
 *  Created on: 21.7.2011
 *      Author: PC
 */

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

template<typename Data>
class Vector {
   size_t d_size; // Stores no. of actually stored objects
   size_t d_capacity; // Stores allocated capacity
   Data *d_data; // Stores data
   public:
     Vector() :
	 	d_size(0), d_capacity(0), d_data(0)
	 {
	 }; // Default constructor


	 Vector(Vector const &other) : d_size(other.d_size), d_capacity(other.d_capacity), d_data(0)
	 {
		//d_data = (Data *)malloc((size_t)(d_capacity*sizeof(Data)));
		d_data = (Data *)new Data[d_capacity];
		memcpy(d_data, other.d_data, d_size*sizeof(Data));
	 }; // Copy constuctor

     ~Vector()
	 {
    	for (uint8_t i=0;i<d_capacity;i++) delete(d_data[i]);
	 	delete[] d_data;
	 } // Destructor

     Vector &operator=(Vector const &other)
     {
    	 free(d_data);
    	 d_size = other.d_size;
    	 d_capacity = other.d_capacity;
    	 //d_data = (Data *)malloc(d_capacity*sizeof(Data));
    	 d_data = (Data *)new Data[d_capacity];
    	 memcpy(d_data, other.d_data, d_size*sizeof(Data));
    	 return *this;
     } // Needed for memory management

	 void RemoveAt(unsigned int i)
	 {
		 if (i < 0 || i > d_capacity) return;
	 	int prevsize = i;
	 	int aftersize = d_capacity - i - 1;
	 	d_capacity = prevsize + aftersize;
	 	d_size = d_capacity;

	 	Data *newdata = (Data *)new Data[d_capacity];
	 	if (prevsize>0)
			memcpy(newdata, d_data, prevsize * sizeof(Data));
	 	if (aftersize>0)
			memcpy(&newdata[i], &d_data[i+1], aftersize * sizeof(Data));
	 	delete(d_data);
	 	d_data = newdata;
	 }

     void Add(Data x)
     {
    	 if (d_capacity == d_size)
    		 addresize();
    	 d_data[d_size++] = x;
     } // Adds new value. If needed, allocates more space

     size_t size() const { return d_size; }; // Size getter
     Data const &operator[](size_t idx) const
     {
    	 if (idx < 0 || idx > d_capacity) return ' ';
    	 return d_data[idx];
     }; // Const getter

     int FindIndex(Data const &value)
     {
    	 for(size_t i=0;i<d_size;i++)
    		 if (d_data[i]==value) return i;
    	 return -1;
     }

     typedef bool (*FindProc)(Data d,void* par);

     int8_t FindIndex(FindProc p,void* par)
     {
    	 for(uint8_t i=0;i<d_capacity;i++)
    	 {
    		 if (p(d_data[i],par)) return i;
    	 }
    	 return -1;
     }

     Data &operator[](size_t idx)
     {
    	 //if (idx < 0 || idx > d_capacity) return NULL;
    	 return d_data[idx];
     }; // Changeable getter
   private:
     void addresize()
     {
    	 d_capacity++; //= d_capacity ? d_capacity*2 : 1;
    	 //Data *newdata = (Data *)malloc(d_capacity*sizeof(Data));
    	 Data *newdata = (Data *)new Data[d_capacity];
    	 if (d_data!=NULL)
    	 {
    		 memcpy(newdata, d_data, d_size * sizeof(Data));
    		 delete(d_data);
    	 }
    	 d_data = newdata;
     }// Allocates double the old space
};

#endif /* VECTOR_HPP_ */
