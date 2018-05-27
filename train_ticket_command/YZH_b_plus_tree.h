#ifndef bplustree_model
#define bplustree_model

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstddef>
#include "model_predefined.h"
#include <assert.h>
#include "newstring.h"
#include "vector.hpp"

#define OFFSET_META 0
#define OFFSET_BLOCK OFFSET_META + sizeof(meta_t)
#define SIZE_NO_CHILDREN sizeof(leaf_node_t) - TREE_ORDER * sizeof(record_t)

template <class value_t>
class bplus_tree {

	public:
		//bplustree's characters
		struct meta_t {
			size_t order;
			size_t value_size;
			size_t key_size;
			size_t internal_node_num;
			size_t leaf_node_num;
			size_t height;
			off_t slot;
			off_t root_offset;
			off_t first_leaf_offset;
		};

		//index:key and children_slot_in_file
		struct index_t {
			key_t key;
			off_t child;
		};

		struct internal_node_t {
			typedef index_t * child_t;

			off_t parent;
			off_t next;
			off_t prev;
			size_t num;
			index_t children[TREE_ORDER];

			//children[i].max < children[i].key <= children[i+1].min
		};

		//terminate record
		struct record_t {
			key_t key;
			value_t value;
		};

		struct leaf_node_t {
			typedef record_t* child_t;

			off_t parent;
			off_t next;
			off_t prev;
			size_t num;
			record_t children[TREE_ORDER];

		};

		//tool functions
		inline record_t * begin(leaf_node_t &node) const{
			return node.children;
		}

		inline record_t * end(leaf_node_t &node) const{
			return node.children + node.num;
		}

		inline index_t * begin(internal_node_t &node) const{
			return node.children;
		}

		inline index_t * end(internal_node_t &node) const{
			return node.children + node.num;
		}

		//upper_bound
		inline index_t *find(internal_node_t &node, const key_t &key) const{
			index_t *st = begin(node);
			index_t *ed = end(node) - 1;
			size_t low = 0, high = node.num - 2, mid;
			if (node.num <= 1)
				return ed;
			if (*(st + high) < key || *(st + high) == key)
				return ed;
			while (high - low > 1) {
				mid = (high + low) / 2;
				if (key < *(st + mid))
					high = mid;
				else low = mid;
			}
			if (key < *(st + low)) return st + low;
			else return st + high;
		}

		inline index_t *lower_find(internal_node_t &node, const key_t &key) const{
			index_t *st = begin(node);
			index_t *ed = end(node) - 1;
			size_t low = 0, high = node.num - 2, mid;
			if (node.num <= 1 || *(st + high) < key)
				return ed;
			while (high - low > 1) {
				mid = (high + low) / 2;
				if (key == *(st + mid))
					return st + mid;
				if (key < *(st + mid))
					high = mid;
				else low = mid;
			}
			if (key < *(st + low) || key == *(st + low)) return st + low;
			else return st + high;
		}

		//lower_bound
		inline record_t *find(leaf_node_t &node, const key_t &key) const{
			record_t *st = begin(node);
			record_t *ed = end(node);
			size_t low = 0, high = node.num - 1, mid;
			if (node.num == 0)
				return ed;
			if (*(st + high) < key)
				return ed;
			while (high - low > 1) {
				mid = (high + low) / 2;
				if (key == *(st + mid))
					return st + mid;
				if (key < *(st + mid))
					high = mid;
				else low = mid;
			}
			if (key < *(st + low) || key == *(st + low)) return st + low;
			else return st + high;
		}

		inline record_t *upper_find(leaf_node_t &node, const key_t &key) const{
			record_t *st = begin(node);
			record_t *ed = end(node);
			size_t low = 0, high = node.num - 1, mid;
			if (node.num == 0 || *(st + high) < key || *(st + high) == key)
				return ed;
			while (high - low > 1) {
				mid = (high + low) / 2;
				if (key < *(st + mid))
					high = mid;
				else low = mid;
			}
			if (key < *(st + low)) return st + low;
			else return st + high;
		}

		inline record_t* copy(record_t *first, record_t *last, record_t* result) {
			while (first != last) {
				*result = *first;
				++result;
				++first;
			}
			return result;
		}

		inline index_t* copy(index_t *first, index_t *last, index_t* result) {
			while (first != last) {
				*result = *first;
				++result;
				++first;
			}
			return result;
		}

