#include "minishell.h"

/*
Return allocated head of linked list
*/
t_lsttoken **tokenlst_memalloc(void)
{
    t_lsttoken **head;

    head = malloc(sizeof(t_lsttoken *));
    if (head == NULL)
    {
        perror("Error malloc");
        return (NULL);
    }
    *head = NULL;
    return (head);
}

/*
Allocate new node, assign token and attach to end of linked list
*/
int tokenlst_addtoken(t_lsttoken **head, char *token)
{
    t_lsttoken *node_new;
    t_lsttoken *node_current;

    node_new = malloc(sizeof(t_lsttoken));
    if (node_new == NULL)
    {
        perror("Error malloc");
        return (1);
    }
    node_new->token = token;
    node_new->next = NULL;
    if (*head == NULL)
        *head = node_new;
    else
    {
        node_current = *head;
        while (node_current->next != NULL)
            node_current = node_current->next;
        node_current->next = node_new;
    }
    return (0);
}

/*
Free and nullify all tokens and nodes of linked list
*/
void tokenlst_memfreelist(t_lsttoken **head)
{
    t_lsttoken *node_current;
    t_lsttoken *node_free;

    node_current = *head;
    while(node_current != NULL)
    {
        node_free = node_current;
        node_current = node_current->next;
        free(node_free->token);
        node_free->token = NULL;
        free(node_free);
        node_free = NULL;
    }
    *head = NULL;
}

