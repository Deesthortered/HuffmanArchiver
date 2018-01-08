#pragma once

namespace spaceAVL_Tree
{
	template<class S>
	class AVL_Tree
	{
		struct TreeNode
		{
			S data;
			size_t cnt; // Special for this program
			int height;
			TreeNode *left;
			TreeNode *right;
			TreeNode(S _data): data(_data), height(1), cnt(1), left(nullptr), right(nullptr) {}
		};
		TreeNode *main_root;

	public:
		AVL_Tree();
		~AVL_Tree();
		void Insert(S);
		void DeleteNode(S);
		bool IsEmpty();

		// Special for this program
		S GetRootVal();
		size_t GetRootCount();
		void DeleteRoot();
		//

	private:
		void insert(S, TreeNode *&);
		void deleteNode(S, TreeNode *&);
		void clear(TreeNode *);

		inline int Balance(TreeNode *&);
		inline int Height(TreeNode *&);
		inline void L_rot(TreeNode *&);
		inline void R_rot(TreeNode *&);
		inline void LR_rot(TreeNode *&);
		inline void RL_rot(TreeNode *&);
	};
}