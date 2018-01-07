#include "AVL_Tree.h"

namespace spaceAVL_Tree
{
	inline int max(int a, int b) { return (a > b) ? a : b; }

	template<class S>
	AVL_Tree<S>::AVL_Tree()
	{
		this->main_root = nullptr;
	}

	template<class S>
	AVL_Tree<S>::~AVL_Tree()
	{
		clear(this->main_root);
		this->main_root = nullptr;
	}

	template<class S>
	void AVL_Tree<S>::Insert(S data)
	{
		insert(data, this->main_root);
	}

	template<class S>
	void AVL_Tree<S>::DeleteNode(S data)
	{
		deleteNode(data, this->main_root);
	}


	template<class S>
	void AVL_Tree<S>::insert(S _data, TreeNode *&root)
	{
		if (!root)
		{
			root = new TreeNode(_data);
			return;
		}
		if (_data > root->data)
		{
			if (root->right != nullptr)	insert(_data, root->right);
			else root->right = new TreeNode(_data);
		}
		else if (_data < root->data)
		{
			if (root->left != nullptr) insert(_data, root->left);
			else root->left = new TreeNode(_data);
		}
		else if (_data == root->data) { return; }

		root->height = (Height(root->left) > Height(root->right) ? Height(root->left) : Height(root->right)) + 1;
		int balance = Balance(root);

		if (balance > 1 && _data < root->left->data) R_rot(root);
		else if (balance < -1 && _data > root->right->data) L_rot(root);
		else if (balance > 1 && _data > root->left->data) LR_rot(root);
		else if (balance < -1 && _data < root->right->data) RL_rot(root);
	}

	template<class S>
	void AVL_Tree<S>::deleteNode(S _data, TreeNode *&root)
	{
		if (!root) return;
		if (_data < root->data) deleteNode(_data, root->left);
		else if (_data > root->data) deleteNode(_data, root->right);
		else
		{
			if ((root->left == nullptr) || (root->right == nullptr))
			{
				TreeNode *temp = (root->left ? root->left : root->right);
				if (temp == nullptr)
				{
					temp = root;
					root = nullptr;
				}
				else *root = *temp;
				delete temp;
			}
			else
			{
				TreeNode *temp = root->right;
				while (temp->left != nullptr) temp = temp->left;
				root->data = temp->data;
				deleteNode(temp->data, root->right);
			}
		}
		if (root == nullptr) return;

		root->height = 1 + max(Height(root->left), Height(root->right));
		int balance = Balance(root);

		// Left Left
		if (balance > 1 && Balance(root->left) >= 0) { R_rot(root); return; }
		// Left Right
		if (balance > 1 && Balance(root->left) < 0) { LR_rot(root); return; }
		// Right Right
		if (balance < -1 && Balance(root->right) <= 0) { L_rot(root); return; }
		// Right Left
		if (balance < -1 && Balance(root->right) > 0) { RL_rot(root); return; }
	}

	template<class S>
	void AVL_Tree<S>::clear(TreeNode *root)
	{
		if (!root) return;
		clear(root->left);
		clear(root->right);
		delete root;
	}

	template<class S>
	inline int AVL_Tree<S>::Balance(TreeNode *&root)
	{
		return ((root->left) ? root->left->height : 0) - ((root->right) ? root->right->height : 0);
	}

	template<class S>
	inline int AVL_Tree<S>::Height(TreeNode *&root)
	{
		return (root ? root->height : 0);
	}

	template<class S>
	inline void AVL_Tree<S>::L_rot(TreeNode *&root)
	{
		TreeNode *p = root->right;
		root->right = p->left;
		p->left = root;
		root->height = max(Height(root->left), Height(root->right)) + 1;
		root = p;
		root->height = max(Height(root->left), Height(root->right)) + 1;
	}

	template<class S>
	inline void AVL_Tree<S>::R_rot(TreeNode *&root)
	{
		TreeNode *p = root->left;
		root->left = p->right;
		p->right = root;
		root->height = max(Height(root->left), Height(root->right)) + 1;
		root = p;
		root->height = max(Height(root->left), Height(root->right)) + 1;
	}

	template<class S>
	inline void AVL_Tree<S>::LR_rot(TreeNode *&root)
	{
		TreeNode *p1 = root->left;
		TreeNode *p2 = p1->right;
		p1->right = p2->left;
		root->left = p2;
		p2->left = p1;
		root->left = p2->right;
		p2->right = root;
		p1->height = max(Height(p1->left), Height(p1->right)) + 1;
		root->height = max(Height(root->left), Height(root->right)) + 1;
		p2->height = max(Height(p2->left), Height(p2->right)) + 1;
		root = p2;
	}

	template<class S>
	inline void AVL_Tree<S>::RL_rot(TreeNode *&root)
	{
		TreeNode *p1 = root->right;
		TreeNode *p2 = p1->left;
		p1->left = p2->right;
		root->right = p2;
		p2->right = p1;
		root->right = p2->left;
		p2->left = root;
		p2->height = max(Height(p2->left), Height(p2->right)) + 1;
		root->height = max(Height(root->left), Height(root->right)) + 1;
		p1->height = max(Height(p1->left), Height(p1->right)) + 1;
		root = p2;
	}
}




