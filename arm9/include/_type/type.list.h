#ifndef _FASTLIST_HEADER_
#define _FASTLIST_HEADER_
/**
 * @brief Simple doubly-linked List
 * 
 * @author Jakob Riedle
 * @version 1.0
 */
template<typename T>
class flist
{
	
	private:
	
		class _node
		{
			private:
				_node* prev;
				_node* next;
				
				T val;
			
			public:
				
				/**
				 * @brief Constructor for objects of class _node
				 */
				_node( T value , _node* prev , _node* next ) :
					prev( prev )
					, next( next )
					, val( value )
				{}
				
				/**
				 * @brief Constructor for objects of class _node
				 * 
				 * @param <template-parameter T> value
				 */
				_node( T value ) :
					prev( nullptr )
					, next( nullptr )
					, val( value )
				{}
				
				/**
				 * @brief Set the Value of this Node
				 * 
				 * @param <template-parameter T> value the Value to set
				 * @return void
				 */
				void setValue( T value )
				{
					this->val = value;
				}
				
				/**
				 * @brief Get the Value of this Node
				 * 
				 * @return <template-parameter T>
				 */
				T& getValue()
				{
					return this->val;
				}
				
				/**
				 * @brief Set preceeding Node
				 * 
				 * @param _node the Node to be set as the preceeding Node
				 * @return void
				 */
				void setNext( _node* nxt )
				{
					this->next = nxt;
				}
				
				/**
				 * @brief Set anteceeding Node
				 * 
				 * @param _node the Node to be set as the anteceeding Node
				 * @return void
				 */
				void setPrev( _node* prv )
				{
					this->prev = prv;
				}
				
				/**
				 * @brief Get preceeding Node
				 * 
				 * @return _node
				 */
				_node* getNext() const 
				{
					return next;
				}
				
				/**
				 * @brief Get anteceeding Node
				 * 
				 * @return _node
				 */
				_node* getPrev() const 
				{
					return prev;
				}
		};
	
	public:
	
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef int difference_type;
		
		class iterator : public std::iterator<std::bidirectional_iterator_tag,T>
		{
			protected:
			
				_node* my;
			
			public:
			
				/**
				 * @brief Constructor for objects of class iterator
				 * 
				 * @param _node* node to be used as the current Node
				 */
				iterator( _node* node )
				{
					my = node;
				}
				
				/**
				 * @brief Get the Node the iterator is pointing to
				 * 
				 * @return _node*
				 */
				_node* getNode() const {
					return this->my;
				}
				
				/**
				 * @brief Set the Node the iterator is pointing to
				 * 
				 * @param _node*
				 * @return void
				 */
				void setNode( _node* node){
					this->my = node;
				}
				
				/**
				 * @brief Increase the Iterator by one
				 *
				 * @return bool whether there was a Node following this one (=true) or if the current position is the last in the List (=false)
				 */
				virtual iterator&  operator++(){ // prefix ++iter
					this->my && ( this->my = this->my->getNext() );
					return *this;
				}
				virtual iterator& operator++( int ){ // postfix iter++
					this->my && ( this->my = this->my->getNext() );
					return *this;
				}
				
				/**
				 * @brief Decrease the Iterator by one
				 *
				 * @return bool whether there was a Node preceding this one (=true) or if the current position is the first in the List (=false)
				 */
				virtual iterator& operator--(){ // prefix --iter
					this->my && ( this->my = this->my->getPrev() );
					return *this;
				}
				virtual iterator& operator--( int ){ // postfix iter--
					this->my && ( this->my = this->my->getPrev() );
					return *this;
				}
				
				/**
				 * @brief Convert the Iterator to an int referring to the position in the list (array-like index)
				 * 
				 * @note if %this iterator points to the first element in the list, the resulting number is 0
				 * @note and if this iterator points to an invalid node or to null, this function returns -1
				 * @return int
				 */
				operator int()
				{
					int i = -1;
					_node* nd = this->my;
					
					// Increase i as long as i can go forward
					while( nd ){
						nd = nd->getPrev();
						i++;
					}
					return i;
				}
				
				/**
				 * @brief Increase the Iterator %nth times
				 *
				 * @return iterator&
				 */
				iterator operator+( int nth ){
					_node* nd = this->my;
					for( ; nth-- && nd ; nd = nd->getNext() );
					return iterator( nd );
				}
				iterator& operator+=( int nth ){
					for( this->my ; nth-- && this->my ; this->my = this->my->getNext() );
					return *this;
				}
				
