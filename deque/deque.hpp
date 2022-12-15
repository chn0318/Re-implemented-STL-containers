#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include<iostream>
#include<climits>
namespace sjtu {

    template<class T>
    class deque {
    private:
        struct  node {
            T** data;
            node* next;
            int size;
            node(node* n = NULL, int thesize = 0) {
                size = thesize;
                data = new T*[301];
                next = n;
            }
            ~node() {
                for (int i = 0;i < size;i++) {
                    delete data[i];
                }
                delete []data;
                size = 0;

            }
        };
        node* head,*rear;
        int length;
        void find(int i, int& pos, node*& p) const {
            int count = 0;
            p = head;
            while (count <= i) {
                if (i == 0) { pos = 0;return; }
                if (count + p->size <= i && count+p->size < length ) {//可以用find直接找尾节点
                    count += p->size;
                    p = p->next;
                }
                else {
                    pos = i - count;
                    return;
                }
            }
        }
        void split(node* p, int pos) {            //只需要用于insert操作的一种情况,从pos开始断开
            p->next = new node(p->next,0);
            for (int i = pos;i < p->size;i++) {
                p->next->data[i - pos] = new T(*p->data[i]);
                delete p->data[i];
            }
            p->next->size = (p->size) - pos;
            p->size = pos;
        }
        void merge(node* p) {        //用于insert操作split以后判断能不能与下一个节点合并或者删除以后与下一个节点合并          
            node* nextp = p->next;
            if (nextp == NULL) { return; }
            if ((p->size) + (nextp->size) > 300) { return; }//不能合并
            else {
                for (int i = 0;i < nextp->size;i++) {
                    p->data[i + p->size] = new T(*nextp->data[i]);
                }
                if (nextp == rear) { rear = p; }//维护尾指针
                p->next = nextp->next;
                p->size += nextp->size;
                delete nextp;
            }
        }
        friend class iterator;
        friend class const_iterator;
    public:
        class const_iterator;
        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            deque<T>* point;
            node* node_point;
            int node_index;
            int index;
            friend class deque<T>;
            friend class const_iterator;
        public:
            iterator(deque<T>* p1 = NULL, node* p2 = NULL, int index1 = 0, int index2 = 0) :point(p1), node_point(p2), node_index(index1), index(index2) {}
            iterator(const iterator& tmp) { point = tmp.point;node_point = tmp.node_point;node_index = tmp.node_index;index = tmp.index; }
            iterator(const const_iterator& tmp) { point = tmp.point;node_point = tmp.node_point;node_index = tmp.node_index;index = tmp.index; }
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, the behaviour is **undefined**.
             * as well as operator-
             */
            iterator operator+(const int& n) const {
                if (index + n > point->length|| index + n > point->length < 0) {
                    throw index_out_of_bound();
                }
                if (node_index == 0 && n == 0) {    //处理deque为空时头节点加0
                    iterator tmp(point, node_point, 0, 0);
                    return tmp;
                }
                if (n >= 0) {
                    if (node_index + n < node_point->size) {
                        iterator tmp(point, node_point, node_index + n, index + n);   
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        iterator tmp(point, nodep, indexp, index + n);
                        return tmp;
                    }
                }
                else {
                    if (node_index + n >= 0 ) {
                        iterator tmp(point, node_point, node_index + n, index + n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        iterator tmp(point, nodep, indexp, index + n);
                        return tmp;
                    }
                }
            }
            iterator operator-(const int& n) const {
                if (index - n < 0 ||index-n > point->length) {
                    throw index_out_of_bound();
                }
                if (n >= 0) {
                    if (node_index - n >= 0) {
                        iterator tmp(point, node_point, node_index - n, index - n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        iterator tmp(point, nodep, indexp, index - n);
                        return tmp;
                    }
                }
                else {
                    if (node_index - n < node_point->size) {
                        iterator tmp(point, node_point, node_index - n, index - n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        iterator tmp(point, nodep, indexp, index - n);
                        return tmp;
                    }
                }
            }
            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator& rhs) const {
                if (rhs.point != point) { throw invalid_iterator(); }
        
                return index - rhs.index;
            }
            iterator& operator+=(const int& n) {
                if (index + n > point->length||index+n < 0) {
                    throw index_out_of_bound();
                }
                if (n >= 0) {
                    if (node_index + n < node_point->size) {
                        index += n;
                        node_index += n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index += n;
                        return *this;
                    }
                }
                else {
                    if (node_index + n >=0) {
                        index += n;
                        node_index += n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index += n;
                        return *this;
                    }
                }
            }
            iterator& operator-=(const int& n) {
                if (index - n < 0||index-n > point->length) {
                    throw index_out_of_bound();

                }
                if (n >=0) {
                    if (node_index - n > 0) {
                        index -= n;
                        node_index -= n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index -= n;
                        return *this;
                    }
                }
                else {
                    if (node_index - n < node_point->size) {
                        index -= n;
                        node_index -= n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index -= n;
                        return *this;
                    }
                }
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                if (index + 1 > point->length||index < 0 ) { throw index_out_of_bound(); }
                if (node_index + 1 < node_point->size) {
                    iterator tmp(*this);
                    node_index++;
                    index++;
                    return tmp;
                }
                else {
                    iterator tmp(*this);
                    node* nodep;
                    int indexp;
                    point->find(index + 1, indexp, nodep);
                    node_index = indexp;
                    node_point = nodep;
                    index++;
                    return tmp;
                }


            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                if (index + 1 > point->length||index < 0 ) { throw index_out_of_bound(); }
                if (node_index + 1 < node_point->size) {
                    node_index++;
                    index++;
                    return *this;
                }
                else {
                    node* nodep;
                    int indexp;
                    point->find(index+1, indexp, nodep);
                    node_index = indexp;
                    node_point = nodep;
                    index++;
                    return *this;
                }

            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                if (index <= 0|| index > point->length) { throw index_out_of_bound(); }
                if (node_index != 0) {
                    iterator tmp(*this);
                    index--;
                    node_index--;
                    return tmp;
                }
                else {
                    iterator tmp(*this);
                    point->find(index - 1, node_index, node_point);
                    index--;
                    return tmp;
                }
            }

            /**
             * TODO --iter
             */
            iterator& operator--() {
                if (index <= 0||index > point->length) { throw index_out_of_bound(); }
                if (node_index != 0) {
                    index--;
                    node_index--;
                    return *this;
                }
                else {
                    point->find(index - 1, node_index, node_point);
                    index--;
                    return *this;
                }
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                if (point == NULL || node_point == NULL) {
                    throw  invalid_iterator();
                }
                if (node_index < 0 || node_index >= node_point->size || index < 0 || index >= point->length) {
                    throw index_out_of_bound();
                }
                return *(node_point->data[node_index]);
            }
            /**
             * TODO it->field
             */
            T* operator->() const noexcept {
                if (point == NULL || node_point == NULL) {
                    throw  invalid_iterator();
                }
                if (node_index < 0 || node_index >= node_point->size || index < 0 || index >= point->length) {
                    throw index_out_of_bound();
                }
                return ((node_point->data)[node_index]);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator& rhs) const {
                
                return (point == rhs.point && index == rhs.index);
            }
            bool operator==(const const_iterator& rhs) const {
             
                return (point == rhs.point && index == rhs.index);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return (point != rhs.point || index != rhs.index);
            }
            bool operator!=(const const_iterator& rhs) const {
                return (point != rhs.point || index != rhs.index);

            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            const deque<T>* point;
            const node* node_point;
            int node_index;
            int index;
            friend class deque<T>;
            friend class iterator;
        public:
            const_iterator(const deque<T>* p1 = NULL, const node* p2 = NULL, int index1 = 0, int index2 = 0) :point(p1), node_point(p2), node_index(index1), index(index2) {}
            const_iterator(const const_iterator& other) {
                point = other.point;node_point = other.node_point;
                node_index = other.node_index;index = other.index;
            }
            const_iterator(const iterator& other) {
                point = other.point;node_point = other.node_point;
                node_index = other.node_index;index = other.index;
            }
            const_iterator operator+(const int& n) const {
                if (index + n > point->length || index + n > point->length < 0) {
                    throw index_out_of_bound();
                }
                if (node_index == 0 && n == 0) {    //处理deque为空时头节点加0
                    const_iterator tmp(point, node_point, 0, 0);
                    return tmp;
                }
                if (n >= 0) {
                    if (node_index + n < node_point->size) {
                        const_iterator tmp(point, node_point, node_index + n, index + n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        const_iterator tmp(point, nodep, indexp, index + n);
                        return tmp;
                    }
                }
                else {
                    if (node_index + n >= 0) {
                        const_iterator tmp(point, node_point, node_index + n, index + n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        const_iterator tmp(point, nodep, indexp, index + n);
                        return tmp;
                    }
                }
            }
            const_iterator operator-(const int& n) const {
                if (index - n < 0 || index - n > point->length) {
                    throw index_out_of_bound();
                }
                if (n >= 0) {
                    if (node_index - n >= 0) {
                        const_iterator tmp(point, node_point, node_index - n, index - n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        const_iterator tmp(point, nodep, indexp, index - n);
                        return tmp;
                    }
                }
                else {
                    if (node_index - n < node_point->size) {
                        const_iterator tmp(point, node_point, node_index - n, index - n);
                        return tmp;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        const_iterator tmp(point, nodep, indexp, index - n);
                        return tmp;
                    }
                }
            }
            int operator-(const const_iterator& rhs) const {
                if (rhs.point != point) { throw invalid_iterator(); }
             
                return index - rhs.index;
            }
            const_iterator& operator+=(const int& n) {
                if (index + n > point->length || index + n < 0) {
                    throw index_out_of_bound();
                }
                if (n >= 0) {
                    if (node_index + n < node_point->size) {
                        index += n;
                        node_index += n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index += n;
                        return *this;
                    }
                }
                else {
                    if (node_index + n >= 0) {
                        index += n;
                        node_index += n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index + n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index += n;
                        return *this;
                    }
                }
            }
            const_iterator& operator-=(const int& n) {
                if (index - n < 0 || index - n > point->length) {
                    throw index_out_of_bound();

                }
                if (n >= 0) {
                    if (node_index - n > 0) {
                        index -= n;
                        node_index -= n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index -= n;
                        return *this;
                    }
                }
                else {
                    if (node_index - n < node_point->size) {
                        index -= n;
                        node_index -= n;
                        return *this;
                    }
                    else {
                        node* nodep;
                        int indexp;
                        point->find(index - n, indexp, nodep);
                        node_index = indexp;
                        node_point = nodep;
                        index -= n;
                        return *this;
                    }
                }
            }
            const_iterator operator++(int) {
                if (index + 1 > point->length||index < 0 ) { throw index_out_of_bound(); }
                if (node_index + 1 < node_point->size) {
                    const_iterator tmp(*this);
                    node_index++;
                    index++;
                    return tmp;
                }
                else {
                    const_iterator tmp(*this);
                    index++;
                    node_point = node_point->next;
                    node_index = 0;
                    return tmp;
                }


            }
            const_iterator& operator++() {
                if (index + 1 > point->length||index < 0) { throw index_out_of_bound(); }
                if (node_index + 1 < node_point->size) {
                    node_index++;
                    index++;
                    return *this;
                }
                else {
                    index++;
                    node_index = 0;
                    node_point = node_point->next;
                    return *this;
                }

            }
            const_iterator operator--(int) {
                if (index > point->length ||index <= 0) { throw index_out_of_bound(); }
                if (node_index != 0) {
                    const_iterator tmp(*this);
                    index--;
                    node_index--;
                    return tmp;
                }
                else {
                    node* nodep;
                    int indexp;
                    const_iterator tmp(*this);
                    point->find(index - 1, indexp, nodep);
                    node_index = indexp;
                    node_point = nodep;
                    index--;
                    return tmp;
                }
            }
            const_iterator& operator--() {
                if (index > point->length||index <= 0) { throw index_out_of_bound(); }
                if (node_index != 0) {
                    index--;
                    node_index--;
                    return *this;
                }
                else {
                    node* nodep;
                    int indexp;
                    point->find(index - 1, indexp, nodep);
                    index--;
                    node_index = indexp;
                    node_point = nodep;
                    return *this;
                }
            }
            T& operator*() const {
                if (point == NULL || node_point == NULL) {
                    throw  invalid_iterator();
                }
                if (node_index < 0 || node_index >= node_point->size || index < 0 || index >= point->length) {
                    throw index_out_of_bound();
                }
                return *(node_point->data[node_index]);
            }
            T* operator->() const noexcept {
                if (point == NULL || node_point == NULL) {
                    throw  invalid_iterator();
                }
                if (node_index < 0 || node_index >= node_point->size || index < 0 || index >= point->length) {
                    throw index_out_of_bound();
                }
                return (node_point->data)[node_index];
            }
            bool operator==(const iterator& rhs) const {
                return (point == rhs.point && index == rhs.index);
            }
            bool operator==(const const_iterator& rhs) const {
                return (point == rhs.point && index == rhs.index);
            }
            bool operator!=(const iterator& rhs) const {
                return (point != rhs.point || index != rhs.index);
            }
            bool operator!=(const const_iterator& rhs) const {
                return (point != rhs.point || index != rhs.index);
            }
        };
        /**
         * TODO Constructors
         */
        deque() {
            rear=head = new node(NULL,0);
            length = 0;
        }
        deque(const deque& other) {
            node* otherp = other.head, * p ;
            length = other.length;
            p= head = new node(NULL, otherp->size);
            for (int i = 0;i < otherp->size;i++) {
                p->data[i] = new T(* otherp->data[i]);
            }
            otherp = otherp->next;
            while (otherp != NULL) {
                p = p->next = new node(NULL, otherp->size);
                for (int i = 0;i < otherp->size;i++) {
                    p->data[i] = new T(*otherp->data[i]);
                }
                otherp = otherp->next;
            }
            rear = p;


        }
        /**
         * TODO Deconstructor
         */
        ~deque() {
            clear();
            delete head;
            rear = head = NULL;
        }
        /**
         * TODO assignment operator
         */
        deque& operator=(const deque& other) {
            if (this == &other) { return *this; }
            clear();
            delete head;
            head = rear = NULL;
            node* otherp = other.head, * p;
            length = other.length;
            p = head = new node(NULL, otherp->size);
            for (int i = 0;i < otherp->size;i++) {
                p->data[i] = new T(*otherp->data[i]);
            }
            otherp = otherp->next;
            while (otherp != NULL) {
                p = p->next = new node(NULL, otherp->size);
                for (int i = 0;i < otherp->size;i++) {
                    p->data[i] = new T(*otherp->data[i]);
                }
                otherp = otherp->next;
            }
            rear = p;

            return *this;

        }
        /**
         * access specified element with bounds checking
         * throw index_out_of_bound if out of bound.
         */
        T& at(const size_t& pos) {
            if (pos >= length || pos < 0) {
                throw index_out_of_bound();
            }
            node* tmp;
            int local;
            find(pos, local, tmp);
            return *(tmp->data[local]);
        }
        const T& at(const size_t& pos) const {
            if (pos >= length || pos < 0) {
                throw index_out_of_bound();
            }
            node* tmp;
            int local;
            find(pos, local, tmp);
            return *(tmp->data[local]);

        }
        T& operator[](const size_t& pos) {
            if (pos >= length || pos < 0) {
                throw index_out_of_bound();
            }
            node* tmp;
            int local;
            find(pos, local, tmp);
            return *(tmp->data[local]);

        }
        const T& operator[](const size_t& pos) const {
            if (pos >= length || pos < 0) {
                throw index_out_of_bound();
            }
            node* tmp;
            int local;
            find(pos, local, tmp);
            return *(tmp->data[local]);

        }
        /**
         * access the first element
         * throw container_is_empty when the container is empty.
         */
        const T& front() const {
            if (length == 0) { throw container_is_empty(); }
            return *(head->data[0]);
        }
        /**
         * access the last element
         * throw container_is_empty when the container is empty.
         */
        const T& back() const {
            if (length == 0) { throw container_is_empty(); }
            node* tmp=rear;
            int local = rear->size - 1;
            return *(tmp->data[local]);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            iterator tmp(this, head, 0, 0);
            return tmp;
        }
        const_iterator cbegin() const {
            const_iterator tmp(this, head, 0, 0);
            return tmp;
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            if (length == 0) { iterator tmp(this, head, 0, 0);return tmp; }
            node* nodep;
            int indexp;
            find(length, indexp, nodep);
            iterator tmp(this, nodep, indexp, length);
            return tmp;

        }
        const_iterator cend() const {
            if (length == 0) { const_iterator tmp(this, head, 0, 0);return tmp; }
            node* nodep;
            int indexp;
            find(length, indexp, nodep);
            const_iterator tmp(this, nodep, indexp, length);
            return tmp;
        }
        /**
         * checks whether the container is empty.
         */
        bool empty() const {
            return length == 0;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return length;
        }
        /**
         * clears the contents
         */
        void clear() {
            node* tmp = head->next;
            node* p;
            rear = head;
            head->next = NULL;
            while (tmp != NULL) {
                p = tmp->next;
                delete tmp;
                tmp = p;
            }
            for (int i = 0;i < head->size;i++) {
                delete head->data[i];
            }
            head->size = 0;
            length = 0;
        }
        /**
         * inserts elements at the specified locat on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator pos, const T& value) {
            if (pos.point != this) {
                throw invalid_iterator();
            }

            if (pos.node_point == NULL ) {
                throw index_out_of_bound();
            }
            if (pos.node_point->size < 300) {
                if ((pos.node_point)->size == 0) { (pos.node_point)->data[(pos.node_point)->size] = new T(value); }
                else {
                    (pos.node_point)->data[(pos.node_point)->size] = new T(*(pos.node_point)->data[(pos.node_point)->size - 1]);
                }             
                for (int i = (pos.node_point)->size;i > pos.node_index;i--) {
                    *((pos.node_point)->data)[i] = *((pos.node_point)->data)[i - 1];
                }
                *((pos.node_point)->data)[pos.node_index] = value;
                (pos.node_point)->size++;
                length++;
                iterator tmp(this, pos.node_point, pos.node_index, pos.index);
                return tmp;
            }
            else {
                if (pos.node_index < 300) {
                    split(pos.node_point, pos.node_index);
                    if (rear == pos.node_point) { rear = pos.node_point->next; }//维护尾指针
                    ((pos.node_point)->data)[pos.node_index] = new T(value);
                    pos.node_point->size++;
                    length++;
                    merge(pos.node_point->next);
                    iterator tmp(this, pos.node_point, pos.node_index, pos.index);
                    return tmp;
                }
                else {          //最后一个节点满了，向最后插入
                    node* nodep1;
                    rear=nodep1=pos.node_point->next = new node(NULL, 1);//维护尾指针
                    (nodep1->data[0]) = new T( value);
                    length++;
                    iterator tmp(this, nodep1, 0, length - 1);
                    return tmp;
                }
            }

        }

        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos) {
            if (pos.point != this) {
                throw invalid_iterator();
            }
            if (pos.index >= length || pos.node_index >= (pos.node_point)->size) {
                throw index_out_of_bound();
            }
            if (length == 0) {
                throw container_is_empty();
            }
            for (int i = pos.node_index;i < pos.node_point->size - 1;i++) {
                *(pos.node_point->data[i]) =*( pos.node_point->data[i + 1]);
            }
            delete pos.node_point->data[pos.node_point->size - 1];
            length--;
            pos.node_point->size--;
            merge(pos.node_point);
            if (pos.node_index >= pos.node_point->size) {     //删除的元素是node的最后一个并且没有merge上来
                if (pos.index >= length) { //删除的是尾元素
                    if (length != 0 && pos.node_point->size == 0) { 
                        int indexp2;
                        node* nodep2;
                        find(length - 1, indexp2, nodep2);
                        rear = nodep2;
                        nodep2->next = NULL;                       
                        delete pos.node_point; 
                    }  //删了尾元素后尾节点空了
                    return end();    //返回尾迭代器
                }
                else {
                    iterator tmp(this, pos.node_point->next, 0, pos.index);
                    return tmp;

                }
            }
            else {
                iterator tmp(pos);
                return tmp;
            }
        }



        /**
         * adds an element to the end
         */
        void push_back(const T& value) {
            node* nodep=rear;
            int indexp=(rear->size)-1;
            if (length == 0) { head->data[0] = new T(value);head->size++;length++;return; }
            if (indexp < 299) {
                indexp++;
                nodep->data[indexp] = new T(value);
                length++;
                nodep->size++;
            }
            else {
                rear=nodep->next = new node(NULL, 1);
                nodep->next->data[0] =new T (value);
                length++;
            }
        }
        /**
         * removes the last element
         *     throw when the container is empty.
         */
        void pop_back() {
            if (length == 0) {
                throw container_is_empty();
            }
            node* nodep=rear,*nodep2;
            int indexp=(rear->size)-1;
            delete nodep->data[indexp];
            nodep->size--;
            length--;
            if (length != 0 && nodep->size == 0) {
                find(length - 1, indexp, nodep2);
                rear = nodep2;
                nodep2->next = NULL;
                delete nodep;
            }

        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T& value) {
            if (head->size < 300) {
                if (head->size == 0) { 
                    head->data[head->size] = new T(value);  head->size++;
                    length++;return;
                }
                head->data[head->size] = new T(*head->data[head->size - 1]);
                for (int i = head->size;i > 0;i--) {
                    *(head->data[i]) = *(head->data[i - 1]);
                }
                *(head->data[0]) = value;
                head->size++;
                length++;
            }
            else {
                head = new node(head, 1);
                (head->data[0]) = new T(value);
                length++;
            }


        }
        /**
         * removes the first element.
         *     throw when the container is empty.
         */
        void pop_front() {
            if (length == 0) {
                throw container_is_empty();
            }
            for (int i = 0;i < head->size - 1;i++) {
                *(head->data[i]) = *(head->data[i + 1]);
            }
            delete head->data[head->size - 1];
            head->size--;
            merge(head);
            length--;

        }
    };

}

#endif