		//copy_from_back
		inline record_t* copy_back(record_t *first, record_t *last, record_t* back) {
			while (first != last)
				*(--back) = *(--last);
			return back;
		}

		inline index_t* copy_back(index_t *first, index_t *last, index_t * back) {
			while (first != last)
				*(--back) = *(--last);
			return back;
		}

		//private:
	public:
		char path[512];//file_path
		meta_t meta;
		mutable FILE *fp;
		mutable int fp_level;//open_or_close

	public:
		bplus_tree(const char *pa, bool force_empty = false) : fp(nullptr), fp_level(0) {
			memset(path, 0, sizeof(path));
			strcpy(path, pa);

			if (!force_empty)
				if (map(&meta, OFFSET_META) != 0)//find_meta
					force_empty = true;

			if (force_empty) { //init_the_file
				open_file("w+");
				init_from_empty();
				close_file();
			}

		}
		//search the record and put the value into the point
		int search(const key_t& key, value_t *value) const {
			leaf_node_t leaf;
			map(&leaf, search_leaf(key));

			record_t *record = find(leaf, key);
			if (record != leaf.children + leaf.num) {
				*value = record->value;
				return keycmp(record->key, key); //0:success

			}
			else
				return -1;
		}

		int search_range(const key_t &left, const key_t &right, sjtu::vector<record_t> &trainid_sequence) const{
			if (keycmp(left, right) > 0)
				return 0;
			off_t off_left = search_leaf(left);
			off_t off_right = search_leaf(right);
			off_t off = off_left;
			record_t *bg, *ed;

			leaf_node_t leaf;
			while (off != off_right && off != 0) {
				map(&leaf, off);

				if (off_left == off)
					bg = find(leaf, left);
				else
					bg = begin(leaf);
				ed = end(leaf);

				for (; bg != ed; ++bg)
					trainid_sequence.push_back(*bg);

				off = leaf.next;
			}

			if (off != 0) {
				map(&leaf, off_right);
				bg = find(leaf, left);
				ed = upper_find(leaf, right);
				for (; bg != ed; ++bg)
					trainid_sequence.push_back(*bg);
			}
			return trainid_sequence.size();
		}


		//remove the record
		int remove(const key_t & key) {

			internal_node_t parent;
			leaf_node_t leaf;

			off_t parent_offset = search_index(key); //find parent(index)
			map(&parent, parent_offset);

			index_t *pos = find(parent, key);  //find leaf in parent
			off_t offset = pos->child;
			map(&leaf, offset);

			size_t min_num = meta.leaf_node_num == 1 ? 0 : meta.order / 2;

			record_t *to_delete = find(leaf, key);

			if (to_delete == end(leaf))  //record_not_exist
				return -1;

			copy(to_delete + 1, end(leaf), to_delete); //delete and move
			leaf.num--;

			if (leaf.num < min_num) {

				//first borrow
				bool borrowed = false;
				if (leaf.prev != 0)
					borrowed = borrow_key(false, leaf);  //from left

				if (!borrowed && leaf.next != 0)  //from right
					borrowed = borrow_key(true, leaf);

				//finally merge
				if (!borrowed) {
					key_t index_key;

					if (pos == end(parent) - 1) {

						//merge leaf to left
						leaf_node_t prev;
						map(&prev, leaf.prev);
						index_key = begin(prev)->key; //new_key of merged leaves

						merge_leaf(&prev, &leaf);
						node_remove(&prev, &leaf);
						unmap(&prev, leaf.prev);
					}
					else {

						//merge right to leaf
						leaf_node_t nxt;
						map(&nxt, leaf.next);
						index_key = begin(leaf)->key;

						merge_leaf(&leaf, &nxt);
						node_remove(&leaf, &nxt);
						unmap(&leaf, offset);

					}

					remove_from_index(parent_offset, parent, index_key); //merge_index_recursively

				}
				else
					unmap(&leaf, offset);
			}
			else
				unmap(&leaf, offset);

			return 0;
		}

