
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//MyHash.h
// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
	MyHash(double maxLoadFactor = 0.5)
	{
		loadFactor = 0.5;
		if (maxLoadFactor > 2 || maxLoadFactor < 0)
			loadFactor = 2.0;
		totalBuckets = 100;
		maxBucketsAllowed = (int)(loadFactor * totalBuckets);
		bucket_array = new Bucket*[totalBuckets]();
	}
	~MyHash()
	{
		delete[] bucket_array;
	}
	void reset()
	{
		delete[] bucket_array;
		bucket_array = new Bucket*[totalBuckets];
	}
	void associate(const KeyType& key, const ValueType& value)
	{
		unsigned int hasher(const KeyType& k);
		if (getLoadFactor() >= loadFactor) { // load factor increased, reallocate map
			int oldArraySize = getNumItems();
			totalBuckets = totalBuckets * 2;
			maxBucketsAllowed = maxBucketsAllowed * 2;
			Bucket** newArray = new Bucket*[totalBuckets]();
			for (int i = 0; i < oldArraySize; i++) {
				if (bucket_array[i] != nullptr) {
					unsigned int ha = hasher(bucket_array[i]->key);
					ha = ha % totalBuckets;
					newArray[ha] = bucket_array[i];
				}
			}
			reset();
			for (int i = 0; i < totalBuckets; i++) {
				bucket_array[i] = newArray[i];
			}
		}
		unsigned int h = hasher(key);
		h = h % totalBuckets;
		if (bucket_array[h] != nullptr) {
			Bucket* traverser = bucket_array[h];
			while (traverser != nullptr) {
				if (traverser->key == key) {
					traverser->value = value;
					return;
				}
				if (traverser->next == nullptr)
					break;
				traverser = traverser->next;
			}
			Bucket* toAdd = new Bucket;
			toAdd->key = key;
			toAdd->value = value;
			traverser->next = toAdd;
		}
		else {
			Bucket* toAdd = new Bucket;
			toAdd->key = key;
			toAdd->value = value;
			bucket_array[h] = toAdd;
		}
		return;
	}
	int getNumItems() const
	{
		int size = 0;
		for (int i = 0; i < totalBuckets; i++) {
			if (bucket_array[i] != nullptr) {
				Bucket* p = bucket_array[i];
				while (p != nullptr) {
					size++;
					p = p->next;
				}
			}
		}
		return size;
	}
	double getLoadFactor() const {
		return (getNumItems()) / (totalBuckets * 1.0);
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const {
	
		unsigned int hasher(const KeyType& k);
		unsigned int h = hasher(key);
		h = h % totalBuckets;
		ValueType* p;
		if (bucket_array[h] != nullptr) {
			Bucket* t = bucket_array[h];
			while (t != nullptr) {
				if (t->key == key) {
					return &(t->value);
				}
				t = t->next;
			}
		}
		return nullptr;
	
		
		/*
		unsigned int hasher(const KeyType& k);
		unsigned int h = hasher(key);
		h = h % totalBuckets;
		ValueType *p;
		if (bucket_array[h] != nullptr) {
			p = &(bucket_array[h]->value);
			return p;
		}
		return nullptr;
		*/
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyHash(const MyHash&) = delete;
	MyHash& operator=(const MyHash&) = delete;

private:
	struct Bucket {
		KeyType key;
		ValueType value;
		bool occupied = false;
		Bucket *next = nullptr;
	};
	double loadFactor;
	int totalBuckets;
	int maxBucketsAllowed;
	Bucket** bucket_array;
};
