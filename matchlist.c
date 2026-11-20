#include "matchlist.h"
#include "match.h"
#include <float.h>
#include <stdlib.h>

struct node {
    Match *match;
    struct node *next;
    struct node *prev;
};

struct matchlist {
    struct node *first;
    struct node *last;
    int size;
};

MatchList *ml_create(void) {
    MatchList *ml = malloc(sizeof(*ml));

    if (ml == NULL) 
        return NULL;

    ml->first = NULL;
    ml->last = NULL;
    ml->size = 0;

    return ml;
}

void ml_append(MatchList *ml, Match *m) {
  struct node *new_node;

  if (ml == NULL || m == NULL)
      return;

  new_node = malloc(sizeof(*new_node));
  if (new_node == NULL)
      return;

  new_node->match = m;
  new_node->next = NULL;
  new_node->prev = ml->last;

  if (ml->size > 0) 
      ml->last->next = new_node;
  else
      ml->first = new_node;

  ml->last = new_node;
  ml->size++;
}

int ml_size(MatchList *ml){
    return ml != NULL ? ml->size : 0;
}

Match *ml_get(MatchList *ml, int i){
    if (ml == NULL || i < 0 || i >= ml->size)
        return NULL;

    struct node *current = ml->first;
    for (int j = 0; j < i; j++)
        current = current->next;
    
    return current->match;
}

Match *BiggestVictory(MatchList *ml) {
    if (ml == NULL || ml->first == NULL)
        return NULL;

    Match *m = ml->first->match;
    int biggest = abs(match_golsT1(m) - match_golsT2(m));

    struct node *node = ml->first->next;
    while (node != NULL) {
        Match *current = node->match;
        int difference = abs(match_golsT1(current) - match_golsT2(current));

        if (difference > biggest) {
            m = current;
            biggest = difference;
        }

        node = node->next;
    }

    return m;
}

void ml_free(MatchList *ml) {
    if (ml == NULL)
        return;

    struct node *current = ml->first;
    while (current != NULL) {
        struct node *next = current->next;
        match_free(current->match);
        free(current);
        current = next;
    }

    free(ml);
}