				/**
				 * @brief Decrease the Iterator %nth times
				 *
				 * @return iterator&
				 */
				iterator operator-( int nth ){
					_node* nd = this->my;
					for( ; nth-- && nd ; nd = nd->getPrev() );
					return iterator( nd );
				}
				iterator& operator-=( int nth ){
					for( this->my ; nth-- && this->my ; this->my = this->my->getPrev() );
					return *this;
				}
				
				/**
				 * @brief Get the number of nodes between two iterators
				 * If the supplied iterator points to an element after %this element, the resulting number is negative
				 * 
				 * @param iterator it the iterator to compare to
				 */
				int operator-( iterator&& it ){
					return int(*this) - int(it);
				}
				int operator-( iterator& it ){
					return int(*this) - int(it);
				}
				
				/**
				 * @brief Compare two iterators
				 */
				bool operator==( const iterator& it ) const { return my == it.my; }
				bool operator!=( const iterator& it ) const { return my != it.my; }
				bool operator>( const iterator& it ) const { return int(*this) > int(it); }
				bool operator>=( const iterator& it ) const { return int(*this) >= int(it); }
				bool operator<( const iterator& it ) const { return int(*this) < int(it); }
				bool operator<=( const iterator& it ) const { return int(*this) <= int(it); }
				
				/**
				 * @brief Returns the elements value (might be nullptr)
				 *
				 * @return <template-parameter T>
				 */
				T& getValue(){
					return my->getValue();
				}
				
				/**
				 * @brief Returns the elements value (might be nullptr)
				 *
				 * @return <template-parameter T>
				 */
				T& operator*(){
					return my->getValue();
				}
				
				/**
				 * @brief Returns the elements value (might be nullptr)
				 *
				 * @return <template-parameter T>
				 */
				T* operator->(){
					return &my->getValue();
				}
				
				/**
				 * @brief Sets the elements value (only possible if the iterator points to a valid _node
				 *
				 * @param <template-parameter T> value
				 * @return void
				 */
				void setValue( T value ){
					if( my )
						my->setValue( value );
				}
				
				/**
				 * @brief Check whether there is a Node following this one
				 *
				 * @return bool true if there is one or fals if not
				 */
				bool hasNext() const {
					return my->getNext();
				}
				
				/**
				 * @brief Check whether there is a Node preceding this one
				 *
				 * @return bool true if there is one or fals if not
				 */
				bool hasPrev() const {
					return my->getPrev();
				}
				
				/**
				 * @brief Check if the Iterator is valid
				 * 
				 * @return bool
				 */
				operator bool() const {
					return this->my;
				}
				
				/**
				 * @brief Check if the Iterator is valid
				 * 
				 * @return bool
				 */
				bool valid() const {
					return this->my;
				}
		};
		
		class reverse_iterator : public iterator
		{
			public:
				// Swap 'em
				reverse_iterator& operator++(){ // postfix iter--
					this->my && ( this->my = this->my->getPrev() );
					return *this;
				}
				reverse_iterator& operator++( int ){ // postfix iter--
					this->my && ( this->my = this->my->getPrev() );
					return *this;
				}
				reverse_iterator& operator--(){ // prefix --iter
					this->my && ( this->my = this->my->getNext() );
					return *this;
				}
				reverse_iterator& operator--( int ){ // postfix iter--
					this->my && ( this->my = this->my->getNext() );
					return *this;
				}
				
				/**
				 * @brief From iterator to reverse_iterator
				 */
				reverse_iterator& operator=( const iterator& it ){ this->my = it.my; }
				reverse_iterator( const iterator& it ) : iterator( it ) { }
		};
		
	private:
	
		_node* top;
		_node* bottom;
		
		unsigned int count;
	
	public:
	
		/**
		 * @brief Constructor for objects of class flist
		 */
		flist() : count( 0 ) , top( nullptr ) , bottom( nullptr )
		{
			
		}// flist
		
		flist( std::initializer_list<T>& list ) : count( 0 ) , top( nullptr ) , bottom( nullptr )
		{
			for( auto& val : list )
				this->push_back( val );
		}// flist
		
		/**
		 * @brief Copy all values from %list into %this list
		 * @param flist list the list to Copy
		 * @return flist&
		 */
		template<class K>
		flist& operator=( flist<K>& list )
		{
			this->clear();
			
			for( auto val : list )
				this->push_back( val );
			return *this;
		}
		
