#ifndef SJTU_VECTOR_HPP

#define SJTU_VECTOR_HPP



#include "exceptions.hpp"



#include <climits>

#include <cstddef>



namespace sjtu {


    template<typename T>

    class vector {
    private:
        unsigned int thesize;
        unsigned int theCapacity;
        T* objects;
        void doublespace() {
            int i;
            theCapacity *= 2;
            T* tmp = objects;
            objects = (T*)malloc(sizeof(T) * theCapacity);
            for (i = 0; i < thesize; i++) {
                objects[i] = tmp[i];
            }
            for (i = 0; i < theCapacity / 2; i++) {
                tmp[i].~T();
            }
            free(tmp);
        }
    public:

        

        class const_iterator;

        class iterator {
        public:
            vector<T>* tmp;
            int index;
            
            iterator(vector<T>* a=NULL,int num=0) { tmp = a; index = num; }
            iterator operator+(const int& n) const {
                if (index + n >= tmp->thesize) { throw index_out_of_bound(); }
                iterator a;
                a.tmp = tmp;
                a.index = index + n;
                return a;
            }

            iterator operator-(const int& n) const {
                if (index - n < 0) { throw index_out_of_bound(); }
                iterator a;
                a.tmp = tmp;
                a.index = index - n;
                return a;

            }

        
            int operator-(const iterator& rhs) const {
                if (rhs.tmp != tmp) {
                    throw invalid_iterator();
                }
                if (rhs.index > index) { return rhs.index - index; }
                else { return index - rhs.index; }

            }

            iterator& operator+=(const int& n) {
                if (index + n >= tmp->thesize) { throw index_out_of_bound(); }
                index += n;
                return *this;

            }

            iterator& operator-=(const int& n) {
                if (index - n < 0) { throw index_out_of_bound(); }
                index -= n;
                return *this;


            }

           

            iterator operator++(int) {
                iterator a;
                a = *this;
                index++;
                return a;
            }

         

            iterator& operator++() {
                index++;
                return *this;

            }

           

            iterator operator--(int) {
                iterator a;
                a = *this;
                index--;
                return a;
            }

           

            iterator& operator--() {
                index--;
                return *this;
            }

           
            T& operator*() const {
                return tmp->objects[index];

            }

            bool operator==(const iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return true; }
                else { return false; }
            }

            bool operator==(const const_iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return true; }
                else { return false; }
            }

            

