#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QMultiMap>

/**
  \brief Priority Queue. QQueue wrapper.
  */
template<class T>
    class PriorityQueue {
    private:
        QMultiMap<int, T> mNodes;
    public:
        /**
          Adds item to the queue due to its priority.
          \param item priority
          */
        void Add(T node, int priority);
        /**
          Remove item from queue with least priority.
          \return  item  with least priority.
          */
        T Pop();
        /**
          Adds item to the queue due to its priority. (the same as Add)
          \param item priority
          */
        void Update(T item, int priority);
        /**
          \return true - if queue is empty.
          */
        bool IsEmpty() const;
        /**
          \return count of items in queue.
          */
        int Count() const;
    };

template<class T>
    void PriorityQueue<T>::Add(T node, int priority)
    {
        mNodes.insertMulti(priority, node);
    }

template<class T>
    T PriorityQueue<T>::Pop()
    {
        typename QMap<int, T>::iterator iter = mNodes.begin();

        T item = iter.value();
        mNodes.erase(iter);

        return item;
    }

template<class T>
    void PriorityQueue<T>::Update(T item, int priority)
    {
        mNodes.insertMulti(priority, item);
    }

template<class T>
    int PriorityQueue<T>::Count() const
    {
        return mNodes.count();
    }

template<class T>
    bool PriorityQueue<T>::IsEmpty() const
    {
        return mNodes.empty();
    }

#endif // PRIORITYQUEUE_H