		/**
		 * @brief Add a new element to the Lists end
		 *
		 * @param <template-parameter T> val the new Element to be pushed onto the List
		 * @return void
		 */
		void push_back( T val )
		{			
			_node* newNode = new _node( val );
			
			// Rewrite Pointers
			newNode->setPrev( this->bottom );
			
			if( this->count++ ) // Items in the List??
				this->bottom->setNext( newNode );
			else
				this->top = newNode; // Create startingNode and endingNode
			
			this->bottom = newNode;
		}// push_back
		
		/**
		 * @brief Add a new element to the Lists front
		 *
		 * @param <template-parameter T> val the new Element to be pushed onto the List
		 * @return void
		 */
		void push_front( T val )
		{
			_node* newNode = new _node( val );
			
			// Rewrite Pointers
			newNode->setNext( this->top );
			
			if( this->count++ ) // Items in the List??
				this->top->setPrev( newNode );
			else
				this->bottom = newNode; // Create startingNode and endingNode
			
			this->top = newNode;
		}//push_front
		
		/**
		 * @brief Returns an Iterator to the first Node
		 *
		 * @return iterator
		 */
		iterator begin() const {
			return iterator(this->top);
		}// begin
		
		/**
		 * @brief Returns an Iterator to the last Node
		 *
		 * @return iterator
		 */
		iterator end() const {
			return iterator(nullptr);
		}// end
		
		/**
		 * @brief Returns a Reverse-Iterator to the first Node
		 * 
		 * @return iterator
		 */
		reverse_iterator rbegin() const {
			return reverse_iterator(this->bottom);
		}// begin
		
		/**
		 * @brief Returns a Reverse-Iterator to the last Node
		 *
		 * @return iterator
		 */
		reverse_iterator rend() const {
			return reverse_iterator(nullptr);
		}// end
		
		/**
		 * @brief Returns the first elements value or nullptr if the List is empty
		 *
		 * @return <template-parameter T>
		 */
		T front() const {
			if( !this->count )
			{
				T tmp;
				return tmp;
			}
			return this->top->getValue();
		}// front
		
		/**
		 * @brief Return the last elements value or nullptr if the List is empty
		 *
		 * @return <template-parameter T>
		 */
		T back() const {
			if( !this->count )
			{
				T tmp;
				return tmp;
			}
			return this->bottom->getValue();
		}// bottom

		/**
		 * @brief Returns the elements value at the position <index> 
		 * or nullptr if the List is empty or if the index is not outside the Lists range
		 * 
		 * @param int index the Index at which the value should be returned
		 * @return <template-parameter T>
		 */
		T get( int index ) const 
		{
			if( index < 0 || index >= this->count )
			{
				T tmp;
				return tmp;
			}
			
			iterator i = this->begin();
			
			for( int temp = 0 ; i ; i++ )
			{
				if( temp++ == index )
					return i.getValue();
			}
			T tmp;
			return tmp;
		}// get
		
		/**
		 * @brief Returns an iterator to the nth position
		 * or nullptr if the List is empty or if the index is not outside the Lists range
		 * 
		 * @param int index the Index at which the iterator should point
		 * @return iterator
		 */
		iterator at( int index ) const 
		{
			if( index < 0 || index >= this->count )
				return iterator(nullptr);
			
			return this->begin()+index;
		}// get

		/**
		 * @brief Returns the number of elements in the List
		 *
		 * @return int Number fo elements in the List
		 */
		int size() const {
			return this->count;
		}// front
		
		/**
		 * @brief Returns true if the List is empty
		 *
		 * @return bool result
		 */
		bool empty() const {
			return !this->count;
		}// front
		
		/**
		 * @brief Calls every Elements "toString()" Method and prints them onto the screen
		 *
		 * @return void
		 */
		//void print()
		//{
		//	// Range-Based!!!!!
		//	for( T tmp : *this )
		//		printf( "%d | ", tmp );
		//	printf("\n");
		//}// print
		
		/**
		 * @brief Calls every Elements "toString()" Method and prints them onto the screen
		 *
		 * @return void
		 */
		//void print2()
		//{
		//	for( auto i = this->rbegin() ; i != this->rend() ; ++i )
		//		printf( "%d | ", *i );
		//	printf("\n");
		//}// print
		