            bool operator!=(const iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return false; }
                else { return true; }
            }

            bool operator!=(const const_iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return false; }
                else { return true; }
            }

        };

     

        class const_iterator {
      
           
        public: 
            const vector* tmp;
            int index;
            const_iterator(const vector* a = NULL, int num = 0) { tmp = a; index = num; }

            const_iterator operator+(const int& n) const {
                if (index + n >= tmp->thesize) { throw index_out_of_bound(); }
                const_iterator a;
                a.tmp = tmp;
                a.index = index + n;
                return a;
            }

            const_iterator operator-(const int& n) const {
                if (index - n < 0) { throw index_out_of_bound(); }
                const_iterator a;
                a.tmp = tmp;
                a.index = index - n;
                return a;

            }

           

            int operator-(const  const_iterator& rhs) const {
                if (rhs.tmp != tmp) {
                    throw invalid_iterator();
                }
                if (rhs.index > index) { return rhs.index - index; }
                else { return index - rhs.index; }

            }

            const_iterator& operator+=(const int& n) {
                if (index + n >= tmp->thesize) { throw index_out_of_bound(); }
                index += n;
                return *this;

            }

            const_iterator& operator-=(const int& n) {
                if (index - n < 0) { throw index_out_of_bound(); }
                index -= n;
                return *this;


            }

         

            const_iterator operator++(int) {
                const_iterator a;
                a = *this;
                index++;
                return a;
            }

           

            const_iterator& operator++() {
                index++;
                return *this;

            }

           

            const_iterator operator--(int) {
                const_iterator a;
                a = *this;
                index--;
                return a;
            }


            const_iterator& operator--() {
                index--;
                return *this;
            }


            const T& operator*() const{
                return tmp->objects[index];
            }

           
            bool operator==(const iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return true; }
                else { return false; }
            }

            bool operator==(const const_iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return true; }
                else { return false; }
            }


            bool operator!=(const iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return false; }
                else { return true; }
            }

            bool operator!=(const const_iterator& rhs) const {
                if (tmp == rhs.tmp && index == rhs.index) { return false; }
                else { return true; }
            }


        };


    public:

   
        vector() {
            objects = (T*)malloc(sizeof(T) * 10);
            memset(objects, 0, sizeof(T) * 10);
            theCapacity = 10;
            thesize = 0;
        }

        vector(const vector& other) {
            thesize = other.thesize;
            theCapacity = other.theCapacity;
            objects = (T*)malloc(sizeof(T) * theCapacity);
            for (int i = 0; i < thesize; i++) {
                objects[i] = other.objects[i];
            }

        }


        ~vector() {
            for (int i = 0; i < theCapacity; i++) { objects[i].~T(); }
            free(objects);
        }

      
        vector& operator=(const vector& other) {
            if (this== &other) { return *this; }
            for (int i = 0; i < theCapacity; i++) {
                objects[i].~T();
            }
            free(objects);
            theCapacity = other.theCapacity;
            objects = (T*)malloc(sizeof(T) * theCapacity);
            memset(objects, 0, sizeof(T) * theCapacity);
            for (int i = 0; i < other.thesize; i++) {
                objects[i] = other.objects[i];
            }
            thesize = other.thesize;
            return *this;

        }

       

        T& at(const size_t& pos) {
            if (pos >= thesize||pos < 0) {
                throw index_out_of_bound();
            }
            else { return objects[pos]; }
        }

        const T& at(const size_t& pos) const {
            if (pos >= thesize||pos < 0) {
                throw index_out_of_bound();
            }
            else { return objects[pos]; }
        }

      

        T& operator[](const size_t& pos) {
            if (pos >= thesize||pos < 0) {
                throw index_out_of_bound();
            }
            else { return objects[pos]; }
        }

        const T& operator[](const size_t& pos) const {
            if (pos >= thesize||pos < 0) {
                throw index_out_of_bound();
            }
            else { return objects[pos]; }
        }

      

        const T& front() const {
            if (thesize == 0) {
                throw container_is_empty();
            }
            else {
                return objects[0];
            }
        }


        const T& back() const {
            if (thesize == 0) { throw container_is_empty(); }
            else {
                return objects[thesize - 1];
            }
        }


        iterator begin() {
            iterator a(this,0);
          
            return a;
        }

        const_iterator cbegin() const {
            const_iterator a(this,0);
           
            return a;
        }

       

        iterator end() {
            iterator a(this,thesize);
          
            return a;
        }

        const_iterator cend() const {
            const_iterator a(this,thesize);
           
            return a;
        }


        bool empty() const {
            if (thesize == 0) { return true; }
            else { return false; }
        }

    

        size_t size() const {
            return thesize;
        }

  

        void clear() {
            memset(objects, 0, sizeof(T) * theCapacity);
            thesize = 0;
        }

       

        iterator insert(iterator pos, const T& value) {
            if (thesize == theCapacity) {
                doublespace();
            }
            for (int i = thesize; i >pos.index ; i--) {
                objects[i] = objects[i - 1];
            }
            objects[pos.index] = value;
            thesize++;
            iterator a(this, pos.index);
             return a;
        }

     

        iterator insert(const size_t& ind, const T& value) {
            if (ind > thesize) {
                throw index_out_of_bound();
            }
            else {
                if (thesize == theCapacity) { doublespace(); }
                for (int i = thesize; i >ind; i --) {
                    objects[i] = objects[i - 1];
                }
                objects[ind] = value;
                thesize++;
                iterator a(this, ind);
                return a;
            }
        }

      

        iterator erase(iterator pos) {
            int i;
            for (i=pos.index; i < thesize - 1; i++) {
                objects[i] = objects[i + 1];
            }
            iterator a(this,pos.index);
            thesize--;
            return a;
        }

      

        iterator erase(const size_t& ind) {
            if (ind >= thesize) {
                throw index_out_of_bound();
            }
          
                for (int i = ind; i < thesize - 1; i++) {
                    objects[i] = objects[i + 1];
                }
                iterator a(this,ind);
                thesize--;
                return a;
            
        }


        void push_back(const T& value) {
            if (thesize < theCapacity) {
                objects[thesize] = value;
                thesize++;
            }
            else {
                doublespace();
                objects[thesize] = value;
                thesize++;
            }
        }


        void pop_back() {
            if (thesize == 0) {
                throw container_is_empty();
            }
            
                thesize--;
            
        }

    };





}



#endif