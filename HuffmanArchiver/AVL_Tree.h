#pragma once
#include <string>
#include "PriorityQueue.h"
using namespace std;

namespace spaceAVL_Tree
{
	template <class T>
	class AVL_Tree
	{
		struct TreeNode
		{
			T data;
			int height;
			TreeNode *left;
			TreeNode *right;
			TreeNode *parent;
			TreeNode(T _data, TreeNode *_parent) : data(_data), height(1), left(nullptr), right(nullptr), parent(_parent) {}
		};
		TreeNode *main_root;

	public:
		AVL_Tree()
		{
			this->clear(this->main_root);
			this->main_root = nullptr;
		}
		~AVL_Tree()
		{
			this->clear(this->main_root);
			this->main_root = nullptr;
		}

		bool IsEmpty()
		{
			return (this->main_root ? false : true);
		}
		bool Insert(T data)
		{
			if (this->main_root == nullptr)
			{
				this->main_root = new TreeNode(data, nullptr);
				return false;
			}
			TreeNode *root = main_root;
			while (true)
			{
				if (data > root->data)
				{
					if (root->right != nullptr) root = root->right;
					else { root->right = new TreeNode(data, root); break; }
				}
				else if (data < root->data)
				{
					if (root->left != nullptr) root = root->left;
					else { root->left = new TreeNode(data, root); break; }
				}
				else if (data == root->data) { ++(root->data); return true; }
			}
			while (root)
			{
				root->height = 1 + max(Height(root->left), Height(root->right));
				int balance = Balance(root);
					 if (balance >  1 && Balance(root->left)  >= 0) R_rot(root);
				else if (balance >  1 && Balance(root->left)  <  0) LR_rot(root);
				else if (balance < -1 && Balance(root->right) <= 0) L_rot(root);
				else if (balance < -1 && Balance(root->right) >  0) RL_rot(root);
				root = root->parent;
			}
			return false;
		}
		void DeleteNode(T data)
		{
			if (this->main_root == nullptr) return;
			TreeNode *root = this->main_root;
			while (root && root->data != data)
			{
				if (data < root->data) root = root->left;
				else if (data > root->data) root = root->right;
			}
			if (!root) return;
			if ((root->left == nullptr) || (root->right == nullptr))
			{
				TreeNode *temp = (root->left ? root->left : root->right);
				if (temp == nullptr)
				{
					temp = root;
					root = root->parent;
					if (root->left == temp) root->left = nullptr;
					else if (root->right == temp) root->right = nullptr;
					delete temp;
				}
				else
				{
					temp->parent = root->parent;
					if (temp->parent)
						if (temp->parent->left == root) temp->parent->left = temp;
						else if (temp->parent->right == root) temp->parent->right = temp;
					delete root;
					root = temp->parent;
				}
			}
			else
			{
				TreeNode *temp = root->right;
				while (temp->left != nullptr) temp = temp->left;

				root->data = temp->data;
				root->right = temp->right;
				if (temp->right) temp->right->parent = root;
				delete temp;
			}
			while (root)
			{
				root->height = 1 + max(Height(root->left), Height(root->right));
				int balance = Balance(root);
					 if (balance >  1 && Balance(root->left)  >= 0) R_rot(root);
				else if (balance >  1 && Balance(root->left)  <  0) LR_rot(root);
				else if (balance < -1 && Balance(root->right) <= 0) L_rot(root);
				else if (balance < -1 && Balance(root->right) >  0) RL_rot(root);
				root = root->parent;
			}
		}
		T FindVal(T data)
		{
			TreeNode *p = this->main_root;
			while (p && p->data != data)
			{
				if (p->data < data) p = p->right;
				else if (p->data > data) p = p->left;
			}
			if (!p) return T();
			return p->data;
		}
		void ReplaceVal(T old_val, T new_val)
		{
			TreeNode *p = this->main_root;
			while (p && p->data != old_val)
			{
				if (p->data < old_val) p = p->right;
				else if (p->data > old_val) p = p->left;
			}
			if (!p) return;
			p->data = new_val;
		}
		spaceArray::Array<T> ReturnAllVals()
		{
			spaceArray::Array<T> ar;
			returnallvals(this->main_root, ar);
			return ar;
		}