		/**
		 * @brief Remove the first Element in the List
		 *
		 * @note (Won't do anything if the List is empty)
		 * @return void
		 */
		void pop_front()
		{
			if( !this->count )
				return;
			
			//! This way is faster
			if( !--this->count )
			{
				delete this->top;
				this->top = this->bottom = nullptr;
			}
			else
			{
				_node* tmp = this->top->getNext();
				
				// delete the Top
				delete this->top;
				
				// Set new top
				this->top = tmp;
				
				// Sibling Node won't have a previous one!
				this->top->setPrev( nullptr );
			}
			
		}// pop_front
		
		/**
		 * @brief Remove the first Element in the List
		 *
		 * @note (Won't do anything if the List is empty)
		 * @return void
		 */
		void pop_back()
		{
			if( !this->count )
				return;
			
			if( !--this->count )
			{
				delete this->bottom;
				this->top = this->bottom = nullptr;
			}
			else
			{
				_node* tmp = this->bottom->getPrev();
				
				// delete the Node
				delete this->bottom;
				
				// Set new bottom
				this->bottom = tmp;
				
				// Sibling Node won't have a previous one!
				this->bottom->setNext( nullptr );
			}
			
		}// pop_back
		
		/**
		 * @brief Insert an element into the List before specified position iter
		 * 
		 * @note if iter is not valid the value will be pushed at the start!
		 * @param iterator iter position to insert before
		 * @param <template-parameter T> the value to be inserted
		 * @return void
		 */
		void insert_before( iterator& iter , T value )
		{
			// check for validity and maybe push at the front
			if( !iter )
			{
				this->push_front( value );
				iter.setNode( this->top );
			}
			else
				this->insert_before_internal( iter.getNode() , value );
		}
		void insert_before( iterator&& iter , T value )
		{
			// check for validity and maybe push at the front
			if( iter )
				this->insert_before_internal( iter.getNode() , value );
			else
				this->push_front( value );
		}
		private:
		void insert_before_internal( _node* node , T value )
		{			
			// create new Node
			_node* newNode = new _node( value );
			
			if( node == this->top )
				// Set the "top" to a new Value? 
				this->top = newNode;
			else
				// set the next node of the previous Node to the newNode
				node->getPrev()->setNext( newNode );
			
			// set the ptrs at the new Node
			newNode->setPrev( node->getPrev() );
			newNode->setNext( node );
			
			// set the next node of the current one to the newNode
			node->setPrev( newNode );
			
			this->count++;
		}
		public:
		
		/**
		 * @brief Insert an element into the List after the specified position iter
		 * 
		 * @note if iter is not valid the value will be pushed at the end!
		 * @param iterator iter position to insert after
		 * @param <template-parameter T> the value to be inserted
		 * @return void
		 */
		void insert_after_preserve_validity( iterator& iter , T value )
		{
			// check for validity and maybe push at the front
			// Validate the Iterator
			_node* node = iter.getNode();
			if( node )
				this->insert_after_internal( node , value );
			else
			{
				this->push_back( value );
				iter.setNode( this->bottom );
			}
		}
		void insert_after( iterator&& iter , T value )
		{
			// check for validity and maybe push at the front
			_node* node = iter.getNode();
			if( node )
				this->insert_after_internal( node , value );
			else
				this->push_back( value );
		}
		void insert_after( iterator& iter , T value )
		{
			// check for validity and maybe push at the front
			_node* node = iter.getNode();
			if( node )
				this->insert_after_internal( node , value );
			else
				this->push_back( value );
		}
		private:
		void insert_after_internal( _node* node , T value )
		{			
			// create new Node
			_node* newNode = new _node( value );
			
			if( node == this->bottom  )
				// Set the "bottom" to a new Value? 
				this->bottom = newNode;
			else
				// set the previous node of the next Node to the newNode
				node->getNext()->setPrev( newNode ); 
			
			// set the ptrs at the new Node
			newNode->setNext( node->getNext() );
			newNode->setPrev( node );
			
			// set the next node of the current one to the newNode
			node->setNext( newNode );
			
			this->count++;
		}
		public:
		
