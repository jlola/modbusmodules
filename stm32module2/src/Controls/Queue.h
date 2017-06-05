#ifndef __Queue_H__
#define __Queue_H__


template <class Elem>
class Queue
{
  public:
    Queue(int MaxSize=500);
    Queue(const Queue<Elem> &OtherQueue);
    ~Queue(void);

    void       Enqueue(const Elem &Item);    // Adds Item to Queue end
    Elem       Dequeue(void);                // Returns Item from Queue
    inline int ElemNum(void);                // Returns Number of Elements

  protected:
    const int MAX_NUM = 500;   // The actual spaces will be one more than this
    Elem     Data[500 + 1];      // The actual Data array
    int       Beginning, // Numbered location of the start and end
              End;

    // Instead of calculating the number of elements, using this variable
    // is much more convenient.
    int       ElemCount;
};

// Queue Constructor function
template <class Elem>
Queue<Elem>::Queue(int MaxSize) :
    MAX_NUM( MaxSize )   // Initialize the constant
{
  // This extra space added will allow us to distinguish between
  // the Beginning and the End locations.
  //Data      = new Elem[MAX_NUM + 1];
  Beginning = 0;
  End       = 0;
  ElemCount = 0;
}

// Queue Copy Constructor function
template <class Elem>
Queue<Elem>::Queue(const Queue &OtherQueue) :
                MAX_NUM( OtherQueue.MAX_NUM )  // Initialize the constant
{
  Beginning = OtherQueue.Beginning;
  End       = OtherQueue.End;
  ElemCount = OtherQueue.ElemCount;

  //Data      = new Elem[MAX_NUM + 1];
  for (int i = 0; i < MAX_NUM; i++)
    Data[i] = OtherQueue.Data[i];
}

// Queue Destructor function
template <class Elem>
Queue<Elem>::~Queue(void)
{
  delete[] Data;
}

// Enqueue() function
template <class Elem>
void Queue<Elem>::Enqueue(const Elem &Item)
{
  // Error Check: Make sure we aren't exceeding our maximum storage space
  //assert( ElemCount < MAX_NUM );
	if (ElemCount >= MAX_NUM) return;

  Data[ End++ ] = Item;
  ++ElemCount;

  // Check for wrap-around
  if (End > MAX_NUM)
    End -= (MAX_NUM + 1);
}

// Dequeue() function
template <class Elem>
Elem Queue<Elem>::Dequeue(void)
{
  // Error Check: Make sure we aren't dequeueing from an empty queue
  //assert( ElemCount > 0 );

  Elem ReturnValue = Data[ Beginning++ ];
  --ElemCount;

  // Check for wrap-around
  if (Beginning > MAX_NUM)
    Beginning -= (MAX_NUM + 1);

  return ReturnValue;
}

// ElemNum() function
template <class Elem>
inline int Queue<Elem>::ElemNum(void)
{
  return ElemCount;
}



#endif
