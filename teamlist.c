#include "team.h"
#include "teamlist.h"
#include <stdlib.h>

struct node {
    Team *team;
    struct node *next;
    struct node *prev;
};

struct teamlist {
    struct node *first;
    struct node *last;
    int size;
};

TeamList *tl_create(void) {
    TeamList *tl = malloc(sizeof(*tl));

    if (tl == NULL)
        return NULL;

    tl->first = NULL;
    tl->last = NULL;
    tl->size = 0;

    return tl;
}

void tl_append(TeamList *tl, Team *t) {
    struct node *new_node;

    if(t == NULL)
        return;

    new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        team_free(t);
        return;
    }
    
    new_node->team = t;
    new_node->next = NULL;
    new_node->prev = tl->last;    
    
    if (tl->size == 0) 
        tl->first = new_node;
    else 
        tl->last->next = new_node;

    tl->last = new_node;
    tl->size++;    
}

int team_comes_before(Team *a, Team *b) {
    int pg_a = 3 * team_wins(a) + team_draws(a);
    int pg_b = 3 * team_wins(b) + team_draws(b);
    if (pg_a != pg_b)
        return pg_a > pg_b;

    if (team_wins(a) != team_wins(b))
        return team_wins(a) > team_wins(b);

    int sg_a = team_goals_scored(a) - team_goals_conceded(a);
    int sg_b = team_goals_scored(b) - team_goals_conceded(b);
    if (sg_a != sg_b)
        return sg_a > sg_b;

    if (team_goals_scored(a) != team_goals_scored(b))
        return team_goals_scored(a) > team_goals_scored(b);

    return team_id(a) < team_id(b);
}

void list_append_ordered(TeamList *l, Team *t) {
    struct node *new_node = malloc(sizeof(*new_node));

    if (l == NULL || t == NULL)
        return;

    if (new_node == NULL) {
        return;
    }
    new_node->team = t;
    
    // Caso 1: Lista vazia
    if (l->size == 0) {
        new_node->prev = NULL;
        new_node->next = NULL;
        l->first = new_node;
        l->last = new_node;
        l->size++;
        return;
    }

    // Procura o primeiro nó que seja "menor" que o new_node elemento
    struct node *current = l->first;
    while (current != NULL && team_comes_before(current->team, t)) {
        current = current->next;
    }

    // Caso 2: Inserir no início (new_node valor é maior que o primeiro)
    if (current == l->first) {
        new_node->prev = NULL;
        new_node->next = l->first;
        l->first->prev = new_node;
        l->first = new_node;
    }
    // Caso 3: Inserir no final (chegou ao fim sem achar ninguém menor)
    else if (current == NULL) {
        new_node->next = NULL;
        new_node->prev = l->last;
        l->last->next = new_node;
        l->last = new_node;
    }
    // Caso 4: Inserir no meio (logo antes do nó 'current')
    else {
        new_node->next = current;
        new_node->prev = current->prev;
        current->prev->next = new_node;
        current->prev = new_node;
    }

    l->size++;
}


int tl_size(TeamList *tl) {
    return tl != NULL ? tl->size : 0;
}

Team *tl_get(TeamList *tl, int i) {
    struct node *current;
    int position;

    if (tl == NULL || i < 0 || i >= tl->size)
        return NULL;

    current = tl->first;
    for(position = 0; position < i; position++)
     current = current->next;

    return current->team;
}

Team *bestAttack(TeamList *tl){
    if (tl == NULL || tl->first == NULL)
        return NULL;

    Team *t = tl->first->team;
    int best = team_goals_scored(t);

    struct node *node = tl->first->next;
    while(node != NULL) {
        Team *current = node->team;
        int gols = team_goals_scored(current);
        if (gols > best) {
            t = current;
            best = gols;
        }

        node = node->next;
    }

    return t;
}

Team *bestDefense(TeamList *tl){
    if (tl == NULL || tl->first == NULL)
        return NULL;

    Team *t = tl->first->team;
    int best = team_goals_conceded(t);

    struct node *node = tl->first->next;
    while(node != NULL) {
        Team *current = node->team;
        int gols = team_goals_conceded(current);
        if (gols < best) {
            t = current;
            best = gols;
        }
        node = node->next;
    }

    return t;
}


void tl_free(TeamList *tl) {
    struct node *next;
    struct node *current;
    int i;
    
    if (tl == NULL)
        return;

    current = tl->first;
    for(i = 0; i < tl->size; i++){
        next = current->next;
        team_free(current->team);
        free(current);
        current = next;
    };

    free(tl);
}

void tl_free_nodes(TeamList *tl) {
    struct node *next;
    struct node *current;
    int i;
    
    if (tl == NULL)
        return;

    current = tl->first;
    for(i = 0; i < tl->size; i++){
        next = current->next;
        free(current);
        current = next;
    };

    free(tl);
}