		/**
		 * @brief Erase an element from the List at specified position iter
		 * 
		 * @param iterator iter position to erase at
		 * @return void
		 */
		void erase_preserve_validity( iterator& iter ){ 
			_node* node = iter.getNode();
			iter++;
			this->erase_internal( node );
		}
		void erase( iterator& iter ){ this->erase_internal( iter.getNode() ); }
		void erase( iterator&& iter ){ this->erase_internal( iter.getNode() ); }
		private:
		void erase_internal( _node* node )
		{
			if( !node )
				return;
			_node* next = node->getNext();
			_node* prev = node->getPrev();
			
			// Set the next Nodes' previous Ptr
			if( next )
				next->setPrev( prev );
			else
				this->bottom = prev;
			
			// Set the previous Nodes' next Ptr
			if( prev )
				prev->setNext( next );
			else
				this->top = next;
			
			// delete the Node
			delete node;
			
			this->count--;
		}
		public:
		
		/**
		 * @brief Remove all Elements in the List having the value <value>
		 * 
		 * @param <template-parameter T> value
		 * @return void
		 */
		void remove( T value )
		{
			for( iterator i = this->begin() ; i ; )
			{
				if( value == i.getValue() )
					this->erase_preserve_validity( i );
				else // used to keep the iterator valid
					i++;
			}
		}
		
		/**
		 * @brief Remove all Elements where the evalutor-func returns true
		 * 
		 * @param <evaluator-function> func the function to be evaluated
		 * @return void
		 */
		template<typename evaluator>
		void remove_if( evaluator func )
		{
			for( iterator i = this->begin() ; i ; )
			{
				if( func( i.getValue() ) )
					this->erase_preserve_validity( i );
				else // used to keep the iterator valid
					i++;
			}
		}
		
		/**
		 * @brief Finds the first element where the evalutor-func returns true and 
		 * returns an iterator to this element, or an invalid iterator if the evaluator never returned true
		 * 
		 * @param <evaluator-function> func the function to be evaluated
		 * @return iterator
		 */
		template<typename evaluator>
		iterator find_if( evaluator func ) const 
		{
			for( iterator i = this->begin() ; i ; )
			{
				if( func( *i ) )
					return i;
				else // used to keep the iterator valid
					i++;
			}
			return iterator(nullptr);
		}
		
		/**
		 * @brief Finds the first element that matches the passed value and return an iterator to it
		 * if the value didn't match any of the elements in the list, an invalid iterator is returned
		 * 
		 * @param T the value to be compared to
		 * @return iterator
		 */
		iterator find( T value ) const 
		{
			for( iterator i = this->begin() ; i ; )
			{
				if( value == *i )
					return i;
				else // used to keep the iterator valid
					i++;
			}
			return iterator(nullptr);
		}
		
		/**
		 * @brief Evokes an operation on every single element in the list
		 * 
		 * @param <evaluator-function> func the function to be evaluated
		 * @return iterator
		 */
		template<typename evaluator>
		void for_each( evaluator func )
		{
			for( T& val : *this )
				func( *val );
		}
		
		/**
		 * @brief Finds the first element where the evalutor-func returns true and 
		 * returns an iterator to this element, or an invalid iterator if the evaluator never returned true
		 * 
		 * @param <evaluator-function> func the function to be evaluated
		 * @return iterator
		 */
		template<typename evaluator>
		void sort( evaluator func )
		{
			for( iterator i = this->begin() ; i ; )
			{
				if( func( i ) )
					return i;
				else // used to keep the iterator valid
					i++;
			}
			return iterator(nullptr);
		}
		
		/**
		 * @brief Reverses the Order of elements. The first elmenemt eficiently becomes the last etc.
		 * @return void
		 */
		void reverse()
		{
			_node* tmp;
			
			for( _node* cur = this->bottom ; cur ;  )
			{
				tmp = cur->getPrev();
				cur->setPrev( cur->getNext() );
				cur->setNext( tmp );
				cur = tmp;
			}
			tmp = this->bottom;
			this->bottom = this->top;
			this->top = tmp;
		}
		
		/**
		 * @brief Clear the List
		 *
		 * @return void
		 */
		void clear()
		{
			for( _node* cur = this->top ; cur ; )
			{
				_node* tmp = cur;
				cur = cur->getNext();
				delete tmp;
			}
				
			this->bottom = nullptr;
			this->top = nullptr;
			this->count = 0;
		}// clear
		
		/**
		 * @brief Check the List for Errors (returns false, if the were errors)
		 * 
		 * @return bool
		 */
		bool check() const 
		{
			_node* prev = nullptr;
			for( _node* cur = this->top ; cur ; )
			{
				if( cur->getPrev() != prev )
					return false;
				prev = cur;
				cur = cur->getNext();
			}
			return true;
		}
        
};// class flist

#endif