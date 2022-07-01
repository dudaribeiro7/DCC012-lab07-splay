#include "splay.h"
/*
    Implement the SplayNode class
 */
SplayNode::SplayNode(int key): key(key), left(nullptr), right(nullptr), parent(nullptr)
{ }

SplayNode::SplayNode(int key, SplayNode *left, SplayNode *right): key(key), left(left), right(right), parent(nullptr)
{ }

int SplayNode::getKey()
{
    return key;
}

SplayNode* SplayNode::getLeft()
{
    return left;
}

SplayNode* SplayNode::getRight()
{
    return right;
}

SplayNode* SplayNode::getParent()
{
    return parent;
}

void SplayNode::setLeft(SplayNode *left)
{
    this->left = left;
}

void SplayNode::setRight(SplayNode *right)
{
    this->right = right;
}

void SplayNode::setParent(SplayNode *parent)
{
    this->parent = parent;
}

void SplayNode::setKey(int key)
{
    this->key = key;
}

/*
   Implement the SplayTree class
*/

// Cria um SplayTree vazia
SplayTree::SplayTree(): root(nullptr)
{ }

// Implementação da rotina de Rotação à Esquerda
/*
       Y                             X     
      / \        rotate left        / \    
     X   ^      <------------      ^   Y   
    / \ /C\                       /A\ / \  
   ^  ^                               ^  ^ 
  /A\/B\                             /B\/C\
  */
SplayNode* SplayTree::rotateLeft(SplayNode *x)
{
    // o nó x é o pai do nó que se deseja subir
    // o nó y é o nó que se deseja subir e que será retornado
    SplayNode *y = x->right;
    if(y == nullptr)
        return y;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;  
    return y; 
}

/*
    Implementação da rotina de Rotação à Direita
       Y                             X     
      / \                           / \    
     X   ^                         ^   Y   
    / \ /C\                       /A\ / \  
   ^  ^         ------------>         ^  ^ 
  /A\/B\         rotate right        /B\/C\

*/ 
SplayNode* SplayTree::rotateRight(SplayNode *y)
{
    // o nó y é o pai do nó que se deseja subir
    // o nó x é o nó que se deseja subir e que será retornado
    SplayNode *x = y->left;
    if(x == nullptr)
        return x;
    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;  
    x->right = y;
    y->parent = x;
    return x;
}

// TODO: OK Implemente a rotina de splay. 
// O parâmetro node é o nó que deve ser splayado.
void SplayTree::splay(SplayNode *node)
{
    while(node != root)
    {
        if(node->getParent() == root)
        {
            // ZIG: o pai de node é a raiz, rotação em torno da raiz para trazer node para a raiz

            if(node == root->getLeft())
            {
                // LEFT CASE: node é o filho à esquerda da raiz 
                // aplica rotação à direita:
                node = rotateRight(root);
            }
            else
            {
                // RIGHT CASE: node é o filho à direita da raiz
                // aplica rotação à esquerda:
                node = rotateLeft(root);
            }
        }
        else
        {
            SplayNode *p = node->getParent(); // p é o pai de node
            SplayNode *g = p->getParent();    // g é o pai de p, ou seja, avô de node

            if(((node == p->getLeft()) && (p == g->getLeft())) || ((node == p->getRight()) && (p == g->getRight())))
            {
                // ZIG-ZIG: rotação em torno de g e depois em torno de p para subir node 2 niveis

                if((node == p->getLeft()) && (p == g->getLeft()))
                {
                    // LEFT LEFT CASE: node é o filho esquerdo de um filho esquerdo
                    // aplica rotações à direita:
                    p = rotateRight(g);
                    node = rotateRight(p);
                }
                else
                {
                    // RIGHT RIGHT CASE: node é o filho direito de um filho direito
                    // aplica rotações à esquerda:
                    p = rotateLeft(g);
                    node = rotateLeft(p);
                }
            }
            else
            {
                // ZIG-ZAG: rotação em torno de p e depois em torno de g para subir node 2 niveis

                if((node == p->getRight()) && (p == g->getLeft()))
                {
                    // LEFT RIGHT CASE: node é o filho direito de um filho esquerdo
                    // aplica rotação à esquerda e depois à direita:
                    node = rotateLeft(p);
                    node = rotateRight(g);
                }
                else
                {
                    // RIGHT LEFT CASE: node é o filho esquerdo de um filho direito
                    // aplica rotação à direita e depois à esquerda:
                    node = rotateRight(p);
                    node = rotateLeft(g);
                }
            }
        }
    }
}

