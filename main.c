#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryTreeNode TreeNode;

struct BinaryTreeNode {
  int key;
  TreeNode *left;
  TreeNode *right;
  int leftCount;
  int rightCount;
};

typedef struct {
  TreeNode *root;
} BinaryTree;

TreeNode *create_node(int key) {
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->leftCount = 0;
  node->rightCount = 0;

  return node;
}

BinaryTree *create_b_tree(int key) {
  BinaryTree *tree = (BinaryTree *)malloc(sizeof(BinaryTree));
  tree->root = create_node(key);

  return tree;
}

void free_b_node(TreeNode *node) {
  if (node->left != NULL)
    free_b_node(node->left);
  if (node->right != NULL)
    free_b_node(node->right);

  free(node);
}

void free_b_tree(BinaryTree *tree) {

  free_b_node(tree->root);
  free(tree);
}

void insert_recursive_node(TreeNode *treeNode, TreeNode *node) {
  if (treeNode->key >= node->key) {
    if (treeNode->left != NULL) {
      return insert_recursive_node(treeNode->left, node);
    }

    treeNode->left = node;
  } else {
    if (treeNode->right != NULL) {
      return insert_recursive_node(treeNode->right, node);
    }

    treeNode->right = node;
  }
}

void print_repeated_str(char *s, int times) {
  for (int i = 0; i < times; ++i) {
    printf("%s", s);
  }
}

void print_node_recursive(TreeNode *node, int space) {
  if (node == NULL) {
    return;
  }

  space += 10;
  print_node_recursive(node->right, space);
  printf("\n");
  print_repeated_str(" ", space);
  printf("%d\n", node->key);
  print_node_recursive(node->left, space);
}

void print_tree(BinaryTree *tree) {
  return print_node_recursive(tree->root, -10);
}

void swap_nodes_addr(TreeNode *node, TreeNode *o, TreeNode *r) {
  if (node == NULL) {
    return;
  }

  if (node->right == o) {
    node->right = r;
    return;
  }

  if (node->left == o) {
    node->left = r;
    return;
  }

  if (node->left != NULL) {
    swap_nodes_addr(node->left, o, r);
  }

  if (node->right != NULL) {
    swap_nodes_addr(node->right, o, r);
  }
}

void swap_nodes(TreeNode *a, TreeNode *b) {
  TreeNode tmp = *a;

  *a = *b;
  *b = tmp;

  swap_nodes_addr(a, a, b);
}

TreeNode *right_rotate_node(BinaryTree *tree, TreeNode *node) {
  if (node->right == NULL) {
    return NULL;
  }

  TreeNode *highest = node;

  node = node->right;
  highest->right = NULL;

  insert_recursive_node(node, highest);

  if (tree->root == highest) {
    tree->root = node;
    return tree->root;
  }
  swap_nodes(highest, node);
  return highest;
}

TreeNode *left_rotate_node(BinaryTree *tree, TreeNode *node) {
  if (node->left == NULL) {
    return NULL;
  }

  TreeNode *highest = node;

  node = node->left;
  highest->left = NULL;

  insert_recursive_node(node, highest);

  if (tree->root == highest) {
    tree->root = node;
    return tree->root;
  } else {
    swap_nodes(highest, node);
    return highest;
  }
}

/// states:
/// 0 забаланшено
/// 1 правая ветка больше левой
/// 2 левая больше правой
/// 3 не существует ветки
int get_node_balanced_state(TreeNode *node) {
  if (node == NULL) {
    return 3;
  }

  int diff = node->leftCount - node->rightCount;

  if (diff > 1) {
    return 1;
  } else if (diff < -1) {
    return 2;
  } else {
    return 0;
  }
}

int calculate_node_counts(TreeNode *node, int count) {
  if (node->right != NULL) {
    count += 1;
    count = calculate_node_counts(node->right, count);
  }

  if (node->left != NULL) {
    count += 1;
    count = calculate_node_counts(node->left, count);
  }

  return count;
}

void recalculate_node_counts(TreeNode *node) {
  if (node == NULL) {
    return;
  }

  int ln = node->left != NULL ? calculate_node_counts(node->left, 1) : 0;
  int rn = node->right != NULL ? calculate_node_counts(node->right, 1) : 0;

  node->leftCount = ln;
  node->rightCount = rn;

  recalculate_node_counts(node->left);
  recalculate_node_counts(node->right);
}

void recalculate_tree_counts(BinaryTree *tree) {
  return recalculate_node_counts(tree->root);
}

TreeNode *balance_node(BinaryTree *tree, TreeNode *node) {
  int state = get_node_balanced_state(node);

  if (state == 0) {
    return node;
  } else if (state == 1) {
    return left_rotate_node(tree, node);
  } else if (state == 2) {
    return right_rotate_node(tree, node);
  } else {
    return NULL;
  }
}

void balance_nodes_recursive(BinaryTree *tree, TreeNode *node) {
  if (node == NULL) {
    return;
  }

  TreeNode *highest = balance_node(tree, node);
  recalculate_tree_counts(tree);
  int balanced_state = get_node_balanced_state(highest);

  while (!(balanced_state == 0 || balanced_state == 3)) {
    highest = balance_node(tree, highest);
    recalculate_tree_counts(tree);

    balanced_state = get_node_balanced_state(highest);
  }

  if (highest != NULL && highest->left != NULL) {
    balance_nodes_recursive(tree, highest->left);
  }

  if (highest != NULL && highest->right != NULL) {
    balance_nodes_recursive(tree, highest->right);
  }
}

void balance_tree(BinaryTree *tree) {
  balance_nodes_recursive(tree, tree->root);
}

void insert_node(BinaryTree *tree, TreeNode *node) {
  insert_recursive_node(tree->root, node);
  recalculate_tree_counts(tree);
}

int main() {
  BinaryTree *tree = create_b_tree(42);
  insert_node(tree, create_node(63));
  insert_node(tree, create_node(86));
  insert_node(tree, create_node(51));
  insert_node(tree, create_node(40));
  insert_node(tree, create_node(3));
  insert_node(tree, create_node(74));
  insert_node(tree, create_node(77));

  print_tree(tree);
  printf("\n\n\n=============================================================="
         "\n\n\n");
  balance_tree(tree);
  print_tree(tree);

  free_b_tree(tree);

  return 0;
}