		int insert(const key_t& key, const value_t& value) {

			off_t parent = search_index(key);
			off_t offset = search_leaf(parent, key);
			leaf_node_t leaf;
			map(&leaf, offset);

			record_t* haskey = find(leaf, key);

			if (*haskey == key) //record_exist
				return 1;

			if (leaf.num < meta.order) {  //insert_directly
				insert_record_without_split(&leaf, key, value);
				unmap(&leaf, offset);
				return 0;
			}

			leaf_node_t new_leaf;  //new leaf
			node_create(offset, &leaf, &new_leaf);

			size_t point = leaf.num / 2;  //split evenly
			bool _right = keycmp(key, leaf.children[point].key) > 0;  //new record in right?
			if (_right)
				++point;

			copy(leaf.children + point, leaf.children + leaf.num, new_leaf.children);  //move record to new
			new_leaf.num = leaf.num - point;
			leaf.num = point;

			if (_right)
				insert_record_without_split(&new_leaf, key, value);
			else
				insert_record_without_split(&leaf, key, value);

			unmap(&leaf, offset);
			unmap(&new_leaf, leaf.next);

			insert_key_to_index(parent, new_leaf.children[0].key, offset, leaf.next);  //split_index_recursively

			return 0;
		}

		//change value
		int update(const key_t& key, const value_t &value) {

			off_t offset = search_leaf(key);
			leaf_node_t leaf;
			map(&leaf, offset);

			record_t *record = find(leaf, key);
			if (record != leaf.children + leaf.num) {
				if (keycmp(key, record->key) == 0) { //record_exist
					record->value = value;
					unmap(&leaf, offset);
					return 0;
				}
				else
					return 1;  //record_not_exist
			}
			else return -1;

		}

		meta_t get_meta() const {
			return meta;
		}

		//private:
	public:
		void init_from_empty() {
			memset(&meta, 0, sizeof(meta_t));
			meta.order = TREE_ORDER;
			meta.value_size = sizeof(value_t);
			meta.key_size = sizeof(key_t);
			meta.height = 1;
			meta.slot = OFFSET_BLOCK;

			internal_node_t root;
			root.next = root.prev = root.parent = 0;
			meta.root_offset = alloc(&root);

			leaf_node_t leaf;
			leaf.next = leaf.prev = 0;
			leaf.parent = meta.root_offset;
			meta.first_leaf_offset = root.children[0].child = alloc(&leaf);

			unmap(&meta, OFFSET_META);
			unmap(&root, meta.root_offset);
			unmap(&leaf, root.children[0].child);
		}

		off_t search_index(const key_t &key) const {

			off_t now = meta.root_offset;
			size_t h = meta.height;
			while (h > 1) {
				internal_node_t node;
				map(&node, now);
				index_t *i = find(node, key);
				now = i->child;
				--h;
			}
			return now;
		}

		off_t search_leaf(off_t index, const key_t &key) const {

			internal_node_t node;
			map(&node, index);
			index_t *i = find(node, key);
			return i->child;
		}

		off_t search_leaf(const key_t &key) const {
			return search_leaf(search_index(key), key);
		}

		void remove_from_index(off_t offset, internal_node_t &node, const key_t &key) {

			size_t min_num = (meta.root_offset == offset) ? 1 : meta.order / 2;

			key_t index_key = begin(node)->key;
			index_t *to_delete = find(node, key); //find next key(need delete)
			if (to_delete != end(node)) {
				(to_delete + 1)->child = to_delete->child;  //change offset
				copy(to_delete + 1, end(node), to_delete);  //delete
			}
			node.num--;

			//root_need_delete
			if (node.num == 1 && meta.root_offset == offset && meta.internal_node_num != 1) {
				unalloc(&node, meta.root_offset);
				meta.height--;
				meta.root_offset = node.children[0].child;
				unmap(&meta, OFFSET_META);
				return;
			}

			//recursion_end
			if (node.num >= min_num) {
				unmap(&node, offset);
				return;
			}

			internal_node_t parent;
			map(&parent, node.parent);

			//first borrow
			bool borrowed = false;
			if (offset != begin(parent)->child)  //from left
				borrowed = borrow_key(false, node, offset);

			if (!borrowed && offset != (end(parent) - 1)->child)  //from right
				borrowed = borrow_key(true, node, offset);

			//finally merge again
			if (!borrowed) {

				//merge node to left
				if (offset == (end(parent) - 1)->child) {

					internal_node_t prev;
					map(&prev, node.prev);

					index_t *pos = find(parent, begin(prev)->key); //new key of merged index
					change_children_parent(begin(node), end(node), node.prev);
					merge_index(pos, prev, node);
					unmap(&prev, node.prev);
				}
				else {

					//merge right to node;
					internal_node_t nxt;
					map(&nxt, node.next);

					index_t *pos = find(parent, index_key);
					change_children_parent(begin(nxt), end(nxt), offset);
					merge_index(pos, node, nxt);
					unmap(&node, offset);

				}
				remove_from_index(node.parent, parent, index_key); //up

			}
			else
				unmap(&node, offset);
		}