	private:
		void clear(TreeNode *root)
		{
			if (!root) return;
			clear(root->left);
			clear(root->right);
			delete root;
		}
		void returnallvals(TreeNode *node, spaceArray::Array<T> &ar)
		{
			if (!node) return;
			ar.Push_back(node->data);
			returnallvals(node->left, ar);
			returnallvals(node->right, ar);
		}

		inline int max(int a, int b) 
			{ return (a > b) ? a : b; }
		inline int Balance(TreeNode *&root)
		{
			return ((root->left) ? root->left->height : 0) - ((root->right) ? root->right->height : 0);
		}
		inline int Height(TreeNode *&root)
		{
			return (root ? root->height : 0);
		}
		inline void L_rot(TreeNode *&p1)
		{
			TreeNode *p2 = p1->right;
			if (p1->parent)
				if (p1->parent->left == p1) p1->parent->left = p2;
				else if (p1->parent->right == p1) p1->parent->right = p2;
			p2->parent = p1->parent;

			p1->right = p2->left;
			if (p2->left) p2->left->parent = p1;
			
			p2->left = p1;
			p1->parent = p2;

			p1->height = max(Height(p1->left), Height(p1->right)) + 1;
			p2->height = max(Height(p2->left), Height(p2->right)) + 1;
			if (p1 == this->main_root) this->main_root = p2;
			p1 = p2;
		}
		inline void R_rot(TreeNode *&p1)
		{
			TreeNode *p2 = p1->left;
			if (p1->parent)
				if (p1->parent->left == p1) p1->parent->left = p2;
				else if (p1->parent->right == p1) p1->parent->right = p2;
			p2->parent = p1->parent;

			p1->left = p2->right;
			if (p2->right) p2->right->parent = p1;

			p2->right = p1;
			p1->parent = p2;

			p1->height = max(Height(p1->left), Height(p1->right)) + 1;
			p2->height = max(Height(p2->left), Height(p2->right)) + 1;
			if (p1 == this->main_root) this->main_root = p2;
			p1 = p2;
		}
		inline void LR_rot(TreeNode *&p1)
		{
			TreeNode *p2 = p1->left;
			TreeNode *p3 = p2->right;

			p2->right = p3->left;
			if (p3->left) p3->left->parent = p2;

			p3->left = p2;
			p2->parent = p3;

			p1->left = p3->right;
			if (p3->right) p3->right->parent = p1;

			p3->parent = p1->parent;
			p3->right = p1;
			p1->parent = p3;

			if (p3->parent)
				if (p3->parent->left == p1) p3->parent->left = p3;
				else if (p3->parent->right == p1) p3->parent->right = p3;

			p1->height = max(Height(p1->left), Height(p1->right)) + 1;
			p2->height = max(Height(p2->left), Height(p2->right)) + 1;
			p3->height = max(Height(p3->left), Height(p3->right)) + 1;
			if (p1 == this->main_root) this->main_root = p3;
			p1 = p3;
		}
		inline void RL_rot(TreeNode *&p1)
		{
			TreeNode *p2 = p1->right;
			TreeNode *p3 = p2->left;

			p2->left = p3->right;
			if (p3->right) p3->right->parent = p2;

			p3->right = p2;
			p2->parent = p3;

			p1->right = p3->left;
			if (p3->left) p3->left->parent = p1;

			p3->parent = p1->parent;
			p3->left = p1;
			p1->parent = p3;

			if (p3->parent)
				if (p3->parent->left == p1) p3->parent->left = p3;
				else if (p3->parent->right == p1) p3->parent->right = p3;

			p1->height = max(Height(p1->left), Height(p1->right)) + 1;
			p2->height = max(Height(p2->left), Height(p2->right)) + 1;
			p3->height = max(Height(p3->left), Height(p3->right)) + 1;
			if (p1 == this->main_root) this->main_root = p3;
			p1 = p3;
		}
	};
}