SplayNode* SplayTree::find(int key)
{
    SplayNode *node = root;
    SplayNode *parent = nullptr;
    while (node != nullptr && node->getKey() != key)
    {
        parent = node;
        if (key < node->getKey())
            node = node->getLeft();
        else
            node = node->getRight();
    }
    // TODO: OK chame a rotina de splay para o no encontrado a raiz. 
    // Se no for null, eleve o pai.
    if(node == nullptr)
        splay(node->getParent());
    else
        splay(node);

    return node;
}

void SplayTree::insert(int key)
{
    SplayNode *node = new SplayNode(key);
    SplayNode *x    = root;
    SplayNode *y    = nullptr;
    while(x != nullptr)
    {
        y = x;
        if (key < x->getKey())
            x = x->getLeft();
        else
            x = x->getRight();
    }
    node->setParent(y);
    if (y == nullptr)
        root = node;
    else if (key < y->getKey())
        y->setLeft(node);
    else
        y->setRight(node);

    // TODO: OK chame a rotina de splay para elevar o node ao nível da raiz
    splay(node);
}

// Retorna o nó que contem a maior chave
SplayNode* SplayTree::max()
{
    SplayNode *node = root;
    if(node == nullptr)
        return nullptr;
    while (node->right != nullptr)
        node = node->right;
    return node;
}

// Retorna o nó que contem a menor chave
SplayNode* SplayTree::min()
{
    SplayNode *node = root;
    if(node == nullptr)
        return nullptr;
    while (node->left != nullptr)
        node = node->left;
    return node;
}

void SplayTree::remove(int key)
{
    // TODO: OK Verifique a implementação da rotina de remoção

    SplayNode *node = find(key);

    // nó com chave key não foi encontrado:
    if (node == nullptr) 
        return;
    if(node->getKey() != key)
        return;

    // 1- eleva-se o nó de chave key até a raiz:
    splay(node);
    
    // 2- aponta-se as subasrvores à esquerda e à direita para as variáveis r1 e r2, respectivamente:
    SplayNode* r1 = node->getLeft();
    SplayNode* r2 = node->getRight();

    // 3- se r1 for nullptr, a nova raiz é r2:
    if(r1 == nullptr)
        root = r2;
    else
    {
        // 4.1- obtém-se o nó de maior chave de r1:
        while(r1->getRight() != nullptr)
            r1 = r1->getRight();

        // 4.2- eleva-se esse nó para a raiz de r1:
        splay(r1);

        // 4.3- r2 é o novo filho à direita de r1:
        r1->setRight(r2);
        if(r2 != nullptr)
            r2->setParent(r1);

        // 4.3- r1 é a nova raiz:
        root = r1;
    }
    // 5- deleta o nó com chave key:
    delete node;
}

void SplayTree::print_tree(SplayNode* node, const std::string& prefix, bool isLeft, std::ostream& out)
{
    if(node == nullptr)
        return;
    
    out << prefix;
    out << (isLeft ? "├──" : "└──" );

    // print the value of the node
    out <<"["<<node->getKey()<<"]" << std::endl;

    print_tree(node->getLeft() , prefix + (isLeft ? "│   " : "    "), true, out);
    print_tree(node->getRight(), prefix + (isLeft ? "│   " : "    "), false, out);
}

void SplayTree::print_in_order(SplayNode* node, std::ostream& out)
{
    if(node == nullptr)
        return;
    print_in_order(node->left, out);
    out << node->key << " ";
    print_in_order(node->right, out);
}


void SplayTree::print_in_order()
{
    print_in_order(root, std::cout);
    std::cout << std::endl;
}

void SplayTree::print_tree()
{
    print_tree(root, "", true, std::cout);
    std::cout << std::endl;
}

void SplayTree::deallocateMemory(SplayNode *node)
{
    if (node == nullptr)
    {
        return;
    }
    deallocateMemory(node->getLeft());
    deallocateMemory(node->getRight());
    delete node;
}

SplayTree::~SplayTree()
{
    deallocateMemory(root);
    root = nullptr;
}