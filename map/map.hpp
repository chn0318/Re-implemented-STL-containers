/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

 // only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    enum colourT { RED, BLACK };
    template<
        class Key,
        class T,
        class Compare = std::less<Key>
    > class map {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        struct redblacknode {
            value_type* data;
            redblacknode* left, * right;
            redblacknode* parent;
            colourT colour;
            redblacknode(const value_type* element, redblacknode* p = NULL, redblacknode* lt = NULL, redblacknode* rt = NULL, colourT h = RED) :parent(p), left(lt), right(rt), colour(h) { data = new value_type(*element); }
            ~redblacknode() { delete data; }
        };
        redblacknode* root;
        int length;
        void LL(redblacknode*& t) {//保持迭代器不失效的LL
            colourT tmp1 = t->colour;
            redblacknode* t1 = t->left;
            colourT tmp2 = t1->colour;
            t->left = t1->right;
            if (t1->right) { t1->right->parent = t; }
            t1->parent = t->parent;
            if (t->parent) {
                if (t == t->parent->right) { t->parent->right = t1; }
                else { t->parent->left = t1; }
            }
            t1->right = t;
            t->parent = t1;
            t1->colour = tmp1;
            t->colour = tmp2;
            t = t1;
        }
        void LR(redblacknode*& gp) {
            redblacknode* tmp = gp->left;
            RR(tmp);
            LL(gp);

        }
        void RR(redblacknode*& t) {
            colourT tmp1 = t->colour;
            redblacknode* t1 = t->right;
            colourT tmp2 = t1->colour;
            t->right = t1->left;
            if (t1->left) { t1->left->parent = t; }
            t1->parent = t->parent;
            if (t->parent) {
                if (t == t->parent->right) { t->parent->right = t1; }
                else { t->parent->left = t1; }
            }
            t1->left = t;
            t->parent = t1;
            t1->colour = tmp1;
            t->colour = tmp2;
            t = t1;
        }
        void RL(redblacknode*& gp) {
            redblacknode* tmp = gp->right;
            LL(tmp);
            RR(gp);
        }
        void clear(redblacknode*& t) {
            if (t == NULL) { return; }
            clear(t->left);
            clear(t->right);
            delete t;
            t = NULL;
        }
        void insertadjust(redblacknode*& gp, redblacknode*& p, redblacknode*& t) { //insertadjust的目的就是解决：gp的儿子p和孙子t出现了连续红节点的情况
            if (p->colour) return;    //如果父节点p是黑色，不用调整
            if (p == root) {
                p->colour = BLACK;
                return;
            }
            if (gp->left == p) {
                if (p->left == t) {
                    if (root == gp) { LL(root); }
                    else { LL(gp); }
                }
                else {
                    if (root == gp) { LR(root);p = gp = root; }
                    else { LR(gp); p = t->parent; }

                }
            }
            else {
                if (p->right == t) {
                    if (root == gp) { RR(root); }
                    else { RR(gp); }
                }
                else {
                    if (root == gp) {
                        RL(root); p = gp = root;
                    }
                    else {
                        RL(gp);  p = t->parent;
                    }
                }
            }
        }
        redblacknode* create(redblacknode* other) {
            if (other == NULL) return NULL;
            redblacknode* t = new redblacknode(other->data, NULL, NULL, NULL, other->colour);
            if (other->left != NULL) { t->left = create(other->left); t->left->parent = t; }
            if (other->right != NULL) { t->right = create(other->right); t->right->parent = t; }
            return t;
        }
        void removeadjust(redblacknode*& p, redblacknode*& c, redblacknode*& t, Key del) {  //调整当前节点为红色
            Compare cmp;
            if (c->colour == RED) { return; }
            if (c == root) {
                if (c->left && c->right && c->right->colour == c->left->colour) {
                    c->colour = RED;
                    c->left->colour = c->right->colour = BLACK;
                    return;
                }
            }
            if (((c->left && c->left->colour) || c->left == NULL) && ((c->right && c->right->colour) || c->right == NULL)) {//c有两个黑色儿子         
                if (((t->left && t->left->colour) || t->left == NULL) && ((t->right && t->right->colour) || t->right == NULL)) {     //t也有两个黑色儿子
                    p->colour = BLACK;
                    t->colour = c->colour = RED;
                }
                else {                                                                                  //t有红儿子
                    if (p->left == t) {                                                                 //t是p的左儿子
                        if (t->left && t->left->colour == RED) {                                        //t的左儿子是红的或者t的儿子都红
                            t->left->colour = BLACK;
                            if (p == root) { LL(root);p = root->right; }
                            else {
                                LL(p);
                                p = p->right;
                            }
                        }
                        else {                                                                          //t的左儿子黑但右儿子红
                            if (p == root) { LR(root);root->left->colour = BLACK;p = root->right; }
                            else {
                                LR(p);
                                p->left->colour = BLACK;
                                p = p->right;
                            }
                        }

                    }
                    else {                                                                             //t是p的右儿子
                        if (t->right && t->right->colour == RED) {                                      //t的右儿子是红的或者t的儿子都红
                            t->right->colour = BLACK;
                            if (p == root) { RR(root);p = root->left; }
                            else {
                                RR(p);
                                p = p->left;
                            }
                        }
                        else {                                                               //t的左儿子是红的且右儿子黑
                            if (p == root) { RL(root);root->right->colour = BLACK;p = root->left; }
                            else {
                                RL(p);
                                p->right->colour = BLACK;
                                p = p->left;
                            }

                        }
                    }

                }
                c->colour = RED;
            }
            else {                                                                                      //c有红儿子
                if (!cmp(c->data->first, del) && !cmp(del, c->data->first)) {                             //c是被删节点
                    if (c->left && c->right) {                                                          //c有两个儿子
                        if (c->right->colour == BLACK) {  //c的右孩子是黑色
                            if (c == root) { LL(root);c = root->right; }
                            else {
                                LL(c);
                                c = c->right;
                            }
                        }
                        return;
                    }
                    if (c->left) {     //c有只有一个儿子，是红色的左儿子
                        if (c == root) { LL(root);p = root;c = root->right; }
                        else {
                            LL(c);
                            p = c;
                            c = c->right;
                        }
                    }
                    else {   //c有只有一个儿子，是红色的右儿子
                        if (c == root) { RR(root);p = root;c = root->left; }
                        else {
                            RR(c);
                            p = c;
                            c = c->left;
                        }
                    }
                }
                else {                                                                                  //c不是被删节点
                    p = c;
                    if (cmp(del, p->data->first)) {
                        c = p->left;
                        t = p->right;
                    }
                    else {
                        c = p->right;
                        t = p->left;
                    }
                    if (c->colour == BLACK) {                                                           //运气不好，走到了黑节点
                        if (t == p->right) {
                            if (p == root) { RR(root);p = root->left; }
                            else {
                                RR(p);
                                p = p->left;
                            }
                        }
                        else {
                            if (p == root) { LL(root);p = root->right; }
                            else {
                                LL(p);
                                p = p->right;
                            }
                        }

                        t = ((c == p->left) ? p->right : p->left);
                        removeadjust(p, c, t, del);
                    }
                }
            }
        }
        class const_iterator;
        class iterator {
            friend class map;
        public:
            redblacknode* current;
            map* address;
        public:
            iterator(redblacknode* c = NULL, map* a = NULL) {
                current = c;
                address = a;
            }
            iterator(const iterator& other) {
                current = other.current;
                address = other.address;
            }
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
             //重载x++;
            iterator operator++(int) {
                if (current == NULL) throw invalid_iterator();
                iterator tmp(current, address);
                if (current->right != NULL) {  //如果该节点有右儿子，找右儿子的左儿子
                    redblacknode* p1 = current->right;
                    while (p1->left != NULL) {
                        p1 = p1->left;
                    }
                    current = p1;
                    return tmp;
                }
                else {                              //tmp没有右儿子->找到一个节点，该节点的左子树包含了tmp。
                    redblacknode* tmq = current->parent;
                    redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->right)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;           //如果tmp指向的是最后一个元素，则return NULL
                    return tmp;
                }

            }
            //重载++x
            iterator& operator++() {
                if (current == NULL) throw invalid_iterator();
                if (current->right != NULL) {  //如果该节点有右儿子，找右儿子的左儿子
                    redblacknode* p1 = current->right;
                    while (p1->left != NULL) {
                        p1 = p1->left;
                    }
                    current = p1;
                    return *this;
                }
                else {                              //tmp没有右儿子->找到一个节点，该节点的左子树包含了tmp。
                    redblacknode* tmq = current->parent;
                    redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->right)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;           //如果tmp指向的是最后一个元素，则return NULL
                    return *this;
                }

            }
            iterator operator--(int) {
                iterator tmp(current, address);
                if (address != NULL && current == NULL) {
                    current = address->root;
                    if (current == NULL) throw invalid_iterator();
                    while (current->right != NULL) {
                        current = current->right;
                    }
                    return tmp;
                }
                if (current == NULL) throw invalid_iterator();
                if (current->left != NULL) {  //如果该节点有左儿子，找左儿子的右儿子
                    redblacknode* p1 = current->left;
                    while (p1->right != NULL) {
                        p1 = p1->right;
                    }
                    current = p1;
                    return tmp;
                }
                else {                              //tmp没有左儿子->找到一个节点，该节点的右子树包含了tmp。
                    redblacknode* tmq = current->parent;
                    redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->left)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;           //如果tmp指向的是最后一个元素，则return NULL
                    return tmp;
                }

            }
            iterator& operator--() {
                if (address != NULL && current == NULL) {
                    current = address->root;
                    if (current == NULL) throw invalid_iterator();
                    while (current->right != NULL) {
                        current = current->right;
                    }
                    return *this;
                }
                if (current == NULL) throw invalid_iterator();
                if (current->left != NULL) {  //如果该节点有左儿子，找左儿子的右儿子
                    redblacknode* p1 = current->left;
                    while (p1->right != NULL) {
                        p1 = p1->right;
                    }
                    current = p1;
                    return *this;
                }
                else {
                    redblacknode* tmq = current->parent;
                    redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->left)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;
                    return *this;
                }
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type& operator*() const {
                return (*(current->data));
            }
            bool operator==(const iterator& rhs) const {
                return (rhs.address == address) && (rhs.current == current);
            }
            bool operator==(const const_iterator& rhs) const {
                return (rhs.address == address) && (rhs.current == current);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return (rhs.address != address) || (rhs.current != current);
            }
            bool operator!=(const const_iterator& rhs) const {
                return (rhs.address != address) || (rhs.current != current);
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return current->data;
            }
        };
        class const_iterator {
            friend class map;
        private:
            const redblacknode* current;
            const map* address;
        public:
            const_iterator(const redblacknode* a = NULL, const map* t = NULL) {
                current = a;
                address = t;
            }
            const_iterator(const const_iterator& other) {
                current = other.current;
                address = other.address;
            }
            const_iterator(const iterator& other) {
                current = other.current;
                address = other.address;
            }
            const_iterator operator++(int) {
                if (current == NULL) throw invalid_iterator();
                const_iterator tmp(current, address);
                if (current->right != NULL) {  //如果该节点有右儿子，找右儿子的左儿子
                    const redblacknode* p1 = current->right;
                    while (p1->left != NULL) {
                        p1 = p1->left;
                    }
                    current = p1;
                    return tmp;
                }
                else {                              //tmp没有右儿子->找到一个节点，该节点的左子树包含了tmp。
                    const redblacknode* tmq = current->parent;
                    const redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->right)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;           //如果tmp指向的是最后一个元素，则return NULL
                    return tmp;
                }

            }
            //重载++x
            const_iterator& operator++() {
                // iterator tmp(current, address);
                if (current == NULL) throw invalid_iterator();
                if (current->right != NULL) {  //如果该节点有右儿子，找右儿子的左儿子
                    const redblacknode* p1 = current->right;
                    while (p1->left != NULL) {
                        p1 = p1->left;
                    }
                    current = p1;
                    return *this;
                }
                else {                              //tmp没有右儿子->找到一个节点，该节点的左子树包含了tmp。
                    const redblacknode* tmq = current->parent;
                    const redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->right)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;           //如果tmp指向的是最后一个元素，则return NULL
                    return *this;
                }

            }
            const_iterator operator--(int) {
                const_iterator tmp(current, address);
                if (address != NULL && current == NULL) {
                    current = address->root;
                    if (current == NULL) throw invalid_iterator();
                    while (current->right != NULL) {
                        current = current->right;
                    }
                    return tmp;
                }
                if (current == NULL) throw invalid_iterator();
                if (current->left != NULL) {  //如果该节点有左儿子，找左儿子的右儿子
                    const redblacknode* p1 = current->left;
                    while (p1->right != NULL) {
                        p1 = p1->right;
                    }
                    current = p1;
                    return tmp;
                }
                else {                              //tmp没有左儿子->找到一个节点，该节点的右子树包含了tmp。
                    const redblacknode* tmq = current->parent;
                    const redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->left)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;           //如果tmp指向的是最后一个元素，则return NULL
                    return tmp;
                }

            }
            const_iterator& operator--() {
                if (address != NULL && current == NULL) {
                    current = address->root;
                    if (current == NULL) throw invalid_iterator();
                    while (current->right != NULL) {
                        current = current->right;
                    }
                    return *this;
                }
                if (current == NULL) throw invalid_iterator();
                if (current->left != NULL) {  //如果该节点有左儿子，找左儿子的右儿子
                    const redblacknode* p1 = current->left;
                    while (p1->right != NULL) {
                        p1 = p1->right;
                    }
                    current = p1;
                    return *this;
                }
                else {
                    const redblacknode* tmq = current->parent;
                    const redblacknode* x = current;
                    while ((tmq != NULL) && (x == tmq->left)) {
                        x = tmq;
                        tmq = tmq->parent;
                    }
                    current = tmq;
                    return *this;
                }
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type& operator*() const {
                return (*(current->data));
            }
            bool operator==(const iterator& rhs) const {
                return (rhs.address == address) && (rhs.current == current);
            }
            bool operator==(const const_iterator& rhs) const {
                return (rhs.address == address) && (rhs.current == current);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return (rhs.address != address) || (rhs.current != current);
            }
            bool operator!=(const const_iterator& rhs) const {
                return (rhs.address != address) || (rhs.current != current);
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            const value_type* operator->() const noexcept {
                return (current->data);
            }
        };
        map() { root = NULL;length = 0; }
        map(const map& other) {
            length = other.length;
            root = create(other.root);
        }
        map& operator=(const map& other) {
            if (this == &other) { return *this; }
            length = other.length;
            clear(root);
            root = create(other.root);
            return *this;
        }
        ~map() { clear();length = 0; }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T& at(const Key& key) {
            iterator tmp;
            tmp = find(key);
            if (tmp.current == NULL) {
                throw index_out_of_bound();
            }
            else {
                return tmp.current->data->second;
            }
        }
        const T& at(const Key& key) const {
            const_iterator tmp;
            tmp = find(key);
            if (tmp.current == NULL) {
                throw index_out_of_bound();
            }
            else {
                return tmp.current->data->second;
            }
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T& operator[](const Key& key) {
            iterator ans;
            ans = find(key);
            if (ans.current != NULL) { return ans.current->data->second; }
            pair<iterator, bool> p = insert(pair<Key, T>(key, T()));
            return p.first.current->data->second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T& operator[](const Key& key) const {
            const_iterator ans;
            ans = find(key);
            if (ans.current == NULL) { throw index_out_of_bound(); }
            return ans.current->data->second;
        }
        iterator begin() {
            iterator tmp(NULL, this);
            tmp.current = root;
            if (root == NULL) { return tmp; }
            while (tmp.current->left != NULL) {
                tmp.current = tmp.current->left;
            }
            return tmp;
        }
        const_iterator cbegin() const {
            const_iterator tmp(NULL, this);
            tmp.current = root;
            if (root == NULL) { return tmp; }
            while (tmp.current->left != NULL) {
                tmp.current = tmp.current->left;
            }
            return tmp;
        }
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            iterator ans(NULL, this);
            return ans;
        }
        const_iterator cend() const {
            const_iterator  ans(NULL, this);
            return ans;
        }
        bool empty() const { return length == 0; }
        size_t size() const { return length; }
        void clear() { clear(root);length = 0; }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type& value) {
            iterator ans;
            ans = find(value.first);
            if (ans.address == this && ans.current != NULL) { return pair<iterator, bool>(ans, false); }
            Compare cmp;
            redblacknode* t, * parent, * grandp;
            if (root == NULL) {                                                   //插入为空树的特殊情况
                const value_type* x;
                x = &value;
                root = new redblacknode(x, NULL, NULL, NULL, BLACK);
                iterator ans(root, this);
                length++;
                return pair<iterator, bool>(ans, true);
            }
            parent = grandp = t = root;
            length++;
            while (true) {
                if (t != NULL) {
                    if (t->left && t->left->colour == RED && t->right && t->right->colour == RED) {  //t的两个儿子都是红色，需要重新着色，再判断其父亲节点是否为红色
                        t->left->colour = t->right->colour = BLACK;
                        t->colour = RED;
                        insertadjust(grandp, parent, t);
                    }
                    grandp = parent;                                 // 下延一层
                    parent = t;
                    if (cmp(value.first, t->data->first)) {
                        t = t->left;
                    }
                    else {
                        t = t->right;
                    }
                }
                else {
                    const value_type* x;
                    x = &value;
                    t = new redblacknode(x);
                    if (cmp(value.first, parent->data->first)) {
                        parent->left = t;
                        t->parent = parent;
                    }
                    else {
                        parent->right = t;
                        t->parent = parent;
                    }
                    insertadjust(grandp, parent, t);
                    root->colour = BLACK;
                    iterator ans(t, this);//t在insertadjust之后指向出现了问题！！！！！！！
                    return pair<iterator, bool>(ans, true);
                }
            }


        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            Compare cmp;
            if (pos.address != this || pos.current == NULL) { throw invalid_iterator(); }
            length--;
            Key x = pos.current->data->first;
            Key* del = new Key(x);
            redblacknode* t, * p, * c;     //c是当前节点，p是父亲节点，t是兄弟节点
            if (root == NULL) {
                return;
            }
            if ((!cmp(root->data->first, x)) && (!cmp(x, root->data->first)) && root->left == NULL && root->right == NULL) {
                delete root;
                root = NULL;
                delete del;
                return;
            }
            p = c = t = root;
            while (true) {
                removeadjust(p, c, t, *del);
                if ((!cmp(c->data->first, *del)) && (!cmp(*del, c->data->first)) && c->left && c->right) {  //c是被删节点并且有两个孩子
                    redblacknode* tmp = c->right;
                    bool flag = 0;
                    while (tmp->left) { tmp = tmp->left; }
                    delete c->data;
                    c->data = new value_type(*(tmp->data));                    
                    delete del;
                    del = new Key(tmp->data->first);
                    colourT ccolour = c->colour;
                    colourT tmpcolour = tmp->colour;
                    if (tmp != c->right) {
                        redblacknode* tmpright = tmp->right;
                        redblacknode* tmpparent = tmp->parent;
                        tmp->parent = c->parent;
                        if (c->parent) {
                            if (c->parent->left == c) { c->parent->left = tmp; }
                            else { c->parent->right = tmp; }
                        }
                        else {
                            flag = 1;
                        }
                        tmp->left = c->left;c->left->parent = tmp;
                        tmp->right = c->right;c->right->parent = tmp;
                        c->parent = tmpparent;
                        tmpparent->left = c;
                        c->right = tmpright;
                        c->left = NULL;
                        if (tmpright) {
                            tmpright->parent = c;
                        }
                        tmp->colour = ccolour;
                        c->colour = tmpcolour;
                        if (flag == 1) { root = tmp; }
                        c = tmp;
                    }
                    else {
                        c->right = tmp->right;if (tmp->right) { tmp->right->parent = c; }
                        tmp->left = c->left; c->left->parent = tmp;
                        c->left = NULL;
                        tmp->parent = c->parent;
                        if (c->parent) {
                            if (c->parent->left == c) { c->parent->left = tmp; }
                            else { c->parent->right = tmp; }
                        }
                        else {
                            flag = 1;
                        }
                        c->parent = tmp;
                        tmp->right = c;
                        tmp->colour = ccolour;
                        c->colour = tmpcolour;
                        if (flag == 1) { root = tmp; }
                        c = tmp;
                    }
                    p = c;
                    c = c->right;
                    t = p->left;
                    continue;
                }
                if ((!cmp(c->data->first, *del)) && (!cmp(*del, c->data->first))) {                   //c是被删节点              
                    if (p->left == c) {
                        p->left = NULL;
                    }
                    else {
                        p->right = NULL;
                    }
                    delete c;
                    root->colour = BLACK;
                    delete del;
                    return;
                }
                p = c;
                if (cmp(*del, c->data->first)) {
                    c = p->left;
                    t = p->right;
                }
                else {
                    c = p->right;
                    t = p->left;
                }
            }
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key& key) const {
            const_iterator tmp;
            tmp = find(key);
            if (tmp.current == NULL) { return 0; }
            else { return 1; }
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key& key) {
            redblacknode* tmp = root;
            Compare cmp;
            while (tmp != NULL && (cmp(key, tmp->data->first) || cmp(tmp->data->first, key))) {
                if (cmp(key, tmp->data->first)) {  //key<节点键值
                    tmp = tmp->left;
                }
                else {
                    tmp = tmp->right;
                }
            }
            iterator ans(tmp, this);
            return ans;
        }
        const_iterator find(const Key& key) const {
            redblacknode* tmp = root;
            Compare cmp;
            while (tmp != NULL && (cmp(key, tmp->data->first) || cmp(tmp->data->first, key))) {
                if (cmp(key, tmp->data->first)) {  //key<节点键值
                    tmp = tmp->left;
                }
                else {
                    tmp = tmp->right;
                }
            }
            const_iterator ans(tmp, this);
            return ans;
        }
    };

}

#endif
