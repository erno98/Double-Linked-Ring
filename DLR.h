//
// Created by Ernest Pokropek
//


/***************************************************************************
* DLR is a Double Linked Ring, that can store parameters of any type.
* It has two main templates: Key, and Info. Both of the parameters are used
* in methods, and can be used variously. In other words, DLR stores
* pairs of elements of the constant type relation (int-int, int-string, etc.).
* As Info value may be unrestricted, the Key is unique for every Node of the
* DLR.
*
* Also, in the class DLR there's implemented class Iterator. Iterator
* points to elements (Nodes) of the DLR, with many useful operators
* to iterate through it. Iterator in this class is implemented as a pointer.
*
* First section of the file is devoted to definitions, and the second one to
* declarations.
*
* Nomenclature:
 * DLR - Double Linked Ring, this shortcut will be used frequently in the
 *       comment section
 * any -> "first" element of the DLR, defaultly set to first added element
 * Node -> structure of single element of the DLR
 *         (element of Key, element of Info, pointers to next and previous nodes)
****************************************************************************/

#ifndef EADS2_DLR_H
#define EADS2_DLR_H

#include <new>
#include <memory>
#include <iostream>

template<typename Key, typename Info>
class DLR{

private:

/***************************************************************************
*  NODE DECLARATION
****************************************************************************/

    struct Node{
        Key key;
        Info info;
        Node *next;
        Node *previous;

        // default constructor
        Node(): key(), info(){
            next = nullptr;
            previous = nullptr;
        }

        // destructor
        ~Node() = default;

        // copy constructor
        Node(const Key &aKey, const Info &aInfo){
            next = nullptr;
            previous = nullptr;
            key = aKey;
            info = aInfo;
        }

    };

    Node *any;


public:

/***************************************************************************
*  ITERATOR
****************************************************************************/

    class Iterator{
    private:
        friend class DLR;
        mutable Node *travel;

    public:
        struct Content{
            Key &key;
            Info &info;
        };

    /****************************************************
    *  ITERATOR MEMBER METHODS
    *****************************************************/

        // default constructor
        Iterator(){
            travel = nullptr;
        }

        // default destructor
        ~Iterator() = default;

        // copy constructor
        Iterator(const Iterator &aIterator){
            travel = aIterator.travel;
        }

        // assignment operator
        Iterator &operator=(const Iterator &aIterator){
            travel = aIterator.travel;
            return *this;
        }

        // support constructor
        Iterator(Node *node){
            travel = node;
        }

    /****************************************************
     *  ITERATOR MOVEMENT OPERATORS
     *****************************************************/

        Iterator &operator++() const{
            travel = travel -> next;
            return *this;
        }

        Iterator operator++(int) const{
            Iterator temp(travel);
            travel = travel -> next;
            return temp;
        }

        Iterator &operator--() const{
            travel = travel -> previous;
            return *this;
        }

        const Iterator operator--(int){
            Iterator temp(travel);
            travel = travel -> previous;
            return temp;
        }

        Iterator operator+ (int moveBy){
            Iterator temp(travel);
            for(int i = 0; i < moveBy; i++){
                temp.travel = temp.travel -> next;
            }
            return temp;
        }

        Iterator operator- (int moveBy){
            Iterator temp(travel);
            for(int i = 0; i < moveBy; i++){
                temp.travel = temp.travel -> previous;
            }
            return temp;
        }

     /****************************************************
     *  ITERATOR ACCESS OPERATORS
     *****************************************************/

        Content operator*(){
            Content out{
                travel -> key,
                travel -> info
            };
            return out;
        }

        const Content operator*() const{
            Content out{
                    travel -> key,
                    travel -> info
            };
            return out;
        }

        Content* operator->(){
            std::unique_ptr<Content> out(new Content{
                travel -> key,
                travel -> info
            });
            return out.get();
        }

        const Content* operator->() const{
            std::unique_ptr<Content> out(new Content{
                    travel -> key,
                    travel -> info
            });
            return &out;
        }


     /****************************************************
     *  ITERATOR LOGIC OPERATORS
     *****************************************************/
        bool operator==(const Iterator &aIterator) const{
            return travel == aIterator.travel;
        }