		bool borrow_key(bool from_right, internal_node_t &to, off_t offset) {
			typedef typename internal_node_t::child_t child_t; //for convenience

			off_t from_offset = from_right ? to.next : to.prev;
			internal_node_t from;
			map(&from, from_offset);

			if (from.num != meta.order / 2) {
				child_t lend_pos, put_pos;
				internal_node_t parent;

				if (from_right) {
					lend_pos = begin(from);
					put_pos = end(to);
					map(&parent, to.parent);
					child_t pos = lower_find(parent, (end(to) - 1)->key);
					pos->key = lend_pos->key;
					unmap(&parent, to.parent);
				}
				else {
					lend_pos = end(from) - 1;
					put_pos = begin(to);
					map(&parent, from.parent);
					child_t pos = find(parent, begin(from)->key);
					put_pos->key = pos->key;
					pos->key = (lend_pos - 1)->key;
					unmap(&parent, from.parent);
				}

				copy_back(put_pos, end(to), end(to) + 1);
				*put_pos = *lend_pos;
				++to.num;

				change_children_parent(lend_pos, lend_pos + 1, offset);
				copy(lend_pos + 1, end(from), lend_pos);
				--from.num;
				unmap(&from, from_offset);
				return true;
			}

			return false;
		}

		bool borrow_key(bool from_right, leaf_node_t &to) {

			off_t from_offset = from_right ? to.next : to.prev; //borrow from where
			leaf_node_t from;
			map(&from, from_offset);

			if (from.num != meta.order / 2) {
				typename leaf_node_t::child_t lend_pos, put_pos;

				if (from_right) {
					lend_pos = begin(from);
					put_pos = end(to);
					change_parent_key(to.parent, begin(to)->key, from.children[1].key);  //change index
				}
				else {
					lend_pos = end(from) - 1;
					put_pos = begin(to);
					change_parent_key(from.parent, begin(from)->key, lend_pos->key);
				}

				copy_back(put_pos, end(to), end(to) + 1); //move borrower
				*put_pos = *lend_pos; //put
				++to.num;

				copy(lend_pos + 1, end(from), lend_pos); //move lender
				--from.num;
				unmap(&from, from_offset);
				return true;
			}
			return false;

		}

		void change_parent_key(off_t parent, const key_t &o, const key_t &n) {
			internal_node_t node;
			map(&node, parent);
			index_t *w = find(node, o);  //find next key(bigger than o)

			w->key = n;   //change key
			unmap(&node, parent);
			if (w == node.children + node.num - 1)  //borrower and lender have different parent
				change_parent_key(node.parent, o, n);  //change key recursively
		}

		void merge_leaf(leaf_node_t *left, leaf_node_t *right) {
			copy(begin(*right), end(*right), end(*left));
			left->num += right->num;
		}

		void merge_index(index_t *where, internal_node_t &left, internal_node_t &right) {
			copy(begin(right), end(right), end(left));
			left.num += right.num;
			node_remove(&left, &right);
		}

		void insert_record_without_split(leaf_node_t *leaf, const key_t &key,
			const value_t &value) {

			record_t *pos = upper_find(*leaf, key);
			copy_back(pos, end(*leaf), end(*leaf) + 1);
			pos->key = key;
			pos->value = value;
			++leaf->num;
		}

