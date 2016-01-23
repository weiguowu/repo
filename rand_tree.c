#include <stdio.h>
#include <stdlib.h>
struct node_t{
	long int	num;
	long int	mem_size;
	void		*mem;
	long int 	child_num;	
	int			bvalid;
	long int	depth;			// depth of current node
	struct 	node_t** child;	// header of child list
};

typedef struct tree{
	long int depth;
	struct 	node_t *root;
}tree_t;

int node_size = sizeof(struct node_t);

#define NODE_DEPTH random()
#define NODE_NUM random()
#define MEM_SIZE random()
#define CHILD_NUM random()
static node_index = 0;

void print_space(unsigned int n)
{
	while(n--)
	{
		putchar(' ');	
		putchar(' ');
	}
}

struct node_t *create_node(long int cur_depth,long int tot_depth)
{
	struct node_t *node = NULL;
	struct node_t *child = NULL;
	long int index = 0;

	if(cur_depth == 0)
		return NULL;

	node = (struct node_t*)malloc(node_size);
	if(!node)
	{
		perror("Failed to create node!!!");
		return NULL;
	}

	cur_depth --;
	node->num = NODE_NUM;
	node->mem_size = MEM_SIZE;
	node->mem = NULL; // Alloc mem when we need
	node->depth = tot_depth - cur_depth;

	if(cur_depth)
	{
		node->child_num = CHILD_NUM;
		node->child = (struct node_t**)malloc(node->child_num * sizeof(struct node_t*));
		if(!node->child)
		{
			perror("Failed to create child list!!!");
			return NULL;
		}
		
		print_space(node->depth);
		printf("creating node depth:%ld,num:%ld,child count:%ld\n",node->depth,node->num,node->child_num);
		index = 0;
		while(index < node->child_num)
		{
			child = create_node(cur_depth,tot_depth);
			if(!child)
				return NULL;
			node->child[index] = child;
			index ++;
		}
	}
	else
	{
		node->child_num = 0;
		node->child = NULL;		
		print_space(node->depth);
		printf("creating node depth:%ld,num:%ld,child count:%ld\n",node->depth,node->num,node->child_num);
	}
	
	return node;
}
/*
* 0 means success,
* 1 failed
* -1 invalid parameter
*/
int create_tree(tree_t *ptree)
{
	if(!ptree)
		return -1;
	
	ptree->depth = NODE_DEPTH;
	printf("Begin to create a random tree, its depth is:%ld\n",ptree->depth);
	ptree->root = create_node(ptree->depth, ptree->depth);

	if(!ptree->root)
		return 1;
	else
		return 0;
}


/*
* 0 means success, other whise failed
*/
int insert(struct node_t *pnode)
{
	struct node_t *node = NULL, *tmp = NULL;
	long int index = 0;
	int ret = 0;
	
	if(!pnode)
		return 1;
	
	/*
	* insert for current node and its child node
	*/
	if((pnode->num%2) && pnode->child_num)
	{
		index = 0;
		while(index < pnode->child_num)
		{
			node = pnode->child[index];
			if(node->num%2)
			{
				tmp = (struct node_t*)malloc(node_size);
				if(!tmp)
				{
					perror("Failed to create node!!!");
					return 1;
				}
				
				tmp->num = 10; // set it as 0, since no particular requirement for the even num;
				tmp->mem_size = 0;
				tmp->child_num = 1;
				tmp->depth = node->depth;
				node->depth ++;
				tmp->child = (struct node_t **)malloc(tmp->child_num * sizeof(struct node_t*));
				if(!tmp->child)
				{
					perror("Failed to create child list for new node!!!");
					return 1;
				}
				
				tmp->child[0] = node;
				pnode->child[index] = tmp;
				
				printf("	Insert between parent num:%ld,child num:%ld,new node num:%ld\n",pnode->num,node->num,tmp->num);
			}
			index ++;
		}
	}
	/*
	* insert for child node
	*/
	if(pnode->child_num)
	{
		index = 0;
		while(index < pnode->child_num && ret == 0)
		{
			ret = insert(pnode->child[index]);
			index ++;
		}
	}
	
	return ret;
}
/*
* 0 means success, other wise failed
*/
int insert_node(tree_t *ptree)
{
	int ret = 0;
	
	if(!ptree)
	{
		printf("Invalid parameter!\n");
		return;
	}
	if(ptree->depth >= 2)
		ret = insert(ptree->root);
	
	return ret;
}

void traverse_node(struct node_t *pnode)
{
	struct node_t *node;
	long int index = 0;
	
	if(!pnode)
	{
		printf("node == NULL \n");
		return;
	}
	
	if(pnode->child_num > 0)
	{	
		print_space(pnode->depth);
		printf("(depth:%ld,num:%ld,child count:%ld)\n",pnode->depth,pnode->num,pnode->child_num);

		index = 0;
		while(index < pnode->child_num)			// child node
		{
			node = pnode->child[index];
			traverse_node(node);
			index ++;
		}
	}
	else
	{	
		print_space(pnode->depth);
		printf("(depth:%ld,num:%ld,child count:%ld)\n",pnode->depth,pnode->num,pnode->child_num);
	}
	
	return;
}
int traverse(tree_t *ptree)
{
	struct node_t *node = NULL, *tmp = NULL;
	long int index = 0;
	int ret = 0;
	
	if(!ptree)
		return -1;
	
	/*
	* print current node num
	*/
	if(!ptree->root) // means depth of this tree is 0
	{
		printf("Depth of this tree is 0\n");
		return 1;
	}
		
	printf("Traverse begin ... \n");
	traverse_node(ptree->root);
	printf("Traverse end. \n");
	
	return 0;
}


void destroy_node(struct node_t *pnode)
{
	struct node_t *node = NULL;
	long int index = 0;
	
	if(!pnode)
		return;
	
	index = 0;
	while(index < pnode->child_num)
	{	
		node = pnode->child[index];
		destroy_node(node);
		index ++;
	}
	if (pnode->mem)
	{
		free(node->mem);
		node->mem = NULL;
	}
	if(pnode->child)
	{
		free(pnode->child);
		pnode->child = NULL;
	}
	
	return;
}

void destroy_tree(tree_t *ptree)
{
	if(!ptree)
		return ;
	
	/*
	* 
	*/
	if(!ptree->root) // means depth of this tree is 0
		return;
	destroy_node(ptree->root);
	free(ptree->root);
	ptree->root = NULL;
	
	return ;
}

int main()
{
	tree_t r_tree;
	int ret = 0;
	
	ret = create_tree(&r_tree);
	if(ret)
	{
		printf("Failed to create the random tree!\n");
		goto DESTROY;
	}
	printf("\nA random tree is created. Traverse it now ...\n");
	traverse(&r_tree);
	
	printf("\nTry to insert nodes into it now ...\n");
	ret = insert_node(&r_tree);
	if(ret)
	{
		printf("Failed to insert nodes into random tree!\n");
		goto DESTROY;
	}
	printf("\nInsert nodes into random tree successfully, Traverse it now ...\n");
	traverse(&r_tree);
	
DESTROY:
	destroy_tree(&r_tree);
	printf("The random tree is destroyed.\n");
	
	return 0;
}