        bool operator!=(const Iterator &aIterator) const{
            return travel != aIterator.travel;
        }

    };
    typedef const Iterator ConstIterator;

    /****************************************************
    *  ITERATOR METHODS
    *****************************************************/
        Iterator begin() const{
            return Iterator(any);
        }

        Iterator find(const Key &aKey, int occurrence = 1) const;


/***************************************************************************
*  DLR METHODS
****************************************************************************/

    /****************************************************
    *  MEMBER METHODS
    *****************************************************/

    // default constructor
        DLR(){
            any = nullptr;
        }

    // default destructor
        ~DLR(){
            clear();
        }

    // copy constructor
        DLR(const DLR<Key, Info> &aDLR){
            any = nullptr;
            *this = aDLR;
        }

    // assignment operator
        DLR<Key, Info> &operator=(const DLR<Key, Info> &aDLR);



    /***************************************************************************
    *  CAPACITY
    ****************************************************************************/

    bool exists(const Key &key);
    // RETURNS:
    //    true, if the element exists in the DLR
    //    false, if the element doesn't exist in the DLR
    // PARAMETERS: key of the sought node

    unsigned int howMany(const Key &aKey);
    // RETURNS:
    //   an integer number of how much elements of given
    //   key there are in the sequence
    // PARAMETERS: key of the sought node(s)

    bool isEmpty();
    // RETURNS:
    //    true, if the DLR has no elements
    //    false, if the DLR has at least 1 element

    unsigned int length() const;
    // RETURNS:
    //    number of nodes in the DLR


    /***************************************************************************
    *  DISPLAY
    ****************************************************************************/

        void print();
        // prints the DLR into the output stream

    /***************************************************************************
    *  MODIFIERS
    ****************************************************************************/

        /***********************************************************************
        *  methods of adding to the DLR
       ************************************************************************/

        void pushBack(const Key &newKey, const Info &newInfo);
        // inserts a new element at the end of the DLR
        // PARAMETERS: Key and Info of new node
        // THROWS:
        //    std::bad_alloc in case of memory allocation failure

        bool insertAfter(const Key &key, const Key &newKey, const Info &newInfo, int occurrence = 1);
        // inserts a new element after the given one
        // PARAMETERS: Key and Info of new node,
        //             Key of the node before which the new one is added,
        //             number of node's occurrence*, defaultly 1
        // RETURNS:
        //    true, if the insert was successful
        //    false, if the element hasn't been added
        // THROWS:
        //    std::bad_alloc in case of memory allocation failure

        ///*consider a DLR 2 3 5 1 5. If we want to insert element after second 5, the occurrence value
        /// for the function will be equal to 2. If we won't specify it, element will be added after the first one.
        /// occurrence index is being counted from 'any' pointer.

        bool insertAfter(const Iterator &location, const Key &newKey, const Info &newInfo);
        // inserts a new element after the one which iterator is pointing at
        // PARAMETERS: Key and Info of new node,
        //             an Iterator
        // RETURNS:
        //    true, if the insert was successful
        //    false, if the element hasn't been added
        // THROWS:
        //    std::bad_alloc in case of memory allocation failure


        bool insertBefore(const Key &key, const Key &newKey, const Info &newInfo, int occurrence = 1);
        // inserts a new element before the given one
        // PARAMETERS: Key and Info of new node,
        //             Key of the node before which the new one is added,
        //             number of node's occurrence, defaultly 1
        // RETURNS:
        //    true, if the insert was successful
        //    false, if the element hasn't been added
        // THROWS:
        //    std::bad_alloc in case of memory allocation failure

        bool insertBefore(const Iterator &location, const Key &newKey, const Info &newInfo);
        // inserts a new element before the one which iterator is pointing at
        // PARAMETERS: Key and Info of new node,
        //             an Iterator
        // RETURNS:
        //    true, if the insert was successful
        //    false, if the element hasn't been added
        // THROWS:
        //    std::bad_alloc in case of memory allocation failure


        /***********************************************************************
         *  methods of removing from the DLR
        ************************************************************************/