		void insert_key_to_index(off_t offset, const key_t &key, off_t oldchild, off_t newchild) {

			//root_need_split
			if (offset == 0) {
				internal_node_t root;
				root.next = root.prev = root.parent = 0;
				meta.root_offset = alloc(&root);
				root.num = 2;
				root.children[0].key = key;
				root.children[0].child = oldchild;
				root.children[1].child = newchild;
				++meta.height;

				unmap(&meta, OFFSET_META);
				unmap(&root, meta.root_offset);

				//update parent
				change_children_parent(begin(root), end(root), meta.root_offset);
				return;
			}

			internal_node_t node;
			map(&node, offset);

			//recursion_end
			if (node.num < meta.order) {
				insert_key_to_index_without_split(node, key, newchild);
				unmap(&node, offset);
				return;
			}

			//split
			internal_node_t new_node;
			node_create(offset, &node, &new_node);

			//split_evenly
			size_t point = (node.num - 1) / 2;
			bool _right = keycmp(key, node.children[point].key) > 0;
			if (_right)
				++point;

			//prevent middle key being in right
			if (_right && keycmp(key, node.children[point].key) < 0)
				--point;

			//middle key is left's max
			key_t middle_key = node.children[point].key;

			copy(begin(node) + point + 1, end(node), begin(new_node));
			new_node.num = node.num - point - 1;
			node.num = point + 1;

			if (_right)
				insert_key_to_index_without_split(new_node, key, newchild);
			else
				insert_key_to_index_without_split(node, key, newchild);

			unmap(&node, offset);
			unmap(&new_node, node.next);
			change_children_parent(begin(new_node), end(new_node), node.next);

			//give middle to parent
			insert_key_to_index(node.parent, middle_key, offset, node.next);
		}

		void insert_key_to_index_without_split(internal_node_t &node, const key_t &key,
			off_t newchild) {

			index_t *pos = find(node, key);
			copy_back(pos, end(node), end(node) + 1); //move;
			pos->key = key;
			pos->child = (pos + 1)->child;
			(pos + 1)->child = newchild;
			++node.num;

		}

		void change_children_parent(index_t *beg, index_t *end, off_t parent) {

			//because_children_array_write_in_the_end
			internal_node_t node;
			while (beg != end) {
				map(&node, beg->child);
				node.parent = parent;
				unmap(&node, beg->child, SIZE_NO_CHILDREN);
				++beg;
			}
		}

		template<class node_t>
		void node_create(off_t offset, node_t *node, node_t *next) {

			next->parent = node->parent;
			next->next = node->next;
			next->prev = offset;
			node->next = alloc(next);
			if (next->next != 0) {
				node_t old_next;
				map(&old_next, next->next, SIZE_NO_CHILDREN);
				old_next.prev = node->next;
				unmap(&old_next, next->next, SIZE_NO_CHILDREN);
			}
			unmap(&meta, OFFSET_META);
		}

		template<class node_t>
		void node_remove(node_t *prev, node_t *node) {
			unalloc(node, prev->next);
			prev->next = node->next;
			if (node->next != 0) {
				node_t next;
				map(&next, node->next, SIZE_NO_CHILDREN);
				next.prev = node->prev;
				unmap(&next, node->next, SIZE_NO_CHILDREN);
			}
			unmap(&meta, OFFSET_META);
		}

		void open_file(const char *mode = "rb+") const {
			if (fp_level == 0) {
				fp = fopen(path, mode);
				++fp_level;
			}
		}

		void close_file() const {
			if (fp_level == 1) {
				fclose(fp);
				--fp_level;
			}
		}

		off_t alloc(size_t size) {

			off_t slot = meta.slot;
			meta.slot += size;
			return slot;
		}

		off_t alloc(leaf_node_t *leaf) {

			leaf->num = 0;
			meta.leaf_node_num++;
			return alloc(sizeof(leaf_node_t));
		}

		off_t alloc(internal_node_t *node) {

			node->num = 1;
			meta.internal_node_num++;
			return alloc(sizeof(internal_node_t));
		}

		void unalloc(leaf_node_t *leaf, off_t offset) {

			--meta.leaf_node_num;
		}

		void unalloc(internal_node_t *node, off_t offset) {

			--meta.internal_node_num;
		}

		int map(void *block, off_t offset, size_t size) const {
			open_file();
			fseek(fp, offset, SEEK_SET);
			size_t rd = fread(block, size, 1, fp);
			fflush(fp);

			return rd - 1;
		}

		template<class T>
		int map(T *block, off_t offset) const {
			return map(block, offset, sizeof(T));
		}

		int unmap(void *block, off_t offset, size_t size) const {
			open_file();
			fseek(fp, offset, SEEK_SET);
			size_t wt = fwrite(block, size, 1, fp);
			fflush(fp);

			return wt - 1;
		}

		template<class T>
		int unmap(T *block, off_t offset) const {
			return unmap(block, offset, sizeof(T));
		}






};





#endif