        void remove(const Key &key, int occurrence = 1);
        // removes given element from the DLR
        // PARAMETERS: Key of the node to remove,
        //             number of node's occurrence, defaultly 1

        void remove(const Iterator &location);
        // removes the element from the DLR at which given iterator points at
        // PARAMETERS: an Iterator

        void clear();
        // removes every element from the DLR


    /***************************************************************************
    *  OPERATORS
    ****************************************************************************/

        bool operator==(const DLR<Key, Info> &aDLR) const;
        // compares two DLRs
        // PARAMETERS: constant reference to another DLR
        // RETURNS:
        //      true if the DLRs are identical (order matters)
        //      false, if the DLRs are different
        // !ORDER MATTERS!

        bool operator!=(const DLR<Key, Info> &aDLR) const;
        // compares two DLRs
        // PARAMETERS: constant reference to another DLR
        // RETURNS:
        //      true if the DLRs are different (order matters)
        //      false, if the DLRs are identical
        // !ORDER MATTERS!


};


/***********************************************************************
*   IMPLEMENTATION
************************************************************************/


template<typename Key, typename Info>
typename DLR<Key, Info>::Iterator DLR<Key, Info>::find(const Key &aKey, int occurrence) const {

    if(any == nullptr)
        return Iterator();

    int i = 0;
    auto travel = any;
    do{

        if(travel -> key == aKey && ++i == occurrence)
            return Iterator(travel);
        travel = travel -> next;

    } while(travel != any);

    return Iterator();

}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
DLR<Key, Info> &DLR<Key, Info>::operator=(const DLR<Key, Info> &aDLR) {

    if(this == &aDLR)
        return *this;

    clear();

    if(aDLR.any == nullptr)
        return *this;

    auto travel = aDLR.any;
    do{
        pushBack(travel -> key, travel -> info);
        travel = travel -> next;
    } while(travel != aDLR.any);

    return *this;
}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::exists(const Key &key) {


    //empty DLR
    if(this -> any == nullptr)
        return false;

    //non empty DLR
    auto travel = this->any;
    do{
        if(travel -> key == key)
            return true;
        travel = travel -> next;

    }while(travel != this->any);


    return false;
}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
unsigned int DLR<Key, Info>::howMany(const Key &aKey) {

    //empty DLR
    if(this -> any == nullptr)
        return 0;

     //non empty DLR
     unsigned int count = 0;
     auto travel = this->any;
    do{
        if(travel -> key == aKey)
            count++;
        travel = travel->next;

    }while(travel != any);

    return count;

}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::isEmpty() {

    return any == nullptr;

}


//--------------------------------------------------------------------------



template<typename Key, typename Info>
unsigned int DLR<Key, Info>::length() const {

    //empty DLR
    if(this -> any == nullptr)
        return 0;

    //non empty DLR
    unsigned int count = 0;
    auto travel = this->any;
    do{
        count++;
        travel = travel->next;

    }while(travel != any);

    return count;
}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
void DLR<Key, Info>::print() {

    //empty DLR
    if(this -> any == nullptr) {
        std::cout << "Ring is empty." << std::endl;
        return;
    }


    //non empty DLR
    auto travel = this->any;
    do{
        std::cout << "K:" << travel->key << " I:" << travel->info<< std::endl;
        travel = travel->next;

    }while(travel != any);
}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
void DLR<Key, Info>::pushBack(const Key &newKey, const Info &newInfo) {

    auto newNode = new Node(newKey, newInfo);

    //empty DLR
    if(this -> any == nullptr){
        any = newNode;
        any->next = any;
        any->previous = any;
        return;
    }

    //non empty DLR
    newNode->next = any;
    newNode->previous = any->previous;

    any->previous->next = newNode;
    any->previous = newNode;

}




//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::insertAfter(const Key &key, const Key &newKey, const Info &newInfo, int occurrence) {

    if(any == nullptr){
        std::cerr << "DLR is empty." << std::endl;
        return false;
    }

    if(howMany(key) == 0) {
        std::cerr << "Given key '" << key << "' doesn't exist in the DLR." << std::endl;
        return false;
    }

    if(howMany(key) < occurrence){
        std::cerr << "Given occurrence index exceeds number of given keys" << std::endl;
        std::cerr << "Key: " << key << ", found: " << howMany(key) << " times. Given occurrences: " << occurrence << " ." << std::endl;
        return false;
    }

    auto iterator = find(key, occurrence);
    return insertAfter(newKey, newInfo, iterator);



}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::insertAfter(const DLR::Iterator &location, const Key &newKey, const Info &newInfo) {


    if(location.travel == nullptr) {
        return false;
    }

    auto insert = new Node( newKey, newInfo );
    insert -> previous = location.travel;
    insert -> next = location.travel -> next;
    location.travel -> next -> previous = insert;
    location.travel -> next = insert;


    return true;

}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::insertBefore(const Key &key, const Key &newKey, const Info &newInfo, int occurrence) {

    if(any == nullptr){
        std::cerr << "DLR is empty." << std::endl;
        return false;
    }

    if(howMany(key) == 0) {
        std::cerr << "Given key '" << key << "' doesn't exist in the DLR." << std::endl;
        return false;
    }

    if(howMany(key) < occurrence){
        std::cerr << "Given occurrence index exceeds number of given keys" << std::endl;
        std::cerr << "Key: " << key << ", found: " << howMany(key) << " times. Given occurrences: " << occurrence << " ." << std::endl;
        return false;
    }

    auto iterator = find(key, occurrence);
    return insertBefore(newKey, newInfo, iterator);

}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::insertBefore(const DLR::Iterator &location, const Key &newKey, const Info &newInfo) {

    if(location.travel == nullptr)
        return false;

    auto insert = new Node(newKey, newInfo);
    insert -> next = location.travel;
    insert -> previous = location.travel -> previous;
    location.travel -> previous -> next = insert;
    location.travel -> previous = insert;


    return true;
}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
void DLR<Key, Info>::remove(const Key &key, int occurrence) {

    //empty DLR
    if(any == nullptr){
        std::cerr << "DLR is empty." << std::endl;
        return;
    }

    if(howMany(key) == 0) {
        std::cerr << "Given key '" << key << "' doesn't exist in the DLR." << std::endl;
        return;
    }

    if(howMany(key) < occurrence){
        std::cerr << "Given occurrence index exceeds number of given keys" << std::endl;
        std::cerr << "Key: " << key << ", found: " << howMany(key) << " times. Given occurrences: " << occurrence << " ." << std::endl;
        return;
    }

    auto iterator = find(key, occurrence);
    remove(iterator);


}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
void DLR<Key, Info>::remove(const DLR::Iterator &location) {


    //empty DLR
    if(location.travel == nullptr){
        return;
    }

    //1 elem DLR
    if(any == any->next){
        delete any;
        any = nullptr;
        return;
    }

    //more than 1 elem DLR
    location.travel -> next -> previous = location.travel -> previous;
    location.travel -> previous -> next = location.travel -> next;
    any = location.travel -> next;
    delete location.travel;

}




//--------------------------------------------------------------------------


template<typename Key, typename Info>
void DLR<Key, Info>::clear() {

    //empty DLR
    if(any == nullptr){
        std::cerr << "DLR is empty." << std::endl;
        return;
    }

    auto travel = any -> next;
    do{

        auto temp = travel;
        travel = travel -> next;
        delete temp;

    }while(travel != any);

    delete any;
    any = nullptr;



}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::operator==(const DLR<Key, Info> &aDLR) const {

    //different lengths
    if(this->length() != aDLR.length())
        return false;

    //same lengths and one of them empty, so both empty
    if(this -> any == nullptr)
        return true;

     auto travel1 = any;
     auto travel2 = aDLR.any;

     //non empty DLR
     do{

         if(travel1 -> key != travel2 -> key ||
            travel2 -> info != travel2 ->info)
             return false;

         travel1 = travel1 -> next;
         travel2 = travel2 -> next;

     }while(travel1 != any && travel2 != aDLR.any);

    return true;

}


//--------------------------------------------------------------------------


template<typename Key, typename Info>
bool DLR<Key, Info>::operator!=(const DLR<Key, Info> &aDLR) const {

    return !(*this == aDLR);

}


#endif //EADS2_DLR_H
